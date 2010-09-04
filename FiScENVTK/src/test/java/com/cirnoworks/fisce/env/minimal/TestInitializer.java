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

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.vm.default_impl.DefaultHeap;
import com.cirnoworks.fisce.vm.default_impl.DefaultThreadManager;

public class TestInitializer {

	public static VMContext getContext() {
		VMContext vm = new VMContext();
		vm.setClassLoader(new DefaultClassLoader());
		vm.setHeap(new DefaultHeap());
		vm.setThreadManager(new DefaultThreadManager());
		vm.setConsole(Log4JConsole.getConsole());
		vm.addToolkit(new BaseToolkit());
		return vm;
	}
}
