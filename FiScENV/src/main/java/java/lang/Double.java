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

public final class Double extends Number implements Comparable {

	public static final double POSITIVE_INFINITY = 1.0 / 0.0;

	public static final double NEGATIVE_INFINITY = -1.0 / 0.0;

	public static final double NaN = 0.0d / 0.0;

	public static final double MAX_VALUE = 1.7976931348623157e+308;

	public static final double MIN_VALUE = 4.9e-324;

	public static String toString(double d) {
		return FiScEVM.doubleToString(d);
	}

	public static Double valueOf(String s) throws NumberFormatException {
		return new Double(FiScEVM.stringToDouble(s));
	}

	public static double parseDouble(String s) throws NumberFormatException {
		return FiScEVM.stringToDouble(s);
	}

	static public boolean isNaN(double v) {
		return (v != v);
	}

	static public boolean isInfinite(double v) {
		return (v == POSITIVE_INFINITY) || (v == NEGATIVE_INFINITY);
	}

	private double value;

	public Double(double value) {
		this.value = value;
	}

	public Double(String s) throws NumberFormatException {
		this(valueOf(s).doubleValue());
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
		return (float) value;
	}

	public double doubleValue() {
		return (double) value;
	}

	public int hashCode() {
		long bits = FiScEVM.getDoubleRaw(value);
		return (int) (bits ^ (bits >>> 32));
	}

	public boolean equals(Object obj) {
		return (obj instanceof Double)
				&& (FiScEVM.getDoubleRaw(value) == FiScEVM
						.getDoubleRaw(((Double) obj).doubleValue()));
	}

	public int compareTo(Double anotherDouble) {
		return Double.compare(value, anotherDouble.value);
	}

	public int compareTo(Object o) {
		return compareTo((Double) o);
	}

	public static int compare(double d1, double d2) {
		if (d1 < d2)
			return -1;
		if (d1 > d2)
			return 1;

		if (d1 == d2)
			return 0;

		if ((d1 == -0.0 && d2 == 0.0) || (d1 != NaN && d2 == NaN)) {
			return -1;
		} else {
			return 1;
		}
	}
}
