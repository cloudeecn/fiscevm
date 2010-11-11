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
package com.cirnoworks.fisce.env.minimal;

import junit.framework.AssertionFailedError;
import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassMethod;

public class BaseToolkitTest extends TestCase {
	public void testHelloWorld() throws Exception {
		FiScEVM vm = TestInitializer.getContext();

		vm.bootFromClass("EXCLUDE/fisce/test/MinimalTest");
		vm.start();
		vm.waitTillStopped(0);
	}
	
	public void testHelloWorldFast() throws Exception {
		FiScEVM vm = TestInitializer.getFastContext();

		vm.bootFromClass("EXCLUDE/fisce/test/MinimalTest");
		vm.start();
		vm.waitTillStopped(0);
	}
	
	public void testHelloWorldArray() throws Exception {
		FiScEVM vm = TestInitializer.getArrayContext();

		vm.bootFromClass("EXCLUDE/fisce/test/MinimalTest");
		vm.start();
		vm.waitTillStopped(0);
	}
	
	

	public void testNative() throws Exception {
		VMContext context = TestInitializer.getContext();
		context.bootFromClass("EXCLUDE/fisce/test/MinimalTest");
		String[] classes = { "com/cirnoworks/fisce/privat/FiScEVM",
				"com/cirnoworks/fisce/privat/ResourceInputStream",
				"com/cirnoworks/fisce/privat/SystemInputStream",
				"com/cirnoworks/fisce/privat/SystemOutputStream",
				"java/lang/Class", "java/lang/Object",
				"java/lang/StackTraceElement", "java/lang/String",
				"java/lang/System", "java/lang/Thread", "java/lang/Throwable", };
		boolean error = false;
		StringBuilder sb = new StringBuilder();
		for (String className : classes) {
			ClassBase cb = (ClassBase) context.getClass(className);
			for (ClassMethod method : cb.getMethods()) {
				if (AbstractClass.hasFlag(method.getAccessFlags(),
						AbstractClass.ACC_NATIVE)
						&& context.getNativeHandler(method.getUniqueName()) == null) {
					sb.append(method.getUniqueName());
					sb.append('\n');
					error = true;
				}
			}
		}
		if (error) {
			throw new AssertionFailedError(
					"Can't link native handler for following classes:\n"
							+ sb.toString());
		}
	}
}
