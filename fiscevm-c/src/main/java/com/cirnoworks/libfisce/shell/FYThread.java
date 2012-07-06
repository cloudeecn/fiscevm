package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IThread;

public class FYThread implements IThread {

	private FYContext context;
	private int id;

	public FYThread(FYContext context, int id) {
		super();
		this.context = context;
		this.id = id;
	}

	
	public void nativeReturnHandle(int handle) {
		FisceService.threadReturnHandle(context.getContext(), id, handle);

	}

	
	public void nativeReturnInt(int value) {
		FisceService.threadReturnInt(context.getContext(), id, value);
	}

	
	public void nativeReturnWide(long value) {
		FisceService.threadReturnWide(context.getContext(), id, value);
	}

	
	public int getThreadHandle() {
		return FisceService.getThreadHandle(context.getContext(), id);
	}

	
	public int getThreadId() {
		return id;
	}

}
