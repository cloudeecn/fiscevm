/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.vm.default_impl;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.dom4j.Element;

import com.cirnoworks.fisce.vm.Base64;
import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.IThreadManager;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;
import com.cirnoworks.fisce.vm.data.ClassMethod;

public class FastThreadManager implements Runnable, IThreadManager {
	// constant
	private static final int[] pricmds = new int[] { 1, 100, // 1
			200, // 2
			500, // 3
			700, // 4
			1000, // 5
			1400, // 6
			1800, // 7
			2500, // 8
			3500, // 9
			5000, // 10
	};

	// *********************
	// persist
	private final ArrayList<FastThread> runningThreads = new ArrayList<FastThread>();

	// thread id --> monitor id waiting for monitorentry
	private int[] waitForLockId = new int[MAX_THREADS];
	// thread id --> monitor id waiting for notify
	private int[] waitForNotifyId = new int[MAX_THREADS];
	// thread id --> how much time the thread orignally had
	private int[] pendingLockCount = new int[MAX_THREADS];
	// thread id --> next wakeup time
	private long[] nextWakeUpTime = new long[MAX_THREADS];
	// thread id --> interrupted
	private boolean[] interrupted = new boolean[MAX_THREADS];
	// thread id --> destroyPending
	private boolean[] destroyPending = new boolean[MAX_THREADS];
	// handle --> monitor owner id
	private int[] monitorOwnerId = new int[IHeap.MAX_OBJECTS];
	// handle --> monitor owner times
	private int[] monitorOwnerTimes = new int[IHeap.MAX_OBJECTS];
	//
	// **************************
	// no persist but need resume
	// threadId --> is used
	private boolean[] threadIds = new boolean[MAX_THREADS];
	// **************************
	// no persist
	private VMContext context;
	private IHeap heap;
	private int state;
	private final Object threadsLock = new Object();
	private final Object stateLock = new Object();
	private Thread workingThread;
	private final ArrayList<FastThread> pendingThreads = new ArrayList<FastThread>();
	private long nextWakeUpTimeTotal;
	private int nextThreadId;
	private Throwable exitException;
	private int exitCode;

	public void monitorEnter(FastThread dt, int monitorId) {
		monitorEnter(dt, monitorId, 1);
	}

	public void monitorExit(FastThread dt, int monitorId)
			throws VMException, VMCriticalException {
		monitorExit(dt, monitorId, 1);
	}

	public void sleep(IThread dt, long time) {
		int threadId = ((FastThread) dt).getThreadId();
		nextWakeUpTime[threadId] = System.currentTimeMillis() + time;
		dt.setYield(true);
	}

	public void interrupt(int targetHandle) throws VMException,
			VMCriticalException {
		FastThread target = getThread(targetHandle);
		if (target == null) {
			return;
		}
		int threadId = target.getThreadId();
		// System.out
		// .println("nwut[" + threadId + "]=" + nextWakeUpTime[threadId]);
		if (nextWakeUpTime[threadId] > 0) {
			assert target.getCurrentThrowable() == 0;
			int exceptionHandle = context.getThrower().prepareThrowable(
					new VMException("java/lang/InterruptedException", ""),
					target);
			target.setCurrentThrowable(exceptionHandle);
			nextWakeUpTime[threadId] = 0;
			interrupted[threadId] = true;
		}
	}

	public boolean isInterrupted(int targetHandle, boolean clear) {
		FastThread target = getThread(targetHandle);
		if (target == null) {
			return false;
		}
		boolean ret = interrupted[target.getThreadId()];
		if (clear) {
			interrupted[target.getThreadId()] = false;
		}
		return ret;
	}

	public void wait(IThread thread, int monitorId, long time)
			throws VMException, VMCriticalException {
		FastThread dt = (FastThread) thread;
		int tid = dt.getThreadId();
		assert waitForNotifyId[tid] == 0;
		if (monitorOwnerId[monitorId] != tid) {
			throw new VMException("java/lang/IllegalMonitorStateException", "");
		}
		waitForNotifyId[tid] = monitorId;
		pendingLockCount[tid] = releaseMonitor(dt, monitorId);
		// System.out.println("*****" + tid + " " + pendingLockCount[tid]);
		if (time <= 0) {
			// wait till the end of the time ;)
			nextWakeUpTime[tid] = Long.MAX_VALUE;
		} else {
			nextWakeUpTime[tid] = System.currentTimeMillis() + time;
		}
		thread.setYield(true);
	}

