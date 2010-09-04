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
package java.lang;

import com.cirnoworks.fisce.privat.FiScEVM;

public final class Float extends Number implements Comparable {

	public static final float POSITIVE_INFINITY = 1.0f / 0.0f;

	public static final float NEGATIVE_INFINITY = -1.0f / 0.0f;

	public static final float NaN = 0.0f / 0.0f;

	public static final float MAX_VALUE = 3.4028235e+38f;

	public static final float MIN_VALUE = 1.4e-45f;

	public static String toString(float f) {
		return FiScEVM.floatToString(f);
	}

	public static Float valueOf(String s) throws NumberFormatException {
		return new Float(FiScEVM.stringToFloat(s));
	}

	public static float parseFloat(String s) throws NumberFormatException {
		return FiScEVM.stringToFloat(s);
	}

	static public boolean isNaN(float v) {
		return (v != v);
	}

	static public boolean isInfinite(float v) {
		return (v == POSITIVE_INFINITY) || (v == NEGATIVE_INFINITY);
	}

	private float value;

	public Float(float value) {
		this.value = value;
	}

	public Float(double value) {
		this.value = (float) value;
	}

	public Float(String s) throws NumberFormatException {
		this(valueOf(s).floatValue());
	}

	public boolean isNaN() {
		return isNaN(value);
	}

	public boolean isInfinite() {
		return isInfinite(value);
	}

	public String toString() {
		return String.valueOf(value);
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
		return value;
	}

	public double doubleValue() {
		return (double) value;
	}

	public int hashCode() {
		return FiScEVM.getFloatRaw(value);
	}

	public boolean equals(Object obj) {
		return (obj instanceof Float)
				&& (FiScEVM.getFloatRaw(((Float) obj).value) == FiScEVM
						.getFloatRaw(value));
	}

	public int compareTo(Float anotherFloat) {
		return Float.compare(value, anotherFloat.value);
	}

	public int compareTo(Object o) {
		return compareTo((Float) o);
	}

	public static int compare(float f1, float f2) {
		if (f1 < f2)
			return -1;
		if (f1 > f2)
			return 1;

		int thisBits = FiScEVM.getFloatRaw(f1);
		int anotherBits = FiScEVM.getFloatRaw(f2);

		return (thisBits == anotherBits ? 0 : (thisBits < anotherBits ? -1 : 1));
	}

}
