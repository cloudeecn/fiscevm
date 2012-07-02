package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.idata.IMethod;

public class FYMethod implements IMethod {
//	private FYContext context;
	private int id;

	public FYMethod(FYContext context, int id) {
		super();
//		this.context = context;
		this.id = id;
	}

	public int getId() {
		return id;
	}
}
