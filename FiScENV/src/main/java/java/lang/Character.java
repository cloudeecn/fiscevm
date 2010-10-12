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
package java.lang;

public final class Character extends Object implements Comparable {

	public static final int MIN_RADIX = 2;

	public static final int MAX_RADIX = 36;

	public static final char MIN_VALUE = '\u0000';

	public static final char MAX_VALUE = '\uffff';

	private char value;

	public static final int MAX_CACHE = 256;

	// public static final int CACHE_SHIFT = 0;

	final static Character[] cache = new Character[MAX_CACHE];

	static {
		for (char i = 0, max = MAX_CACHE; i < max; i++) {
			cache[i] = new Character(i);
		}
	}

	public static Character valueOf(char b) {
		if (b >= 0 && b < MAX_CACHE) {
			return cache[b];
		} else {
			return new Character(b);
		}
	}

	public Character(char value) {
		this.value = value;
	}

	public char charValue() {
		return value;
	}

	public int hashCode() {
		return (int) value;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Character) {
			return value == ((Character) obj).charValue();
		}
		return false;
	}

	public String toString() {
		char buf[] = { value };
		return String.valueOf(buf);
	}

	public static String toString(char c) {
		return String.valueOf(c);
	}

	public static boolean isLowerCase(char ch) {
		return ch >= 'a' && ch <= 'z';
	}

	public static boolean isUpperCase(char ch) {
		return ch >= 'A' && ch <= 'Z';
	}

	public static boolean isDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	public static boolean isLetter(char ch) {
		return isUpperCase(ch) || isLowerCase(ch);
	}

	public static boolean isLetterOrDigit(char ch) {
		return isLetter(ch) || isDigit(ch);
	}

	public static char toLowerCase(char ch) {
		return (char) (isUpperCase(ch) ? ch - 'A' + 'a' : ch);
	}

	public static char toUpperCase(char ch) {
		return (char) (isLowerCase(ch) ? ch - 'a' + 'A' : ch);
	}

	public static int digit(char ch, int radix) {
		int ret = isDigit(ch) ? ch - '0' : toLowerCase(ch) - 'a' + 10;
		return ret < radix ? ret : -1;

	}

	public static int getNumericValue(char ch) {
		return digit(ch, 36);
	}

	public static boolean isSpaceChar(char ch) {
		return ch == ' ';
	}

	public static boolean isWhitespace(char ch) {
		return ch == ' ';
	}

	public static char forDigit(int digit, int radix) {
		if ((digit >= radix) || (digit < 0)) {
			return '\0';
		}
		if ((radix < MIN_RADIX) || (radix > MAX_RADIX)) {
			return '\0';
		}
		if (digit < 10) {
			return (char) ('0' + digit);
		}
		return (char) ('a' - 10 + digit);
	}

	public int compareTo(Character anotherCharacter) {
		return this.value - anotherCharacter.value;
	}

	public int compareTo(Object o) {
		return compareTo((Character) o);
	}

	static char toUpperCaseEx(char ch) {
		return toUpperCase(ch);
	}
}
