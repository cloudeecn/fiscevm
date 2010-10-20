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

import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassMethod;

/**
 * @author Cloudee
 * 
 */
public class ClassInvokeMethod extends NativeHandlerTemplate{

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#dealNative(int[],
	 * com.cirnoworks.fisce.vm.VMContext, com.cirnoworks.fisce.vm.IThread)
	 */
	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		int classHandle = args[0];
		int methodNameHandle = args[1];
		int isStaticI = args[2];
		int paramsHandle = args[3];
		IHeap heap = context.getHeap();
		AbstractClass clazz = context.getClassForClassObjectHandle(classHandle);
		String methodName = heap.getString(methodNameHandle);
		boolean isStatic = isStaticI > 0;
		String uniqueName = clazz.getName() + "." + methodName;
		ClassMethod invoke = context.getMethod(uniqueName);
		byte[] pt = invoke.getParamType();
		int count = paramsHandle == 0 ? 0 : heap.getArrayLength(paramsHandle);
		if (count != pt.length) {
			throw new VMException("java/lang/InvocationTargetException",
					"params not match!");
		}
		int[] invokeArgs = new int[count];
		for (int i = 0; i < count; i++) {
			invokeArgs[i] = heap.getArrayInt(paramsHandle, i);
		}

		thread.pushMethod(invoke, isStatic, count, invokeArgs, pt);
		// thread.pushMethod(invoke, isStatic, argsCount, args, types)
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#getUniqueName()
	 */
	public String getUniqueName() {
		return "java/lang/Class.invokeMethodHandleReturn0.(Ljava/lang/String;Z[Ljava/lang/Object;)Ljava/lang/Object;";
	}

}
