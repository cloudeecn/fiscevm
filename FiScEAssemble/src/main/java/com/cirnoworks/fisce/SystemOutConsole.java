package com.cirnoworks.fisce;

import com.cirnoworks.fisce.intf.IDebugConsole;

public class SystemOutConsole implements IDebugConsole {

	public boolean debug(String msg) {
		return true;
	}

	public boolean debug(String msg, Throwable t) {
		return true;
	}

	public boolean info(String msg) {
		System.out.println(msg);
		return true;
	}

	public boolean info(String msg, Throwable t) {
		System.out.println(msg);
		t.printStackTrace();
		return true;
	}

	public boolean warn(String msg) {
		System.out.println(msg);
		return true;
	}

	public boolean warn(String msg, Throwable t) {
		System.out.println(msg);
		t.printStackTrace();
		return true;
	}

	public boolean error(String msg) {
		System.out.println(msg);
		return true;
	}

	public boolean error(String msg, Throwable t) {
		System.out.println(msg);
		t.printStackTrace();
		return true;
	}

}
