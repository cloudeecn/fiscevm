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

/**
 * @author Cloudee
 *
 */
public final class StringUtils {

	public static String format(String format, String... args) {
		throw new UnsupportedOperationException();
	}

	/**
	 * @param codePoint
	 * @return
	 */
	public static int toUpperCase(int codePoint) {
		return (codePoint >= 'a' && codePoint <= 'z') ? codePoint - 'a' + 'A'
				: codePoint;
	}

	/**
	 * @param codePoint
	 * @return
	 */
	public static int toTitleCase(int codePoint) {
		return toUpperCase(codePoint);
	}

	/**
	 * @param codePoint
	 * @return
	 */
	public static int toLowerCase(int codePoint) {
		return (codePoint >= 'A' && codePoint <= 'Z') ? codePoint - 'A' + 'a'
				: codePoint;
	}

	/**
	 * @param codePoint
	 * @return
	 */
	public static boolean isWhitespace(int codePoint) {
		return codePoint == ' ' || codePoint == '\t' || codePoint == '　';
	}

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
