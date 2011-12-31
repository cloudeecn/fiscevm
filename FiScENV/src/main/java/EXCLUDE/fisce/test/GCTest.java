package EXCLUDE.fisce.test;

public class GCTest extends TestService {
	public static void main(String[] args) {
		try {
			for (int i = 0; i < 10; i++) {
				System.out.println(i + "+");
				for (int j = 0; j < 1000; j++) {
					Thread.sleep(1);
//					byte[] b = new byte[10000];
				}
				System.out.println(i + ".");
			}
		} catch (Exception e) {
			fail(e.toString());
		}
	}
}
