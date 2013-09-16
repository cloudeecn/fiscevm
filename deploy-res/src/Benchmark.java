public class Benchmark {
	public static void main(String[] args) {
		int j = 0;
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < 100000; i++) {
			j += i;
		}
		long t2 = System.currentTimeMillis();
		System.out.println((t2 - t1) + " j=" + j);
	}
}
