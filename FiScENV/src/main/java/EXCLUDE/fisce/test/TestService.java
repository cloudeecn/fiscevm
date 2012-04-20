package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class TestService {
	public static native void fail(String message);

	public static void assertTrue(boolean value) {
		if (!value) {
			try {
				throw new AssertionError();
			} catch (AssertionError e) {
				e.printStackTrace(FiScEVM.debug);
			}
			fail("Assertion error!");
		}
	}
}
