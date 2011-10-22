package EXCLUDE.fisce.test;

public class Benchmark {

	private int k;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new Benchmark().run();
	}

	public void run() {
		long t1 = System.currentTimeMillis();
		int j = 0;
		for (int i = 0; i < 20000; i++) {
			j += i;
		}
		long t2 = System.currentTimeMillis();
		for (k = 0; k < 10000; k++) {
			j += k;
		}
		long t3 = System.currentTimeMillis();
		System.out.println("j=" + j);
		System.out.println("time1=" + (t2 - t1));
		System.out.println("time2=" + (t3 - t2));
	}

}
