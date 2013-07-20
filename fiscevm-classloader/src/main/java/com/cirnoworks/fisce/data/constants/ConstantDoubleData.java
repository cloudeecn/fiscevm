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
package com.cirnoworks.fisce.data.constants;

import com.cirnoworks.fisce.classloader.utils.SimpleJSONUtil;
import com.cirnoworks.fisce.classloader.utils.StringPool;

/**
 * 
 * @author cloudee
 */
public class ConstantDoubleData implements ConstantData,
		JSONExportableConstantData {

	protected double data;

	public void setData(double data) {
		this.data = data;
	}

	public double getData() {
		return data;
	}

	public String toString() {
		return "ConstantDouble:" + String.valueOf(data);
	}

	@Override
	public int hashCode() {
		long temp;
		temp = Double.doubleToLongBits(data);
		int result = (int) (temp ^ (temp >>> 32));
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ConstantDoubleData other = (ConstantDoubleData) obj;
		if (Double.doubleToLongBits(data) != Double
				.doubleToLongBits(other.data))
			return false;
		return true;
	}

	public void appendJSON(StringPool spool, StringBuilder sb, int baseIndent,
			boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		long ieeeValue = Double.doubleToLongBits(data);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"value\"", "["
				+ (ieeeValue & 0xffffffffl) + ", "
				+ ((ieeeValue >>> 32) & 0xffffffffl) + "]", false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

	public void fillConstants(ConstantData[] constantPool) {

	}
}
