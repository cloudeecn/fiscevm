package com.cirnoworks.libfisce.shell;

import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.IdentityHashMap;
import java.util.Map;

import com.cirnoworks.fisce.intf.IDebugConsole;
import com.cirnoworks.fisce.intf.IToolkit;
import com.cirnoworks.fisce.intf.SystemDebugConsole;
import com.cirnoworks.fisce.intf.idata.Message;

public final class FisceService {
	private static Map<ByteBuffer, FYContext> contextMap = new IdentityHashMap<ByteBuffer, FYContext>();
	private static IDebugConsole console = new SystemDebugConsole();

	public static IDebugConsole getConsole() {
		return console;
	}

	public static void setConsole(IDebugConsole console) {
		FisceService.console = console;
	}

	public static void initContext(FYContext jcontext) {
		ByteBuffer context = jcontext.getContext();
		if (context == null) {
			throw new NullPointerException();
		}
		if (contextMap.containsKey(context)) {
			throw new RuntimeException("Duplcated initialize context");
		}
		contextMap.put(context, jcontext);
		initContext(context);
	}

	public static void destroyContext(FYContext jcontext) {
		ByteBuffer context = jcontext.getContext();
		if (contextMap.containsKey(context)) {
			destroyContext(context);
		}
		contextMap.remove(context);
	}

	public static native synchronized void execute(ByteBuffer context,
			Message message, int[] params);

	private static native synchronized void initContext(ByteBuffer context);

	public static native synchronized void bootFromClass(ByteBuffer context,
			String name);

	public static native synchronized int getSize();

	public static native synchronized int getClassByName(ByteBuffer context,
			String name);

	public static native synchronized boolean validClassId(ByteBuffer context,
			int id);

	public static native synchronized int getIdFromHandle(ByteBuffer context,
			int handle);

	public static native synchronized int getFieldByUniqueName(
			ByteBuffer context, String name);

	public static native synchronized int lookupField(ByteBuffer context,
			int classId, String name);

	public static native synchronized int getMethodByUniqueName(
			ByteBuffer context, String name);

	public static native synchronized int lookupMethod(ByteBuffer context,
			int classId, String name);

	public static native synchronized String getClassName(ByteBuffer context,
			int classId);

	public static native synchronized int getClassSuper(ByteBuffer context,
			int classId);

	public static native synchronized int getFieldOwner(ByteBuffer context,
			int fieldId);

	public static native synchronized void threadReturnHandle(
			ByteBuffer context, int threadId, int value);

	public static native synchronized void threadReturnInt(ByteBuffer context,
			int threadId, int value);

	public static native synchronized void threadReturnWide(ByteBuffer context,
			int threadId, long value);

	public static native synchronized int getThreadHandle(ByteBuffer context,
			int threadId);

	public static native synchronized void pushVMException(ByteBuffer context,
			int threadId, String name, String desc);

	public static native synchronized int allocateObject(ByteBuffer context,
			int classId);

	public static native synchronized int allocateArray(ByteBuffer context,
			int classId, int length);

	public static native synchronized int clone(ByteBuffer context, int handle);

	public static native synchronized int getLiteral(ByteBuffer context,
			String content);

	public static native synchronized int putString(ByteBuffer context,
			String content);

	public static native synchronized String getString(ByteBuffer context,
			int handle);

	public static native synchronized char[] getStringChars(ByteBuffer context,
			int handle);

	public static native synchronized int getClassOfHandle(ByteBuffer context,
			int handle);

	public static native synchronized int getArrayLength(ByteBuffer context,
			int handle);

	public static native synchronized boolean getFieldBoolean(
			ByteBuffer context, int handle, int fieldId);

