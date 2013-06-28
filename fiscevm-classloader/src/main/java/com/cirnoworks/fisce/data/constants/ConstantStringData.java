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
import com.cirnoworks.fisce.data.constants.internal.ConstantUTF8Data;

/**
 * Used in LDC
 * 
 * @author cloudee
 */
public class ConstantStringData implements ConstantData, JSONExportableConstantData {

	protected int strIdx;
	protected String str;

	public int getStrIdx() {
		return strIdx;
	}

	public void setStrIdx(int strIdx) {
		this.strIdx = strIdx;
	}

	public String getStr() {
		return str;
	}

	public void setStr(String str) {
		this.str = str;
	}

	@Override
	public int hashCode() {
		return str.hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ConstantStringData other = (ConstantStringData) obj;
		if (str == null) {
			if (other.str != null)
				return false;
		} else if (!str.equals(other.str))
			return false;
		return true;
	}

	public String toString() {
		return "ConstantString: " + str;
	}

	@Override
	public void appendJSON(StringPool spool, StringBuilder sb, int baseIndent,
			boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"value\"",
				spool.poolString(str), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

	@Override
	public void fillConstants(ConstantData[] constantPool) {
		ConstantUTF8Data data = (ConstantUTF8Data) constantPool[strIdx];
		str = data.getString();
	}

}
