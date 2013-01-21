package EXCLUDE.fisce.test;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;

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

		ReferenceQueue<Referent> weakQueue = new ReferenceQueue<Referent>();
		ReferenceQueue<Referent> softQueue = new ReferenceQueue<Referent>();
		ReferenceQueue<Referent> phantomQueue = new ReferenceQueue<Referent>();

		System.gc();
		{
			Referent referent = new Referent();
			Reference<Referent> wr = new WeakReference<Referent>(referent,
					weakQueue);
			Reference<Referent> sr = new SoftReference<Referent>(referent,
					softQueue);
			Reference<Referent> pr = new PhantomReference<Referent>(referent,
					phantomQueue);
			System.gc();
			assertTrue(wr.get()!=null, "Weak ref is cleared abnormally");
			assertTrue(sr.get()!=null, "Soft ref is cleared abnormally");
			assertTrue(pr.get()!=null, "Phantom ref is cleared abnormally");
			referent=null;
			System.gc();
			assertTrue(wr.get()==null, "Weak ref is not cleared abnormally");
			assertTrue(sr.get()!=null, "Soft ref is cleared abnormally");
			assertTrue(pr.get()==null, "Phantom ref is not cleared abnormally");
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