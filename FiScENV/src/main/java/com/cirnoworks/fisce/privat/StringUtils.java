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

/**
 * @author Cloudee
 * 
 */
public class StringUtils {

	public static native String toUpperCase(String from);

	public static native String toLowerCase(String from);

	public static native String format(String format, String... args);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native int toUpperCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native int toTitleCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native int toLowerCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isWhitespace(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isUpperCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isUnicodeIdentifierStart(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isUnicodeIdentifierPart(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isTitleCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isSpaceChar(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isLowerCase(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isLetterOrDigit(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isLetter(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isIdentifierIgnorable(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isDigit(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isDefined(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native boolean isMirrored(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native byte getDirectionality(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native int getType(int codePoint);

	/**
	 * @param codePoint
	 * @return
	 */
	public static native int getNumericValue(int codePoint);

	/**
	 * @param codePoint
	 * @param radix
	 * @return
	 */
	public static native int digit(int codePoint, int radix);
}
