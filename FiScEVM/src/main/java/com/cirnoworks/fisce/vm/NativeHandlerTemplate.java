package com.cirnoworks.fisce.vm;

public abstract class NativeHandlerTemplate implements INativeHandler {

	protected VMContext context;
	private boolean inited = false;

	final public void init(VMContext context) throws VMCriticalException,
			VMException {
		if (!inited) {
			this.context = context;
			doInit();
		} else {
			throw new VMCriticalException(
					"Each native handler should only be initialized once.");
		}
	}

	public void doInit() throws VMException, VMCriticalException {

	}
}
