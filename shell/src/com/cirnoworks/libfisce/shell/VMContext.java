package com.cirnoworks.libfisce.shell;

import java.io.Closeable;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class VMContext implements Closeable {

	private ByteBuffer context;

	static {
		System.loadLibrary("fisce");
		System.loadLibrary("fyshl");
	}

	public VMContext() {
		int size = getSize();
		context = ByteBuffer.allocateDirect(size);
		context.order(ByteOrder.nativeOrder());
		initContext();
	}

	public void bootFromClass(String name) {
		bootFromClass0(name);
	}

	public Message execute() {
		return execute0();
	}

	private native Message execute0();

	private native void initContext();

	private native void bootFromClass0(String name);

	private native int getSize();

	@Override
	public void close() throws IOException {

	}
}
