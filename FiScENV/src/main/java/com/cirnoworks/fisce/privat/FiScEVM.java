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
package com.cirnoworks.fisce.privat;

import java.io.UnsupportedEncodingException;

public class FiScEVM {

	private static String defaultEncoding = "utf-8";

	public static native void throwOut(Throwable t);

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
}
