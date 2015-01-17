package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class TestFailHandler extends NativeHandlerTemplate {

	@Override
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		IHeap heap = context.getHeap();
		String message = heap.getString(args[0]);
		throw new RuntimeException("Test failed: " + message);
	}

	@Override
	public String getUniqueName() {
		return "EXCLUDE/fisce/test/TestService.fail0.(Ljava/lang/String;)V";
	}

}
