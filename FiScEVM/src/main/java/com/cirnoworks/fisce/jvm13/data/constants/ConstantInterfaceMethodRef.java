/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.jvm13.data.constants;

import java.io.DataInputStream;
import java.io.IOException;

import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMCriticalException;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.ClassBase;
import com.cirnoworks.fisce.jvm13.data.ClassMethod;
import com.cirnoworks.fisce.jvm13.data.IReference;

/**
 * 
 * @author cloudee
 */
public class ConstantInterfaceMethodRef extends Constant implements IReference {

	private char classIndex;
	private char nameAndTypeIndex;
	private ConstantClass clazz;
	private ConstantNameTypeInfo nameAndType;
	private ClassMethod targetMethod;
	private String uniqueName;

	public ConstantInterfaceMethodRef(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(11, context, owner);
		classIndex = dis.readChar();
		nameAndTypeIndex = dis.readChar();
	}

	public ConstantInterfaceMethodRef(VMContext context, ClassBase owner,
			ConstantClass clazz, ConstantNameTypeInfo nameAndType) {
		super(11, context, owner);
		this.clazz = clazz;
		this.nameAndType = nameAndType;
	}

	public String toString() {
		return "ConstantInterfaceMethodRef:" + (int) tag + ":"
				+ (int) classIndex + ":" + (int) nameAndTypeIndex;
	}

	public synchronized ClassMethod getTargetMethod() throws VMException, VMCriticalException {
		assert owner.isConstantsLoaded();
		if (targetMethod == null) {
			clazz.getClazz();
			targetMethod = context.getMethod(uniqueName);
			// Already registered!!!
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

	public ConstantNameTypeInfo getNameAndType() {
		return nameAndType;
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
		final ConstantInterfaceMethodRef other = (ConstantInterfaceMethodRef) obj;
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
		hash = 89 * hash + (this.clazz != null ? this.clazz.hashCode() : 0);
		hash = 89 * hash
				+ (this.nameAndType != null ? this.nameAndType.hashCode() : 0);
		return hash;
	}

	public String getUniqueName() {
		return uniqueName;
	}
}
