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

/**
 * @author cloudee
 * 
 */
public final class TypeUtil {

	public static long intToLong(int high, int low) {
		return (((long) high) << 32) | (((long) low) & 0xffffffffl);
	}

	public static int getHighInt(long src) {
		return (int) (src >>> 32);
	}

	public static int getLowInt(long src) {
		return (int) src;
	}

	public static int bytesToUnsignedInt(byte ib1, byte ib2) {
		return ((ib1 << 8) + (ib2 & 0xff)) & 0xffff;
	}

	public static int bytesToSignedInt(byte bb1, byte bb2) {
		return ((bb1 << 8) + (bb2 & 0xff));
	}

	public static int bytesToSignedInt(byte bb1, byte bb2, byte bb3, byte bb4) {
		return ((bb1 & 0xff) << 24) + ((bb2 & 0xff) << 16)
				+ ((bb3 & 0xff) << 8) + (bb4 & 0xff);
	}
}
