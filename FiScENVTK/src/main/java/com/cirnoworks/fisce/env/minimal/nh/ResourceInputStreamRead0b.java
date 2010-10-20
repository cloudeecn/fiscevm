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
import java.io.InputStream;

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;

public class ResourceInputStreamRead0b extends NativeHandlerTemplate{
	private static final int POS_BUF_SIZE = 4096;
	private BaseToolkit toolkit;

	public ResourceInputStreamRead0b(BaseToolkit toolkit) {
		super();
		this.toolkit = toolkit;
	}

	public void dealNative(int[] args, IThread thread)
			throws VMException, VMCriticalException {
		try {
			int thisHandle = args[0];
			int nameHandle = args[1];
			int pos = args[2];
			int bufferHandle = args[3];
			int ofs = args[4];
			int len = args[5];
			String name = context.getHeap().getString(nameHandle);
			String resName = name + "*" + thisHandle;
			InputStream is = toolkit.getResource(resName);
			if (is == null) {
				is = getClass().getResourceAsStream(name);
				byte[] buf = new byte[POS_BUF_SIZE];
				for (int i = 0, max = pos / POS_BUF_SIZE; i < max; i++) {
					is.read(buf);
				}
				for (int i = 0, max = pos % POS_BUF_SIZE; i < max; i++) {
					is.read();
				}
				toolkit.putResource(resName, is);
			}
			byte[] buf = new byte[len];
			int ret = is.read(buf);
			IHeap heap = context.getHeap();
			for (int i = 0; i < len; i++) {
				heap.putArrayByte(bufferHandle, i + ofs, buf[i]);
			}
			thread.pushInt(ret);
		} catch (IOException e) {
			throw new VMException("java/io/IOException", String.valueOf(e
					.getMessage()));
		}
	}

	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/ResourceInputStream.read0.(Ljava/lang/String;I[BII)I";
	}

}
