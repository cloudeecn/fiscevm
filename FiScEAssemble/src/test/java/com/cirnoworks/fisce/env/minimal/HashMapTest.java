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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.env.minimal;

import com.cirnoworks.fisce.vm.VMContext;

/**
 * @author cloudee
 * 
 */
public class HashMapTest {
	public void testHashMap() throws Exception {
		doTest(TestInitializer.getContext());
	}

	public void testHashMapFast() throws Exception {
		doTest(TestInitializer.getFastContext());
	}

	private void doTest(VMContext context) throws Exception {
		context.bootFromClass("com/cirnoworks/fisce/test/HashMapTest");
		context.start();
		context.waitTillStopped(0);
	}
}