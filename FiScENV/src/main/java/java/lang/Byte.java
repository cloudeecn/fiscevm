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

public final class Byte extends Number implements Comparable {

	public static final byte MIN_VALUE = -128;

	public static final byte MAX_VALUE = 127;

	public static final int MAX_CACHE = 256;

	public static final int CACHE_SHIFT = 128;

	final static Byte[] cache = new Byte[MAX_CACHE];

	static {
		for (int i = 0, max = MAX_CACHE; i < max; i++) {
			cache[i] = new Byte((byte) (i - CACHE_SHIFT));
		}
	}

	public static Byte valueOf(byte b) {
		return cache[b + CACHE_SHIFT];
	}

	public static String toString(byte b) {
		return Integer.toString((int) b, 10);
	}

	public static byte parseByte(String s) throws NumberFormatException {
		return parseByte(s, 10);
	}

	public static byte parseByte(String s, int radix)
			throws NumberFormatException {
		int i = Integer.parseInt(s, radix);
		if (i < MIN_VALUE || i > MAX_VALUE)
			throw new NumberFormatException("Value out of range. Value:\"" + s
					+ "\" Radix:" + radix);
		return (byte) i;
	}

	public static Byte valueOf(String s, int radix)
			throws NumberFormatException {
		return new Byte(parseByte(s, radix));
	}

	public static Byte valueOf(String s) throws NumberFormatException {
		return valueOf(s, 10);
	}

	public static Byte decode(String nm) throws NumberFormatException {
		int radix = 10;
		int index = 0;
		boolean negative = false;
		Byte result;

		if (nm.startsWith("-")) {
			negative = true;
			index++;
		}

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
			result = Byte.valueOf(nm.substring(index), radix);
			result = negative ? new Byte((byte) -result.byteValue()) : result;
		} catch (NumberFormatException e) {
			String constant = negative ? new String("-" + nm.substring(index))
					: nm.substring(index);
			result = Byte.valueOf(constant, radix);
		}
		return result;
	}

	private byte value;

	public Byte(byte value) {
		this.value = value;
	}

	public Byte(String s) throws NumberFormatException {
		this.value = parseByte(s, 10);
	}

	public byte byteValue() {
		return value;
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
		return String.valueOf((int) value);
	}

	public int hashCode() {
		return (int) value;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Byte) {
			return value == ((Byte) obj).byteValue();
		}
		return false;
	}

	public int compareTo(Byte anotherByte) {
		return this.value - anotherByte.value;
	}

	public int compareTo(Object o) {
		return compareTo((Byte) o);
	}

}
