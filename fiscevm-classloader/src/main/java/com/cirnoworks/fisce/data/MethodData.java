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
package com.cirnoworks.fisce.data;

import com.cirnoworks.fisce.classloader.utils.DescriptorAnalyzer;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.attributes.ExceptionHandler;
import com.cirnoworks.fisce.vm.data.attributes.LineNumber;
import com.cirnoworks.fisce.vm.data.attributes.LocalVariable;

/**
 * 
 * @author yuxuanhuang
 */
public final class MethodData {

	public char accessFlags;
	public char nameIndex;
	public String name;
	public char descriptorIndex;
	public String descriptor;
	public Attribute[] attributes;
	// From AttributeCode
	public char maxStack;
	public char maxLocals;
	public byte[] code;

	public int paramCount;
	public byte[] paramType;
	public byte returnType;

	public ExceptionHandler[] exceptionTable;
	// From AttributeCode.AttributeLineNumberTabe
	public LineNumber[] lineNumberTable;
	// From AttributeCode.AttributeLocalVariableTable
	public LocalVariable[] localVariableTable;
	// from AttributeExceptions
	public int[] exceptionIdxes;

	public int parameterCount;
	public String[] parameterTypeClassNames;
	public String returnTypeClassName;

	public char getAccessFlags() {
		return accessFlags;
	}

	public void setAccessFlags(char accessFlags) {
		this.accessFlags = accessFlags;
	}

	public char getNameIndex() {
		return nameIndex;
	}

	public void setNameIndex(char nameIndex) {
		this.nameIndex = nameIndex;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public char getDescriptorIndex() {
		return descriptorIndex;
	}

	public void setDescriptorIndex(char descriptorIndex) {
		this.descriptorIndex = descriptorIndex;
	}

	public String getDescriptor() {
		return descriptor;
	}

	public void setDescriptor(String descriptor) {
		this.descriptor = descriptor;
	}

	public Attribute[] getAttributes() {
		return attributes;
	}

	public void setAttributes(Attribute[] attributes) {
		this.attributes = attributes;
	}

	public char getMaxStack() {
		return maxStack;
	}

	public void setMaxStack(char maxStack) {
		this.maxStack = maxStack;
	}

	public char getMaxLocals() {
		return maxLocals;
	}

	public void setMaxLocals(char maxLocals) {
		this.maxLocals = maxLocals;
	}

	public byte[] getCode() {
		return code;
	}

	public void setCode(byte[] code) {
		this.code = code;
	}

	public int getParamCount() {
		return paramCount;
	}

	public void setParamCount(int paramCount) {
		this.paramCount = paramCount;
	}

	public byte[] getParamType() {
		return paramType;
	}

	public void setParamType(byte[] paramType) {
		this.paramType = paramType;
	}

	public byte getReturnType() {
		return returnType;
	}

	public void setReturnType(byte returnType) {
		this.returnType = returnType;
	}

	public ExceptionHandler[] getExceptionTable() {
		return exceptionTable;
	}

	public void setExceptionTable(ExceptionHandler[] exceptionTable) {
		this.exceptionTable = exceptionTable;
	}

	public LineNumber[] getLineNumberTable() {
		return lineNumberTable;
	}

	public void setLineNumberTable(LineNumber[] lineNumberTable) {
		this.lineNumberTable = lineNumberTable;
	}

	public LocalVariable[] getLocalVariableTable() {
		return localVariableTable;
	}

	public void setLocalVariableTable(LocalVariable[] localVariableTable) {
		this.localVariableTable = localVariableTable;
	}

	public int[] getExceptionIdxes() {
		return exceptionIdxes;
	}

	public void setExceptionIdxes(int[] exceptionIdxes) {
		this.exceptionIdxes = exceptionIdxes;
	}

	public int getParameterCount() {
		return parameterCount;
	}

	public void setParameterCount(int parameterCount) {
		this.parameterCount = parameterCount;
	}

	public String[] getParameterTypeClassNames() {
		return parameterTypeClassNames;
	}

	public void setParameterTypeClassNames(String[] parameterTypeClassNames) {
		this.parameterTypeClassNames = parameterTypeClassNames;
	}

	public String getReturnTypeClassName() {
		return returnTypeClassName;
	}

	public void setReturnTypeClassName(String returnTypeClassName) {
		this.returnTypeClassName = returnTypeClassName;
	}

	public void countParams() {
		DescriptorAnalyzer da = new DescriptorAnalyzer(descriptor);
		returnType = da.getReturnType();
		returnTypeClassName = da.getReturnClassName();

		paramCount = da.getParamStackUsage();
		paramType = da.getParamTypes();

		parameterCount = da.getParamCount();
		parameterTypeClassNames = da.getParamClassNames();
	}
}
