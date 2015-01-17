package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClassArray;

public class VMDecodeHandler extends NativeHandlerTemplate {

	@Override
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		try {
			IHeap heap = context.getHeap();
			String code = heap.getString(args[0]);
			int ofs = args[2];
			int len = args[3];
			byte[] bytes = new byte[len];
			heap.getArrayByte(bytes, 0, args[1], ofs, len);
			char[] chars = new String(bytes, code).toCharArray();
			int retHandle = heap.allocate((IClassArray) context.getClass("[C"),
					chars.length);
			for (int i = 0, max = chars.length; i < max; i++) {
				heap.putArrayChar(retHandle, i, chars[i]);
			}
			thread.nativeReturnHandle(retHandle);
		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	@Override
	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/FiScEVM.decode.(Ljava/lang/String;[BII)[C";
	}

}
