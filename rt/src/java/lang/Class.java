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

import java.io.InputStream;

import com.cirnoworks.fisce.privat.FiScEVM;
import com.cirnoworks.fisce.privat.ResourceInputStream;

import fisce.util.HashMap;

public final class Class<T> {

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

	@SuppressWarnings("unchecked")
	public T newInstance() throws InstantiationException,
			IllegalAccessException {
		return (T) newInstance0();
	}

	@SuppressWarnings("unchecked")
	public T newArray(int size) throws InstantiationException,
			IllegalAccessException {
		return (T) newInstance0(size);
	}

	private native Object newInstance0() throws InstantiationException,
			IllegalAccessException;

	private native Object[] newInstance0(int size)
			throws InstantiationException, IllegalAccessException;

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
	
	private final transient int classId;

	private transient String name;

	private native String getName0();

	public native Class getSuperclass();

	public native Class[] getInterfaces();

	public native Class getComponentType();

	public InputStream getResourceAsStream(String name) {
		return new ResourceInputStream(name);
	}

	private native Object invokeMethodHandleReturn0(String methodName,
			boolean isStatic, Object... params) throws Throwable;

	private Object invokeMethodHandleReturn(String methodName,
			boolean isStatic, Object... params)
			throws InvocationTargetException, NoSuchMethodException {
		try {
			Object o = invokeMethodHandleReturn0(methodName, isStatic, params);
			return o;
		} catch (InvocationTargetException e) {
			throw e;
		} catch (NoSuchMethodException e) {
			throw e;
		} catch (Throwable t) {
			t.printStackTrace();
			throw new InvocationTargetException(t);
		}
	}

	T[] enums;

	@SuppressWarnings("unchecked")
	private T[] getEnums() {
		if (enums == null) {
			try {
				enums = (T[]) invokeMethodHandleReturn("values.()[L"
						+ getName().replace('.', '/') + ";", true,
						(Object[]) null);
			} catch (InvocationTargetException e) {

				e.printStackTrace();
				FiScEVM.throwOut(e);
			} catch (NoSuchMethodException e) {

				e.printStackTrace();
				FiScEVM.throwOut(e);
			}
		}
		return enums;
	}

	public T[] getEnumConstants() {
		return getEnums().clone();
	}

	HashMap<String, T> enumConstantDirectory;

	HashMap<String, T> enumConstantDirectory() {
		if (enumConstantDirectory == null) {
			T[] universe = getEnums();
			if (universe == null)
				throw new IllegalArgumentException(getName()
						+ " is not an enum type");
			HashMap<String, T> m = new HashMap<String, T>(2 * universe.length);
			for (T constant : universe)
				m.put(((Enum) constant).name(), constant);
			enumConstantDirectory = m;
		}
		return enumConstantDirectory;
	}

	/**
	 * @return
	 */
	public String getCanonicalName() {
		return getName();
	}

}
