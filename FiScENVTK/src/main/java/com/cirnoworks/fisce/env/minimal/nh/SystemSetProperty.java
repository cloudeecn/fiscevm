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

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;

public class SystemSetProperty extends NativeHandlerTemplate{

	private BaseToolkit toolkit;

	public SystemSetProperty(BaseToolkit toolkit) {
		super();
		this.toolkit = toolkit;
	}

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		int keyHandle = args[0];
		int valueHandle = args[1];
		String key;
		String value;
		if (keyHandle == 0) {
			key = null;
		} else {
			key = context.getHeap().getString(keyHandle);
		}
		if (valueHandle == 0) {
			value = null;
		} else {
			value = context.getHeap().getString(valueHandle);
		}
		toolkit.setProperty(key, value);
	}

	public String getUniqueName() {
		return "java/lang/System.setProperty.(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;";
	}

}
