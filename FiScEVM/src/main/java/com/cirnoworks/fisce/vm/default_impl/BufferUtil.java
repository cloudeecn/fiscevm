package com.cirnoworks.fisce.vm.default_impl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public final class BufferUtil {

	public static ByteBuffer createBuffer(int size){
		return ByteBuffer.allocateDirect(size).order(ByteOrder.LITTLE_ENDIAN);
	}
}
