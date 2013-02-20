package com.cirnoworks.libfisce.shell;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.concurrent.atomic.AtomicBoolean;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.IDebugConsole;
import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.INativeHandler;
import com.cirnoworks.fisce.intf.IStateListener;
import com.cirnoworks.fisce.intf.IThreadManager;
import com.cirnoworks.fisce.intf.IToolkit;
import com.cirnoworks.fisce.intf.SaveDataPostProcesser;
import com.cirnoworks.fisce.intf.SystemDebugConsole;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClass;
import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.intf.idata.IField;
import com.cirnoworks.fisce.intf.idata.Message;

public class FYContext implements Runnable, FiScEVM {

	private final ByteBuffer context;
	private final Message message = new Message();

	private final FYHeap heap;
	private final FYThreadManager threadManager;

	private final HashMap<Integer, FYClass> classCache = new HashMap<Integer, FYClass>();
	private final HashMap<Integer, FYThread> threadCache = new HashMap<Integer, FYThread>();
	private final HashMap<Integer, FYMethod> methodCache = new HashMap<Integer, FYMethod>();
	private final HashMap<Integer, FYField> fieldCache = new HashMap<Integer, FYField>();

	private final HashMap<String, INativeHandler> handlers = new HashMap<String, INativeHandler>();
	private final HashMap<String, INativeHandler> optimizedHandlers = new HashMap<String, INativeHandler>();
	private final LinkedHashSet<IStateListener> stateListeners = new LinkedHashSet<IStateListener>();
	private final LinkedHashSet<IToolkit> toolkits = new LinkedHashSet<IToolkit>();

	private final AtomicBoolean runningSet = new AtomicBoolean(false);
	private final AtomicBoolean runningCurrent = new AtomicBoolean(false);

	static {
		ArrayList<Throwable> errors = new ArrayList<Throwable>();
		boolean succeed = false;
		if (!succeed) {
			try {
				System.loadLibrary("fyjni64");
				succeed = true;
			} catch (UnsatisfiedLinkError e) {
				errors.add(e);
			}
		}

		if (!succeed) {
			try {
				System.loadLibrary("fyjni");
				succeed = true;
			} catch (UnsatisfiedLinkError e) {
				errors.add(e);
			}
		}
		if (!succeed) {
			try {
				System.loadLibrary("libfyjni-0");
				succeed = true;
			} catch (UnsatisfiedLinkError e) {
				errors.add(e);
			}
		}

		try {
			FisceService.getSize();
		} catch (UnsatisfiedLinkError e) {
			for (Throwable ee : errors) {
				ee.printStackTrace();
			}
			throw new RuntimeException(e);
		}
	}

	public FYContext() {
		int size = FisceService.getSize();
		context = ByteBuffer.allocateDirect(size);
		context.order(ByteOrder.nativeOrder());
		initContext();
		heap = new FYHeap(this);
		threadManager = new FYThreadManager(this);
	}

	private void initialize() {
		for (IToolkit toolkit : toolkits) {
			toolkit.setContext(this);
		}

		for (IToolkit toolkit : toolkits) {
			toolkit.setupContext();
		}
	}

	public void bootFromClass(String name) {
		if (name == null) {
			throw new NullPointerException();
		}
		initialize();
		for (INativeHandler inh : handlers.values()) {
			inh.init(this);
		}
		FisceService.bootFromClass(context, name);
	}

	public Message execute(int[] params) {
		FisceService.execute(context, message, params);
		return message;
	}

	public INativeHandler getHandler(String name) {
		INativeHandler ret = optimizedHandlers.get(name);
		if (ret == null) {
			ret = handlers.get(name);
			if ((ret = handlers.get(name)) != null) {
				optimizedHandlers.put(name, ret);
				handlers.remove(name);
			}
		}
		return ret;
	}

	private void initContext() {
		FisceService.initContext(this);
	}

	public Message getMessage() {
		return message;
	}

	public static void main(String[] args) {
		FYContext context = new FYContext();
		context.bootFromClass("com/cirnoworks/fisce/privat/Profile");
		context.start();
	}

	public ByteBuffer getContext() {
		return context;
	}

	public Element getSaveData(InputStream is) throws VMCriticalException {
		// TODO Auto-generated method stub
		return null;
	}

	public void bootFromData(InputStream is) throws VMCriticalException,
			IOException {
		// TODO Auto-generated method stub

	}

	public void saveData(OutputStream os) throws VMCriticalException,
			IOException {
		// TODO Auto-generated method stub

	}

	public void saveData(OutputStream os, SaveDataPostProcesser postProcesser)
			throws VMCriticalException, IOException {
		// TODO Auto-generated method stub

	}

	public void requestStop() {
		runningSet.set(false);
	}

	public int waitTillStopped(long waitTime) throws InterruptedException {
		long time = System.currentTimeMillis() + waitTime;
		while (System.currentTimeMillis() < time) {
			if (!runningCurrent.get()) {
				return 0;
			}
			Thread.sleep(10);
		}
		return 0;
	}

	public void start() throws VMException {
		runningSet.set(true);
		runningCurrent.set(true);
		new Thread(this).start();
	}

	public void addStateListener(IStateListener isl) {
		this.stateListeners.add(isl);
	}

	public void addToolkit(IToolkit toolkit) {
		this.toolkits.add(toolkit);
	}

