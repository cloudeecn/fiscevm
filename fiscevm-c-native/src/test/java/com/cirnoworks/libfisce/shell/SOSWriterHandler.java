package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class SOSWriterHandler extends NativeHandlerTemplate {

	@Override
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		System.out.write(args[1]);
	}

	@Override
	public String getUniqueName() {
		return "com/cirnoworks/fisce/privat/SystemOutputStream.write0.(ILjava/lang/String;)V";
	}

}
