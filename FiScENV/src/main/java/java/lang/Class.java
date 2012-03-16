/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

package java.lang;

import java.io.InputStream;
import java.io.Serializable;
import java.lang.annotation.Annotation;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.GenericDeclaration;
import java.lang.reflect.GenericSignatureFormatError;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.MalformedParameterizedTypeException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;

import com.cirnoworks.fisce.privat.FiScEVM;
import com.cirnoworks.fisce.privat.ResourceInputStream;

/**
 * Runtime representation of a class
 *
 * @author Evgueni Brevnov, Serguei S. Zapreyev, Alexey V. Varlamov
 */
public final class Class<T> implements Serializable, AnnotatedElement,
		GenericDeclaration, Type {

	private static native Class<?> forName0(String name, boolean initialize)
			throws ClassNotFoundException;

	private native Object newInstance0() throws InstantiationException,
			IllegalAccessException;

	public native boolean isInstance(Object obj);

	public native boolean isAssignableFrom(Class<?> cls);

	public native boolean isInterface();

	public native boolean isArray();

	public native boolean isPrimitive();

	private native String getName0();

	public native Class<?> getSuperclass();

	public native Class[] getInterfaces();

	public native Class<?> getComponentType();

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

	private static final long serialVersionUID = 3206093459760846163L;

	private static final Class<Cloneable> CLONEABLE_CLASS = Cloneable.class;
	@SuppressWarnings("unchecked")
	private static final Class<Enum> ENUM_CLASS = Enum.class;
	private static final Class<Object> OBJECT_CLASS = Object.class;
	private static final Class<Serializable> SERIALIZABLE_CLASS = Serializable.class;

	public static Class<?> forName(String name) throws ClassNotFoundException {
		return forName0(name, true);
	}

	public static Class<?> forName(String name, boolean initialize,
			ClassLoader classLoader) throws ClassNotFoundException {
		return forName0(name, initialize);
	}

	/**
	 * Answers whether the arrays are equal
	 */
	static boolean isTypeMatches(Class<?>[] t1, Class<?>[] t2) {
		if (t1 == null) {
			return t2 == null || t2.length == 0;
		}
		if (t2 == null) {
			return t1 == null || t1.length == 0;
		}
		if (t1.length != t2.length) {
			return false;
		}
		for (int i = 0; i < t2.length; i++) {
			if (t1[i] != t2[i]) {
				return false;
			}
		}
		return true;
	}

	private static Method findMatchingMethod(Method[] methods,
			String methodName, Class<?>[] argumentTypes)
			throws NoSuchMethodException {
		Method matcher = null;
		for (int i = 0; i < methods.length; i++) {
			Method m = methods[i];
			if (matcher != null
					&& matcher.getDeclaringClass() != m.getDeclaringClass()) {
				return matcher;
			}
			try {
				if (methodName.equals(m.getName())
						&& isTypeMatches(argumentTypes, m.getParameterTypes())
						&& (matcher == null || matcher.getReturnType()
								.isAssignableFrom(m.getReturnType()))) {
					matcher = m;
				}
			} catch (LinkageError ignore) {
			}
		}
		if (matcher == null) {
			throw new NoSuchMethodException(methodName.toString()
					+ printMethodSignature(argumentTypes));
		}
		return matcher;
	}

	private static String getParentName(String name) {
		int dotPosition = name.lastIndexOf('.');
		return dotPosition == -1 ? "" : name.substring(0, dotPosition);
	}

	private static String printMethodSignature(Class<?>[] types) {
		StringBuffer sb = new StringBuffer("(");
		if (types != null && types.length > 0) {
			sb.append(types[0] != null ? types[0].getName() : "null");
			for (int i = 1; i < types.length; i++) {
				sb.append(", ");
				sb.append(types[i] != null ? types[i].getName() : "null");
			}
		}
		sb.append(")");
		return sb.toString();
	}

	/**
	 * Provides strong referencing between the classloader and it's defined
	 * classes. Intended for class unloading implementation.
	 *
	 * @see java.lang.ClassLoader#loadedClasses
	 */
	ClassLoader definingLoader;

	/** It is required for synchronization in newInstance method. */
	private volatile boolean isDefaultConstructorInitialized;

	/**
	 * Only VM can instantiate this class.
	 */
	private Class() {
		throw new RuntimeException("Should not be called!");
	}

	public boolean desiredAssertionStatus() {
		return false;
	}

	/**
	 * Note: We don't check member access permission for each super class. Java
	 * 1.5 API specification doesn't require this check.
	 */
	public Class[] getClasses() {
		Class<?> clss = this;
		ArrayList<Class<?>> classes = null;
		while (clss != null) {
			Class<?>[] declared = clss.getDeclaredClasses();
			if (declared.length != 0) {
				if (classes == null) {
					classes = new ArrayList<Class<?>>();
				}
				for (Class<?> c : declared) {
					if (Modifier.isPublic(c.getModifiers())) {
						classes.add(c);
					}
				}
			}
			clss = clss.getSuperclass();
		}
		if (classes == null) {
			return new Class[0];
		} else {
			return classes.toArray(new Class[classes.size()]);
		}
	}

	public ClassLoader getClassLoader() {
		return ClassLoader.getSystemClassLoader();
	}

	private native Constructor<T>[] getPublicConstructors0();

	private native Constructor<T>[] getConstructors0();

	public Constructor<T> getConstructor(Class... argumentTypes)
			throws NoSuchMethodException {

		Constructor<T> ctors[] = getPublicConstructors0();
		for (int i = 0; i < ctors.length; i++) {
			Constructor<T> c = ctors[i];
			try {
				if (isTypeMatches(argumentTypes, c.getParameterTypes())) {
					return c;
				}
			} catch (LinkageError ignore) {
			}
		}
		throw new NoSuchMethodException(getName()
				+ printMethodSignature(argumentTypes));
	}

	public native Constructor[] getConstructors();

	public native Class[] getDeclaredClasses();

	public native Constructor<T> getDeclaredConstructor(Class... argumentTypes)
			throws NoSuchMethodException;

	public native Constructor[] getDeclaredConstructors();

	public native Field getDeclaredField(String fieldName)
			throws NoSuchFieldException;

	public native Field[] getDeclaredFields();

	public native Method getDeclaredMethod(String methodName,
			Class... argumentTypes) throws NoSuchMethodException;

	public native Method[] getDeclaredMethods();

	public Class<?> getDeclaringClass() {
		throw new RuntimeException(
				"Anonymous and Enclosing classes' reflection are not supported");
	}

	public native Field getField(String fieldName) throws NoSuchFieldException;

	public native Field[] getFields();

	public native Method getMethod(String methodName, Class... argumentTypes)
			throws NoSuchMethodException;

	public native Method[] getMethods();

	public native int getModifiers();

	private transient String name;

	public String getName() {
		if (name == null)
			name = getName0();
		return name;
	}

	public Package getPackage() {
		return null;
	}

	public Object getProtectionDomain() {
		return null;
	}

	public URL getResource(String resource) {
		return null;
	}

	public InputStream getResourceAsStream(String name) {
		return new ResourceInputStream(name);
	}

	public Object[] getSigners() {
		return null;
	}

	@SuppressWarnings("unchecked")
	public T newInstance() throws InstantiationException,
			IllegalAccessException {
		return (T) newInstance0();
	}

	@Override
	public String toString() {
		return isPrimitive() ? getName() : (isInterface() ? "interface "
				: "class ") + getName();
	}

	String getPackageName() {
		return "";
	}

	void setProtectionDomain(Object protectionDomain) {
	}

	private String getAbsoluteResource(String resource) {
		if (resource.startsWith("/")) {
			return resource.substring(1);
		}
		String pkgName = getPackageName();
		if (pkgName.length() > 0) {
			resource = pkgName.replace('.', '/') + '/' + resource;
		}
		return resource;
	}

	public Annotation[] getDeclaredAnnotations() {
		return null;
	}

	public Annotation[] getAnnotations() {
		return null;
	}

	@SuppressWarnings("unchecked")
	public <A extends Annotation> A getAnnotation(Class<A> annotationClass) {
		return null;
	}

	public boolean isAnnotationPresent(
			Class<? extends Annotation> annotationClass) {

		return false;
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

	public native boolean isEnum();

	public boolean isAnnotation() {
		// TODO Annotation is not loaded in this version
		return false;
	}

	@SuppressWarnings("unchecked")
	public native <U> Class<? extends U> asSubclass(Class<U> clazz)
			throws ClassCastException;

	@SuppressWarnings("unchecked")
	public native T cast(Object obj) throws ClassCastException;

	public TypeVariable<Class<T>>[] getTypeParameters()
			throws GenericSignatureFormatError {
		return null;
	}

	public Method getEnclosingMethod() {
		return null;
	}

	public Constructor<?> getEnclosingConstructor() {
		return null;
	}

	public Type[] getGenericInterfaces() throws GenericSignatureFormatError,
			TypeNotPresentException, MalformedParameterizedTypeException {
		return null;
	}

	public Type getGenericSuperclass() throws GenericSignatureFormatError,
			TypeNotPresentException, MalformedParameterizedTypeException {
		return null;
	}

	public Class<?> getEnclosingClass() {
		return null;
	}

	public boolean isMemberClass() {
		return false;
	}

	public boolean isLocalClass() {
		return true;
	}

	public boolean isAnonymousClass() {
		return false;
	}

	public boolean isSynthetic() {
		return false;
	}

	public String getCanonicalName() {
		if (isLocalClass() || isAnonymousClass()) {
			return null;
		}
		if (isArray()) {
			String res = getComponentType().getCanonicalName();
			return res != null ? res + "[]" : null;
		}

		StringBuffer sb = new StringBuffer(getPackageName());
		ArrayList<String> sympleNames = new ArrayList<String>();
		Class<?> clss = this;
		while ((clss = clss.getDeclaringClass()) != null) {
			if (clss.isLocalClass() || clss.isAnonymousClass()) {
				return null;
			}
			sympleNames.add(clss.getSimpleName());
		}
		if (sb.length() > 0) {
			sb.append(".");
		}
		for (int i = sympleNames.size() - 1; i > -1; i--) {
			sb.append(sympleNames.get(i)).append(".");
		}
		sb.append(getSimpleName());

		return sb.toString();
	}

	public native String getSimpleName();

}
