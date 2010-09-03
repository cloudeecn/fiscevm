/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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

import java.util.HashMap;
import java.util.List;

import com.cirnoworks.fisce.jvm13.IThread;
import com.cirnoworks.fisce.jvm13.IThrower;
import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMCriticalException;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.ClassArray;
import com.cirnoworks.fisce.jvm13.data.ClassBase;
import com.cirnoworks.fisce.jvm13.data.ClassField;
import com.cirnoworks.fisce.jvm13.data.StackTraceElement;

public class BaseThrower implements IThrower {
	private VMContext context;
	private BaseToolkit toolkit;

	public BaseThrower(VMContext context, BaseToolkit toolkit) {
		this.context = context;
		this.toolkit = toolkit;
	}

	public int prepareThrowable(VMException e, IThread thread)
			throws VMException, VMCriticalException {
		ClassBase cb = (ClassBase) context.getClass(e.getInnerThrowableName());
		// assert assertClassThrown(context, cb, thread) : cb.getName();
		int handle = context.getHeap().allocate(cb);
		ClassField field = context.lookupFieldVirtual(cb,
				"detailMessage.Ljava/lang/String;");
		int sh = context.getHeap().putString(
				String.valueOf(e.getInnerMessage()));
		context.getHeap().putFieldHandle(handle, field, sh);
		fillInStackTrace(context, thread, handle);
		return handle;
	}

	public void fillInStackTrace(VMContext context, IThread thread,
			int throwableHandle) throws VMException, VMCriticalException {
		List<StackTraceElement> stes = thread.dumpStackTrace(null);

		/**
		 * String declaringClass, methodName, fileName;<br />
		 * int lineNumber;
		 */
		ClassBase clazz = (ClassBase) context
				.getClass("java/lang/StackTraceElement");
		ClassArray array = (ClassArray) context
				.getClass("[Ljava/lang/StackTraceElement;");
		// context.getClass("java/lang/String");
		ClassField declaringClass = context
				.getField("java/lang/StackTraceElement.declaringClass.Ljava/lang/String;");
		assert declaringClass != null : clazz;
		ClassField methodName = context
				.getField("java/lang/StackTraceElement.methodName.Ljava/lang/String;");
		assert declaringClass != null : clazz;
		ClassField fileName = context
				.getField("java/lang/StackTraceElement.fileName.Ljava/lang/String;");
		assert declaringClass != null : clazz;
		ClassField lineNumber = context
				.getField("java/lang/StackTraceElement.lineNumber.I");
		assert declaringClass != null : clazz;
		int arrayHandle = context.getHeap().allocate(array, stes.size());
		StackTraceElement ste;
		for (int i = 0, max = stes.size(); i < max; i++) {
			ste = stes.get(i);
			int handle = context.getHeap().allocate(clazz);
			context.getHeap().putFieldHandle(handle, declaringClass,
					context.getHeap().putString(ste.getClassName()));
			context.getHeap().putFieldHandle(handle, methodName,
					context.getHeap().putString(ste.getMethodName()));
			context.getHeap().putFieldHandle(handle, fileName,
					context.getHeap().putString(ste.getFileName()));
			context.getHeap().putFieldInt(handle, lineNumber,
					ste.getLineNumber());
			context.getHeap().putArrayHandle(arrayHandle, i, handle);
		}
		ClassField backtrace = context
				.getField("java/lang/Throwable.stackTrace.[Ljava/lang/StackTraceElement;");
		assert backtrace != null : context.getClass("java/lang/Throwable");
		context.getHeap().putFieldHandle(throwableHandle, backtrace,
				arrayHandle);
	}

}
