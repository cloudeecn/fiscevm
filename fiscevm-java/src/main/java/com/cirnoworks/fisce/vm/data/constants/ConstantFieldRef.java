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
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;
import com.cirnoworks.fisce.vm.data.IReference;

/**
 * 
 * @author cloudee
 */
public class ConstantFieldRef extends Constant implements IReference {

	private char classIndex;
	private char nameAndTypeIndex;
	private ConstantClass clazz;
	private ConstantNameTypeInfo nameAndType;
	private ClassField targetField;
	private String uniqueName;

	public ConstantFieldRef(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(9, context, owner);
		classIndex = dis.readChar();
		nameAndTypeIndex = dis.readChar();
	}

	public String toString() {
		return "ConstantRef:" + (int) tag + ":" + (int) classIndex + ":"
				+ (int) nameAndTypeIndex;
	}

	public synchronized ClassField getTargetField() throws VMException,
			VMCriticalException {
		assert owner.isConstantsLoaded();
		if (targetField == null) {
			ClassBase cb = (ClassBase) clazz.getClazz();
			targetField = context.getField(uniqueName);
			// Already registered!
			if (targetField == null) {
				targetField = context.lookupFieldVirtual(
						cb,
						nameAndType.getName() + "."
								+ nameAndType.getDescriptor());
			}
			if (targetField == null) {
				throw new VMException("java/lang/NoSuchFieldError", uniqueName);
			}
		}
		return targetField;
	}

	public char getClassIndex() {
		return classIndex;
	}

	public char getNameAndTypeIndex() {
		return nameAndTypeIndex;
	}

	public void fillConstants() {
		clazz = (ConstantClass) owner.getConstantPool()[classIndex];
		nameAndType = (ConstantNameTypeInfo) owner.getConstantPool()[nameAndTypeIndex];

	}

	public void fillUniqueName() {
		uniqueName = clazz.getName() + "." + nameAndType.getName() + "."
				+ nameAndType.getDescriptor();
	}

	public String getUniqueName() {
		return uniqueName;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final ConstantFieldRef other = (ConstantFieldRef) obj;
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

	public ConstantClass getClazz() {
		return clazz;
	}

	public void setClazz(ConstantClass clazz) {
		this.clazz = clazz;
	}

	public ConstantNameTypeInfo getNameAndType() {
		return nameAndType;
	}

	public void setNameAndType(ConstantNameTypeInfo nameAndType) {
		this.nameAndType = nameAndType;
	}

	public void setUniqueName(String uniqueName) {
		this.uniqueName = uniqueName;
	}

	@Override
	public void appendJSON(StringBuilder sb, int baseIndent, boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(
				sb,
				baseIndent + 1,
				"nameAndType",
				SimpleJSONUtil.escapeString("." + nameAndType.getName() + "."
						+ nameAndType.getDescriptor(), true), true);
		SimpleJSONUtil.add(sb, baseIndent + 1, "uniqueName",
				SimpleJSONUtil.escapeString(uniqueName, true), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}

}
