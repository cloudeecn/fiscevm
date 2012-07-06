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

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.JThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassPrimitive;

public class ClassNewInstance0Ex extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		AbstractClass clazz = ((VMContext) context)
				.getClassForClassObjectHandle(args[0]/* this */);
		if (!(clazz instanceof ClassBase)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"class is an array!");
		}
		int handle = context.getHeap().allocate((ClassBase) clazz);
		thread.nativeReturnHandle(handle);
		IHeap heap = context.getHeap();
		int argCount = heap.getArrayLength(args[1]);
		StringBuilder sb = new StringBuilder(128);
		for (int i = 0; i < argCount; i++) {
			AbstractClass cl = ((VMContext) context)
					.getClassForClassObjectHandle(heap.getArrayHandle(args[1],
							i));
			if (cl instanceof ClassBase) {
				sb.append("L").append(cl.getName()).append(";");
			} else if (cl instanceof ClassArray) {
				sb.append(cl.getName());
			} else if (cl instanceof ClassPrimitive) {
				sb.append(((ClassPrimitive) cl).getpType());
			}
		}

		/* object.<init>() */
		((JThread) thread).pushFrame(((VMContext) context).getMethod(clazz
				.getName() + ".<init>.(" + sb.toString() + ")V"));
		((JThread) thread).putLocalHandle(0, handle);
		for (int i = 0; i < argCount; i++) {
			((JThread) thread).putLocalHandle(i + 1,
					heap.getArrayHandle(args[2], i));
		}
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.newInstance0.(Ljava/lang/Class;[Ljava/lang/Class;[Ljava/lang/Object;)Ljava/lang/Object;";
	}

}
