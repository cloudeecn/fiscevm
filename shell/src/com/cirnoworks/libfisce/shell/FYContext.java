package com.cirnoworks.libfisce.shell;

import java.io.Closeable;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

public class FYContext implements Closeable {

	private final ByteBuffer context;
	private final Message message = new Message();

	static {
		System.loadLibrary("fisce");
		System.loadLibrary("fyjni");
	}

	public FYContext() {
		int size = FisceService.getSize();
		context = ByteBuffer.allocateDirect(size);
		context.order(ByteOrder.nativeOrder());
		initContext();
	}

	public void bootFromClass(String name) {
		if (name == null) {
			throw new NullPointerException();
		}
		FisceService.bootFromClass(context, name);
	}

	public Message execute() {
		FisceService.execute(context, message);
		return message;
	}

	public void initContext() {
		FisceService.initContext(context);
	}

	public Message getMessage() {
		return message;
	}

	@Override
	public void close() throws IOException {

	}

	public static void main(String[] args) {
		FYContext context = new FYContext();
		context.bootFromClass("com/cirnoworks/fisce/privat/Profile");
		all: while (true) {
			Message msg = context.execute();
			switch (msg.getMessageType()) {
			case Message.message_invoke_native:
				System.out.println("Stopped at invoke native "
						+ msg.getNativeUniqueName() + " with "
						+ msg.getParams().length + " params."
						+ Arrays.toString(msg.getParams()) + " at thread "
						+ msg.getThreadId());
				break all;
			case Message.message_exception:
				System.out.println("Stopped at invoke exception: "
						+ msg.getExceptionName() + ": "
						+ msg.getExceptionDesc() + " at thread "
						+ msg.getThreadId());
				break all;
			case Message.message_vm_dead:
				break all;
			case Message.message_sleep:
				try {
					Thread.sleep(msg.getSleepTime());
				} catch (InterruptedException ie) {
					ie.printStackTrace();
				}
				break;
			}
		}
		System.out.println("over");

	}
}
