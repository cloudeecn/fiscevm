/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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

import java.io.InputStream;

import com.cirnoworks.fisce.privat.ResourceInputStream;

public final class Class {

	private Class() {
	}

	public String toString() {
		return (isInterface() ? "interface " : (isPrimitive() ? "" : "class "))
				+ getName();
	}

	public static Class forName(String className) throws ClassNotFoundException {
		return forName0(className, true);
	}

	public static Class forName(String name, boolean initialize, Object dummy)
			throws ClassNotFoundException {
		return forName0(name, initialize);
	}

	private static native Class forName0(String name, boolean initialize)
			throws ClassNotFoundException;

	public Object newInstance() throws InstantiationException,
			IllegalAccessException {
		return newInstance0();
	}

	private native Object newInstance0() throws InstantiationException,
			IllegalAccessException;

	public native boolean isInstance(Object obj);

	public native boolean isAssignableFrom(Class cls);

	public native boolean isInterface();

	public native boolean isArray();

	public native boolean isPrimitive();

	public String getName() {
		if (name == null)
			name = getName0();
		return name;
	}

	private transient String name;

	private native String getName0();

	public native Class getSuperclass();

	public native Class[] getInterfaces();

	public native Class getComponentType();

	public InputStream getResourceAsStream(String name) {
		return new ResourceInputStream(name);
	}

}
