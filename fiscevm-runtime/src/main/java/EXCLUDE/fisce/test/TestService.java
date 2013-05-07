package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class TestService {
	public static native void fail0(String message);

	public static void fail(String message) {
		if (FiScEVM.isFiScE) {
			fail0(message);
		} else {
			System.out.println("Test Failed: " + message);
		}
	}

	public static void fail(String message, Throwable e) {
		e.printStackTrace(FiScEVM.debug);
		fail(message);
	}

	public static void assertTrue(boolean value) {
		assertTrue(value, " no message.");
	}

	public static void assertTrue(boolean value, String failMessage) {
		if (!value) {
			try {
				throw new AssertionError(failMessage);
			} catch (AssertionError e) {
				e.printStackTrace(FiScEVM.debug);
			}
			fail("Assertion error: " + failMessage);
		}
	}

	public static void assertEqual(Object expected, Object actual) {
		if (expected == null) {
			if (actual != null) {
				fail("Assertion error: expected[" + expected + "] but actual["
						+ actual + "]");
			}
		} else {
			if (!expected.equals(actual)) {
				fail("Assertion error: expected[" + expected + "] but actual["
						+ actual + "]");
			}
		}
	}

	public static boolean equalsArray(byte[] array1, byte[] array2) {
		int len;
		if ((len = array1.length) == array2.length) {
			for (int i = 0; i < len; i++) {
				if (array1[i] != array2[i]) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
}
