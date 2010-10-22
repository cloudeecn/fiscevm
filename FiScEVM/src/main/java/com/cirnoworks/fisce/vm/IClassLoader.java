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
package com.cirnoworks.fisce.vm;

import com.cirnoworks.fisce.vm.data.AbstractClass;

/**
 * 
 * @author cloudee
 */
public interface IClassLoader {

	public static final char TYPE_BYTE = 'B';
	public static final char TYPE_CHAR = 'C';
	public static final char TYPE_DOUBLE = 'D';
	public static final char TYPE_FLOAT = 'F';
	public static final char TYPE_INT = 'I';
	public static final char TYPE_LONG = 'J';
	public static final char TYPE_HANDLE = 'L';
	public static final char TYPE_SHORT = 'S';
	public static final char TYPE_BOOLEAN = 'Z';
	public static final char TYPE_ARRAY = '[';

	public static final String TOP_CLASS = "java/lang/Object";

	/**
	 * 1. Load a class by the class name into the specific VMContext.
	 * 
	 * 2. Do the verify(4.9.1) pass 1.
	 * 
	 * After this pass all the info not related to Heap memory is ready.
	 * 
	 * @param name
	 *            the name of the class eg. java/lang/String
	 *            com/cirnoworks/Example$Inner
	 * @param VMContext
	 *            the VMContext
	 * @throws VMCriticalException
	 * @throws ClassLoadException
	 *             when the class file is malformed.
	 */
	public AbstractClass loadClassByName(String name, VMContext context)
			throws VMException, VMCriticalException;

	/**
	 * 1. Do the verify(4.9.1) pass 2.
	 * 
	 * 2. Fill in string fields is each attribute and constants.
	 * 
	 * 3. move the attributes to the proper fields in the class/method/field
	 * 
	 * 4. Allocate static areas for this class
	 * 
	 * After this pass, the info related to Heap memory is ready too.
	 * 
	 * @param base
	 * @throws VMCriticalException
	 * @throws ClassLoadException
	 */
	public void verifyPass2(AbstractClass base, VMContext context)
			throws VMException, VMCriticalException;
}