	public void setConsole(IDebugConsole console) {
		FisceService.setConsole(console == null ? new SystemDebugConsole()
				: console);
	}

	public void registerNativeHandler(INativeHandler inh) {
		// if (handlers.containsKey(inh.getUniqueName())) {
		// throw new VMCriticalException("Dupcated native handler "
		// + inh.getUniqueName());
		// } else {
		handlers.put(inh.getUniqueName(), inh);
		// }
	}

	public IHeap getHeap() {
		return heap;
	}

	public IThreadManager getThreadManager() {
		return threadManager;
	}

	public FYClass getClassById(int id) {
		FYClass ret = classCache.get(id);
		if (ret == null) {
			ret = new FYClass(this, id);
			classCache.put(id, ret);
		}
		return ret;
	}

	public FYThread getThreadById(int id) {
		FYThread ret = threadCache.get(id);
		if (ret == null) {
			ret = new FYThread(this, id);
			threadCache.put(id, ret);
		}
		return ret;
	}

	public FYMethod getMethodById(int id) {
		FYMethod ret = methodCache.get(id);
		if (ret == null) {
			ret = new FYMethod(this, id);
			methodCache.put(id, ret);
		}
		return ret;
	}

	public FYField getFieldById(int id) {
		FYField ret = fieldCache.get(id);
		if (ret == null) {
			ret = new FYField(this, id);
			fieldCache.put(id, ret);
		}
		return ret;
	}

	public IClass getClass(String name) throws VMException, VMCriticalException {
		int id = FisceService.getClassByName(context, name);
		if (id < 0) {
			throw new VMCriticalException("Can't find class " + name);
		}
		return getClassById(id);
	}

	public IField getField(String name) throws VMException, VMCriticalException {
		int id = FisceService.getFieldByUniqueName(context, name);
		if (id < 0) {
			throw new VMCriticalException("Can't find field " + name);
		}
		return getFieldById(id);
	}

	public IField lookupFieldVirtual(IClassBase clazz, String name)
			throws VMException, VMCriticalException {
		int id = FisceService.lookupField(context, ((FYClass) clazz).getId(),
				"." + name);
		if (id < 0) {
			throw new VMCriticalException("Can't find field " + name);
		}
		return getFieldById(id);
	}

	public IClass getClass(int i) throws VMException {
		if (FisceService.validClassId(context, i)) {
			return getClassById(i);
		} else {
			return null;
		}
	}

	public void run() {
		int[] params = new int[256];
		runningCurrent.set(true);
		try {
			all: while (runningSet.get()) {
				Message msg = execute(params);
				switch (msg.getMessageType()) {
				case Message.message_invoke_native:
					INativeHandler nh = handlers.get(msg.getNativeUniqueName());
					if (nh != null) {
						nh.dealNative(params, getThreadById(msg.getThreadId()));
					} else {
						try {
							throw (new VMCriticalException(
									"Stopped at invoke native "
											+ msg.getNativeUniqueName()
											+ " at thread " + msg.getThreadId()));
						} catch (VMCriticalException e) {
							onException(e);
						}
						break all;
					}
					break;
				case Message.message_exception:
					try {
						throw (new VMCriticalException(
								"Stopped at invoke exception: "
										+ msg.getExceptionName() + ": "
										+ msg.getExceptionDesc()
										+ " at thread " + msg.getThreadId()));
					} catch (VMCriticalException e) {
						onException(e);
					}
					break all;
				case Message.message_vm_dead:
					break all;
				case Message.message_sleep:
					try {
						if (msg.getSleepTime() > 0) {
							Thread.sleep(msg.getSleepTime());
						}
					} catch (InterruptedException ie) {
						ie.printStackTrace();
					}
					break;
				default:
					throw new RuntimeException("Illegal message type: "
							+ msg.getMessageType());
				}
			}
		} catch (Throwable e) {
			onException(e);
		} finally {
			runningCurrent.set(false);
			close();
		}
	}

	public void onException(Throwable e) {
		e.printStackTrace();
		for (IStateListener listener : stateListeners) {
			listener.onException(e);
		}
	}

	public Collection<IToolkit> getToolkits() {
		return toolkits;
	}

	public IClass getClassForClassObjectHandle(int handle) throws VMException {
		int classId = FisceService.getIdFromHandle(context, handle);
		return getClass(classId);
	}

	@Override
	public void logD(String msg) {
		FisceService.logD(msg);
	}

	@Override
	public void logD(String msg, Throwable t) {
		FisceService.logD(msg, t);
	}

	@Override
	public void logI(String msg) {
		FisceService.logI(msg);
	}

	@Override
	public void logI(String msg, Throwable t) {
		FisceService.logI(msg, t);
	}

	@Override
	public void logW(String msg) {
		FisceService.logW(msg);
	}

	@Override
	public void logW(String msg, Throwable t) {
		FisceService.logW(msg, t);
	}

	@Override
	public void logE(String msg) {
		FisceService.logE(msg);
	}

	@Override
	public void logE(String msg, Throwable t) {
		FisceService.logE(msg, t);
	}

	@Override
	public void unregisterNativeHandler(String string) {
		FisceService.unregisterNativeHandler(context, string);
	}

	@Override
	public void close() {
		System.out.println("over");
		FisceService.destroyContext(this);
		for (IStateListener listener : stateListeners) {
			listener.onVMDead();
		}
	}

}
