/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.cirnoworks.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @author cloudee
 * 
 */
public class ArrayTest {

	public static void main(String[] args) {
		int[][][] array = new int[5][3][4];
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 4; k++) {
					array[i][j][k] = i * 100 + j * 10 + k;
				}
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 4; k++) {
					if (array[i][j][k] != i * 100 + j * 10 + k) {
						FiScEVM.throwOut(new Exception("Integer array error!"));
					}
				}
			}
		}
		String[][][] array1 = new String[5][3][4];
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 4; k++) {
					array1[i][j][k] = i * 100 + j * 10 + k + "";
				}
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 4; k++) {
					if (array1[i][j][k].equals("" + i * 100 + j * 10 + k)) {
						FiScEVM.throwOut(new Exception("Integer array error!"));
					}
				}
			}
		}
		System.out.println("Array test ok!");
	}
}
