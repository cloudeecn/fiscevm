/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package EXCLUDE.fisce.test;

import fisce.util.SimpleList;

/**
 * @author Cloudee
 * 
 */
public class ForEachTest extends TestService {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SimpleList<Integer> sl = new SimpleList<Integer>();
		int[] input = { 1, 2, 5, 6, 0x7f7f7f7f, 5 };
		for (int in : input) {
			sl.add(in);
		}
		int i = 0;
		for (int in : sl) {
			if (input[i] != in || input[i] != sl.get(i)) {
				RuntimeException e = new RuntimeException(i + " " + in + " "
						+ input[i] + " " + sl.get(i));
				e.printStackTrace();
				fail("for each");
			}
			i++;
		}
	}
}
