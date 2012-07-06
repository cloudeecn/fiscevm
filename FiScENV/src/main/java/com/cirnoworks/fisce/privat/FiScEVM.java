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

	private static String defaultEncoding = "utf-8";

	public static final PrintStream debug = new PrintStream(
			new DebugOutOutputStream());

	public static native void debugOut(String str);

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
}
