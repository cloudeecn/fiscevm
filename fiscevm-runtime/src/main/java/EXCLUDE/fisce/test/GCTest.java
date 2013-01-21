package EXCLUDE.fisce.test;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;

import com.cirnoworks.fisce.privat.FiScEVM;

public class GCTest extends TestService {

	private static void clearStack(int a, int b, int c, int d, int e, int f,
			int g, int h, int i, int j) {

	}

	public static void testGCBase() throws Exception {
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
	}

	public static void testReference() throws Exception {

		{
			ReferenceQueue<Referent> weakQueue = new ReferenceQueue<Referent>();
			ReferenceQueue<Referent> phantomQueue = new ReferenceQueue<Referent>();
			Referent referent = new Referent();
			System.gc();
			System.out.println("Reference test 1##################Referent="
					+ System.identityHashCode(referent));
			Reference<Referent> wr = new WeakReference<Referent>(referent,
					weakQueue);
			// Reference<Referent> sr = new SoftReference<Referent>(referent,
			// softQueue);
			Reference<Referent> pr = new PhantomReference<Referent>(referent,
					phantomQueue);
			clearStack(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			System.gc();
			System.gc();
			System.gc();
			assertTrue(wr.get() != null, "Weak ref is cleared abnormally");
			// assertTrue(sr.get() != null, "Soft ref is cleared abnormally");
			assertTrue(pr.get() != null, "Phantom ref is cleared abnormally");
			referent = null;
			clearStack(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			System.gc();
			assertTrue(wr.get() == null, "Weak ref is not cleared abnormally");
			// assertTrue(sr.get() != null, "Soft ref is cleared abnormally");
			assertTrue(pr.get() == null,
					"Phantom ref is not cleared abnormally");
			Thread.sleep(500);
			assertTrue(weakQueue.poll() == wr);
			assertTrue(phantomQueue.poll() == pr);
			assertTrue(weakQueue.poll() == null);
			assertTrue(phantomQueue.poll() == null);
		}
		clearStack(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		{
			ReferenceQueue<Referent> softQueue = new ReferenceQueue<Referent>();
			int max = 2000;
			Reference<?>[] refs = new Reference[max];
			System.gc();
			for (int i = 0; i < 2000; i++) {
				Reference<Referent> sr = new SoftReference<Referent>(
						new Referent(), softQueue);
				clearStack(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//				refs[i] = sr;
				if (i == 10000) {
					System.gc();
					Thread.sleep(500);
					assertTrue(softQueue.poll() == null);
					assertTrue(refs[0].get() != null);
					assertTrue(refs[1].get() != null);
				}
			}
			Thread.sleep(500);
			assertTrue(softQueue.poll() != null);

		}
	}

	public static void main(String[] args) {
		try {
			// testGCBase();
			testReference();
		} catch (Exception e) {
			// e.printStackTrace(FiScEVM.debug);
			fail(e.getMessage(), e);
		}

	}
}

class Referent {
	final int[] payload = new int[65536];

	public Referent() {
	}

}

class StrongReferent {
	private static StrongReferent holder;
	final int[] payload = new int[65536];

	public StrongReferent() {
	}

	public void finallize() {
		holder = this;
	}
}