package EXCLUDE.fisce.test;

import java.beans.PropertyChangeListener;

import org.apache.harmony.luni.internal.reflect.ProxyConstants;

public class SmokeTest implements ProxyConstants {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		if (Init == null || Init[0] != '<') {
			TestService.fail("init");
		}
		for (int i = 0; i < 5; i++) {
			new SmokeTest().genLoadArg(i);
		}
	}

	private void genLoadArg(int argByteOffset) {
		if (argByteOffset > 255) {
			writeUnsignedByte(OPC_wide);
			writeUnsignedByte(OPC_aload);
			writeUnsignedByte(argByteOffset);
		} else {
			switch (argByteOffset) {
			case 0:
				writeUnsignedByte(OPC_aload_0);
				break;
			case 1:
				writeUnsignedByte(OPC_aload_1);
				break;
			case 2:
				writeUnsignedByte(OPC_aload_2);
				break;
			case 3:
				writeUnsignedByte(OPC_aload_3);
				break;
			default:
				writeUnsignedByte(OPC_aload);
				writeUnsignedByte(argByteOffset);
			}
		}
	}

	private void writeUnsignedByte(int opcWide) {
		// TODO Auto-generated method stub

	}
}
