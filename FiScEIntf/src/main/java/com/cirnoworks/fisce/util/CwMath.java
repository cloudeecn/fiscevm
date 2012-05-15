package com.cirnoworks.fisce.util;

public final class CwMath {
	private CwMath() {

	}

	public static boolean isPowerOf2(int value) {
		return (value & (value - 1)) == 0;
	}

	public static int nextPowerOf2(int value) {
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value++;
		return value;
	}
}
