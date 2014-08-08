package EXCLUDE.fisce.test;

import java.util.ArrayList;
import java.util.HashMap;

import com.cirnoworks.fisce.privat.FiScEVM;

interface IF1 {
	final char[] c = { 'a', 'b', 'c' };
}

interface IF2 {
	final char[] c = { 'a', 'd', 'e' };
}

class P {
	final char[] c = { 'a', 'f', 'g' };
}

/**
 * Test if the basic architecture of the VM works well
 * 
 * @author cloudee
 * 
 */
public class ArchitectureTest implements IF1 {
	private final char[] chars = { '<', 'i', 'n', 'i', 't', '>' };

	public void run() {
		System.out.println(new String(c));
	}

	public static void main(String[] args) {
		FiScEVM.infoOut("Begin!");
		{
			String a = "" + 1 + 2 + 3;
			String b = String.valueOf(6);
			if (a.equals(b)) {
				TestService.fail("String compare error");
				throw new RuntimeException();
			}
		}
		new ArchitectureTest().run();
		FiScEVM.infoOut(ArchitectureTest.class.getName());
		try {
			Object o = new HashMap();
			ArrayList a = (ArrayList) o;
		} catch (Exception e) {
			e.printStackTrace();
		}

		int a = 5000;
		int b = 6000;
		int c = 0;
		int na = -5000;
		long al = 500000000000L;
		long bl = 600000000000L;
		long cl = 5;
		long nal = -500000000000L;
		byte[] ab = { 1, 2, 3, 4, 5, 6 };
		char[] ac = { 1, 2, 3, 4, 5, 6, 1000, 2000 };
		float f1 = 1.1f;
		float f2 = 2.25f;
		float f3 = 20000000000f;
		float nf2 = -2.25f;
		float nf3 = -20000000000f;
		float f4 = 3f;
		double d1 = 1.1f;
		double d2 = 2.2f;
		double d3 = 20000000000.0;
		double d4 = 3f;
		double nd2 = -2.2f;
		double nd3 = -20000000000.0;

		int i1 = 130;
		byte b1;
		b1 = (byte) i1;

		int i1a;
		i1a = b1;

		if (b1 > 0 || i1a > 0) {
			TestService.fail("I2B Error sign");
		}

		int i2 = 50000;
		short s1 = (short) i2;
		char c1 = (char) i2;

		int i2a = s1;
		int i2b = c1;

		if (s1 > 0 || i2a > 0) {
			TestService.fail("I2S Error sign");
		}

		if (c1 < 0 || i2b < 0) {
			TestService.fail("I2C Error sign");
		}

		int i3 = -10000;
		short s2 = (short) i3;
		char c2 = (char) i3;

		int i3a = s2;
		int i3b = c2;

		if (s2 > 0 || i3a > 0) {
			TestService.fail("I2S Error sign2");
		}

		if (c2 < 0 || i3b < 0) {
			TestService.fail("I2C Error sign2");
		}

		int i4 = -40000;
		short s3 = (short) i4;
		char c3 = (char) i4;

		int i4a = s3;
		int i4b = c3;

		if (s3 < 0 || i4a < 0) {
			TestService.fail("I2S Error sign3");
		}

		if (c3 < 0 || i4b < 0) {
			TestService.fail("I2C Error sign3");
		}

		if (((float) a) != 5000.0f) {
			TestService.fail("I2F Error: 5000 => " + ((float) a));
		}

		if (((long) a) != 5000l) {
			TestService.fail("I2L Error: 5000 => " + ((long) a));
		}

		if (((double) a) != 5000.0) {
			TestService.fail("I2D Error: 5000 => " + ((double) a));
		}

		if (((float) al) != 500000000000.0f) {
			TestService.fail("L2F Error: 500000000000 => " + ((float) al));
		}

		if (((int) al) != 1783793664) {
			TestService.fail("L2I Error: 1783793664 => " + ((int) al));
		}

		if (((double) al) != 500000000000.0) {
			TestService.fail("L2D Error: 500000000000 => " + ((double) al));
		}

		if (((long) f2) != 2l) {
			TestService.fail("F2L Error: 2 => " + ((long) f2));
		}

		if (((long) f3) != 20000000000l) {
			TestService.fail("F2L Error: 20000000000 => " + ((long) f3));
		}

		if (((int) f2) != 2) {
			TestService.fail("F2I Error: 2 => " + ((int) f2));
		}

		if (((double) f2) != 2.25) {
			TestService.fail("F2D Error: 2.2 => " + ((double) f2));
		}

		if (((long) d2) != 2l) {
			TestService.fail("D2L Error: 2 => " + ((long) d2));
		}

		if (((long) d3) != 20000000000l) {
			TestService.fail("D2L Error: 20000000000 => " + ((long) d3));
		}

		if (((int) d2) != 2) {
			TestService.fail("D2I Error: 2 => " + ((int) d2));
		}

		if (((float) d2) != 2.2f) {
			TestService.fail("D2F Error: 2.25 => " + ((float) d2));
		}

		if (((float) na) != -5000.0f) {
			TestService.fail("I2F Error: -5000 => " + ((float) na));
		}

		if (((long) na) != -5000l) {
			TestService.fail("I2L Error: -5000 => " + ((long) na));
		}

		if (((double) na) != -5000.0) {
			TestService.fail("I2D Error: -5000 => " + ((double) na));
		}

		if (((float) nal) != -500000000000.0f) {
			TestService.fail("L2F Error: -500000000000 => " + ((float) nal));
		}

		if (((int) nal) != -1783793664) {
			TestService.fail("L2I Error: -1783793664 => " + ((int) nal));
		}

		if (((double) nal) != -500000000000.0) {
			TestService.fail("L2D Error: -500000000000 => " + ((double) nal));
		}

		if (((long) nf2) != -2l) {
			TestService.fail("F2L Error: -2 => " + ((long) nf2));
		}

		if (((long) nf3) != -20000000000l) {
			TestService.fail("F2L Error: -20000000000 => " + ((long) nf3));
		}

		if (((int) nf2) != -2) {
			TestService.fail("F2I Error: -2 => " + ((int) nf2));
		}

		if (((double) nf2) != -2.25) {
			TestService.fail("F2D Error: -2.25 => " + ((double) nf2));
		}

		if (((long) nd2) != -2l) {
			TestService.fail("D2L Error: -2 => " + ((long) nd2));
		}

		if (((long) nd3) != -20000000000l) {
			TestService.fail("D2L Error: -20000000000 => " + ((long) nd3));
		}

		if (((int) nd2) != -2) {
			TestService.fail("D2I Error: -2 => " + ((int) nd2));
		}

		if (((float) nd2) != -2.2f) {
			TestService.fail("D2F Error: -2.2 => " + ((float) nd2));
		}

		if (a + b != 11000) {
			TestService.fail("mistake in adding two integers");
		}
		if (a - b != -1000) {
			TestService.fail("mistake in subing two integers");
		}
		if (a * b != 30000000) {
			TestService.fail("mistake in muling two integers");
		}
		if (b / a != 1) {
			TestService.fail("mistake in diving two integers 1");
		}
		if (a / b != 0) {
			TestService.fail("mistake in diving two integers 2");
		}
		if (!String.valueOf(al + bl).equals("1100000000000")) {
			TestService.fail("mistake in adding two longs "
					+ String.valueOf(al + bl));
		}
		if (!String.valueOf(al - bl).equals("-100000000000")) {
			TestService.fail("mistake in subing two longs "
					+ String.valueOf(al - bl));
		}
		if (!String.valueOf(al * bl).equals("601129261562068992")) {
			TestService.fail("mistake in muling two longs"
					+ String.valueOf(al * bl));
		}
		if (!String.valueOf(bl / al).equals("1")) {
			TestService.fail("mistake in diving two longs"
					+ String.valueOf(al * bl));
		}
		if (!String.valueOf(bl / 1000).equals("600000000")) {
			TestService.fail("mistake in diving long with int"
					+ String.valueOf(al * bl));
		}
		try {
			c = a / c;
			TestService.fail("mistake in invoking divided by zero");
		} catch (ArithmeticException ae) {
			if (ae == null) {
				TestService.fail("mistake in exception throw");
			}
			ae.printStackTrace();
		}

	}
}
