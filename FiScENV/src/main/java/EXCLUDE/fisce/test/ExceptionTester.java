package EXCLUDE.fisce.test;


public class ExceptionTester {

	static public void main(String[] args) {
		System.out.println("Phase 0");
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
				System.out.println("Phase 2.1");
				if (args == null) {
					return;
				}
			} finally {
				System.out.println("Phase 4");
			}
		} catch (Exception e) {
			System.out.println("Phase 4.1");
		} finally {
			System.out.println("Phase 4.2");
		}
		try {
			System.out.println("Phase 4.3");
			if (args == null) {
				return;
			}
		} finally {
			System.out.println("Phase 4.4");
		}
		ExceptionTester et = new ExceptionTester();
		et.method1(false);
		System.out.println("Phase 5");
		try {
			System.out.println("Phase 6");
			et.method1(true);
		} catch (Exception e) {
			System.out.println("Phase 7");
		}
		try {
			Class.forName("ooxxoxxxxoox");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		System.out.println("Phase 8");
	}

	public void method1(boolean err) {
		int i = 6;
		try {
			method2(err);
			i = 3;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			i = 5;
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
