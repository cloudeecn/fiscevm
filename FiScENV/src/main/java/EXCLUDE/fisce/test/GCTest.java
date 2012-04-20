package EXCLUDE.fisce.test;

public class GCTest extends TestService {
	public static void main(String[] args) {
		try {
			System.out.println("++++++++++++++++++=+");
			for (int i = 0; i < 10; i++) {
				System.out.println(i + "+");
				for (int j = 0; j < 100; j++) {
					Thread.sleep(1);
					// byte[] b = new byte[10000];
				}
				System.gc();
				System.out.println(i + ".");
			}
			System.gc();
			System.gc();
			System.gc();
			System.gc();
		} catch (Exception e) {
			fail(e.toString());
		}
	}
}
