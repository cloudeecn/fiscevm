package EXCLUDE.fisce.test;

public class ExceptionTester extends TestService {

	static public void main(String[] args) {
		try {
			Class.forName("ooxxoxxxxoox");
			fail("CNFE not raisen");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		System.out.println("Phase 0");
		boolean af = true, bf = true;
		if (af)
			args = null;
		try {
			try {
				System.out.println("Phase 1");
				Exception e = new NullPointerException();
				System.out.println("Phase 2");
				int a = 3;
				int b = 3;
				if (a * b == 9) {
					throw e;
				}
				fail("NPT not caught");
				System.out.println("Phase 2.1");
				if (args == null) {
					return;
				}
			} finally {
				bf = false;
				System.out.println("Phase 4");
			}
			if (bf) {
				fail("finally");
			}
		} catch (Exception e) {
			System.out.println("Phase 4.1");
		} finally {
			af = false;
			System.out.println("Phase 4.2");
		}
		if (af || bf) {
			fail("finally2 " + af + " " + bf);
		}

		ExceptionTester et = new ExceptionTester();
		et.method1(false);
		System.out.println("Phase 5");
		af = true;
		try {
			System.out.println("Phase 6");
			et.method1(true);
			fail("phase6");
		} catch (Exception e) {
			af = false;
			System.out.println("Phase 7");
		}
		if (af) {
			fail("phase7");
		}

		af = true;
		try {
			Class.forName("ooxxoxxxxoox");
			fail("CNFE not raisen");
		} catch (ClassNotFoundException e) {
			af = false;
			e.printStackTrace();
		}
		if (af) {
			fail("CNFE not caught");
		}
		System.out.println("Phase 8");
		try {
			System.out.println("Phase 8.1");
			if (args == null) {
				return;
			}
		} finally {
			System.out.println("Phase 8.2");
		}
		fail("return not invoked");
	}

	public void method1(boolean err) {
		int i = 6;
		try {
			method2(err);
			i = 3;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (err && i != 6) {
				fail("exception2");
			}
			if (!err && i != 3) {
				fail("exception3");
			}
			i = 5;
		}
		if (i != 5) {
			fail("finally3");
		}
		System.out.println(i);

		if (err) {
			throw new RuntimeException();
		}
	}

	public void method2(boolean err) {
		if (err) {
			throw new RuntimeException();
		}
	}
}
