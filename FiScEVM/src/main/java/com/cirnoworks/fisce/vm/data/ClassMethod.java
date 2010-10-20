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

import java.io.ByteArrayOutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;

import com.cirnoworks.fisce.vm.INativeHandler;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.attributes.ExceptionHandler;
import com.cirnoworks.fisce.vm.data.attributes.LineNumber;
import com.cirnoworks.fisce.vm.data.attributes.LocalVariable;
import com.cirnoworks.fisce.vm.data.constants.ConstantClass;

/**
 * 
 * @author yuxuanhuang
 */
public final class ClassMethod implements IAttributesHolder {
	public static final byte TYPE_INT = 'I';
	public static final byte TYPE_WIDE = 'W';
	public static final byte TYPE_HANDLE = 'H';
	public static final byte TYPE_RETURN = 'R';
	public static final byte TYPE_WIDE2 = '_';
	public static final byte TYPE_UNKNOWN = 0;

	private VMContext context;
	private ClassBase owner;
	private char accessFlags;
	private char nameIndex;
	private String name;
	private char descriptorIndex;
	private String descriptor;
	private Attribute[] attributes;
	// From AttributeCode
	private char maxStack;
	private char maxLocals;
	private byte[] code;
	private String uniqueName;
	private String methodName;
	private int paramCount;
	private byte[] paramType;
	private byte returnType;
	private ExceptionHandler[] exceptionTable;
	// From AttributeCode.AttributeLineNumberTabe
	private LineNumber[] lineNumberTable;
	// From AttributeCode.AttributeLocalVariableTable
	private LocalVariable[] localVariableTable;
	// from AttributeExceptions
	private ConstantClass[] exceptions;
	private boolean clinit;
	private INativeHandler nativeHandler;

	public String toString() {
		StringWriter sw = new StringWriter();
		java.io.PrintWriter out = new PrintWriter(sw, true);
		out.println("##Method: " + (int) accessFlags + " " + uniqueName);
		if (code != null) {
			out.println("#maxStack=" + (int) maxStack);
			out.println("#maxLocals=" + (int) maxLocals);
			out.println("#code size=" + (int) code.length);
			if (exceptionTable != null) {
				out.println("#exception handlers=======");
				out.println("#startPc\tendPc\thandlerPc\texception");
				for (int i = 0; i < exceptionTable.length; i++) {
					out.println("#" + exceptionTable[i]);
				}
			}
			if (lineNumberTable != null) {
				out.println("#line numbers=============");
				out.println("#start pc\tline number");
				for (int i = 0; i < lineNumberTable.length; i++) {
					out.println("#" + lineNumberTable[i]);
				}
			}
			if (localVariableTable != null) {
				out.println("#local variables==========");
				out.println("#index\tstartPc\tlength\tname\tdescriptor");
				for (int i = 0; i < localVariableTable.length; i++) {
					out.println("#" + localVariableTable[i]);
				}
			}
		}
		if (exceptions != null) {
			out.print("#throws:");
			for (int i = 0; i < exceptions.length; i++) {
				out.print(" " + exceptions[i].getName());
			}
		}
		out.println();
		return sw.toString();
	}

	public ClassMethod(VMContext context, ClassBase owner) {
		this.context = context;
		this.owner = owner;
	}

	public static byte getType2(String type) {
		switch (type.charAt(0)) {
		case 'B':
		case 'C':
		case 'F':
		case 'I':
		case 'S':
		case 'Z':
			return TYPE_INT;
		case 'D':
		case 'J':
			return TYPE_WIDE;
		case '[':
		case 'L':
			return TYPE_HANDLE;
		case 'V':
			return TYPE_UNKNOWN;
		default:
			return TYPE_UNKNOWN;
		}
	}

