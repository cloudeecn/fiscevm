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

import java.io.PrintWriter;
import java.io.StringWriter;

import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.attributes.InnerClass;
import com.cirnoworks.fisce.vm.data.constants.Constant;
import com.cirnoworks.fisce.vm.data.constants.ConstantClass;
import com.cirnoworks.fisce.vm.data.constants.ConstantUTF8;

/**
 * 
 * @author yuxuanhuang
 */
public final class ClassBase extends AbstractClass implements IConstantHolder,
		IAttributesHolder, IClassBase {

	public ClassBase(VMContext context, IClassLoader loader) {
		super(context, loader);
	}

	// private boolean clinited = false;

	// Filled in directly from file
	private int magic;
	private char minorVersion;
	private char majorVersion;
	private Constant[] constantPool;
	private int[] absPos;
	private char[] isWide;
	private int thisClassInfoIndex;
	private int superClassInfoIndex;
	private int[] interfaceInfoIndexs;
	private ClassField[] fields;
	private ClassMethod[] methods;
	private ClassMethod clinit;
	private Attribute[] attributes;
	private String sourceFile;

	private InnerClass[] innerClasses;

	public ClassMethod searchForMethodSlow(String name, String desc) {
		for (int i = 0, max = methods.length; i < max; i++) {
			ClassMethod method = methods[i];
			if (method.getName().equals(name)
					&& method.getDescriptor().equals(desc)) {
				return method;
			}
		}
		return null;
	}

	public int getMagic() {
		return magic;
	}

	public void setMagic(int magic) {
		this.magic = magic;
	}

	public char getMajorVersion() {
		return majorVersion;
	}

	public void setMajorVersion(char majorVersion) {
		this.majorVersion = majorVersion;
	}

	public char getMinorVersion() {
		return minorVersion;
	}

	public void setMinorVersion(char minorVersion) {
		this.minorVersion = minorVersion;
	}

	public void createConstantPool(int size) {
		constantPool = new Constant[size];
		absPos = new int[size];
		for (int i = 0; i < size; i++) {
			absPos[i] = -1;
		}
		isWide = new char[size];
	}

	public String getStringFromUTF8Constant(int idx) {
		return ((ConstantUTF8) constantPool[idx]).getString();
	}

	public String getSourceFile() {
		return sourceFile;
	}

	public void setSourceFile(String sourceFile) {
		this.sourceFile = sourceFile;
	}

	public int getSuperClassInfoIndex() {
		return superClassInfoIndex;
	}

	public void setSuperClassInfoIndex(int superClassInfoIndex) {
		this.superClassInfoIndex = superClassInfoIndex;
	}

	public int getThisClassInfoIndex() {
		return thisClassInfoIndex;
	}

	public void setThisClassInfoIndex(int thisClassInfoIndex) {
		this.thisClassInfoIndex = thisClassInfoIndex;
	}

	public int[] getInterfaceInfoIndexs() {
		return interfaceInfoIndexs;
	}

	public void createInterfaceInfoIndexs(int size) {
		interfaceInfoIndexs = new int[size];
	}

	public void createFields(int size) {
		fields = new ClassField[size];
	}

	public void createMethods(int size) {
		methods = new ClassMethod[size];
	}

	public void createAttributes(int size) {
		attributes = new Attribute[size];
	}

	public Attribute[] getAttributes() {
		return attributes;
	}

	public Constant[] getConstantPool() {
		return constantPool;
	}

	public int[] getAbsPos() {
		return absPos;
	}

	public char[] getIsWide() {
		return isWide;
	}

	public ClassField[] getFields() {
		return fields;
	}

	public ClassMethod[] getMethods() {
		return methods;
	}

	public String toString() {
		StringWriter sw = new StringWriter();
		java.io.PrintWriter out = new PrintWriter(sw, true);
		out.println("####ClassBase####");
		out.println("#mageic=" + Integer.toHexString(magic));
		out.println("#version=" + (int) majorVersion + "." + (int) minorVersion);

		out.println("#access flag=" + Integer.toBinaryString(accessFlags));

		out.println("#Class Name="
				+ (thisClassInfo == null ? "NULL" : thisClassInfo.getName()));
		out.println("#Super Class="
				+ (superClassInfo == null ? "NULL" : superClassInfo.getName()));
		out.print("#Implements ");
		for (ConstantClass intf : interfaceInfos) {
			out.print(intf.getName() + " ");
		}
		out.println();
		out.println("###Constant Pool:");
		for (int i = 1, max = constantPool.length; i < max; i++) {
			out.println("#" + i + ":" + constantPool[i]);
		}
		out.println("###FIELDS");
		for (ClassField f : fields) {
			out.println(f);
		}
		out.println("###METHODS");
		for (ClassMethod m : methods) {
			out.println(m);
		}
		out.println("###Memory");
		out.println("#Heap   Offset=" + offsetInHeap);
		out.println("#Heap   Size=" + sizeInHeap);
		out.println("#Total  Heap=" + totalSizeInHeap);
		out.println("#Static Size=" + sizeInStatic);
		return sw.toString();
	}

	public void fillConstants() {
		thisClassInfo = (ConstantClass) constantPool[thisClassInfoIndex];
		superClassInfo = (ConstantClass) constantPool[superClassInfoIndex];
		int interfaceCount = interfaceInfoIndexs.length;
		interfaceInfos = new ConstantClass[interfaceCount];
		for (int i = 0; i < interfaceCount; i++) {
			interfaceInfos[i] = (ConstantClass) constantPool[interfaceInfoIndexs[i]];
		}

	}

	public void setAttribute(int idx, Attribute a) {
		attributes[idx] = a;
	}

	public Attribute getAttribute(int idx) {
		return attributes[idx];
	}

	public int countAttribute() {
		return attributes.length;
	}

	public InnerClass[] getInnerClasses() {
		return innerClasses;
	}

	public void setInnerClasses(InnerClass[] classes) {
		this.innerClasses = classes;
	}

	public ClassMethod getClinit() {
		return clinit;
	}

	public void setClinit(ClassMethod clinit) {
		this.clinit = clinit;
	}
}
