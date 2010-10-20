package com.cirnoworks.fisce.env.minimal.nh;

import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;

public class SystemNanoTime extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		thread.pushLong(System.nanoTime());
	}

	public String getUniqueName() {
		return "java/lang/System.nanoTime.()J";
	}

}
