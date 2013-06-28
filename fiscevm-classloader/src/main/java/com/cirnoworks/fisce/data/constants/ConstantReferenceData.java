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
import com.cirnoworks.fisce.data.constants.internal.ConstantNameTypeInfoData;

/**
 * 
 * @author cloudee
 */
public class ConstantReferenceData implements ConstantData, JSONExportableConstantData {

	protected int clazzIdx;
	protected int nameAndTypeIdx;
	protected String className;
	protected String name;
	protected String descriptior;

	public int getClazzIdx() {
		return clazzIdx;
	}

	public void setClazzIdx(int clazzIdx) {
		this.clazzIdx = clazzIdx;
	}

	public int getNameAndTypeIdx() {
		return nameAndTypeIdx;
	}

	public void setNameAndTypeIdx(int nameAndTypeIdx) {
		this.nameAndTypeIdx = nameAndTypeIdx;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((className == null) ? 0 : className.hashCode());
		result = prime * result
				+ ((descriptior == null) ? 0 : descriptior.hashCode());
		result = prime * result + ((name == null) ? 0 : name.hashCode());
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
		ConstantReferenceData other = (ConstantReferenceData) obj;
		if (className == null) {
			if (other.className != null)
				return false;
		} else if (!className.equals(other.className))
			return false;
		if (descriptior == null) {
			if (other.descriptior != null)
				return false;
		} else if (!descriptior.equals(other.descriptior))
			return false;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

	@Override
	public void appendJSON(StringPool spool, StringBuilder sb, int baseIndent,
			boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"className\"",
				spool.poolString(className), true);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"nameAndType\"",
				spool.poolString("." + name + "." + descriptior), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

	@Override
	public void fillConstants(ConstantData[] constantPool) {
		ConstantClassData clazz = (ConstantClassData) constantPool[clazzIdx];
		ConstantNameTypeInfoData nameAndType = (ConstantNameTypeInfoData) constantPool[nameAndTypeIdx];
		className = clazz.getName();
		name = nameAndType.getName();
		descriptior = nameAndType.getDescriptor();
	}

}
