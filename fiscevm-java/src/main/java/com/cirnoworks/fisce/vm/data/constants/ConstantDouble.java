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
package com.cirnoworks.fisce.vm.data.constants;

import java.io.DataInputStream;
import java.io.IOException;

import com.cirnoworks.fisce.classloader.utils.SimpleJSONUtil;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;

/**
 * 
 * @author cloudee
 */
public class ConstantDouble extends Constant {

	private double data;

	public ConstantDouble(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(6, context, owner);
		data = dis.readDouble();
	}

	public double getData() {
		return data;
	}

	public void fillConstants() {

	}

	public String toString() {
		return "ConstantDouble:" + String.valueOf(data);
	}

	@Override
	public void appendJSON(StringBuilder sb, int baseIndent, boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(sb, baseIndent + 1, "value", String.valueOf(data),
				true);
		long ieeeValue = Double.doubleToLongBits(data);
		SimpleJSONUtil.add(sb, baseIndent + 1, "ieeeValueHigh",
				String.valueOf(ieeeValue >>> 32), true);
		SimpleJSONUtil.add(sb, baseIndent + 1, "ieeeValueLow",
				String.valueOf(ieeeValue & 0xffffffff), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}
}
