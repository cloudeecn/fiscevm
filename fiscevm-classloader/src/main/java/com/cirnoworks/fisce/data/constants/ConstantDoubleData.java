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

/**
 * 
 * @author cloudee
 */
public class ConstantDoubleData implements ConstantData, JSONExportable {

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

	@Override
	public void appendJSON(StringBuilder sb, int baseIndent, boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		if (Double.isNaN(data)) {
			SimpleJSONUtil.add(sb, baseIndent + 1, "\"special\"", "\"nan\"",
					true);
			SimpleJSONUtil.add(sb, baseIndent + 1, "\"value\"", "null", true);
		} else if (Double.isInfinite(data)) {
			if (data > 0) {
				SimpleJSONUtil.add(sb, baseIndent + 1, "\"special\"",
						"\"inf\"", true);
			} else {
				SimpleJSONUtil.add(sb, baseIndent + 1, "\"special\"",
						"\"-inf\"", true);
			}
			SimpleJSONUtil.add(sb, baseIndent + 1, "\"value\"", "null", true);
		} else {
			SimpleJSONUtil.add(sb, baseIndent + 1, "\"value\"",
					String.valueOf(data), true);
		}
		long ieeeValue = Double.doubleToLongBits(data);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"ieeeValueHigh\"",
				String.valueOf(ieeeValue >>> 32), true);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"ieeeValueLow\"",
				String.valueOf(ieeeValue & 0xffffffff), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

	@Override
	public void fillConstants(ConstantData[] constantPool) {

	}
}
