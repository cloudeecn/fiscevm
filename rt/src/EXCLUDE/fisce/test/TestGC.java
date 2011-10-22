package EXCLUDE.fisce.test;

import static java.lang.System.out;

import com.cirnoworks.fisce.privat.FiScEVM;

public class TestGC extends Thread {

	static String 中文测试 = "1122";
	private int tmpl = 0;

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
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public void run() {
		new Thread() {
			public void run() {
				int i;
				i = 0;
				while (true) {
					if (i != tmpl) {
						FiScEVM.throwOut(new Error("Thread running error."));
					}
					i++;
					tmpl = i;
				}

			}
		}.start();
		int i = 0;
		while (true) {
			out.println("T:" + i);
			i++;
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
