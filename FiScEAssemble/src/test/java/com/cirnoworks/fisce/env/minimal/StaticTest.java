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

import com.cirnoworks.fisce.vm.VMContext;

/**
 * @author Cloudee
 * 
 */
public class StaticTest {
	public void testStatic() throws Exception {
		VMContext vm = TestInitializer.getContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}

	public void testStaticFast() throws Exception {
		VMContext vm = TestInitializer.getFastContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}

	public void testStaticArray() throws Exception {
		VMContext vm = TestInitializer.getArrayContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}

	private void doTest(VMContext context) throws Exception {
		context.bootFromClass("EXCLUDE/fisce/test/StaticTest");
		context.start();
		context.waitTillStopped(0);
	}
}
