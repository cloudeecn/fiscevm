/**
 *  Copyright 2013 Yuxuan Huang. All rights reserved.
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

import com.cirnoworks.fisce.data.constants.ConstantData;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;

/**
 * 
 * @author yuxuanhuang
 */
public final class ClassData {

	/**
	 * Declared abstract; may not be instantiated.
	 */
	public static final char ACC_ABSTRACT = 1024;
	/**
	 * Declared final; no subclasses allowed.
	 */
	public static final char ACC_FINAL = 16;
	/**
	 * Is an interface, not a class.
	 */
	public static final char ACC_INTERFACE = 512;
	/**
	 * Declared native; implemented in a language other than Java.
	 */
	public static final char ACC_NATIVE = 256;
	/**
	 * Declared private; usable only within the defining class.
	 */
	public static final char ACC_PRIVATE = 2;
	/**
	 * Declared protected; may be accessed within subclasses.
	 */
	public static final char ACC_PROTECTED = 4;
	/**
	 * Declared public; may be accessed from outside its package.
	 */
	public static final char ACC_PUBLIC = 1;
	/**
	 * Declared static.
	 */
	public static final char ACC_STATIC = 8;
	/**
	 * Declared strictfp, floating-point mode is FP-strict
	 */
	public static final char ACC_STRICT = 2048;
	/**
	 * Treat superclass methods specially when invoked by the invokespecial
	 * instruction.
	 */
	public static final char ACC_SUPER = 32;
	/**
	 * Declared synchronized; invocation is wrapped in a monitor lock.
	 */
	public static final char ACC_SYNCHRONIZED = 32;
	/**
	 * Declared transient; not written or read by a persistent object manager.
	 */
	public static final char ACC_TRANSIENT = 128;
	/**
	 * Declared volatile; cannot be cached.
	 */
	public static final char ACC_VOLATILE = 64;

	private String name;
	private String sourceFile;

	private int magic;
	private char minorVersion;
	private char majorVersion;
	private ConstantData[] constantPool;

	protected char accessFlags;

	private int thisClassInfoIndex;
	private int superClassInfoIndex;
	private int[] interfaceInfoIndexs;
	private FieldData[] fields;
	private MethodData[] methods;
	private Attribute[] attributes;

	protected int sizeInHeap;
	protected int sizeInStatic;

	public int getMagic() {
		return magic;
	}

	public void setMagic(int magic) {
		this.magic = magic;
	}

	public char getMinorVersion() {
		return minorVersion;
	}

	public void setMinorVersion(char minorVersion) {
		this.minorVersion = minorVersion;
	}

	public char getMajorVersion() {
		return majorVersion;
	}

	public void setMajorVersion(char majorVersion) {
		this.majorVersion = majorVersion;
	}

	public ConstantData[] getConstantPool() {
		return constantPool;
	}

	public void setConstantPool(ConstantData[] constantPool) {
		this.constantPool = constantPool;
	}

	public int getThisClassInfoIndex() {
		return thisClassInfoIndex;
	}

	public void setThisClassInfoIndex(int thisClassInfoIndex) {
		this.thisClassInfoIndex = thisClassInfoIndex;
	}

	public int getSuperClassInfoIndex() {
		return superClassInfoIndex;
	}

	public void setSuperClassInfoIndex(int superClassInfoIndex) {
		this.superClassInfoIndex = superClassInfoIndex;
	}

	public int[] getInterfaceInfoIndexs() {
		return interfaceInfoIndexs;
	}

	public void setInterfaceInfoIndexs(int[] interfaceInfoIndexs) {
		this.interfaceInfoIndexs = interfaceInfoIndexs;
	}

	public FieldData[] getFields() {
		return fields;
	}

	public void setFields(FieldData[] fields) {
		this.fields = fields;
	}

	public MethodData[] getMethods() {
		return methods;
	}

	public void setMethods(MethodData[] methods) {
		this.methods = methods;
	}

	public Attribute[] getAttributes() {
		return attributes;
	}

	public void setAttributes(Attribute[] attributes) {
		this.attributes = attributes;
	}

	public String getSourceFile() {
		return sourceFile;
	}

	public void setSourceFile(String sourceFile) {
		this.sourceFile = sourceFile;
	}

	public int getSizeInHeap() {
		return sizeInHeap;
	}

	public void setSizeInHeap(int sizeInHeap) {
		this.sizeInHeap = sizeInHeap;
	}

	public int getSizeInStatic() {
		return sizeInStatic;
	}

	public void setSizeInStatic(int sizeInStatic) {
		this.sizeInStatic = sizeInStatic;
	}

	public char getAccessFlags() {
		return accessFlags;
	}

	public void setAccessFlags(char accessFlags) {
		this.accessFlags = accessFlags;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

}
