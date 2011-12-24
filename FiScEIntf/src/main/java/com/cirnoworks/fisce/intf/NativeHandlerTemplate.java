package com.cirnoworks.fisce.intf;

public abstract class NativeHandlerTemplate implements INativeHandler {

	protected FiScEVM context;
	private boolean inited = false;

	final public void init(FiScEVM context) throws VMCriticalException,
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
