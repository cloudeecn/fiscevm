package com.cirnoworks.libfisce.shell;

import java.nio.ByteBuffer;

public final class FisceService {
	public static native void execute(ByteBuffer context, Message message);

	public static native void initContext(ByteBuffer context);

	public static native void bootFromClass(ByteBuffer context, String name);

	public static native int getSize();
}
