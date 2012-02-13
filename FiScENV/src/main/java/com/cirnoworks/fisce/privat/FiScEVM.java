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
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.privat;

import java.io.UnsupportedEncodingException;

public class FiScEVM {

	private static String defaultEncoding = "utf-8";

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
