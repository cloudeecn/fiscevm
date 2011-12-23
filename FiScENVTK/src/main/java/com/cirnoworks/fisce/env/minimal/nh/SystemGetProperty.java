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
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class SystemGetProperty extends NativeHandlerTemplate{

	private BaseToolkit toolkit;

	public SystemGetProperty(BaseToolkit toolkit) {
		super();
		this.toolkit = toolkit;
	}

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		int keyHandle = args[0];
		String key;
		String value;
		if (keyHandle == 0) {
			key = null;
		} else {
			key = context.getHeap().getString(keyHandle);
		}
		value = toolkit.getProperty(key);
		if (value == null) {
			thread.nativeReturnHandle(0);
		} else {
			thread.nativeReturnHandle(context.getHeap().putString(value));
		}
	}

	public String getUniqueName() {
		return "java/lang/System.getProperty.(Ljava/lang/String;)Ljava/lang/String;";
	}
}
