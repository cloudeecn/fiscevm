package com.cirnoworks.libfisce.shell;

import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.IdentityHashMap;
import java.util.Map;

import com.cirnoworks.fisce.intf.IDebugConsole;
import com.cirnoworks.fisce.intf.IToolkit;
import com.cirnoworks.fisce.intf.SystemDebugConsole;

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

	public static native void execute(ByteBuffer context, Message message,
			int[] params);

	private static native void initContext(ByteBuffer context);

	public static native void bootFromClass(ByteBuffer context, String name);

	public static native int getSize();

	public static native int getClassByName(ByteBuffer context, String name);

	public static native boolean validClassId(ByteBuffer context, int id);

	public static native int getIdFromHandle(ByteBuffer context, int handle);

	public static native int getFieldByUniqueName(ByteBuffer context,
			String name);

	public static native int lookupField(ByteBuffer context, int classId,
			String name);

	public static native int getMethodByUniqueName(ByteBuffer context,
			String name);

	public static native int lookupMethod(ByteBuffer context, int classId,
			String name);

	public static native String getClassName(ByteBuffer context, int classId);

	public static native int getClassSuper(ByteBuffer context, int classId);

	public static native int getFieldOwner(ByteBuffer context, int fieldId);

	public static native void threadReturnHandle(ByteBuffer context,
			int threadId, int value);

	public static native void threadReturnInt(ByteBuffer context, int threadId,
			int value);

	public static native void threadReturnWide(ByteBuffer context,
			int threadId, long value);

	public static native int getThreadHandle(ByteBuffer context, int threadId);

	public static native void pushVMException(ByteBuffer context, int threadId,
			String name, String desc);

	public static native int allocateObject(ByteBuffer context, int classId);

	public static native int allocateArray(ByteBuffer context, int classId,
			int length);

	public static native int clone(ByteBuffer context, int handle);

	public static native int getLiteral(ByteBuffer context, String content);

	public static native int putString(ByteBuffer context, String content);

	public static native String getString(ByteBuffer context, int handle);

	public static native char[] getStringChars(ByteBuffer context, int handle);

	public static native int getClassOfHandle(ByteBuffer context, int handle);

	public static native int getArrayLength(ByteBuffer context, int handle);

	public static native boolean getFieldBoolean(ByteBuffer context,
			int handle, int fieldId);

	public static native byte getFieldByte(ByteBuffer context, int handle,
			int fieldId);

	public static native char getFieldChar(ByteBuffer context, int handle,
			int fieldId);

	public static native short getFieldShort(ByteBuffer context, int handle,
			int fieldId);

	public static native int getFieldHandle(ByteBuffer context, int handle,
			int fieldId);

	public static native int getFieldInt(ByteBuffer context, int handle,
			int fieldId);

	public static native float getFieldFloat(ByteBuffer context, int handle,
			int fieldId);

	public static native long getFieldLong(ByteBuffer context, int handle,
			int fieldId);

	public static native double getFieldDouble(ByteBuffer context, int handle,
			int fieldId);

	public static native void setFieldBoolean(ByteBuffer context, int handle,
			int fieldId, boolean value);

	public static native void setFieldByte(ByteBuffer context, int handle,
			int fieldId, byte value);

	public static native void setFieldChar(ByteBuffer context, int handle,
			int fieldId, char value);

	public static native void setFieldShort(ByteBuffer context, int handle,
			int fieldId, short value);

	public static native void setFieldHandle(ByteBuffer context, int handle,
			int fieldId, int value);

	public static native void setFieldInt(ByteBuffer context, int handle,
			int fieldId, int value);

	public static native void setFieldFloat(ByteBuffer context, int handle,
			int fieldId, float value);

	public static native void setFieldLong(ByteBuffer context, int handle,
			int fieldId, long value);

	public static native void setFieldDouble(ByteBuffer context, int handle,
			int fieldId, double value);

	public static native boolean getArrayBoolean(ByteBuffer context,
			int handle, int pos);

	public static native byte getArrayByte(ByteBuffer context, int handle,
			int pos);

	public static native char getArrayChar(ByteBuffer context, int handle,
			int pos);

	public static native short getArrayShort(ByteBuffer context, int handle,
			int pos);

	public static native int getArrayHandle(ByteBuffer context, int handle,
			int pos);

	public static native int getArrayInt(ByteBuffer context, int handle, int pos);

	public static native float getArrayFloat(ByteBuffer context, int handle,
			int pos);

	public static native long getArrayLong(ByteBuffer context, int handle,
			int pos);

	public static native double getArrayDouble(ByteBuffer context, int handle,
			int pos);

	public static native void setArrayBoolean(ByteBuffer context, int handle,
			int pos, boolean value);

	public static native void setArrayByte(ByteBuffer context, int handle,
			int pos, byte value);

	public static native void setArrayChar(ByteBuffer context, int handle,
			int pos, char value);

	public static native void setArrayShort(ByteBuffer context, int handle,
			int pos, short value);

	public static native void setArrayHandle(ByteBuffer context, int handle,
			int pos, int value);

	public static native void setArrayInt(ByteBuffer context, int handle,
			int pos, int value);

	public static native void setArrayFloat(ByteBuffer context, int handle,
			int pos, float value);

	public static native void setArrayLong(ByteBuffer context, int handle,
			int pos, long value);

	public static native void setArrayDouble(ByteBuffer context, int handle,
			int pos, double value);

	public static native boolean getStaticBoolean(ByteBuffer context,
			int fieldId);

	public static native byte getStaticByte(ByteBuffer context, int fieldId);

	public static native char getStaticChar(ByteBuffer context, int fieldId);

	public static native short getStaticShort(ByteBuffer context, int fieldId);

	public static native int getStaticHandle(ByteBuffer context, int fieldId);

	public static native int getStaticInt(ByteBuffer context, int fieldId);

	public static native float getStaticFloat(ByteBuffer context, int fieldId);

	public static native long getStaticLong(ByteBuffer context, int fieldId);

	public static native double getStaticDouble(ByteBuffer context, int fieldId);

	public static native void setStaticBoolean(ByteBuffer context, int fieldId,
			boolean value);

	public static native void setStaticByte(ByteBuffer context, int fieldId,
			byte value);

	public static native void setStaticChar(ByteBuffer context, int fieldId,
			char value);

	public static native void setStaticShort(ByteBuffer context, int fieldId,
			short value);

	public static native void setStaticHandle(ByteBuffer context, int fieldId,
			int value);

	public static native void setStaticInt(ByteBuffer context, int fieldId,
			int value);

	public static native void setStaticFloat(ByteBuffer context, int fieldId,
			float value);

	public static native void setStaticLong(ByteBuffer context, int fieldId,
			long value);

	public static native void setStaticDouble(ByteBuffer context, int fieldId,
			double value);

	public static native void fillArrayBoolean(ByteBuffer context, int handle,
			int dstPos, boolean[] src, int srcPos, int len);

	public static native void fillArrayByte(ByteBuffer context, int handle,
			int dstPos, byte[] src, int srcPos, int len);

	public static native void fillArrayChar(ByteBuffer context, int handle,
			int dstPos, char[] src, int srcPos, int len);

	public static native void fillArrayShort(ByteBuffer context, int handle,
			int dstPos, short[] src, int srcPos, int len);

	public static native void fillArrayInt(ByteBuffer context, int handle,
			int dstPos, int[] src, int srcPos, int len);

	public static native void fillArrayHandle(ByteBuffer context, int handle,
			int dstPos, int[] src, int srcPos, int len);

	public static native void fillArrayFloat(ByteBuffer context, int handle,
			int dstPos, float[] src, int srcPos, int len);

	public static native void fillArrayLong(ByteBuffer context, int handle,
			int dstPos, long[] src, int srcPos, int len);

	public static native void fillArrayDouble(ByteBuffer context, int handle,
			int dstPos, double[] src, int srcPos, int len);

	public static native void fetchArrayBoolean(ByteBuffer context,
			boolean[] dst, int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayByteBuffer(ByteBuffer context,
			ByteBuffer dst, int handle, int srcPos, int len);

	public static native void fetchArrayByte(ByteBuffer context, byte[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayChar(ByteBuffer context, char[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayShort(ByteBuffer context, short[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayInt(ByteBuffer context, int[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayHandle(ByteBuffer context, int[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayFloat(ByteBuffer context, float[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayLong(ByteBuffer context, long[] dst,
			int dstPos, int srcHandle, int srcPos, int len);

	public static native void fetchArrayDouble(ByteBuffer context,
			double[] dst, int dstPos, int srcHandle, int srcPos, int len);

	private static native void destroyContext(ByteBuffer context);

	public static InputStream getInputStream(ByteBuffer context, String name) {
		FYContext jcontext = contextMap.get(context);
		InputStream is = null;
		for (IToolkit toolkit : jcontext.getToolkits()) {
			is = toolkit.getResourceByClassName(name);
			if (is != null) {
				break;
			}
		}
		return is;
	}

	public static native void unregisterNativeHandler(ByteBuffer context,
			String uniqueName);

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
//			logD0("\t");
			logD0(ste.toString());
			logD0("\n");
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
//			logI0("\t");
			logI0(ste.toString());
			logI0("\n");
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
//			logW0("\t");
			logW0(ste.toString());
			logW0("\n");
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
//			logE0("\t");
			logE0(ste.toString());
			logE0("\n");
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
