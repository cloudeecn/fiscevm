package com.cirnoworks.fisce.intf;

public class SystemDebugConsole implements IDebugConsole {

	@Override
	public boolean debug(String msg) {
		System.out.println(msg);
		return true;
	}

	@Override
	public boolean debug(String msg, Throwable t) {
		System.out.println(msg);
		t.printStackTrace(System.out);
		return true;
	}

	@Override
	public boolean info(String msg) {
		System.out.println(msg);
		return true;
	}

	@Override
	public boolean info(String msg, Throwable t) {
		System.out.println(msg);
		t.printStackTrace(System.out);
		return true;
	}

	@Override
	public boolean warn(String msg) {
		System.err.println(msg);
		return true;
	}

	@Override
	public boolean warn(String msg, Throwable t) {
		System.err.println(msg);
		t.printStackTrace();
		return true;
	}

	@Override
	public boolean error(String msg) {
		System.err.println(msg);
		return true;
	}

	@Override
	public boolean error(String msg, Throwable t) {
		System.err.println(msg);
		t.printStackTrace();
		return true;
	}

}
