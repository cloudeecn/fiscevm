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

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.security.ProtectionDomain;
import java.util.Enumeration;

/**
 * Base class for all class loaders
 *
 * @author Evgueni Brevnov
 */
public class ClassLoader {

	/**
	 * system default class loader. It is initialized while
	 * getSystemClassLoader(..) method is executing.
	 */
	private static ClassLoader systemClassLoader = new ClassLoader();

	protected ClassLoader() {
	}

	public static ClassLoader getSystemClassLoader() {
		return systemClassLoader;
	}

	public static URL getSystemResource(String name) {
		return getSystemClassLoader().getResource(name);
	}

	public static InputStream getSystemResourceAsStream(String name) {
		return getSystemClassLoader().getResourceAsStream(name);
	}

	public static Enumeration<URL> getSystemResources(String name)
			throws IOException {
		return getSystemClassLoader().getResources(name);
	}

	public void clearAssertionStatus() {
	}

	public final ClassLoader getParent() {
		return null;
	}

	public URL getResource(String name) {
		return null;
	}

	public InputStream getResourceAsStream(String name) {
		return getClass().getResourceAsStream(name);
	}

	public Enumeration<URL> getResources(String name) throws IOException {
		return null;
	}

	public Class<?> loadClass(String name) throws ClassNotFoundException {
		return Class.forName(name);
	}

	public void setClassAssertionStatus(String name, boolean flag) {
	}

	public void setDefaultAssertionStatus(boolean flag) {
	}

	/**
	 * Empty string is used to denote default package.
	 */
	public void setPackageAssertionStatus(String name, boolean flag) {
	}

	/*
	 * NON API SECTION
	 */

	final boolean isSameOrAncestor(ClassLoader loader) {
		if (this == loader) {
			return true;
		}
		return false;
	}

	/**
	 * Constructs a new instance of this class with the specified class loader
	 * as its parent.
	 *
	 * @param parentLoader
	 *            The {@code ClassLoader} to use as the new class loader's
	 *            parent.
	 * @throws SecurityException
	 *             if a security manager exists and it does not allow the
	 *             creation of new a new {@code ClassLoader}.
	 */
	protected ClassLoader(ClassLoader parentLoader) {
		super();
	}

	/**
	 * Constructs a new class from an array of bytes containing a class
	 * definition in class file format.
	 *
	 * @param classRep
	 *            the memory image of a class file.
	 * @param offset
	 *            the offset into {@code classRep}.
	 * @param length
	 *            the length of the class file.
	 * @return the {@code Class} object created from the specified subset of
	 *         data in {@code classRep}.
	 * @throws ClassFormatError
	 *             if {@code classRep} does not contain a valid class.
	 * @throws IndexOutOfBoundsException
	 *             if {@code offset < 0}, {@code length < 0} or if
	 *             {@code offset + length} is greater than the length of
	 *             {@code classRep}.
	 * @deprecated Use {@link #defineClass(String, byte[], int, int)}
	 */
	@Deprecated
	protected final Class<?> defineClass(byte[] classRep, int offset, int length)
			throws ClassFormatError {
		return defineClass(null, classRep, offset, length);
	}

	/**
	 * Constructs a new class from an array of bytes containing a class
	 * definition in class file format.
	 *
	 * @param className
	 *            the expected name of the new class, may be {@code null} if not
	 *            known.
	 * @param classRep
	 *            the memory image of a class file.
	 * @param offset
	 *            the offset into {@code classRep}.
	 * @param length
	 *            the length of the class file.
	 * @return the {@code Class} object created from the specified subset of
	 *         data in {@code classRep}.
	 * @throws ClassFormatError
	 *             if {@code classRep} does not contain a valid class.
	 * @throws IndexOutOfBoundsException
	 *             if {@code offset < 0}, {@code length < 0} or if
	 *             {@code offset + length} is greater than the length of
	 *             {@code classRep}.
	 */
	protected final Class<?> defineClass(String className, byte[] classRep,
			int offset, int length) throws ClassFormatError {
		if (offset < 0 || length < 0 || offset + length > classRep.length) {
			throw new ArrayIndexOutOfBoundsException(offset + length);
		}
		return defineClassImpl(className, classRep, offset, length);
	}

	/**
	 * Constructs a new class from an array of bytes containing a class
	 * definition in class file format and assigns the specified protection
	 * domain to the new class. If the provided protection domain is
	 * {@code null} then a default protection domain is assigned to the class.
	 *
	 * @param className
	 *            the expected name of the new class, may be {@code null} if not
	 *            known.
	 * @param classRep
	 *            the memory image of a class file.
	 * @param offset
	 *            the offset into {@code classRep}.
	 * @param length
	 *            the length of the class file.
	 * @param protectionDomain
	 *            the protection domain to assign to the loaded class, may be
	 *            {@code null}.
	 * @return the {@code Class} object created from the specified subset of
	 *         data in {@code classRep}.
	 * @throws ClassFormatError
	 *             if {@code classRep} does not contain a valid class.
	 * @throws IndexOutOfBoundsException
	 *             if {@code offset < 0}, {@code length < 0} or if
	 *             {@code offset + length} is greater than the length of
	 *             {@code classRep}.
	 * @throws NoClassDefFoundError
	 *             if {@code className} is not equal to the name of the class
	 *             contained in {@code classRep}.
	 */
	protected final Class<?> defineClass(String className, byte[] classRep,
			int offset, int length, ProtectionDomain protectionDomain)
			throws java.lang.ClassFormatError {
		return defineClass(className, classRep, offset, length);
	}

	protected final native Class<?> defineClassImpl(String className,
			byte[] classRep, int offset, int length) throws ClassFormatError;
}
