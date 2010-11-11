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
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;
import com.cirnoworks.fisce.vm.data.StackTraceElement;

public class FiScEVMThrowOut extends NativeHandlerTemplate{

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		int throwableHandle = args[0];
		int msgHandle = args[1];
		ClassField backtrace = context
				.getField("java/lang/Throwable.stackTrace.[Ljava/lang/StackTraceElement;");
		assert backtrace != null;
		ClassBase clazz = (ClassBase) context
				.getClass("java/lang/StackTraceElement");
		ClassArray array = (ClassArray) context
				.getClass("[Ljava/lang/StackTraceElement;");
		ClassField declaringClass = context
				.getField("java/lang/StackTraceElement.declaringClass.Ljava/lang/String;");
		assert declaringClass != null : clazz;
		ClassField methodName = context
				.getField("java/lang/StackTraceElement.methodName.Ljava/lang/String;");
		assert methodName != null : clazz;
		ClassField fileName = context
				.getField("java/lang/StackTraceElement.fileName.Ljava/lang/String;");
		assert fileName != null : clazz;
		ClassField lineNumber = context
				.getField("java/lang/StackTraceElement.lineNumber.I");
		assert lineNumber != null : clazz;

		IHeap heap = context.getHeap();
		int stackTracesHandle = heap.getFieldHandle(throwableHandle, backtrace);
		int size = heap.getArrayLength(stackTracesHandle);
		StackTraceElement[] stes = new StackTraceElement[size];
		StringBuilder msgBuilder = new StringBuilder();
		msgBuilder.append("Exception " + context.getClass(args[0]).getName()
				+ " thrown out.\n");
		for (int i = 0; i < size; i++) {
			int stackTraceHandle = heap.getArrayHandle(stackTracesHandle, i);
			String cn = heap.getString(heap.getFieldHandle(stackTraceHandle,
					declaringClass));
			String mn = heap.getString(heap.getFieldHandle(stackTraceHandle,
					methodName));
			String fn = heap.getString(heap.getFieldHandle(stackTraceHandle,
					fileName));
			int ln = heap.getFieldInt(stackTraceHandle, lineNumber);
			StackTraceElement ste = new StackTraceElement(cn, mn, fn, ln);
			msgBuilder.append("*\t" + cn + "." + mn + "(" + fn + ":" + ln
					+ ")\n");
			stes[i] = ste;
		}
		throw new VMCriticalException(msgBuilder.toString());
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.throwOut.(Ljava/lang/Throwable;Ljava/lang/String;)V";
	}

}
