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
import java.util.Enumeration;

/**
 * Base class for all class loaders
 *
 * @author Evgueni Brevnov
 */
public final class ClassLoader {

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

}
