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

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @com.intel.drl.spec_ref
 */
public final class Constructor<T> extends AccessibleObject implements Member,
		GenericDeclaration {

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
	@SuppressWarnings("unchecked")
	public TypeVariable<Constructor<T>>[] getTypeParameters()
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
	public native Class<T> getDeclaringClass();

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
	@Override
	public int hashCode() {
		return getDeclaringClass().getName().hashCode();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public T newInstance(Object... args) throws InstantiationException,
			IllegalAccessException, IllegalArgumentException,
			InvocationTargetException {
		int length;
		Class<?>[] types = getParameterTypes();
		if ((length = args.length) != types.length) {
			throw new IllegalArgumentException("Parameters length mismatch");
		}
		for (int i = 0; i < length; i++) {
			args[i] = FiScEVM.wide(args[i], types[i]);
		}
		return newInstance0(args);
	}

	private native T newInstance0(Object... args) throws InstantiationException,
			IllegalAccessException, IllegalArgumentException,
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
