package EXCLUDE.fisce.test;

import static java.lang.System.out;

public class TestGC extends Thread {

	static String 中文测试 = "1122";

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// System.out.println("O");
		new TestGC().start();
		out.println(中文测试);
		int i = 0;
		while (true) {
			out.println("M:" + i);
			i++;
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public void run() {
		int i = 0;
		while (true) {
			out.println("T:" + i);
			i++;
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
