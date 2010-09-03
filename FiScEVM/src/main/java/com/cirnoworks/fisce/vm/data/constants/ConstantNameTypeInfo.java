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

import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.ClassBase;
import java.io.DataInputStream;
import java.io.IOException;

/**
 * 
 * @author cloudee
 */
public class ConstantNameTypeInfo extends Constant {

	private char nameIndex;
	private char descriptorIndex;
	//
	private String name;
	private String descriptor;

	public ConstantNameTypeInfo(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(12, context, owner);
		nameIndex = dis.readChar();
		descriptorIndex = dis.readChar();
	}

	public ConstantNameTypeInfo(VMContext context, ClassBase owner,
			String name, String descriptor) throws IOException {
		super(12, context, owner);
		this.name = name;
		this.descriptor = descriptor;
	}

	public synchronized String getDescriptor() {
		assert descriptor != null && name != null;
		return descriptor;
	}

	public String getName() {
		assert descriptor != null && name != null;
		return name;
	}

	public void fillConstants() throws VMException {
		assert owner.isConstantsLoaded();
		assert name == null && descriptor == null;
		name = owner.getStringFromUTF8Constant(nameIndex);
		descriptor = owner.getStringFromUTF8Constant(descriptorIndex);
		if (name == null || descriptor == null) {
			throw new VMException("java/lang/VirtualMachineError",
					"fill name and desc for class " + owner.getName());
		}
	}

	public String toString() {
		return "ConstantNameTypeInfo:" + (int) nameIndex + ":"
				+ (int) descriptorIndex + ":" + getName() + ":"
				+ getDescriptor();
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final ConstantNameTypeInfo other = (ConstantNameTypeInfo) obj;
		if ((this.name == null) ? (other.name != null) : !this.name
				.equals(other.name)) {
			return false;
		}
		if ((this.descriptor == null) ? (other.descriptor != null)
				: !this.descriptor.equals(other.descriptor)) {
			return false;
		}
		return true;
	}

	@Override
	public int hashCode() {
		int hash = 3;
		hash = 89 * hash + (this.name != null ? this.name.hashCode() : 0);
		hash = 89 * hash
				+ (this.descriptor != null ? this.descriptor.hashCode() : 0);
		return hash;
	}
}
