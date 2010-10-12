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
package com.cirnoworks.fisce.env.minimal.nh;

import com.cirnoworks.fisce.vm.INativeHandler;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassBase;

public class ClassNewInstance0 implements INativeHandler {

	public void dealNative(int[] args, VMContext context, IThread thread)
			throws VMException, VMCriticalException {
		AbstractClass clazz = context
				.getClassForClassObjectHandle(args[0]/* this */);
		if (!(clazz instanceof ClassBase)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"class is an array!");
		}
		int handle = context.getHeap().allocate((ClassBase) clazz);
		thread.pushHandle(handle);

		/* object.<init>() */
		thread.pushFrame(context.getMethod(clazz.getName() + ".<init>.()V"));
		thread.putLocalHandle(0, handle);
	}

	public String getUniqueName() {
		return "java/lang/Class.newInstance0.()Ljava/lang/Object;";
	}

}
