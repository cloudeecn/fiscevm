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

import com.cirnoworks.fisce.privat.FiScEVM;

/**
 * @author Cloudee
 * 
 */
public class AutoBoxingTest extends TestService {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			Boolean z = true;
			Byte b = 120;
			Short s0 = 115;
			Short s1 = 2000;
			Character c0 = 112;
			Character c1 = 1500;
			Integer i0 = -5;
			Integer i1 = -3000;
			Long l0 = -100l;
			Long l1 = -1321341235231541325l;
			Float f = 3.555f;
			Double d = 5.666;
			if (z != z.booleanValue()) {
				fail("");
			}
			if (b != b.byteValue()) {
				fail("");
			}
			if (s0 != s0.intValue()) {
				fail("");
			}
			if (s1 != s1.intValue()) {
				fail("");
			}
			if (c0 != c0.charValue()) {
				fail("");
			}
			if (c1 != c1.charValue()) {
				fail("");
			}
			if (i0 != i0.intValue()) {
				fail("");
			}
			if (i1 != i1.intValue()) {
				fail("");
			}
			if (l0 != l0.longValue()) {
				fail("");
			}
			if (l1 != l1.longValue()) {
				fail("");
			}
			if (f != f.floatValue()) {
				fail("");
			}
			if (d != d.doubleValue()) {
				fail("");
			}
		} catch (Throwable e) {
			e.printStackTrace();
			fail(e.toString());
		}
	}

}
