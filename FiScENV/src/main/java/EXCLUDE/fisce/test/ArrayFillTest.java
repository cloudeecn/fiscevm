package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class ArrayFillTest {

	public static void main(String[] args) {
		final int SIZE = 32;
		boolean[] distBoolean = new boolean[SIZE];
		boolean[] srcBoolean = new boolean[SIZE];
		byte[] distByte = new byte[SIZE];
		byte[] srcByte = new byte[SIZE];
		short[] distShort = new short[SIZE];
		short[] srcShort = new short[SIZE];
		char[] distChar = new char[SIZE];
		char[] srcChar = new char[SIZE];
		int[] distInt = new int[SIZE];
		int[] srcInt = new int[SIZE];
		long[] distLong = new long[SIZE];
		long[] srcLong = new long[SIZE];
		float[] distFloat = new float[SIZE];
		float[] srcFloat = new float[SIZE];
		double[] distDouble = new double[SIZE];
		double[] srcDouble = new double[SIZE];
		fillArrayTest(distBoolean, srcBoolean);
		fillArrayTest(distByte, srcByte);
		fillArrayTest(distShort, srcShort);
		fillArrayTest(distChar, srcChar);
		fillArrayTest(distInt, srcInt);
		fillArrayTest(distLong, srcLong);
		fillArrayTest(distFloat, srcFloat);
		fillArrayTest(distDouble, srcDouble);
		for (int i = 0; i < SIZE; i++) {
			if (distBoolean[i] != srcBoolean[i]) {
				FiScEVM.throwOut(new Error(), "boolean fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distByte[i] != srcByte[i]) {
				FiScEVM.throwOut(new Error(), "byte fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distShort[i] != srcShort[i]) {
				FiScEVM.throwOut(new Error(), "short fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distChar[i] != srcChar[i]) {
				FiScEVM.throwOut(new Error(), "char fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distInt[i] != srcInt[i]) {
				FiScEVM.throwOut(new Error(), "int fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distLong[i] != srcLong[i]) {
				FiScEVM.throwOut(new Error(), "long fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distFloat[i] != srcFloat[i]) {
				FiScEVM.throwOut(new Error(), "float fill mismatch");
			}
		}
		for (int i = 0; i < SIZE; i++) {
			if (distDouble[i] != srcDouble[i]) {
				FiScEVM.throwOut(new Error(), "double fill mismatch");
			}
		}
	}

	private static native void fillArrayTest(Object toFillOpt,
			Object toFillNormal);
}
