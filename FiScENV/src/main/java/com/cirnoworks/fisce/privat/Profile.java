/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.cirnoworks.fisce.privat;

/**
 *
 * @author yuxuanhuang
 */
public class Profile {

	public static void main(String[] args) {
		int i = 0;
		long begin = System.currentTimeMillis();
		for (i = 0; i < 500001; i++) {
			if (i % 100000 == 0) {
				System.out.println(i + "\t" + (System.currentTimeMillis() - begin));
			}
		}
	}
}
