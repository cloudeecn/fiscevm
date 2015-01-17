package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClassArray;

public class VMEncodeHandler extends NativeHandlerTemplate {

	@Override
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		try {
			IHeap heap = context.getHeap();
			String code = heap.getString(args[0]);
			int ofs = args[2];
			int len = args[3];
			char[] chars = new char[len];
			heap.getArrayChar(chars, 0, args[1], ofs, len);
			byte[] bytes = new String(chars).getBytes(code);
			int retHandle = heap.allocate((IClassArray) context.getClass("[B"),
					bytes.length);
			for (int i = 0, max = bytes.length; i < max; i++) {
				heap.putArrayByte(retHandle, i, bytes[i]);
			}
			thread.nativeReturnHandle(retHandle);
		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	@Override
	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.encode.(Ljava/lang/String;[CII)[B";
	}

}
