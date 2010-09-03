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

import com.cirnoworks.fisce.vm.INativeHandler;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMException;

public class SystemArrayCopy implements INativeHandler {

	public void dealNative(int[] args, VMContext context, IThread thread)
			throws VMException {
		int srcHandle = args[0];
		int srcOfs = args[1];
		int dstHandle = args[2];
		int dstOfs = args[3];
		int count = args[4];
		context.getHeap()
				.arrayCopy(srcHandle, srcOfs, dstHandle, dstOfs, count);
	}

	public String getUniqueName() {
		return "java/lang/System.arraycopy.(Ljava/lang/Object;ILjava/lang/Object;II)V";
	}

}