	public static native synchronized byte getFieldByte(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized char getFieldChar(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized short getFieldShort(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized int getFieldHandle(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized int getFieldInt(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized float getFieldFloat(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized long getFieldLong(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized double getFieldDouble(ByteBuffer context,
			int handle, int fieldId);

	public static native synchronized void setFieldBoolean(ByteBuffer context,
			int handle, int fieldId, boolean value);

	public static native synchronized void setFieldByte(ByteBuffer context,
			int handle, int fieldId, byte value);

	public static native synchronized void setFieldChar(ByteBuffer context,
			int handle, int fieldId, char value);

	public static native synchronized void setFieldShort(ByteBuffer context,
			int handle, int fieldId, short value);

	public static native synchronized void setFieldHandle(ByteBuffer context,
			int handle, int fieldId, int value);

	public static native synchronized void setFieldInt(ByteBuffer context,
			int handle, int fieldId, int value);

	public static native synchronized void setFieldFloat(ByteBuffer context,
			int handle, int fieldId, float value);

	public static native synchronized void setFieldLong(ByteBuffer context,
			int handle, int fieldId, long value);

	public static native synchronized void setFieldDouble(ByteBuffer context,
			int handle, int fieldId, double value);

	public static native synchronized boolean getArrayBoolean(
			ByteBuffer context, int handle, int pos);

	public static native synchronized byte getArrayByte(ByteBuffer context,
			int handle, int pos);

	public static native synchronized char getArrayChar(ByteBuffer context,
			int handle, int pos);

	public static native synchronized short getArrayShort(ByteBuffer context,
			int handle, int pos);

	public static native synchronized int getArrayHandle(ByteBuffer context,
			int handle, int pos);

	public static native synchronized int getArrayInt(ByteBuffer context,
			int handle, int pos);

	public static native synchronized float getArrayFloat(ByteBuffer context,
			int handle, int pos);

	public static native synchronized long getArrayLong(ByteBuffer context,
			int handle, int pos);

	public static native synchronized double getArrayDouble(ByteBuffer context,
			int handle, int pos);

	public static native synchronized void setArrayBoolean(ByteBuffer context,
			int handle, int pos, boolean value);

	public static native synchronized void setArrayByte(ByteBuffer context,
			int handle, int pos, byte value);

	public static native synchronized void setArrayChar(ByteBuffer context,
			int handle, int pos, char value);

	public static native synchronized void setArrayShort(ByteBuffer context,
			int handle, int pos, short value);

	public static native synchronized void setArrayHandle(ByteBuffer context,
			int handle, int pos, int value);

	public static native synchronized void setArrayInt(ByteBuffer context,
			int handle, int pos, int value);

	public static native synchronized void setArrayFloat(ByteBuffer context,
			int handle, int pos, float value);

	public static native synchronized void setArrayLong(ByteBuffer context,
			int handle, int pos, long value);

	public static native synchronized void setArrayDouble(ByteBuffer context,
			int handle, int pos, double value);

	public static native synchronized boolean getStaticBoolean(
			ByteBuffer context, int fieldId);

	public static native synchronized byte getStaticByte(ByteBuffer context,
			int fieldId);

	public static native synchronized char getStaticChar(ByteBuffer context,
			int fieldId);

	public static native synchronized short getStaticShort(ByteBuffer context,
			int fieldId);

	public static native synchronized int getStaticHandle(ByteBuffer context,
			int fieldId);

	public static native synchronized int getStaticInt(ByteBuffer context,
			int fieldId);

	public static native synchronized float getStaticFloat(ByteBuffer context,
			int fieldId);

	public static native synchronized long getStaticLong(ByteBuffer context,
			int fieldId);

	public static native synchronized double getStaticDouble(
			ByteBuffer context, int fieldId);

	public static native synchronized void setStaticBoolean(ByteBuffer context,
			int fieldId, boolean value);

	public static native synchronized void setStaticByte(ByteBuffer context,
			int fieldId, byte value);

	public static native synchronized void setStaticChar(ByteBuffer context,
			int fieldId, char value);

	public static native synchronized void setStaticShort(ByteBuffer context,
			int fieldId, short value);

	public static native synchronized void setStaticHandle(ByteBuffer context,
			int fieldId, int value);

	public static native synchronized void setStaticInt(ByteBuffer context,
			int fieldId, int value);

	public static native synchronized void setStaticFloat(ByteBuffer context,
			int fieldId, float value);

	public static native synchronized void setStaticLong(ByteBuffer context,
			int fieldId, long value);

	public static native synchronized void setStaticDouble(ByteBuffer context,
			int fieldId, double value);

	public static native synchronized void fillArrayBoolean(ByteBuffer context,
			int handle, int dstPos, boolean[] src, int srcPos, int len);

	public static native synchronized void fillArrayByte(ByteBuffer context,
			int handle, int dstPos, byte[] src, int srcPos, int len);

	public static native synchronized void fillArrayChar(ByteBuffer context,
			int handle, int dstPos, char[] src, int srcPos, int len);

	public static native synchronized void fillArrayShort(ByteBuffer context,
			int handle, int dstPos, short[] src, int srcPos, int len);

	public static native synchronized void fillArrayInt(ByteBuffer context,
			int handle, int dstPos, int[] src, int srcPos, int len);

	public static native synchronized void fillArrayHandle(ByteBuffer context,
			int handle, int dstPos, int[] src, int srcPos, int len);

	public static native synchronized void fillArrayFloat(ByteBuffer context,
			int handle, int dstPos, float[] src, int srcPos, int len);

	public static native synchronized void fillArrayLong(ByteBuffer context,
			int handle, int dstPos, long[] src, int srcPos, int len);

	public static native synchronized void fillArrayDouble(ByteBuffer context,
			int handle, int dstPos, double[] src, int srcPos, int len);

	public static native synchronized void fetchArrayBoolean(
			ByteBuffer context, boolean[] dst, int dstPos, int srcHandle,
			int srcPos, int len);

	public static native synchronized void fetchArrayByteBuffer(
			ByteBuffer context, ByteBuffer dst, int handle, int srcPos, int len);

	public static native synchronized void fetchArrayByte(ByteBuffer context,
			byte[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayChar(ByteBuffer context,
			char[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayShort(ByteBuffer context,
			short[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayInt(ByteBuffer context,
			int[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayHandle(ByteBuffer context,
			int[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayFloat(ByteBuffer context,
			float[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayLong(ByteBuffer context,
			long[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native synchronized void fetchArrayDouble(ByteBuffer context,
			double[] dst, int dstPos, int srcHandle, int srcPos, int len);

	private static native synchronized void destroyContext(ByteBuffer context);

	public static InputStream getInputStream(ByteBuffer context, String name) {
		FYContext jcontext = contextMap.get(context);
		InputStream is = null;
		for (IToolkit toolkit : jcontext.getToolkits()) {
			is = toolkit.getResourceByName(name);
			if (is != null) {
				break;
			}
		}
		return is;
	}

	public static native synchronized void unregisterNativeHandler(
			ByteBuffer context, String uniqueName);

	private static StringBuilder debug = new StringBuilder();
	private static StringBuilder info = new StringBuilder();
	private static StringBuilder warn = new StringBuilder();
	private static StringBuilder error = new StringBuilder();

	public static void logD(String msg) {
		logD0(msg);
		logD0("\n");
	}

	public static void logD(String msg, Throwable t) {
		logD0(msg);
		logD0("\n");
		logD0(t.toString());
		logD0("\n");
		for (StackTraceElement ste : t.getStackTrace()) {
			logD0("\tat ");
			logD0(ste.toString());
			logD0("\n");
		}
		if (t.getCause() != null) {
			logD("Caused by:", t.getCause());
		}
	}

	public static void logI(String msg) {
		logI0(msg);
		logI0("\n");
	}

	public static void logI(String msg, Throwable t) {
		logI0(msg);
		logI0("\n");
		logI0(t.toString());
		logI0("\n");
		for (StackTraceElement ste : t.getStackTrace()) {
			logI0("\tat ");
			logI0(ste.toString());
			logI0("\n");
		}
		if (t.getCause() != null) {
			logI("Caused by:", t.getCause());
		}
	}

	public static void logW(String msg) {
		logW0(msg);
		logW0("\n");
	}

	public static void logW(String msg, Throwable t) {
		logW0(msg);
		logW0("\n");
		logW0(t.toString());
		logW0("\n");
		for (StackTraceElement ste : t.getStackTrace()) {
			logW0("\tat ");
			logW0(ste.toString());
			logW0("\n");
		}
		if (t.getCause() != null) {
			logW("Caused by:", t.getCause());
		}
	}

	public static void logE(String msg) {
		logE0(msg);
		logE0("\n");
	}

	public static void logE(String msg, Throwable t) {
		logE0(msg);
		logE0("\n");
		logE0(t.toString());
		logE0("\n");
		for (StackTraceElement ste : t.getStackTrace()) {
			logE0("\tat ");
			logE0(ste.toString());
			logE0("\n");
		}
		if (t.getCause() != null) {
			logE("Caused by:", t.getCause());
		}
	}

	public static void logD0(String msg) {
		for (int i = 0, max = msg.length(); i < max; i++) {
			char c = msg.charAt(i);
			if (c == '\r') {
				continue;
			} else if (c == '\n') {
				console.debug(error.toString());
				error.setLength(0);
			} else {
				error.append(c);
			}
		}
	}

	public static void logI0(String msg) {
		for (int i = 0, max = msg.length(); i < max; i++) {
			char c = msg.charAt(i);
			if (c == '\r') {
				continue;
			} else if (c == '\n') {
				console.info(info.toString());
				info.setLength(0);
			} else {
				info.append(c);
			}
		}
	}

	public static void logW0(String msg) {
		for (int i = 0, max = msg.length(); i < max; i++) {
			char c = msg.charAt(i);
			if (c == '\r') {
				continue;
			} else if (c == '\n') {
				console.warn(warn.toString());
				warn.setLength(0);
			} else {
				warn.append(c);
			}
		}
	}

	public static void logE0(String msg) {
		for (int i = 0, max = msg.length(); i < max; i++) {
			char c = msg.charAt(i);
			if (c == '\r') {
				continue;
			} else if (c == '\n') {
				console.error(error.toString());
				error.setLength(0);
			} else {
				error.append(c);
			}
		}
	}
}
