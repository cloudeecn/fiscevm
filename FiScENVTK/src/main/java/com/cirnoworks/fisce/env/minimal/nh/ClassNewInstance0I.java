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

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.JThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;

public class ClassNewInstance0I extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		AbstractClass clazz = ((VMContext) context)
				.getClassForClassObjectHandle(args[0]/* this */);
		int size = args[1];
		if (!(clazz instanceof ClassArray)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"class is not an array!");
		}
		int handle = context.getHeap().allocate((ClassArray) clazz, size);
		thread.nativeReturnHandle(handle);

		/* object.<init>() */
		((JThread) thread).pushFrame(((VMContext) context).getMethod(clazz
				.getName() + ".<init>.()V"));
		((JThread) thread).putLocalHandle(0, handle);
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.newInstance0.(Ljava/lang/Class;I)[Ljava/lang/Object;";
	}

}
