package EXCLUDE.fisce.test;

public class Benchmark {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		long t1 = System.currentTimeMillis();
		int j = 0;
		for (int i = 0; i < 20000; i++) {
			j += i;
		}
		long t2 = System.currentTimeMillis();
		System.out.println("j=" + j + " time=" + (t2 - t1));
	}

}
