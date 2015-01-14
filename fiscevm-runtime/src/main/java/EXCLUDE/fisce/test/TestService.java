package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class TestService {
	public static native void fail0(String message);

	public static void fail(String message) {
		if (FiScEVM.isFiScE) {
			FiScEVM.errorOut(message);
			FiScEVM.breakpoint();
		}
		try {
			throw new RuntimeException(message);
		} catch (RuntimeException e) {
			e.printStackTrace(FiScEVM.debug);
		}
		if (FiScEVM.isFiScE) {
			fail0(message);
		} else {
			System.out.println("Test Failed: " + message);
		}
	}

	public static void fail(String message, Throwable e) {
		if (FiScEVM.isFiScE) {
			FiScEVM.errorOut("Exception occored");
			FiScEVM.errorOut(e.getClass().getName());
			FiScEVM.errorOut(message);
			FiScEVM.breakpoint();
		}
		e.printStackTrace(FiScEVM.debug);
		try {
			throw new RuntimeException(message);
		} catch (RuntimeException ex) {
			ex.printStackTrace(FiScEVM.debug);
		}
		if (FiScEVM.isFiScE) {
			fail0(message);
		} else {
			System.out.println("Test Failed: " + message);
		}
	}

	public static void assertTrue(boolean value) {
		assertTrue(value, " no message.");
	}

	public static void assertTrue(boolean value, String failMessage) {
		if (!value) {
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

	public static void assertEqual(Object expected, Object actual,
			String description) {
		if (expected == null) {
			if (actual != null) {
				fail("Assertion error:[" + description + "] = expected["
						+ expected + "] but actual[" + actual + "]");
			}
		} else {
			if (!expected.equals(actual)) {
				fail("Assertion error:[" + description + "] = expected["
						+ expected + "] but actual[" + actual + "]");
			}
		}
	}

	public static void assertIdentityEqual(Object expected, Object actual) {
		if (expected == null) {
			if (actual != null) {
				fail("Assertion error: expected[" + expected + "]("
						+ System.identityHashCode(expected) + ") but actual["
						+ actual + "](" + System.identityHashCode(actual) + ")");
			}
		} else {
			if (expected != actual) {
				fail("Assertion error: expected[" + expected + "]("
						+ System.identityHashCode(expected) + ") but actual["
						+ actual + "](" + System.identityHashCode(actual) + ")");
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
