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
import com.cirnoworks.fisce.data.constants.internal.ConstantNameTypeInfoData;

/**
 * 
 * @author cloudee
 */
public class ConstantReferenceData implements ConstantData, JSONExportable {

	protected int clazzIdx;
	protected ConstantClassData clazz;
	protected int nameAndTypeIdx;
	protected ConstantNameTypeInfoData nameAndType;
	private String uniqueName;

	public String toString() {
		return "ConstantRef: " + uniqueName;
	}

	public void fillUniqueName() {
		uniqueName = clazz.getName() + "." + nameAndType.getName() + "."
				+ nameAndType.getDescriptor();
	}

	public String getUniqueName() {
		return uniqueName;
	}

	public void setClazz(ConstantClassData clazz) {
		this.clazz = clazz;
	}

	public void setNameAndType(ConstantNameTypeInfoData nameAndType) {
		this.nameAndType = nameAndType;
	}

	public ConstantClassData getClazz() {
		return clazz;
	}

	public ConstantNameTypeInfoData getNameAndType() {
		return nameAndType;
	}

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
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final ConstantReferenceData other = (ConstantReferenceData) obj;
		if (this.clazz != other.clazz
				&& (this.clazz == null || !this.clazz.equals(other.clazz))) {
			return false;
		}
		if (this.nameAndType != other.nameAndType
				&& (this.nameAndType == null || !this.nameAndType
						.equals(other.nameAndType))) {
			return false;
		}
		return true;
	}

	@Override
	public int hashCode() {
		int hash = 7;
		hash = 41 * hash + (this.clazz != null ? this.clazz.hashCode() : 0);
		hash = 41 * hash
				+ (this.nameAndType != null ? this.nameAndType.hashCode() : 0);
		return hash;
	}

	@Override
	public void appendJSON(StringBuilder sb, int baseIndent, boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(sb, baseIndent + 1, "\"className\"",
				SimpleJSONUtil.escapeString(clazz.getName(), true), true);
		SimpleJSONUtil.add(
				sb,
				baseIndent + 1,
				"\"nameAndType\"",
				SimpleJSONUtil.escapeString("." + nameAndType.getName() + "."
						+ nameAndType.getDescriptor(), true), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

	@Override
	public void fillConstants(ConstantData[] constantPool) {
		clazz = (ConstantClassData) constantPool[clazzIdx];
		nameAndType = (ConstantNameTypeInfoData) constantPool[nameAndTypeIdx];
		fillUniqueName();
	}

}
