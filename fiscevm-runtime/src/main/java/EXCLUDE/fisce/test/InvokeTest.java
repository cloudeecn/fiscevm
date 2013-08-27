package EXCLUDE.fisce.test;

import java.lang.reflect.Method;

public class InvokeTest extends TestService {
	public static void main(String[] args) {
		try {
			InvokeTest it = new InvokeTest();
			Method vm = InvokeTest.class.getMethod("virtualMethod",
					String.class, String.class);
			Method sm = InvokeTest.class.getMethod("staticMethod",
					String.class, String.class);
			assertEqual("aaa", vm.invoke(it, "abc", "def"));
			assertEqual("bbb", sm.invoke(null, "abc", "def"));
		} catch (Throwable e) {
			fail("Exception occored", e);
		}
	}

	public String virtualMethod(String a, String b) {
		return "aaa";
	}

	public static String staticMethod(String a, String b) {
		return "bbb";
	}
}