	public void notify(IThread thread, int monitorId, boolean all)
			throws VMException {
		FastThread dt = (FastThread) thread;
		int tid = dt.getThreadId();
		if (monitorOwnerId[monitorId] != tid) {
			throw new VMException("java/lang/IllegalMonitorStateException", "");
		}
		for (int i = 0; i < MAX_THREADS; i++) {
			if (waitForNotifyId[i] == monitorId) {
				waitForNotifyId[i] = 0;
				assert waitForLockId[i] == 0;
				waitForLockId[i] = monitorId;
				nextWakeUpTime[i] = 0;
				if (!all) {
					// only notify one
					break;
				}
			}
		}
	}

	public boolean isAlive(int threadHandle) {
		FastThread t = getThread(threadHandle);
		return t != null;
	}

	public void destroyThread(FastThread dt) {
		synchronized (threadsLock) {
			if (runningThreads.contains(dt)) {
				destroyPending[dt.getThreadId()] = true;
			}
		}
	}

	public FastThreadManager() {
		for (int i = 0; i < IHeap.MAX_OBJECTS; i++) {
			monitorOwnerId[i] = -1;
		}
	}

	public void setContext(VMContext context) {
		this.context = context;
		heap = context.getHeap();
	}

	private void monitorEnter(FastThread dt, int monitorId, int times) {
		int threadId = dt.getThreadId();
		int owner = monitorOwnerId[monitorId];
		if (owner == threadId) {
			monitorOwnerTimes[monitorId] += times;
		} else if (owner < 0) {
			monitorOwnerId[monitorId] = threadId;
			monitorOwnerTimes[monitorId] = times;
		} else {
			assert waitForLockId[threadId] == 0;
			waitForLockId[threadId] = monitorId;
			pendingLockCount[threadId] = 1;
			dt.setYield(true);
		}
	}

	private void monitorExit(FastThread dt, int monitorId, int times)
			throws VMException, VMCriticalException {
		int threadId = dt.getThreadId();
		if (monitorOwnerId[monitorId] != threadId) {
			throw new VMException("java/lang/IllegalMonitorStateException", "");
		}
		monitorOwnerTimes[monitorId] -= times;
		if (monitorOwnerTimes[monitorId] == 0) {
			monitorOwnerId[monitorId] = -1;
			dt.setYield(true);
		} else if (monitorOwnerTimes[monitorId] < 0) {
			context.getConsole().error("Too much monitors released!");
			throw new VMCriticalException(
					"java/lang/IllegalMonitorStateException");
		}
	}

	private int releaseMonitor(FastThread dt, int monitorId)
			throws VMException, VMCriticalException {
		int threadId = dt.getThreadId();
		if (monitorOwnerId[monitorId] != threadId) {
			throw new VMException("java/lang/IllegalMonitorStateException", "");
		}
		int times = monitorOwnerTimes[monitorId];
		monitorExit(dt, monitorId, times);
		return times;
	}

	private int fetchNextThreadId() throws VMException {
		int h = nextThreadId;
		while (threadIds[h]) {
			// used
			h++;
			if (h == MAX_THREADS) {
				h = 0;
			}
			if (h == nextThreadId) {
				throw new VMException("java/lang/VirtualMachineError",
						"Threads used up!");
			}
		}
		nextThreadId = (h + 1) % MAX_THREADS;
		return h;
	}

	private FastThread getThread(int threadHandle) {
		synchronized (threadsLock) {
			Iterator<FastThread> idt = runningThreads.iterator();
			while (idt.hasNext()) {
				FastThread dt = idt.next();
				if (threadHandle == dt.getThreadHandle()) {
					return dt;
				}
			}
			idt = pendingThreads.iterator();
			while (idt.hasNext()) {
				FastThread dt = idt.next();
				if (threadHandle == dt.getThreadHandle()) {
					return dt;
				}
			}
		}
		return null;
	}

	private void onThreadAdd(FastThread dt) {

	}

