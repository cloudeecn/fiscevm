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

public final class Integer extends Number implements Comparable {

	public static final int MIN_VALUE = 0x80000000;

	public static final int MAX_VALUE = 0x7fffffff;

	public static final int MAX_CACHE = 256;

	public static final int CACHE_SHIFT = 128;

	final static Integer[] cache = new Integer[MAX_CACHE];

	static {
		for (int i = 0, max = MAX_CACHE; i < max; i++) {
			cache[i] = new Integer(i - CACHE_SHIFT);
		}
	}

	/**
	 * All possible chars for representing a number as a String
	 */
	final static char[] digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
			'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
			'z' };

	public static String toString(int i, int radix) {

		if (radix < Character.MIN_RADIX || radix > Character.MAX_RADIX)
			radix = 10;

		/* Use the faster version */
		if (radix == 10) {
			return toString(i);
		}

		char buf[] = new char[33];
		boolean negative = (i < 0);
		int charPos = 32;

		if (!negative) {
			i = -i;
		}

		while (i <= -radix) {
			buf[charPos--] = digits[-(i % radix)];
			i = i / radix;
		}
		buf[charPos] = digits[-i];

		if (negative) {
			buf[--charPos] = '-';
		}

		return new String(buf, charPos, (33 - charPos));
	}

	public static String toHexString(int i) {
		return toUnsignedString(i, 4);
	}

	public static String toOctalString(int i) {
		return toUnsignedString(i, 3);
	}

	public static String toBinaryString(int i) {
		return toUnsignedString(i, 1);
	}

	private static String toUnsignedString(int i, int shift) {
		char[] buf = new char[32];
		int charPos = 32;
		int radix = 1 << shift;
		int mask = radix - 1;
		do {
			buf[--charPos] = digits[i & mask];
			i >>>= shift;
		} while (i != 0);

		return new String(buf, charPos, (32 - charPos));
	}

	final static char[] DigitTens = { '0', '0', '0', '0', '0', '0', '0', '0',
			'0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '2',
			'2', '2', '2', '2', '2', '2', '2', '2', '2', '3', '3', '3', '3',
			'3', '3', '3', '3', '3', '3', '4', '4', '4', '4', '4', '4', '4',
			'4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
			'6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '7', '7', '7',
			'7', '7', '7', '7', '7', '7', '7', '8', '8', '8', '8', '8', '8',
			'8', '8', '8', '8', '9', '9', '9', '9', '9', '9', '9', '9', '9',
			'9', };

	final static char[] DigitOnes = { '0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
			'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6',
			'7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2',
			'3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', };

	public static String toString(int i) {
		switch (i) {
		case Integer.MIN_VALUE:
			return "-2147483648";
		case -3:
			return "-3";
		case -2:
			return "-2";
		case -1:
			return "-1";
		case 0:
			return "0";
		case 1:
			return "1";
		case 2:
			return "2";
		case 3:
			return "3";
		case 4:
			return "4";
		case 5:
			return "5";
		case 6:
			return "6";
		case 7:
			return "7";
		case 8:
			return "8";
		case 9:
			return "9";
		case 10:
			return "10";
		}
		char[] buf = new char[12];
		int charPos = getChars(i, buf);
		return new String(buf, charPos, 12 - charPos);
	}

	private static int getChars(int i, char[] buf) {
		int q, r;
		int charPos = 12;
		char sign = 0;

		if (i < 0) {
			sign = '-';
			i = -i;
		}

		while (i >= 65536) {
			q = i / 100;
			r = i - ((q << 6) + (q << 5) + (q << 2));
			i = q;
			buf[--charPos] = DigitOnes[r];
			buf[--charPos] = DigitTens[r];
		}

		for (;;) {
			q = (i * 52429) >>> (16 + 3);
			r = i - ((q << 3) + (q << 1));
			buf[--charPos] = digits[r];
			i = q;
			if (i == 0)
				break;
		}
		if (sign != 0) {
			buf[--charPos] = sign;
		}
		return charPos;
	}

	static void appendTo(int i, StringBuffer sb) {
		switch (i) {
		case Integer.MIN_VALUE:
			sb.append("-2147483648");
			return;
		case -3:
			sb.append("-3");
			return;
		case -2:
			sb.append("-2");
			return;
		case -1:
			sb.append("-1");
			return;
		case 0:
			sb.append("0");
			return;
		case 1:
			sb.append("1");
			return;
		case 2:
			sb.append("2");
			return;
		case 3:
			sb.append("3");
			return;
		case 4:
			sb.append("4");
			return;
		case 5:
			sb.append("5");
			return;
		case 6:
			sb.append("6");
			return;
		case 7:
			sb.append("7");
			return;
		case 8:
			sb.append("8");
			return;
		case 9:
			sb.append("9");
			return;
		case 10:
			sb.append("10");
			return;
		}
		char[] buf = new char[12];
		int charPos = getChars(i, buf);
		sb.append(buf, charPos, 12 - charPos);
	}

	static void appendTo(int i, StringBuilder sb) {
		switch (i) {
		case Integer.MIN_VALUE:
			sb.append("-2147483648");
			return;
		case -3:
			sb.append("-3");
			return;
		case -2:
			sb.append("-2");
			return;
		case -1:
			sb.append("-1");
			return;
		case 0:
			sb.append("0");
			return;
		case 1:
			sb.append("1");
			return;
		case 2:
			sb.append("2");
			return;
		case 3:
			sb.append("3");
			return;
		case 4:
			sb.append("4");
			return;
		case 5:
			sb.append("5");
			return;
		case 6:
			sb.append("6");
			return;
		case 7:
			sb.append("7");
			return;
		case 8:
			sb.append("8");
			return;
		case 9:
			sb.append("9");
			return;
		case 10:
			sb.append("10");
			return;
		}
		char[] buf = new char[12];
		int charPos = getChars(i, buf);
		sb.append(buf, charPos, 12 - charPos);
	}

	public static int parseInt(String s, int radix)
			throws NumberFormatException {
		if (s == null) {
			throw new NumberFormatException("null");
		}

		if (radix < Character.MIN_RADIX) {
			throw new NumberFormatException("radix " + radix
					+ " less than Character.MIN_RADIX");
		}

		if (radix > Character.MAX_RADIX) {
			throw new NumberFormatException("radix " + radix
					+ " greater than Character.MAX_RADIX");
		}

		int result = 0;
		boolean negative = false;
		int i = 0, max = s.length();
		int limit;
		int multmin;
		int digit;

		if (max > 0) {
			if (s.charAt(0) == '-') {
				negative = true;
				limit = Integer.MIN_VALUE;
				i++;
			} else {
				limit = -Integer.MAX_VALUE;
			}
			multmin = limit / radix;
			if (i < max) {
				digit = Character.digit(s.charAt(i++), radix);
				if (digit < 0) {
					throw NumberFormatException.forInputString(s);
				} else {
					result = -digit;
				}
			}
			while (i < max) {
				// Accumulating negatively avoids surprises near MAX_VALUE
				digit = Character.digit(s.charAt(i++), radix);
				if (digit < 0) {
					throw NumberFormatException.forInputString(s);
				}
				if (result < multmin) {
					throw NumberFormatException.forInputString(s);
				}
				result *= radix;
				if (result < limit + digit) {
					throw NumberFormatException.forInputString(s);
				}
				result -= digit;
			}
		} else {
			throw NumberFormatException.forInputString(s);
		}
		if (negative) {
			if (i > 1) {
				return result;
			} else { /* Only got "-" */
				throw NumberFormatException.forInputString(s);
			}
		} else {
			return -result;
		}
	}

	public static int parseInt(String s) throws NumberFormatException {
		return parseInt(s, 10);
	}

	public static Integer valueOf(String s, int radix)
			throws NumberFormatException {
		return new Integer(parseInt(s, radix));
	}

	public static Integer valueOf(String s) throws NumberFormatException {
		return new Integer(parseInt(s, 10));
	}

	public static Integer valueOf(int i) {
		int pos = i + CACHE_SHIFT;
		if (pos >= 0 && pos < MAX_CACHE) {
			return cache[pos];
		} else {
			return new Integer(i);
		}
	}

	private int value;

	public Integer(int value) {
		this.value = value;
	}

	public Integer(String s) throws NumberFormatException {
		this.value = parseInt(s, 10);
	}

	public byte byteValue() {
		return (byte) value;
	}

	public short shortValue() {
		return (short) value;
	}

	public int intValue() {
		return value;
	}

	public long longValue() {
		return (long) value;
	}

	public float floatValue() {
		return (float) value;
	}

	public double doubleValue() {
		return (double) value;
	}

	public String toString() {
		return String.valueOf(value);
	}

	public int hashCode() {
		return value;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Integer) {
			return value == ((Integer) obj).intValue();
		}
		return false;
	}

	public static Integer getInteger(String nm) {
		return getInteger(nm, null);
	}

	public static Integer getInteger(String nm, int val) {
		Integer result = getInteger(nm, null);
		return (result == null) ? new Integer(val) : result;
	}

	public static Integer getInteger(String nm, Integer val) {
		String v = null;
		try {
			v = System.getProperty(nm);
		} catch (IllegalArgumentException e) {
		} catch (NullPointerException e) {
		}
		if (v != null) {
			try {
				return Integer.decode(v);
			} catch (NumberFormatException e) {
			}
		}
		return val;
	}

	public static Integer decode(String nm) throws NumberFormatException {
		int radix = 10;
		int index = 0;
		boolean negative = false;
		Integer result;

		// Handle minus sign, if present
		if (nm.startsWith("-")) {
			negative = true;
			index++;
		}

		// Handle radix specifier, if present
		if (nm.startsWith("0x", index) || nm.startsWith("0X", index)) {
			index += 2;
			radix = 16;
		} else if (nm.startsWith("#", index)) {
			index++;
			radix = 16;
		} else if (nm.startsWith("0", index) && nm.length() > 1 + index) {
			index++;
			radix = 8;
		}

		if (nm.startsWith("-", index))
			throw new NumberFormatException("Negative sign in wrong position");

		try {
			result = Integer.valueOf(nm.substring(index), radix);
			result = negative ? new Integer(-result.intValue()) : result;
		} catch (NumberFormatException e) {

			String constant = negative ? new String("-" + nm.substring(index))
					: nm.substring(index);
			result = Integer.valueOf(constant, radix);
		}
		return result;
	}

	public int compareTo(Integer anotherInteger) {
		int thisVal = this.value;
		int anotherVal = anotherInteger.value;
		return (thisVal < anotherVal ? -1 : (thisVal == anotherVal ? 0 : 1));
	}

	public int compareTo(Object o) {
		return compareTo((Integer) o);
	}

}
