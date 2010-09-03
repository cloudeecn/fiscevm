/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.cirnoworks.fisce.test;


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
		System.out.println("mie" + is2);
		// Tester t = new Tester();
		// System.out.println("i2=" + t.i2);
		System.out.println("Hello " + System.currentTimeMillis() + "world");
		System.out.println("5*3=" + 5 * 3 + "\n" + "5.0*3.3=" + 5.0f * 3.3 + "\n"
				+ "77*65=" + 77 * 65 + "\n");
		is1 = 5555;
		System.out.println("" + is1);
		System.out.println("Hello world!");
	}

	
}
