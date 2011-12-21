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
		bootFromClass0(context, name);
	}

	public Message execute() {
		return execute0(context);
	}

	public void initContext() {
		initContext0(context);
	}

	private native Message execute0(ByteBuffer context);

	private native void initContext0(ByteBuffer context);

	private native void bootFromClass0(ByteBuffer context, String name);

	private native int getSize();

	@Override
	public void close() throws IOException {

	}
}
