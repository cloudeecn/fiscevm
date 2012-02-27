package EXCLUDE.fisce.test;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Test if the basic architecture of the VM works well
 * 
 * @author cloudee
 * 
 */
public class ArchitectureTest extends TestService {
	public static void main(String[] args) {

		try {
			Object o = new HashMap();
			ArrayList a = (ArrayList) o;
		} catch (Exception e) {
			e.printStackTrace();
		}

		int a = 5000;
		int b = 6000;
		int c = 0;
		long al = 500000000000L;
		long bl = 600000000000L;
		long cl = 5;
		byte[] ab = { 1, 2, 3, 4, 5, 6 };
		char[] ac = { 1, 2, 3, 4, 5, 6, 1000, 2000 };
		float f1 = 1.1f;
		float f2 = 2.2f;
		float f3 = 2f;
		float f4 = 3f;
		double d1 = 1.1f;
		double d2 = 2.2f;
		double d3 = 2f;
		double d4 = 3f;
		if (a + b != 11000) {
			fail("mistake in adding two integers");
		}
		if (a - b != -1000) {
			fail("mistake in subing two integers");
		}
		if (a * b != 30000000) {
			fail("mistake in muling two integers");
		}
		if (b / a != 1) {
			fail("mistake in diving two integers 1");
		}
		if (a / b != 0) {
			fail("mistake in diving two integers 2");
		}
		try {
			c = a / c;
			fail("mistake in invoking divided by zero");
		} catch (ArithmeticException ae) {
			if (ae == null) {
				fail("mistake in exception throw");
			}
			ae.printStackTrace();
		}
	}
}
