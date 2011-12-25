package com.cirnoworks.libfisce.shell;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
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
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClass;
import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.intf.idata.IField;

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
	private final LinkedHashSet<IStateListener> stateListeners = new LinkedHashSet<IStateListener>();
	private final LinkedHashSet<IToolkit> toolkits = new LinkedHashSet<IToolkit>();

	private final AtomicBoolean runningSet = new AtomicBoolean(false);
	private final AtomicBoolean runningCurrent = new AtomicBoolean(false);

	static {
		System.loadLibrary("fisce");
		System.loadLibrary("fyjni");
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

	private Message execute() {
		FisceService.execute(context, message);
		return message;
	}

	private void initContext() {
		FisceService.initContext(context);
	}

	public Message getMessage() {
		return message;
	}

	public static void main(String[] args) {
		FYContext context = new FYContext();
		context.bootFromClass("com/cirnoworks/fisce/privat/Profile");

	}

	public ByteBuffer getContext() {
		return context;
	}

	@Override
	public Element getSaveData(InputStream is) throws VMCriticalException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void bootFromData(InputStream is) throws VMCriticalException,
			IOException {
		// TODO Auto-generated method stub

	}

	@Override
	public void saveData(OutputStream os) throws VMCriticalException,
			IOException {
		// TODO Auto-generated method stub

	}

	@Override
	public void saveData(OutputStream os, SaveDataPostProcesser postProcesser)
			throws VMCriticalException, IOException {
		// TODO Auto-generated method stub

	}

	@Override
	public void requestStop() {
		runningSet.set(false);
	}

	@Override
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

	@Override
	public void start() throws VMException {
		runningSet.set(true);
		runningCurrent.set(true);
		new Thread(this).start();
	}

	@Override
	public void addStateListener(IStateListener isl) {
		this.stateListeners.add(isl);
	}

	@Override
	public void addToolkit(IToolkit toolkit) {
		this.toolkits.add(toolkit);
	}

	@Override
	public void setConsole(IDebugConsole console) {

	}

	@Override
	public void registerNativeHandler(INativeHandler inh) {
		if (handlers.containsKey(inh.getUniqueName())) {
			throw new VMCriticalException("Dupcated native handler "
					+ inh.getUniqueName());
		} else {
			handlers.put(inh.getUniqueName(), inh);
		}
	}

	@Override
	public IHeap getHeap() {
		return heap;
	}

	@Override
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

	@Override
	public IClass getClass(String name) throws VMException, VMCriticalException {
		int id = FisceService.getClassByName(context, name);
		if (id < 0) {
			throw new VMCriticalException("Can't find class " + name);
		}
		return getClassById(id);
	}

	@Override
	public IField getField(String name) throws VMException, VMCriticalException {
		int id = FisceService.getFieldByUniqueName(context, name);
		if (id < 0) {
			throw new VMCriticalException("Can't find field " + name);
		}
		return getFieldById(id);
	}

	@Override
	public IField lookupFieldVirtual(IClassBase clazz, String name)
			throws VMException, VMCriticalException {
		int id = FisceService.lookupField(context, ((FYClass) clazz).getId(),
				"." + name);
		if (id < 0) {
			throw new VMCriticalException("Can't find field " + name);
		}
		return getFieldById(id);
	}

	@Override
	public IClass getClass(int i) throws VMException {
		if (FisceService.validClassId(context, i)) {
			return getClassById(i);
		} else {
			return null;
		}
	}

	@Override
	public void run() {
		runningCurrent.set(true);
		try {
			all: while (runningSet.get()) {
				Message msg = execute();
				switch (msg.getMessageType()) {
				case Message.message_invoke_native:
					INativeHandler nh = handlers.get(msg.getNativeUniqueName());
					if (nh != null) {
						nh.dealNative(msg.getParams(),
								getThreadById(msg.getThreadId()));
					} else {
						try {
							throw (new VMCriticalException(
									"Stopped at invoke native "
											+ msg.getNativeUniqueName()
											+ " with " + msg.getParams().length
											+ " params."
											+ Arrays.toString(msg.getParams())
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
						Thread.sleep(msg.getSleepTime());
					} catch (InterruptedException ie) {
						ie.printStackTrace();
					}
					break;
				}
			}
		} finally {
			runningCurrent.set(false);
			System.out.println("over");
		}
	}

	public void onException(Throwable e) {
		e.printStackTrace();
		for (IStateListener listener : stateListeners) {
			listener.onException(e);
		}
	}
}
