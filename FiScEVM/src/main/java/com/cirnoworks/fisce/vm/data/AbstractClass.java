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

import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClass;
import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.constants.ConstantClass;

/**
 * 
 * @author cloudee
 */
public abstract class AbstractClass implements IClass {
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

	public static boolean hasFlag(int accessFlag, int flag) {
		return (accessFlag & flag) > 0;
	}

	protected int sizeInHeap;
	protected int offsetInHeap;
	protected int totalSizeInHeap;
	protected int sizeInStatic;

	protected char accessFlags;
	protected ConstantClass thisClassInfo;
	protected ConstantClass superClassInfo;
	protected String name;
	protected ClassBase superClass;
	protected ConstantClass[] interfaceInfos;
	protected ClassBase[] interfaces;
	private VMContext context;
	private IClassLoader loader;
	protected boolean constantsLoaded = false;
	protected boolean constantsFilled = false;
	protected boolean loaded = false;
	protected boolean needFinalize = false;

	public AbstractClass(VMContext context, IClassLoader loader) {
		this.context = context;
		this.loader = loader;
	}

	/**
	 * Get the offset of the class field in the heap.
	 * 
	 * ie. sum of the sizeInHeap of all the super classes of this class
	 * 
	 * @return
	 */
	public int getOffsetInHeap() {
		return offsetInHeap;
	}

	/**
	 * Set the offset of the class field in the heap.
	 * 
	 * Must be done in the class load phase 2 as all the super-class must be
	 * loaded and in phase 2.
	 * 
	 * ie. sum of the sizeInHeap of all the super classes of this class
	 * 
	 * @param offsetInHeap
	 */
	public VMContext getContext() {
		return context;
	}

	public IClassLoader getLoader() {
		return loader;
	}

	public void setOffsetInHeap(int offsetInHeap) {
		this.offsetInHeap = offsetInHeap;
	}

	/**
	 * The size of the class will consume in heap memory(Not including size
	 * ofsuper class)
	 * 
	 * @return
	 */
	public int getSizeInHeap() {
		return sizeInHeap;
	}

	/**
	 * Set the size of the class will consume in heap memory.(Not including size
	 * ofsuper class)
	 * 
	 * MUST be set before phase 2.
	 * 
	 * @param sizeInHeap
	 */
	public void setSizeInHeap(int sizeInHeap) {
		this.sizeInHeap = sizeInHeap;
	}

	/**
	 * Get the total size of the class including all the super class fields.
	 * 
	 * @return
	 */
	public int getTotalSizeInHeap() {
		return totalSizeInHeap;
	}

	/**
	 * Set the total size of
	 * 
	 * @param totalSize
	 */
	public void setTotalSizeInHeap(int totalSize) {
		this.totalSizeInHeap = totalSize;
	}

	public boolean isConstantsLoaded() {
		return constantsLoaded;
	}

	public void setConstantsLoaded(boolean constantsLoaded) {
		this.constantsLoaded = constantsLoaded;
	}

	public boolean isLoaded() {
		return loaded;
	}

	public void setLoaded(boolean loaded) {
		this.loaded = loaded;
	}

	public boolean isConstantsFilled() {
		return constantsFilled;
	}

	public void setConstantsFilled(boolean constantsFilled) {
		this.constantsFilled = constantsFilled;
	}

	public ConstantClass getThisClassInfo() {
		return thisClassInfo;
	}

	public void setThisClassInfo(ConstantClass thisClassInfo) {
		this.thisClassInfo = thisClassInfo;
	}

	public ConstantClass getSuperClassInfo() {
		return superClassInfo;
	}

	public void setSuperClassInfo(ConstantClass superClassInfo) {
		this.superClassInfo = superClassInfo;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public ClassBase getSuperClass() {
		return superClass;
	}

	public void setSuperClass(ClassBase superClass) {
		this.superClass = superClass;
	}

	public int getSizeInStatic() {
		return sizeInStatic;
	}

	public void setSizeInStatic(int sizeInStatic) {
		this.sizeInStatic = sizeInStatic;
	}

	public ConstantClass[] getInterfaceInfos() {
		return interfaceInfos;
	}

	public ClassBase[] getInterfaces() {
		return interfaces;
	}

	public void createInterfaces(int size) {
		this.interfaces = new ClassBase[size];
	}

	public char getAccessFlags() {
		return accessFlags;
	}

	public void setAccessFlags(char accessFlags) {
		this.accessFlags = accessFlags;
	}

	public boolean isNeedFinalize() {
		return needFinalize;
	}

	public void setNeedFinalize(boolean needFinalize) {
		this.needFinalize = needFinalize;
	}

	public boolean isSuperClassOf(AbstractClass other) throws VMException {
		if (this == other)
			return false;
		return other.canCastTo(this);
	}

	public boolean canCastTo(AbstractClass other) throws VMException {
		if (this == other) {
			return true;
		}
		if (this instanceof ClassArray) {
			if (other.getName().equals(IClassLoader.TOP_CLASS)) {
				return true;
			} else if (other instanceof ClassArray) {
				ClassArray aThis = (ClassArray) this;
				ClassArray aOther = (ClassArray) other;
				if (aThis.getContentClass() != null
						&& aOther.getContentClass() != null) {
					return aThis.getContentClass().canCastTo(
							aOther.getContentClass());
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (this instanceof ClassBase) {

			ClassBase cb = (ClassBase) this;
			ClassBase[] interfaces = cb.getInterfaces();
			for (int i = 0, max = interfaces.length; i < max; i++) {
				if (other == interfaces[i]) {
					return true;
				}
			}
			if (superClass != null && superClass.canCastTo(other)) {
				return true;
			}
		} else if (this instanceof ClassPrimitive) {
			return false;
		} else {
			throw new VMException("java/lang/Error",
					"class type not implemented!");
		}
		return false;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((name == null) ? 0 : name.hashCode());
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
		AbstractClass other = (AbstractClass) obj;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

}
