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

import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.util.SimpleJSONUtil;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassMethod;
import com.cirnoworks.fisce.vm.data.IReference;

/**
 * 
 * @author cloudee
 */
public class ConstantMethodRef extends Constant implements IReference {

	private char classIndex;
	private char nameAndTypeIndex;
	private ConstantClass clazz;
	private ConstantNameTypeInfo nameAndType;
	private ClassMethod targetMethod;
	private String uniqueName;

	public ConstantMethodRef(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(10, context, owner);
		classIndex = dis.readChar();
		nameAndTypeIndex = dis.readChar();
	}

	public String toString() {
		return "ConstantMethodRef:" + (int) tag + ":" + (int) classIndex + ":"
				+ (int) nameAndTypeIndex;
	}

	public synchronized ClassMethod getTargetMethod() throws VMException,
			VMCriticalException {
		assert owner.isConstantsLoaded();
		if (targetMethod == null) {
			AbstractClass cb = clazz.getClazz();
			targetMethod = context.getMethod(uniqueName);
			// Already registered!!!
			if (targetMethod == null) {
				targetMethod = context.lookupMethodVirtual(
						cb,
						nameAndType.getName() + "."
								+ nameAndType.getDescriptor());
			}
			if (targetMethod == null) {
				throw new VMException("java/lang/NoSuchMethodError", uniqueName);
			}
		}
		return targetMethod;
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

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final ConstantMethodRef other = (ConstantMethodRef) obj;
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
		hash = 79 * hash + (this.clazz != null ? this.clazz.hashCode() : 0);
		hash = 79 * hash
				+ (this.nameAndType != null ? this.nameAndType.hashCode() : 0);
		return hash;
	}

	public ConstantNameTypeInfo getNameAndType() {
		return nameAndType;
	}

	public void setNameAndType(ConstantNameTypeInfo nameAndType) {
		this.nameAndType = nameAndType;
	}

	public String getUniqueName() {
		return uniqueName;
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
