/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
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
package com.cirnoworks.fisce.vm;


import com.cirnoworks.fisce.vm.data.AbstractClass;

/**
 * 
 * @author cloudee
 */
public interface IClassLoader {

	/**
	 * 1. Load a class by the class name into the specific VMContext.
	 * 
	 * 2. Do the verify(4.9.1) pass 1.
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
	 * @param base
	 * @throws VMCriticalException
	 * @throws ClassLoadException
	 */
	public void verifyPass2(AbstractClass base, VMContext context)
			throws VMException, VMCriticalException;
}
