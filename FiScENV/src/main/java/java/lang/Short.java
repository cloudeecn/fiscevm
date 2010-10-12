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

public final class Short extends Number implements Comparable {

	public static final short MIN_VALUE = -32768;

	public static final short MAX_VALUE = 32767;

	public static final int MAX_CACHE = 256;

	public static final int CACHE_SHIFT = 128;

	final static Short[] cache = new Short[MAX_CACHE];

	static {
		for (int i = 0, max = MAX_CACHE; i < max; i++) {
			cache[i] = new Short((short) (i - CACHE_SHIFT));
		}
	}

	public static Short valueOf(short b) {
		int pos = b + CACHE_SHIFT;
		if (pos >= 0 && pos < MAX_CACHE) {
			return cache[pos];
		} else {
			return new Short(b);
		}
	}

	public static String toString(short s) {
		return Integer.toString((int) s, 10);
	}

	public static short parseShort(String s) throws NumberFormatException {
		return parseShort(s, 10);
	}

	public static short parseShort(String s, int radix)
			throws NumberFormatException {
		int i = Integer.parseInt(s, radix);
		if (i < MIN_VALUE || i > MAX_VALUE)
			throw new NumberFormatException("Value out of range. Value:\"" + s
					+ "\" Radix:" + radix);
		return (short) i;
	}

	public static Short valueOf(String s, int radix)
			throws NumberFormatException {
		return new Short(parseShort(s, radix));
	}

	public static Short valueOf(String s) throws NumberFormatException {
		return valueOf(s, 10);
	}

	public static Short decode(String nm) throws NumberFormatException {
		int radix = 10;
		int index = 0;
		boolean negative = false;
		Short result;

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
			result = Short.valueOf(nm.substring(index), radix);
			result = negative ? new Short((short) -result.shortValue())
					: result;
		} catch (NumberFormatException e) {
			// If number is Short.MIN_VALUE, we'll end up here. The next line
			// handles this case, and causes any genuine format error to be
			// rethrown.
			String constant = negative ? new String("-" + nm.substring(index))
					: nm.substring(index);
			result = Short.valueOf(constant, radix);
		}
		return result;
	}

	private short value;

	public Short(short value) {
		this.value = value;
	}

	public Short(String s) throws NumberFormatException {
		this.value = parseShort(s, 10);
	}

	public byte byteValue() {
		return (byte) value;
	}

	public short shortValue() {
		return value;
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
		return String.valueOf((int) value);
	}

	public int hashCode() {
		return (int) value;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Short) {
			return value == ((Short) obj).shortValue();
		}
		return false;
	}

	public int compareTo(Short anotherShort) {
		return this.value - anotherShort.value;
	}

	public int compareTo(Object o) {
		return compareTo((Short) o);
	}
}
