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

package com.cirnoworks.fisce.vm.default_impl;

import com.cirnoworks.fisce.util.TypeUtil;

/**
 * @author cloudee
 * 
 */
public class TypeUtilTest {
	public void testIntToLong() throws Exception {
		assert TypeUtil.intToLong(0xfedcba98, 0x76543210) == 0xfedcba9876543210l;
		assert TypeUtil.intToLong(0x76543210, 0xfedcba98) == 0x76543210fedcba98l;
		assert TypeUtil.intToLong(0x01234567, 0x89abcdef) == 0x0123456789abcdefl;
		assert TypeUtil.intToLong(0x89abcdef, 0x01234567) == 0x89abcdef01234567l;
	}

	public void testGetHighInt() {
		assert TypeUtil.getHighInt(0xfedcba9876543210l) == 0xfedcba98;
		assert TypeUtil.getHighInt(0x76543210fedcba98l) == 0x76543210;
		assert TypeUtil.getHighInt(0x0123456789abcdefl) == 0x01234567;
		assert TypeUtil.getHighInt(0x89abcdef01234567l) == 0x89abcdef;
	}

	public void getLowInt(long src) {
		assert TypeUtil.getHighInt(0xfedcba9876543210l) == 0xfedcba98;
		assert TypeUtil.getHighInt(0x76543210fedcba98l) == 0x76543210;
		assert TypeUtil.getHighInt(0x0123456789abcdefl) == 0x01234567;
		assert TypeUtil.getHighInt(0x89abcdef01234567l) == 0x89abcdef;
	}

	public void testMergeShortUnsigned() {
		assert TypeUtil.bytesToUnsignedInt((byte) 0xfe, (byte) 0xdc) == 0xfedc;
		assert TypeUtil.bytesToUnsignedInt((byte) 0xfe, (byte) 0x76) == 0xfe76;
		assert TypeUtil.bytesToUnsignedInt((byte) 0x76, (byte) 0xfe) == 0x76fe;
		assert TypeUtil.bytesToUnsignedInt((byte) 0x76, (byte) 0x54) == 0x7654;
	}

	public void testMergeShortSigned() {
		assert TypeUtil.bytesToSignedInt((byte) 0xfe, (byte) 0xdc) == 0xfffffedc;
		assert TypeUtil.bytesToSignedInt((byte) 0xfe, (byte) 0x76) == 0xfffffe76;
		assert TypeUtil.bytesToSignedInt((byte) 0x76, (byte) 0xfe) == 0x76fe;
		assert TypeUtil.bytesToSignedInt((byte) 0x76, (byte) 0x54) == 0x7654;
	}

	public void testMergeInt() {
		assert TypeUtil.bytesToSignedInt((byte) 0xfe, (byte) 0xdc, (byte) 0x76,
				(byte) 0x54) == 0xfedc7654;
		assert TypeUtil.bytesToSignedInt((byte) 0x45, (byte) 0x67, (byte) 0xcd,
				(byte) 0xef) == 0x4567cdef;
	}
}
