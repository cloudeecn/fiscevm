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

import java.io.UnsupportedEncodingException;

import com.cirnoworks.fisce.jvm13.IHeap;
import com.cirnoworks.fisce.jvm13.INativeHandler;
import com.cirnoworks.fisce.jvm13.IThread;
import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMCriticalException;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.ClassArray;

public class FiScEVMEncode implements INativeHandler {

	public void dealNative(int[] args, VMContext context, IThread thread)
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
