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

public final class Long extends Number implements Comparable {

	public static final long MIN_VALUE = 0x8000000000000000L;

	public static final long MAX_VALUE = 0x7fffffffffffffffL;

	public static final int MAX_CACHE = 256;

	public static final int CACHE_SHIFT = 128;

	final static Long[] cache = new Long[MAX_CACHE];

	static {
		for (int i = 0, max = MAX_CACHE; i < max; i++) {
			cache[i] = new Long((i - CACHE_SHIFT));
		}
	}

	public static Long valueOf(long b) {
		int pos = (int) (b + CACHE_SHIFT);
		if (pos >= 0 && pos < MAX_CACHE) {
			return cache[pos];
		} else {
			return new Long(b);
		}
	}

	public static String toString(long i, int radix) {
		if (radix < Character.MIN_RADIX || radix > Character.MAX_RADIX)
			radix = 10;
		if (radix == 10)
			return toString(i);
		char[] buf = new char[65];
		int charPos = 64;
		boolean negative = (i < 0);

		if (!negative) {
			i = -i;
		}

		while (i <= -radix) {
			buf[charPos--] = Integer.digits[(int) (-(i % radix))];
			i = i / radix;
		}
		buf[charPos] = Integer.digits[(int) (-i)];

		if (negative) {
			buf[--charPos] = '-';
		}

		return new String(buf, charPos, (65 - charPos));
	}

	public static String toHexString(long i) {
		return toUnsignedString(i, 4);
	}

	public static String toOctalString(long i) {
		return toUnsignedString(i, 3);
	}

	public static String toBinaryString(long i) {
		return toUnsignedString(i, 1);
	}

	private static String toUnsignedString(long i, int shift) {
		char[] buf = new char[64];
		int charPos = 64;
		int radix = 1 << shift;
		long mask = radix - 1;
		do {
			buf[--charPos] = Integer.digits[(int) (i & mask)];
			i >>>= shift;
		} while (i != 0);
		return new String(buf, charPos, (64 - charPos));
	}

	public static String toString(long i) {
		if (i == Long.MIN_VALUE)
			return "-9223372036854775808";
		char[] buf = new char[20];
		int charPos = getChars(i, buf);
		return new String(buf, charPos, (20 - charPos));
	}

	private static int getChars(long i, char[] buf) {
		long q;
		int r;
		int charPos = 20;
		char sign = 0;

		if (i < 0) {
			sign = '-';
			i = -i;
		}

		while (i > Integer.MAX_VALUE) {
			q = i / 100;
			// really: r = i - (q * 100);
			r = (int) (i - ((q << 6) + (q << 5) + (q << 2)));
			i = q;
			buf[--charPos] = Integer.DigitOnes[r];
			buf[--charPos] = Integer.DigitTens[r];
		}

		// Get 2 digits/iteration using ints
		int q2;
		int i2 = (int) i;
		while (i2 >= 65536) {
			q2 = i2 / 100;
			// really: r = i2 - (q * 100);
			r = i2 - ((q2 << 6) + (q2 << 5) + (q2 << 2));
			i2 = q2;
			buf[--charPos] = Integer.DigitOnes[r];
			buf[--charPos] = Integer.DigitTens[r];
		}

		// Fall thru to fast mode for smaller numbers
		// assert(i2 <= 65536, i2);
		for (;;) {
			q2 = (i2 * 52429) >>> (16 + 3);
			r = i2 - ((q2 << 3) + (q2 << 1)); // r = i2-(q2*10) ...
			buf[--charPos] = Integer.digits[r];
			i2 = q2;
			if (i2 == 0)
				break;
		}
		if (sign != 0) {
			buf[--charPos] = sign;
		}
		return charPos;
	}

	static void appendTo(long i, StringBuffer sb) {
		if (i == Long.MIN_VALUE) {
			sb.append("-9223372036854775808");
			return;
		}
		char[] buf = new char[20];
		int charPos = getChars(i, buf);
		sb.append(buf, charPos, (20 - charPos));
		return;
	}

	static void appendTo(long i, StringBuilder sb) {
		if (i == Long.MIN_VALUE) {
			sb.append("-9223372036854775808");
			return;
		}
		char[] buf = new char[20];
		int charPos = getChars(i, buf);
		sb.append(buf, charPos, (20 - charPos));
		return;
	}

	public static long parseLong(String s, int radix)
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

		long result = 0;
		boolean negative = false;
		int i = 0, max = s.length();
		long limit;
		long multmin;
		int digit;

		if (max > 0) {
			if (s.charAt(0) == '-') {
				negative = true;
				limit = Long.MIN_VALUE;
				i++;
			} else {
				limit = -Long.MAX_VALUE;
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

	public static long parseLong(String s) throws NumberFormatException {
		return parseLong(s, 10);
	}

	public static Long valueOf(String s, int radix)
			throws NumberFormatException {
		return new Long(parseLong(s, radix));
	}

	public static Long valueOf(String s) throws NumberFormatException {
		return new Long(parseLong(s, 10));
	}

	public static Long decode(String nm) throws NumberFormatException {
		int radix = 10;
		int index = 0;
		boolean negative = false;
		Long result;

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
			result = Long.valueOf(nm.substring(index), radix);
			result = negative ? new Long((long) -result.longValue()) : result;
		} catch (NumberFormatException e) {
			// If number is Long.MIN_VALUE, we'll end up here. The next line
			// handles this case, and causes any genuine format error to be
			// rethrown.
			String constant = negative ? new String("-" + nm.substring(index))
					: nm.substring(index);
			result = Long.valueOf(constant, radix);
		}
		return result;
	}

	private long value;

	public Long(long value) {
		this.value = value;
	}

	public Long(String s) throws NumberFormatException {
		this.value = parseLong(s, 10);
	}

	public byte byteValue() {
		return (byte) value;
	}

	public short shortValue() {
		return (short) value;
	}

	public int intValue() {
		return (int) value;
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
		return (int) (value ^ (value >>> 32));
	}

	public boolean equals(Object obj) {
		if (obj instanceof Long) {
			return value == ((Long) obj).longValue();
		}
		return false;
	}

	public static Long getLong(String nm) {
		return getLong(nm, null);
	}

	public static Long getLong(String nm, long val) {
		Long result = Long.getLong(nm, null);
		return (result == null) ? new Long(val) : result;
	}

	public static Long getLong(String nm, Long val) {
		String v = null;
		try {
			v = System.getProperty(nm);
		} catch (IllegalArgumentException e) {
		} catch (NullPointerException e) {
		}
		if (v != null) {
			try {
				return Long.decode(v);
			} catch (NumberFormatException e) {
			}
		}
		return val;
	}

	public int compareTo(Long anotherLong) {
		long thisVal = this.value;
		long anotherVal = anotherLong.value;
		return (thisVal < anotherVal ? -1 : (thisVal == anotherVal ? 0 : 1));
	}

	public int compareTo(Object o) {
		return compareTo((Long) o);
	}
}