	private void onThreadRemove(FastThread dt) {
		int tid = dt.getThreadId();
		// release all locks
		waitForLockId[tid] = 0;
		waitForNotifyId[tid] = 0;
		nextWakeUpTime[tid] = 0;
		pendingLockCount[tid] = 0;
		threadIds[tid] = false;
		destroyPending[tid] = false;
		interrupted[tid] = false;
		for (int i = 0; i < IHeap.MAX_OBJECTS; i++) {
			if (monitorOwnerId[i] == tid) {
				monitorOwnerId[i] = -1;
				monitorOwnerTimes[i] = 0;
			}
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.cirnoworks.fisce.vm.IThreadManager#bootFromMain(com.cirnoworks
	 * .fisce.vm.data.ClassBase)
	 */
	public void bootFromMain(ClassBase clazz) throws VMException,
			VMCriticalException {
		synchronized (stateLock) {
			if (state != STATE_NEW) {
				throw new IllegalStateException();
			}

			setState(STATE_BOOT_PENDING);
		}
		ClassMethod method = context.getMethod(clazz.getName()
				+ ".main.([Ljava/lang/String;)V");
		if (method == null) {
			throw new VMException("java/lang/NoSuchMethodError",
					clazz.getName() + ".main(String[] args)");
		}
		FastThread dt = new FastThread(context, this);
		ClassBase threadClass = (ClassBase) context
				.getClass("java/lang/Thread");
		if (threadClass == null) {
			throw new VMException("java/lang/ClassNotFoundException",
					"java/lang/Thread");
		}
		int threadHandle = heap.allocate(threadClass);
		ClassField nameField = context.getField("java/lang/Thread.name.[C");
		ClassField priorityField = context
				.getField("java/lang/Thread.priority.I");
		if (nameField == null || priorityField == null) {
			throw new VMException("java/lang/NoSuchFieldError",
					"Cannot found fields in Thread");
		}
		char[] name = "Thread-0".toCharArray();
		int nameHandle = heap.allocate((ClassArray) context.getClass("[C"),
				name.length);
		for (int i = 0, max = name.length; i < max; i++) {
			heap.putArrayChar(nameHandle, i, name[i]);
		}
		heap.putFieldHandle(threadHandle, nameField, nameHandle);
		heap.putFieldInt(threadHandle, priorityField, 5);
		dt.create(threadHandle, method);
		dt.setPriority(5);
		dt.setThreadId(fetchNextThreadId());
		onThreadAdd(dt);
		runningThreads.add(dt);
		synchronized (stateLock) {
			setState(STATE_STOP);
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IThreadManager#pushThread(int)
	 */
	public void pushThread(int threadHandle) throws VMException,
			VMCriticalException {
		synchronized (stateLock) {
			if (state != STATE_RUNNING && state != STATE_STOP_PENDING) {
				throw new IllegalStateException();
			}
		}
		FastThread dt = new FastThread(context, this);
		ClassField priorityField = context
				.getField("java/lang/Thread.priority.I");
		int priority = heap.getFieldInt(threadHandle, priorityField);
		if (priority == 0) {
			priority = 5;
		}
		// so we need to set the priority here...
		dt.setPriority(priority);
		dt.create(threadHandle);
		dt.setThreadId(fetchNextThreadId());
		onThreadAdd(dt);
		// System.out.println("XXXX+"+dt.getThreadId());
		pendingThreads.add(dt);
	}

	public void run() {
		synchronized (stateLock) {
			if (state != STATE_RUN_PENDING && state != STATE_STOP_PENDING) {
				throw new IllegalStateException("" + state);
			}
			setState(STATE_RUNNING);
		}
		long nextGC = System.currentTimeMillis() + 5000;
		long nextGCForce = System.currentTimeMillis() + 15000;
		Iterator<FastThread> idt = runningThreads.iterator();
		int stateLocal;
		try {
			while (true) {
				synchronized (stateLock) {
					stateLocal = state;
				}
				switch (stateLocal) {
				case STATE_RUNNING:
					if (idt.hasNext()) {
						FastThread dt = idt.next();
						int threadId = dt.getThreadId();
						if (destroyPending[threadId]) {
							onThreadRemove(dt);
							idt.remove();

						}

						long nwut = nextWakeUpTime[threadId];
						if (nwut > System.currentTimeMillis()) {
							if (nextWakeUpTimeTotal > nwut) {
								nextWakeUpTimeTotal = nwut;
							}
							break;
						}
						nextWakeUpTime[threadId] = 0;
						nextWakeUpTimeTotal = 0;

						int lockId = waitForLockId[threadId];
						if (lockId > 0) {

							// waiting for lock
							if (monitorOwnerId[lockId] < 0) {
								// context.getConsole().info(
								// threadId + ">+++Exit lock " + lockId);
								// lock already released
								monitorOwnerId[lockId] = threadId;
								monitorOwnerTimes[lockId] = pendingLockCount[threadId];
								waitForLockId[threadId] = 0;
							} else {
								// still locked
								break;
							}
						}
						boolean dead = dt.run(pricmds[dt.getPriority()]);
						if (dead) {
							synchronized (runningThreads) {
								onThreadRemove(dt);
								idt.remove();
							}
						}
					} else {
						synchronized (threadsLock) {
							if (pendingThreads.size() > 0) {
								runningThreads.addAll(pendingThreads);
								pendingThreads.clear();
								nextWakeUpTimeTotal = 0;
							}
						}

						if (runningThreads.isEmpty()) {
							synchronized (stateLock) {
								setState(STATE_DEAD);
								workingThread = null;
								return;
							}
						}
						long now = System.currentTimeMillis();
						long sleepTime = nextWakeUpTimeTotal - now;
						if ((sleepTime > 100 && now > nextGC)
								|| now > nextGCForce) {
							nextGC = now + 5000;
							nextGCForce = nextGC + 10000;
							heap.gc();
							now = System.currentTimeMillis();
							sleepTime = nextWakeUpTimeTotal - now;
						}
						if (sleepTime > 0) {
							try {
								Thread.sleep(sleepTime);
							} catch (InterruptedException e) {
							}
						}
						nextWakeUpTimeTotal = Long.MAX_VALUE;
						idt = runningThreads.iterator();
					}
					break;
				case STATE_STOP_PENDING:
					synchronized (stateLock) {
						synchronized (threadsLock) {
							runningThreads.addAll(pendingThreads);
							pendingThreads.clear();
						}
						idt = runningThreads.iterator();
						setState(STATE_STOP);
						workingThread = null;
						return;
					}
				case STATE_DEAD_PENDING:
					synchronized (stateLock) {
						setState(STATE_DEAD);
						workingThread = null;
						return;
					}
				default:
					throw new IllegalThreadStateException();
				}
			}
		} catch (VMCriticalException e) {
			context.getConsole().error("Critical Exception! Shutdown VM", e);
			this.exitException = e;
			synchronized (stateLock) {
				setState(STATE_DEAD);
				workingThread = null;
			}
		} catch (Throwable t) {
			context.getConsole().error("Uncatched Exception! Shutdown VM", t);
			this.exitException = t;
			synchronized (stateLock) {
				setState(STATE_DEAD);
				workingThread = null;
			}
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IThreadManager#start()
	 */
	public void start() throws VMException {
		synchronized (stateLock) {
			if (workingThread != null) {
				throw new IllegalStateException();
			}
			if (state != STATE_STOP) {
				throw new IllegalStateException();
			}
			setState(STATE_RUN_PENDING);
			workingThread = new Thread(this);
			workingThread.start();
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IThreadManager#requestStop()
	 */
	public void requestStop() {
		synchronized (stateLock) {
			switch (state) {
			case STATE_RUN_PENDING:
			case STATE_RUNNING:
				setState(STATE_STOP_PENDING);
			case STATE_STOP_PENDING:
			case STATE_STOP:
			case STATE_DEAD:
			case STATE_DEAD_PENDING:
				break;
			default:
				throw new IllegalStateException("" + state);
			}
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IThreadManager#waitTillStopped(long)
	 */
	public int waitTillStopped(long waitTime) throws InterruptedException {
		synchronized (stateLock) {
			switch (state) {
			case STATE_RUN_PENDING:
			case STATE_RUNNING:
			case STATE_STOP_PENDING:
			case STATE_DEAD_PENDING:
				break;
			case STATE_STOP:
			case STATE_DEAD:
				return exitCode;
			default:
				throw new IllegalThreadStateException();
			}
		}
		long time = waitTime > 0 ? System.currentTimeMillis() + waitTime
				: Long.MAX_VALUE;
		while (System.currentTimeMillis() < time) {
			synchronized (stateLock) {
				if (state == STATE_STOP || state == STATE_DEAD) {
					break;
				}
			}
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				break;
			}
		}
		synchronized (stateLock) {
			if (state == STATE_STOP || state == STATE_DEAD) {
				if (exitException != null) {
					throw new RuntimeException("VMError before finish!",
							exitException);
				}
				return exitCode;
			} else {
				throw new InterruptedException();
			}
		}
	}

	public void setPriority(int threadHandle, int priority) throws VMException {
		FastThread dt = getThread(threadHandle);
		if (dt == null) {
			// The sun cldc impl will call this when Thread object first
			// instanced
			// In this time we don't have a thread here.
			// How can I know when user create a Thread object without some
			// hacking?
			// WTF!
			return;
		}
		dt.setPriority(priority);
	}

	public IThread[] getThreads() throws VMException {
		synchronized (threadsLock) {
			IThread[] rt = runningThreads.toArray(new IThread[runningThreads
					.size() + pendingThreads.size()]);
			IThread[] pt = pendingThreads.toArray(new IThread[pendingThreads
					.size()]);
			System.arraycopy(pt, 0, rt, runningThreads.size(),
					pendingThreads.size());
			return rt;
		}
	}

	public void loadData(Element data) throws VMCriticalException {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			setState(STATE_STOP);
			Element threadsElement = data.element("threads");
			for (Element te : (List<Element>) threadsElement.elements("thread")) {
				int tid = Integer.parseInt(te.attributeValue("tid"));
				int waitForLockId1 = Integer.parseInt(te
						.attributeValue("waitForLockId"));
				int waitForNotifyId1 = Integer.parseInt(te
						.attributeValue("waitForNotifyId"));
				int pendingLockCount1 = Integer.parseInt(te
						.attributeValue("pendingLockCount"));
				long nextWakeUpTime1 = Long.parseLong(te
						.attributeValue("nextWakeUpTime"));
				boolean interrupted1 = Boolean.parseBoolean(te
						.attributeValue("interrupted"));
				boolean destroyPending1 = Boolean.parseBoolean(te
						.attributeValue("destroyPending"));
				baos.reset();
				Base64.decode(te.getText(), baos);
				FastThread dt = new FastThread(context, this);
				dt.createFromData(baos.toByteArray());
				runningThreads.add(dt);
				threadIds[tid] = true;
				waitForLockId[tid] = waitForLockId1;
				waitForNotifyId[tid] = waitForNotifyId1;
				pendingLockCount[tid] = pendingLockCount1;
				nextWakeUpTime[tid] = nextWakeUpTime1;
				interrupted[tid] = interrupted1;
				destroyPending[tid] = destroyPending1;
			}

			Element monitorsElement = data.element("monitors");
			for (Element me : (List<Element>) monitorsElement
					.elements("monitor")) {
				int handle = Integer.parseInt(me.attributeValue("handle"));
				int owner = Integer.parseInt(me.attributeValue("owner"));
				int times = Integer.parseInt(me.attributeValue("times"));
				monitorOwnerId[handle] = owner;
				monitorOwnerTimes[handle] = times;
			}
		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	public void saveData(Element data) {
		// private final ArrayList<FastThread> runningThreads = new
		// ArrayList<FastThread>();
		// thread id --> monitor id waiting for monitorentry
		// private int[] waitForLockId = new int[MAX_THREADS];
		// thread id --> monitor id waiting for notify
		// private int[] waitForNotifyId = new int[MAX_THREADS];
		// thread id --> how much time the thread orignally had
		// private int[] pendingLockCount = new int[MAX_THREADS];
		// thread id --> next wakeup time
		// private long[] nextWakeUpTime = new long[MAX_THREADS];
		// thread id --> interrupted
		// private boolean[] interrupted = new boolean[MAX_THREADS];
		// thread id --> destroyPending
		// private boolean[] destroyPending = new boolean[MAX_THREADS];
		// handle --> monitor owner id
		Element threads = data.addElement("threads");
		for (FastThread dt : runningThreads) {
			Element thread = threads.addElement("thread").addText(
					Base64.encode(dt.getFullStack()));
			int tid = dt.getThreadId();
			thread.addAttribute("tid", String.valueOf(tid));
			thread.addAttribute("waitForLockId",
					String.valueOf(waitForLockId[tid]));
			thread.addAttribute("waitForNotifyId",
					String.valueOf(waitForNotifyId[tid]));
			thread.addAttribute("pendingLockCount",
					String.valueOf(pendingLockCount[tid]));
			thread.addAttribute("nextWakeUpTime",
					String.valueOf(nextWakeUpTime[tid]));
			thread.addAttribute("interrupted", String.valueOf(interrupted[tid]));
			thread.addAttribute("destroyPending",
					String.valueOf(destroyPending[tid]));
		}

		// private int[] monitorOwnerId = new int[IHeap.MAX_OBJECTS];
		// handle --> monitor owner times
		// private int[] monitorOwnerTimes = new int[IHeap.MAX_OBJECTS];
		Element monitors = data.addElement("monitors");
		for (int i = 0; i < IHeap.MAX_OBJECTS; i++) {
			if (monitorOwnerId[i] >= 0) {
				monitors.addElement("monitor")
						.addAttribute("handle", String.valueOf(i))
						.addAttribute("owner",
								String.valueOf(monitorOwnerId[i]))
						.addAttribute("times",
								String.valueOf(monitorOwnerTimes[i]));
			}
		}
	}

	public void exit(int exitCode) {
		synchronized (context) {
			setState(STATE_DEAD_PENDING);
			this.exitCode = exitCode;
		}
	}

	private void setState(int state) {
		synchronized (stateLock) {
			this.state = state;
			context.onStateChange(state);
		}
	}
}
