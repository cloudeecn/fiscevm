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

public final class Boolean {

	public static final Boolean TRUE = new Boolean(true);

	public static final Boolean FALSE = new Boolean(false);

	private boolean value;

	public Boolean(boolean value) {
		this.value = value;
	}

	public Boolean(String s) {
		this(toBoolean(s));
	}

	public boolean booleanValue() {
		return value;
	}

	public static Boolean valueOf(boolean b) {
		return (b ? TRUE : FALSE);
	}

	public static Boolean valueOf(String s) {
		return toBoolean(s) ? TRUE : FALSE;
	}

	public static String toString(boolean b) {
		return b ? "true" : "false";
	}

	public String toString() {
		return value ? "true" : "false";
	}

	public int hashCode() {
		return value ? 1287346153 : -1324128739;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Boolean) {
			return value == ((Boolean) obj).booleanValue();
		}
		return false;
	}

	public static boolean getBoolean(String name) {
		boolean result = false;
		try {
			result = toBoolean(System.getProperty(name));
		} catch (IllegalArgumentException e) {
		} catch (NullPointerException e) {
		}
		return result;
	}

	private static boolean toBoolean(String name) {
		return ((name != null) && name.equalsIgnoreCase("true"));
	}
}
