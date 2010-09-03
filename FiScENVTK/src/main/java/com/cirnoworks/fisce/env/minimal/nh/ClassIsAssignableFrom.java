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
package com.cirnoworks.fisce.env.minimal.nh;

import com.cirnoworks.fisce.jvm13.INativeHandler;
import com.cirnoworks.fisce.jvm13.IThread;
import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMCriticalException;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.AbstractClass;

public class ClassIsAssignableFrom implements INativeHandler {

	public void dealNative(int[] args, VMContext context, IThread thread)
			throws VMException, VMCriticalException {
		int thisHandle = args[0];
		int targetHandle = args[1];
		AbstractClass thisClass = context
				.getClassForClassObjectHandle(thisHandle);
		AbstractClass targetClass = context
				.getClassForClassObjectHandle(targetHandle);
		thread.pushInt(targetClass.canCastTo(thisClass) ? 1 : 0);
	}

	public String getUniqueName() {
		return "java/lang/Class.isAssignableFrom.(Ljava/lang/Class;)Z";
	}

}
