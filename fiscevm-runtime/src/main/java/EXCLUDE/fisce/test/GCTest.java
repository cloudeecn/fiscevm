package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class GCTest extends TestService {
	public static void main(String[] args) {
		try {
			FiScEVM.infoOut("$$$$$$$$$$$$$$$$");
			byte[] big = new byte[262144];
			FiScEVM.infoOut("" + System.identityHashCode(big));
			System.out.println("++++++++++++++++++=+");
			for (int i = 0; i < 10; i++) {
				if (i == 4) {
					big = null;
				}
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
			// e.printStackTrace(FiScEVM.debug);
			fail("aaa");
		}
	}
}
