/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;
import com.cirnoworks.fisce.privat.Profile;

/**
 * 
 * @author cloudee
 */
public class Tester extends Thread implements ITester {

	public int i1;
	public int i2 = 1;
	public static int is1;
	public static int is2 = 2;
	public final int f1;
	public final int f2 = 5;
	public static final int fs1;
	public static final int fs2 = 7;
	int j;

	static {
		fs1 = 6;
	}

	{
		f1 = 4;
	}

	public double mul(double a, double b) {
		return a * b;
	}

	public int imul(int a, int b) {
		return a * b;
	}

	private int dis = 6;

	class LinkedInnerClass {
		private int fielda = 12;

		public void run() {
			System.out.println(fielda);
			System.out.println(dis);
		}
	}

	static class StaticInnerClass {
		private int fielda = 12;

		public void run() {

			System.out.println(fielda);
		}

	}

	LinkedInnerClass a, b;
	StaticInnerClass c, d;

	public void complex(int a, double b, int[][][] arr, long c,
			Object[][] arr2, Object o) {
		LinkedInnerClass g = new LinkedInnerClass();
		StaticInnerClass f = new StaticInnerClass();
		Runnable r = new Runnable() {
			public void run() {
				System.out.println("55");
			}
		};
		g.run();
		f.run();
		r.run();
	}

	public void testInterface() {
		System.out.println("Call Interface Method!!");
	}

	public void run() {
		ITester it = this;
		it.testInterface();
		System.out.println("Hello world!");
	}

	static public void main(String[] args) {
		try {
			new String();
			new Integer(1);
			int a = 0;
			int b = 1234987123;
			String sb = Integer.toString(b);
			FiScEVM.infoOut(sb);
			if (a == 0) {
				return;
			}
			Thread t1 = new Thread(new Profile("Thread1"));
			t1.setPriority(Thread.MAX_PRIORITY);
			new Tester().start();
			t1.start();
			try {
				Thread.sleep(2000);
			} catch (Exception e) {
			}
			int[] ia = null;
			if (a == 1) {
				ia = new int[50];
			}
			System.out.println(ia.length);
			// int a = 0;
			// int ib = 11111;
			// if (a == 0) {
			// ib *= 11111;
			// ib *= 11111;
			// ib *= 11111;
			// }
			// String sb = "b=" + ib;
			// FiScEVM.infoOut(sb);
			// FiScEVM.infoOut(Integer.toString(ib, 10));
			// FiScEVM.infoOut(ib);

			int k = 1000000;
			if (k > 100) {
				k += 5;
			}

			int i = 5;
			double d = 1.1;
			double d2 = 0;
			String s = "Hello";
			for (i = 1; i < 2; i++) {
				i++;
				d2 = d * 1.1;
				d = 0;
				s += " World";
			}
//			FiScEVM.infoOut(d2);
//			FiScEVM.infoOut(d);
//			FiScEVM.infoOut(i);
//			FiScEVM.infoOut(s);
//			FiScEVM.infoOut(System.currentTimeMillis());
//			FiScEVM.infoOut(System.nanoTime());

			FiScEVM.infoOut("Hello world " + 123 + " "
					+ (i + 12345678901234567l) + " " + 123.4f + " "
					+ (123.4d + i) + "  我们来试试中文！");
			FiScEVM.infoOut("" + 0.0 / d + " " + 1.0 / d + " " + -1.0 / d);
			System.out.println("Hello " + 111 + "world");
			System.out.println("5*3=" + 5 * 3 + "\n" + "5.0*3.3=" + 5.0f * 3.3
					+ "\n" + "77*65=" + 77 * 65 + "\n");
			is1 = 5555;
			is2 = 6;
			is1 = 5555;
			System.out.println("" + is1 + " " + is2 + " " + fs1 + " " + fs2);
			System.out.println("Hello world!");
			throw new NullPointerException();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
