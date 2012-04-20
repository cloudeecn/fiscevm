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
/**
 * @author Evgueni Brevnov, Serguei S. Zapreyev, Alexey V. Varlamov
 */

package java.lang.reflect;

import java.lang.annotation.Annotation;

/**
 * @com.intel.drl.spec_ref
 */
public final class Method extends AccessibleObject implements Member,
		GenericDeclaration {

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native boolean isBridge();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native boolean isVarArgs();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Annotation[][] getParameterAnnotations() {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public Annotation[] getDeclaredAnnotations() {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	@SuppressWarnings("unchecked")
	public <A extends Annotation> A getAnnotation(Class<A> annotationClass) {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Type[] getGenericExceptionTypes()
			throws GenericSignatureFormatError, TypeNotPresentException,
			MalformedParameterizedTypeException {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Type[] getGenericParameterTypes()
			throws GenericSignatureFormatError, TypeNotPresentException,
			MalformedParameterizedTypeException {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Type getGenericReturnType() throws GenericSignatureFormatError,
			TypeNotPresentException, MalformedParameterizedTypeException {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@SuppressWarnings("unchecked")
	public TypeVariable<Method>[] getTypeParameters()
			throws GenericSignatureFormatError {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String toGenericString() {
		return getUniqueName();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native boolean isSynthetic();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?> getDeclaringClass();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?>[] getExceptionTypes();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native int getModifiers();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native String getName();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?>[] getParameterTypes();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?> getReturnType();

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public int hashCode() {
		return getDeclaringClass().getName().hashCode() ^ getName().hashCode();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Object invoke(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException {
		try {
			Class<?> rt = getReturnType();
			if (rt.isPrimitive()) {
				if (rt == Boolean.TYPE) {
					return invokeZ(obj, args);
				} else if (rt == Byte.TYPE) {
					return invokeB(obj, args);
				} else if (rt == Short.TYPE) {
					return invokeS(obj, args);
				} else if (rt == Character.TYPE) {
					return invokeC(obj, args);
				} else if (rt == Integer.TYPE) {
					return invokeI(obj, args);
				} else if (rt == Float.TYPE) {
					return invokeF(obj, args);
				} else if (rt == Long.TYPE) {
					return invokeJ(obj, args);
				} else if (rt == Double.TYPE) {
					return invokeD(obj, args);
				} else {
					throw new IllegalArgumentException(
							"Illegal primitive type " + rt);
				}
			} else {
				return invokeL(obj, args);
			}
		} catch (RuntimeException e) {
			throw e;
		} catch (Throwable t) {
			throw new InvocationTargetException(t);
		}

	}

	private native boolean invokeZ(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native byte invokeB(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native short invokeS(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native char invokeC(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native int invokeI(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native float invokeF(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native long invokeJ(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native double invokeD(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	private native Object invokeL(Object obj, Object... args)
			throws IllegalAccessException, IllegalArgumentException,
			InvocationTargetException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public String toString() {
		return getUniqueName();
	}

	/* NON API SECTION */

	native String getUniqueName();
}
