/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.cirnoworks.fisce.privat;

/**
 * 
 * @author yuxuanhuang
 */
public class Profile extends Thread {

	public static void main(String[] args) {
		new Profile().start();
	}

	int j;

	public void run() {
		long t0 = System.nanoTime();
		int k = 0;
		int i;
		while (true) {
			long t1 = System.nanoTime();
			for (i = 0; i < 100000; i++) {
				k += i;
			}
			long t2 = System.nanoTime();
			for (j = 0; j < 100000; j++) {
				k += j;
			}
			long t3 = System.nanoTime();
			System.out.println((t1 - t0) + "\t" + (t2 - t1) + "\t" + (t3 - t2)
					+ "\t\t" + k);
		}
	}
}
