/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package com.cirnoworks.fisce.privat;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

public class FiScEVM {
	/* Access flags */
	public static final int ACC_ABSTRACT = 1024;
	public static final int ACC_FINAL = 16;
	public static final int ACC_INTERFACE = 512;
	public static final int ACC_NATIVE = 256;
	public static final int ACC_PRIVATE = 2;
	public static final int ACC_PROTECTED = 4;
	public static final int ACC_PUBLIC = 1;
	public static final int ACC_STATIC = 8;
	public static final int ACC_STRICT = 2048;
	public static final int ACC_SUPER = 32;
	public static final int ACC_SYNCHRONIZED = 32;
	public static final int ACC_TRANSIENT = 128;
	public static final int ACC_VOLATILE = 64;
	public static final int ACC_VARARGS = 128;
	public static final int ACC_BRIDGE = 64;
	/* Extended access flags */
	public static final int ACC_SYNTHETIC = 0x00001000;
	public static final int ACC_ANNOTATION = 0x00002000;
	public static final int ACC_ENUM = 0x00004000;
	public static final int ACC_CONSTRUCTOR = 0x00010000;
	public static final int ACC_CLINIT = 0x00020000;
	public static final int ACC_VERIFIED = 0x80000000;
	/* Log level */
	public static final int LOG_DEBUG = 0;
	public static final int LOG_INFO = 1;
	public static final int LOG_WARN = 2;
	public static final int LOG_ERROR = 3;

	private static String defaultEncoding = "utf-8";

	public static PrintStream debug = new PrintStream(
			new DebugOutOutputStream());

	public static native void logOut(int level, String str);

	public static void debugOut(String str) {
		logOut(LOG_DEBUG, str);
	}

	public static void infoOut(String str) {
		logOut(LOG_INFO, str);
	}

	public static void warnOut(String str) {
		logOut(LOG_WARN, str);
	}

	public static void errorOut(String str) {
		logOut(LOG_ERROR, str);
	}

	public static void throwOut(Throwable t) {
		throwOut(t, "");
	}

	public static native void throwOut(Throwable t, String message);

	public static native void exit(int code);

	public static native char[] decode(String encoding, byte[] src, int ofs,
			int len) throws UnsupportedEncodingException;

	public static native byte[] encode(String encoding, char[] src, int ofs,
			int len) throws UnsupportedEncodingException;

	public static char[] decode(byte[] src, int ofs, int len) {

		try {
			return decode(defaultEncoding, src, ofs, len);
		} catch (UnsupportedEncodingException e) {
			throwOut(e);
			return null;
		}
	}

	public static byte[] encode(char[] src, int ofs, int len) {
		try {
			return encode(defaultEncoding, src, ofs, len);
		} catch (UnsupportedEncodingException e) {
			throwOut(e);
			return null;
		}
	}

	public static native long getDoubleRaw(double d);

	public static native int getFloatRaw(float f);

	public static native double stringToDouble(String str)
			throws NumberFormatException;

	public static native String doubleToString(double d);

	public static native float stringToFloat(String str)
			throws NumberFormatException;

	public static native String floatToString(float d);

	public static <T> T newInstance(Class<T> clazz, Class<?>[] params,
			Object[] args) throws InstantiationException,
			IllegalAccessException {
		if (params.length != args.length) {
			throw new RuntimeException("Params and args must have same length"
					+ params.length + "/" + args.length);
		}
		return newInstance0(clazz, params, args);
	}

	@SuppressWarnings("unchecked")
	public static <T> T newArray(Class<T> clazz, int size)
			throws InstantiationException, IllegalAccessException {
		return (T) newInstance0(clazz, size);
	}

	public static <T> T[] newContentArray(Class<T> clazz, int size) {
		return (T[]) newArray0(clazz, size);
	}

	private static native <T> T newInstance0(Class<T> clazz, Class<?>[] params,
			Object[] args) throws InstantiationException,
			IllegalAccessException;

	private static native Object[] newArray0(Class<?> clazz, int size);

	private static native Object[] newInstance0(Class<?> clazz, int size)
			throws InstantiationException, IllegalAccessException;

	public static native void save();

	/**
	 * Store caller's params to an int array.
	 * 
	 * @param begin
	 *            the beginning of the params
	 * @param target
	 *            target array
	 * @param pos
	 *            begin pos of target array
	 * @param count
	 *            the count of caller's params
	 */
	public static native void storeParamsToArray(int begin, int[] target,
			int pos, int count);

	public static Object wide(Object from, Class<?> type) {
		if(from==null){
			return null;
		}
		Class<?> argType = from.getClass();
		if (type.isAssignableFrom(argType)) {
			return from;
		} else if (type == Boolean.TYPE || type == Boolean.class) {

			if (argType == Boolean.class) {
				return from;
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Byte.TYPE || type == Byte.class) {

			if (argType == Byte.class) {
				return from;
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Short.TYPE || type == Short.class) {

			if (argType == Short.class) {
				return from;
			}
			if (argType == Byte.class) {
				return ((Number) from).shortValue();
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Character.TYPE || type == Boolean.class) {

			if (argType == Character.class) {
				return from;
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Integer.TYPE || type == Integer.class) {
			if (argType == Integer.class) {
				return from;
			} else if (argType == Byte.TYPE || argType == Short.TYPE
					|| argType == Character.TYPE) {
				return ((Number) from).intValue();
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}
		} else if (type == Float.TYPE || type == Float.class) {
			if (argType == Float.class) {
				return from;
			} else if (argType == Byte.TYPE || argType == Short.TYPE
					|| argType == Character.TYPE || argType == Integer.TYPE
					|| argType == Long.TYPE) {
				return ((Number) from).floatValue();
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Long.TYPE || type == Long.class) {
			if (argType == Long.class) {
				return from;
			} else if (argType == Byte.TYPE || argType == Short.TYPE
					|| argType == Character.TYPE || argType == Integer.TYPE) {
				return ((Number) from).longValue();
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else if (type == Double.TYPE || type == Double.class) {
			if (argType == Double.class) {
				return from;
			} else if (Number.class.isAssignableFrom(argType)) {
				return ((Number) from).doubleValue();
			} else {
				throw new IllegalArgumentException("Can't wide from " + argType
						+ " to" + type);
			}

		} else {
			throw new IllegalArgumentException("Can't wide from " + argType
					+ " to" + type);
		}
	}
}
