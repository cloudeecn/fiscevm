package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.IThreadManager;
import com.cirnoworks.fisce.intf.VMException;

public class FYThreadManager implements IThreadManager {

	private FYContext context;

	public FYThreadManager(FYContext context) {
		super();
		this.context = context;
	}


}
