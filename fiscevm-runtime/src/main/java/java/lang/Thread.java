/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package java.lang;

public class Thread implements Runnable {

	private char name[];
	private int priority;
	private Thread threadQ;
	private long eetop;

	private boolean daemon = false;

	private boolean stillborn = false;

	private boolean started = false;
	private Runnable target;

	private static int threadInitNumber;

	private static synchronized int nextThreadNum() {
		return threadInitNumber++;
	}

	public final static int MIN_PRIORITY = 1;

	public final static int NORM_PRIORITY = 5;

	public final static int MAX_PRIORITY = 10;

	public static native Thread currentThread();

	public static native void yield();

	public static native void sleep(long millis) throws InterruptedException;

	public static void sleep(long millis, int nanos)
			throws InterruptedException {
		if (millis < 0) {
			throw new IllegalArgumentException("timeout value is negative");
		}

		if (nanos < 0 || nanos > 999999) {
			throw new IllegalArgumentException(
					"nanosecond timeout value out of range");
		}

		if (nanos >= 500000 || (nanos != 0 && millis == 0)) {
			millis++;
		}

		sleep(millis);
	}

	private void init(Runnable target, String name) {
		Thread parent = currentThread();

		this.daemon = parent.isDaemon();
		this.priority = parent.getPriority();
		this.name = name.toCharArray();
		this.target = target;
		setPriority(priority);

	}

	public Thread() {
		init(null, "Thread-" + nextThreadNum());
	}

	public Thread(Runnable target) {
		init(target, "Thread-" + nextThreadNum());
	}

	public Thread(String name) {
		init(null, name);
	}

	public Thread(Runnable target, String name) {
		init(target, name);
	}

	public native void start0();

	public synchronized void start() {
		if (started) {
			throw new IllegalThreadStateException();
		}
		started = true;
		start0();
	}

	public void run() {
		if (target != null) {
			target.run();
		}
	}

	private void exit() {
		target = null;
	}

	public void interrupt() {
		interrupt0();
	}

	public static boolean interrupted() {
		return currentThread().isInterrupted(true);
	}

	public boolean isInterrupted() {
		return isInterrupted(false);
	}

	private native boolean isInterrupted(boolean ClearInterrupted);

	public final native boolean isAlive();

	public final void setPriority(int newPriority) {
		if (newPriority > MAX_PRIORITY || newPriority < MIN_PRIORITY) {
			throw new IllegalArgumentException("Illegal priority: "
					+ newPriority + "(" + MIN_PRIORITY + "-" + MAX_PRIORITY
					+ ")");
		}
		setPriority0(priority = newPriority);
	}

	public final int getPriority() {
		return priority;
	}

	public final void setName(String name) {
		this.name = name.toCharArray();
	}

	public final String getName() {
		return String.valueOf(name);
	}

	public final synchronized void join(long millis)
			throws InterruptedException {
		long base = System.currentTimeMillis();
		long now = 0;

		if (millis < 0) {
			throw new IllegalArgumentException("timeout value is negative");
		}

		if (millis == 0) {
			while (isAlive()) {
				wait(0);
			}
		} else {
			while (isAlive()) {
				long delay = millis - now;
				if (delay <= 0) {
					break;
				}
				wait(delay);
				now = System.currentTimeMillis() - base;
			}
		}
	}

	public final synchronized void join(long millis, int nanos)
			throws InterruptedException {

		if (millis < 0) {
			throw new IllegalArgumentException("timeout value is negative");
		}

		if (nanos < 0 || nanos > 999999) {
			throw new IllegalArgumentException(
					"nanosecond timeout value out of range");
		}

		if (nanos >= 500000 || (nanos != 0 && millis == 0)) {
			millis++;
		}

		join(millis);
	}

	public final void join() throws InterruptedException {
		join(0);
	}

	public static void dumpStack() {
		new Exception("Stack trace").printStackTrace();
	}

	public final void setDaemon(boolean on) {
		if (isAlive()) {
			throw new IllegalThreadStateException();
		}
		daemon = on;
	}

	public final boolean isDaemon() {
		return daemon;
	}

	@Override
	public String toString() {

		return "Thread[" + getName() + "," + getPriority() + "," + "" + "]";

	}

	private native void setPriority0(int newPriority);

	private native void interrupt0();

	public ClassLoader getContextClassLoader() {
		return ClassLoader.getSystemClassLoader();
	}

}