	public static void countParams(String descriptor, ClassMethod method)
			throws VMException {
		int pc = 0;
		ByteArrayOutputStream pt = new ByteArrayOutputStream();
		StringBuilder sb = new StringBuilder();
		try {
			String tmp = descriptor.substring(descriptor.indexOf('(') + 1,
					descriptor.indexOf(')'));
			if (method != null) {
				method.returnType = getType2(descriptor.substring(descriptor
						.indexOf(")") + 1));
			}
			for (int i = 0, max = tmp.length(); i < max; i++) {
				char ch;
				switch (tmp.charAt(i)) {
				case 'B':
				case 'C':
				case 'F':
				case 'I':
				case 'S':
				case 'Z':
					if (method != null) {
						pt.write(TYPE_INT);
					}
					pc += 1;
					break;
				case 'D':
				case 'J':
					if (method != null) {
						pt.write(TYPE_WIDE);
						pt.write(TYPE_WIDE2);
					}
					pc += 2;
					break;
				case '[':
					sb.append('[');
					while ((ch = tmp.charAt(++i)) == '[') {
						sb.append(ch);
					}
					sb.append(ch);
					if (ch == 'L') {
						while ((ch = tmp.charAt(++i)) != ';') {
							sb.append(ch);
						}
						sb.append(ch);
					}
					if (method != null) {
						pt.write(TYPE_HANDLE);
					}
					sb.setLength(0);
					pc += 1;
					break;
				case 'L':
					sb.append('L');
					while ((ch = tmp.charAt(++i)) != ';') {
						sb.append(ch);
					}
					sb.append(ch);
					if (method != null) {
						pt.write(TYPE_HANDLE);
					}
					sb.setLength(0);
					pc += 1;
					break;

				default:
					throw new VMException("java/lang/LinkageError",
							"Malformed description data" + method == null ? ""
									: (" for method " + method.uniqueName));
				}
			}
			if (method != null) {
				method.paramCount = pc;
				method.paramType = pt.toByteArray();
			}
		} catch (IndexOutOfBoundsException e) {
			throw new VMException("java/lang/LinkageError", e.toString());
		}
	}

	public void fillStringValues() throws VMException {
		assert name == null && descriptor == null;
		name = owner.getStringFromUTF8Constant(nameIndex);
		descriptor = owner.getStringFromUTF8Constant(descriptorIndex);
		uniqueName = owner.getName() + "." + name + "." + descriptor;
		methodName = name + "." + descriptor;
		if (name.equals("<clinit>")) {
			clinit = true;
		}

		countParams(descriptor, this);

	}

	public boolean isClinit() {
		return clinit;
	}

	public String getUniqueName() {
		return uniqueName;
	}

	public String getMethodName() {
		return methodName;
	}

	public int getParamCount() {
		return paramCount;
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

	public ClassBase getOwner() {
		return owner;
	}

	public byte[] getCode() {
		return code;
	}

	public void setCode(byte[] code) {
		this.code = code;
	}

	public ExceptionHandler[] getExceptionTable() {
		return exceptionTable;
	}

	public void setExceptionTable(ExceptionHandler[] exceptionTable) {
		this.exceptionTable = exceptionTable;
	}

	public ConstantClass[] getExceptions() {
		return exceptions;
	}

	public void setExceptions(ConstantClass[] exceptions) {
		this.exceptions = exceptions;
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

	public char getMaxLocals() {
		return maxLocals;
	}

	public void setMaxLocals(char maxLocals) {
		this.maxLocals = maxLocals;
	}

	public char getMaxStack() {
		return maxStack;
	}

	public void setMaxStack(char maxStack) {
		this.maxStack = maxStack;
	}

	public int countAttribute() {
		return attributes.length;
	}

	public byte[] getParamType() {
		return paramType;
	}

	public byte getReturnType() {
		return returnType;
	}

	public INativeHandler getNativeHandler() {
		return nativeHandler;
	}

	public void setNativeHandler(INativeHandler nativeHandler) {
		this.nativeHandler = nativeHandler;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((uniqueName == null) ? 0 : uniqueName.hashCode());
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
		ClassMethod other = (ClassMethod) obj;
		if (uniqueName == null) {
			if (other.uniqueName != null)
				return false;
		} else if (!uniqueName.equals(other.uniqueName))
			return false;
		return true;
	}

}
