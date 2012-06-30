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

import java.net.URL;
import java.lang.reflect.AnnotatedElement;
import java.lang.annotation.Annotation;

/**
 * @com.intel.drl.spec_ref
 */
public class Package implements AnnotatedElement {

	/**
	 *
	 * @com.intel.drl.spec_ref
	 *
	 **/
	public Annotation[] getDeclaredAnnotations() {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 *
	 * @com.intel.drl.spec_ref
	 *
	 **/
	public Annotation[] getAnnotations() {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 *
	 * @com.intel.drl.spec_ref
	 *
	 **/
	@SuppressWarnings("unchecked")
	public <A extends Annotation> A getAnnotation(Class<A> annotationClass) {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 *
	 * @com.intel.drl.spec_ref
	 *
	 **/
	public boolean isAnnotationPresent(
			Class<? extends Annotation> annotationClass) {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public static Package getPackage(String name) {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public static Package[] getPackages() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getImplementationTitle() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getImplementationVendor() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getImplementationVersion() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getName() {
		return "";
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getSpecificationTitle() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getSpecificationVendor() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String getSpecificationVersion() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public int hashCode() {
		return getName().hashCode();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public boolean isCompatibleWith(String desiredVersion)
			throws NumberFormatException {

		return true;
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public boolean isSealed() {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public boolean isSealed(URL url) {
		throw new RuntimeException("Package in reflection not supported");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public String toString() {
		return super.toString();
	}

}
