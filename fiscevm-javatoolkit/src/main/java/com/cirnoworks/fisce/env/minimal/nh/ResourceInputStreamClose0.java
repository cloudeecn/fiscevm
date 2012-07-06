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

import java.io.IOException;

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class ResourceInputStreamClose0 extends NativeHandlerTemplate{

	private BaseToolkit toolkit;

	public ResourceInputStreamClose0(BaseToolkit toolkit) {
		super();
		this.toolkit = toolkit;
	}

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		try {
			int thisHandle = args[0];
			int nameHandle = args[1];
			String name = context.getHeap().getString(nameHandle);
			String resName = name + "*" + thisHandle;
			toolkit.closeResource(resName);
		} catch (IOException e) {
			throw new VMException("java/io/Exception", e.getMessage());
		}
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/ResourceInputStream.close0.(Ljava/lang/String;)V";
	}

}
