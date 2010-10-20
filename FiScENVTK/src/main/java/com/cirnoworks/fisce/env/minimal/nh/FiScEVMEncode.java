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

import java.io.UnsupportedEncodingException;

import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.ClassArray;

public class FiScEVMEncode extends NativeHandlerTemplate{

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		IHeap heap = context.getHeap();
		int encodeHandle = args[0];
		int content = args[1];
		int ofs = args[2];
		int len = args[3];
		String encode = heap.getString(encodeHandle);
		char[] cs = new char[len];
		for (int i = 0; i < len; i++) {
			cs[i] = context.getHeap().getArrayChar(content, i + ofs);
		}
		try {
			byte[] bs = new String(cs).getBytes(encode);
			int retHandle = heap.allocate((ClassArray) context.getClass("[B"),
					bs.length);
			for (int i = 0, max = bs.length; i < max; i++) {
				heap.putArrayByte(retHandle, i, bs[i]);
			}
			thread.pushHandle(retHandle);
		} catch (UnsupportedEncodingException e) {
			throw new VMException("java/io/UnsupportedEncodingException",
					encode);
		}
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.encode.(Ljava/lang/String;[CII)[B";
	}

}
