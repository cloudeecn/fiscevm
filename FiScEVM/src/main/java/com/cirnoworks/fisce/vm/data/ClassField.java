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
package com.cirnoworks.fisce.vm.data;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.constants.Constant;
import java.io.PrintWriter;
import java.io.StringWriter;

/**
 * 
 * @author cloudee
 */
public final class ClassField implements IAttributesHolder {

	/**
	 * Position in heap memory of this class. filled in the load phase 1. After
	 * the constant table loaded.
	 */
	private int position = -1;
	private int absPos = -1;
	/**
	 * Length of the class. set after the constant table filled.
	 */
	private int length;
	private VMContext context;
	private ClassBase owner;
	private char accessFlags;
	private char nameIndex;
	private String name;
	private char descriptorIndex;
	private String descriptor;
	private Attribute[] attributes;
	private Constant constantValue;
	private String uniqueName;

	public ClassField(VMContext context, ClassBase owner) {
		this.context = context;
		this.owner = owner;
	}

	public void fillStringValues() {
		assert name == null && descriptor == null;
		name = owner.getStringFromUTF8Constant(nameIndex);
		descriptor = owner.getStringFromUTF8Constant(descriptorIndex);
		uniqueName = owner.getName() + "." + name + "." + descriptor;
	}

	public void createAttributes(int size) {
		assert attributes == null;
		attributes = new Attribute[size];
	}

	public void setAttribute(int idx, Attribute a) {
		attributes[idx] = a;
	}

	public Attribute getAttribute(int idx) {
		return attributes[idx];
	}

	public char getAccessFlags() {
		return accessFlags;
	}

	public void setAccessFlags(char accessFlags) {
		this.accessFlags = accessFlags;
	}

	public String getDescriptor() {
		return descriptor;
	}

	public void setDescriptor(String descriptor) {
		this.descriptor = descriptor;
	}

	public char getDescriptorIndex() {
		return descriptorIndex;
	}

	public void setDescriptorIndex(char descriptorIndex) {
		this.descriptorIndex = descriptorIndex;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public char getNameIndex() {
		return nameIndex;
	}

	public void setNameIndex(char nameIndex) {
		this.nameIndex = nameIndex;
	}

	public int getLength() {
		assert length > 0;
		return length;
	}

	public void setLength(int length) throws VMException {
		assert this.length == 0 && owner.constantsFilled;
		if (length <= 0) {
			throw new VMException("java/lang/VirtualMachineError",
					"Set class field length " + length);
		}
		this.length = length;
	}

	public int getPosition() {
		assert position >= 0;
		return position;
	}

	public void setPosition(int position) throws VMException {
		assert this.position < 0 && owner.constantsLoaded;
		if (position < 0) {
			throw new VMException("java/lang/VirtualMachineError",
					"Set class field position " + position);
		}
		this.position = position;
	}

	public int countAttribute() {
		return attributes.length;
	}

	public Constant getConstantValue() {
		return constantValue;
	}

	public void setConstantValue(Constant constantValue) {
		this.constantValue = constantValue;
	}

	public String getUniqueName() {
		return uniqueName;
	}

	public String toString() {
		StringWriter sw = new StringWriter();
		java.io.PrintWriter out = new PrintWriter(sw, true);
		out.println("##Field: " + (int) accessFlags + " " + uniqueName + "="
				+ constantValue);
		out.println("#length=" + (int) length);

		return sw.toString();
	}

	public int getAbsPos() {
		assert absPos >= 0;
		return absPos;
	}

	public void setAbsPos(int absPos) throws VMException {
		assert this.absPos == -1;
		if (absPos < 0) {
			throw new VMException("java/lang/VirtualMachineError",
					"Set class field absPos " + absPos);
		}
		this.absPos = absPos;
	}

	public ClassBase getOwner() {
		return owner;
	}

}
