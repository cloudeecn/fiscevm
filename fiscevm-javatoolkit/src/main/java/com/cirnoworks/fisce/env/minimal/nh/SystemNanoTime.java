package com.cirnoworks.fisce.env.minimal.nh;

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class SystemNanoTime extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		thread.nativeReturnWide(System.nanoTime());
	}

	public String getUniqueName() {
		return "java/lang/System.nanoTime.()J";
	}

}
