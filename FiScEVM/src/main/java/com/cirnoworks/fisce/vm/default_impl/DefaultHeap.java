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
package com.cirnoworks.fisce.vm.default_impl;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Map.Entry;

import org.dom4j.Element;

import com.cirnoworks.fisce.vm.Base64;
import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;

public final class DefaultHeap implements IHeap {

	private int getSizeForArray(String arrayName) {
		char c = arrayName.charAt(1);
		switch (c) {
		case 'Z':
		case 'B':
			return 1;
		case 'D':
		case 'J':
			return 8;
		default:
			return 4;
		}
	}

	// not persist
	private VMContext context;

	// persist
	// String -> String handle
	private HashMap<String, Integer> literals = new HashMap<String, Integer>();
	private ByteBuffer staticArea = ByteBuffer.allocateDirect(MAX_STATIC);
	private int staticCount = 0;
	// class id-> pointer in staticArea
	private int[] classStatic = new int[VMContext.MAX_CLASSES];
	// objects[handle] -> data
	private ByteBuffer[] objects = new ByteBuffer[MAX_OBJECTS];
	// handles[handle] --> class id
	private int classId[] = new int[MAX_OBJECTS];
	private int nextHandle;
	private int handleCount;

	{
		// init
		for (int i = 0; i < MAX_OBJECTS; i++) {
			classId[i] = -1;
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setContext(com.cirnoworks.fisce.vm
	 * .VMContext)
	 */
	public void setContext(VMContext context) {
		this.context = context;

	}

	private void releaseHandle(int handle) {
		objects[handle] = null;
		classId[handle] = -1;
		handleCount--;
	}

	private int allocate(int cid, ByteBuffer obj) throws VMException,
			VMCriticalException {
		if (cid < 0) {
			throw new VMCriticalException("Invalid class id");
		}
		int h = nextHandle;
		boolean gced = false;
		while (classId[h] >= 0 || h == 0) {
			// used
			h++;
			if (h == MAX_OBJECTS) {
				h = 1;
			}
			if (h == nextHandle) {
				if (!gced) {
					gc();
					gced = true;
				} else {
					throw new VMCriticalException("Out of memory!");
				}
			}
		}
		nextHandle = (h + 1) % MAX_OBJECTS;

		objects[h] = obj;
		classId[h] = cid;
		handleCount++;
		// context.println(obj.capacity() + "bytes allocated for "
		// + clazz.getName());
		return h;
	}

	private boolean validate(int handle, ClassField field) throws VMException {

		AbstractClass handleClass = context.getClazzById(classId[handle]);
		AbstractClass fieldClass = field.getOwner();
		return handleClass.canCastTo(fieldClass);
	}

	private boolean isArray(int handle) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		AbstractClass handleClass = context.getClazzById(classId[handle]);
		return handleClass.getName().startsWith("[");
	}

	private ByteBuffer getArrayObj(int handle, int index) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert isArray(handle);
		ByteBuffer obj = objects[handle];
		if (index >= obj.getInt(0)) {
			throw new VMException("java/lang/ArrayIndexOutOfBoundsException",
					String.valueOf(index));
		}
		return obj;
	}

	private ByteBuffer getObj(int handle, ClassField field) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert validate(handle, field);
		return objects[handle];
	}

	private int getClazzStaticPos(int cid) {
		return classStatic[cid];
	}

	public void initStaticAreaForClass(AbstractClass clazz) throws VMException {
		Integer cid = context.getClazzId(clazz);
		if (cid == null) {
			throw new VMException("java/lang/VirtualMachineError",
					"init static area for unregistered class!");
		}
		classStatic[cid] = staticCount;
		staticCount += clazz.getSizeInStatic();
	}

	public int getClazzStaticPos(AbstractClass clazz) throws VMException {
		Integer cid = context.getClazzId(clazz);
		if (cid == null) {
			throw new VMException("java/lang/VirtualMachineError",
					"get static area for unregistered class!");
		}

		return classStatic[cid];
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getInternString(java.lang.String)
	 */
	public int getInternString(String content) throws VMException,
			VMCriticalException {
		return getLiteral(content);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getClass(int)
	 */
	public int getClass(int handle) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		return classId[handle];
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayLength(int)
	 */
	public int getArrayLength(int handle) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert isArray(handle);
		return objects[handle].getInt(0);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#allocate(com.cirnoworks.fisce.vm.
	 * data.ClassBase)
	 */
	public int allocate(ClassBase clazz) throws VMException,
			VMCriticalException {
		ByteBuffer obj = ByteBuffer
				.allocateDirect(clazz.getTotalSizeInHeap() * 4);
		return allocate(context.getClazzId(clazz), obj);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#allocate(com.cirnoworks.fisce.vm.
	 * data.ClassArray, int)
	 */
	public int allocate(ClassArray clazz, int length) throws VMException,
			VMCriticalException {
		assert clazz.getName().startsWith("[");
		int size = length * getSizeForArray(clazz.getName());
		ByteBuffer obj = ByteBuffer.allocateDirect(size + 4);
		obj.putInt(0, length);
		return allocate(context.getClazzId(clazz), obj);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#clone(int)
	 */
	public int clone(int handle) throws VMException, VMCriticalException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		int cid = classId[handle];
		ByteBuffer target = objects[handle].duplicate();
		target.clear();
		ByteBuffer clone = ByteBuffer.allocateDirect(target.capacity());
		clone.put(target);
		int ret = allocate(cid, clone);
		return ret;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldBoolean(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public boolean getFieldBoolean(int handle, ClassField field)
			throws VMException {
		return getObj(handle, field).getInt(field.getAbsPos() << 2) > 0;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldByte(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public byte getFieldByte(int handle, ClassField field) throws VMException {
		return (byte) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldShort(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public short getFieldShort(int handle, ClassField field) throws VMException {
		return (short) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldChar(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public char getFieldChar(int handle, ClassField field) throws VMException {
		return (char) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldInt(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public int getFieldInt(int handle, ClassField field) throws VMException {
		return getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldLong(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public long getFieldLong(int handle, ClassField field) throws VMException {
		return getObj(handle, field).getLong(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldFloat(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public float getFieldFloat(int handle, ClassField field) throws VMException {
		return getObj(handle, field).getFloat(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldDouble(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public double getFieldDouble(int handle, ClassField field)
			throws VMException {
		return getObj(handle, field).getDouble(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldHandle(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public int getFieldHandle(int handle, ClassField field) throws VMException {
		return getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldBoolean(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, boolean)
	 */
	public void putFieldBoolean(int handle, ClassField field, boolean value)
			throws VMException {
		getObj(handle, field).putInt(field.getAbsPos() << 2, value ? 1 : 0);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldByte(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, byte)
	 */
	public void putFieldByte(int handle, ClassField field, byte value)
			throws VMException {
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldShort(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, char)
	 */
	public void putFieldShort(int handle, ClassField field, short value)
			throws VMException {
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldChar(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, char)
	 */
	public void putFieldChar(int handle, ClassField field, char value)
			throws VMException {
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldInt(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, int)
	 */
	public void putFieldInt(int handle, ClassField field, int value)
			throws VMException {
		// context.println(handle + " " + field.getPosition() + " "
		// + field.getAbsPos());
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldLong(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, long)
	 */
	public void putFieldLong(int handle, ClassField field, long value)
			throws VMException {
		getObj(handle, field).putLong(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldFloat(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, float)
	 */
	public void putFieldFloat(int handle, ClassField field, float value)
			throws VMException {
		assert validate(handle, field);
		getObj(handle, field).putFloat(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldDouble(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, double)
	 */
	public void putFieldDouble(int handle, ClassField field, double value)
			throws VMException {
		getObj(handle, field).putDouble(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldHandle(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, int)
	 */
	public void putFieldHandle(int handle, ClassField field, int value)
			throws VMException {
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayBoolean(int, int)
	 */
	public boolean getArrayBoolean(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).get(index + 4) > 0;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayByte(int, int)
	 */
	public byte getArrayByte(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return (byte) getArrayObj(handle, index).get(index + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayShort(int, int)
	 */
	public short getArrayShort(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return (short) getArrayObj(handle, index).getInt((index << 2) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayChar(int, int)
	 */
	public char getArrayChar(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return (char) getArrayObj(handle, index).getInt((index << 2) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayInt(int, int)
	 */
	public int getArrayInt(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).getInt((index << 2) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayLong(int, int)
	 */
	public long getArrayLong(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).getLong((index << 3) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayFloat(int, int)
	 */
	public float getArrayFloat(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).getFloat((index << 2) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayDouble(int, int)
	 */
	public double getArrayDouble(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).getDouble((index << 3) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getArrayHandle(int, int)
	 */
	public int getArrayHandle(int handle, int index) throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		return getArrayObj(handle, index).getInt((index << 2) + 4);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayBoolean(int, int, boolean)
	 */
	public void putArrayBoolean(int handle, int index, boolean value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).put(index + 4, (byte) (value ? 1 : 0));

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayByte(int, int, byte)
	 */
	public void putArrayByte(int handle, int index, byte value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).put(index + 4, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayShort(int, int, char)
	 */
	public void putArrayShort(int handle, int index, short value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putInt((index << 2) + 4, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayChar(int, int, char)
	 */
	public void putArrayChar(int handle, int index, char value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putInt((index << 2) + 4, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayInt(int, int, int)
	 */
	public void putArrayInt(int handle, int index, int value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putInt((index << 2) + 4, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayLong(int, int, long)
	 */
	public void putArrayLong(int handle, int index, long value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putLong((index << 3) + 8, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayFloat(int, int, float)
	 */
	public void putArrayFloat(int handle, int index, float value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putFloat((index << 2) + 4, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayDouble(int, int, double)
	 */
	public void putArrayDouble(int handle, int index, double value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putDouble((index << 3) + 8, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#putArrayHandle(int, int, int)
	 */
	public void putArrayHandle(int handle, int index, int value)
			throws VMException {
		if (index < 0 || index > getArrayLength(handle)) {
			throw new VMException("java/lang/IndexOutOfBoundsException", index
					+ ":" + getArrayLength(handle));
		}
		getArrayObj(handle, index).putInt((index << 2) + 4, value);
	}

	/*
	 * Integer> classMap = new HashMap<String, Integer>(); // Class name ->
	 * Class o
	 */
	public boolean getStaticBoolean(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getInt(field.getAbsPos() << 2) > 0;
	}

	public byte getStaticByte(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (byte) staticArea.getInt(field.getAbsPos() << 2);
	}

	public char getStaticShort(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (char) staticArea.getInt(field.getAbsPos() << 2);
	}

	public char getStaticChar(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (char) staticArea.getInt(field.getAbsPos() << 2);
	}

	public int getStaticInt(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getInt(field.getAbsPos() << 2);
	}

	public long getStaticLong(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getLong(field.getAbsPos() << 2);
	}

	public float getStaticFloat(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getFloat(field.getAbsPos() << 2);
	}

	public double getStaticDouble(ClassField field) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getDouble(field.getAbsPos() << 2);
	}

	public void setStaticBoolean(ClassField field, boolean value)
			throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value ? 1 : 0);
	}

	public void setStaticByte(ClassField field, byte value) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticShort(ClassField field, char value) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticChar(ClassField field, char value) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticInt(ClassField field, int value) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticLong(ClassField field, long value) throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putLong(field.getAbsPos() << 2, value);
	}

	public void setStaticFloat(ClassField field, float value)
			throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putFloat(field.getAbsPos() << 2, value);
	}

	public void setStaticDouble(ClassField field, double value)
			throws VMException {
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putDouble(field.getAbsPos() << 2, value);
	}

	public int getLiteral(String str) throws VMException, VMCriticalException {
		Integer handle = literals.get(str);
		if (handle == null) {
			handle = putString(str);
			literals.put(str, handle);
		}
		return handle;
	}

	public String getString(int handle) throws VMException, VMCriticalException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		context.getClass("java/lang/String");
		ClassField valueField = context.getField("java/lang/String.value.[C");
		ClassField offsetField = context.getField("java/lang/String.offset.I");
		ClassField countField = context.getField("java/lang/String.count.I");
		int ofs = getFieldInt(handle, offsetField);
		int len = getFieldInt(handle, countField);
		int cah = getFieldHandle(handle, valueField);
		StringBuffer sb = new StringBuffer(len);

		for (int i = 0; i < len; i++) {
			int pos = i + ofs;
			sb.append(getArrayChar(cah, pos));
		}
		return sb.toString();
	}

	public int putString(String content) throws VMException,
			VMCriticalException {
		if (content == null) {
			return 0;
		}
		int charHandle = allocate((ClassArray) context.getClass("[C"),
				content.length());
		ClassBase stringClass = (ClassBase) context
				.getClass("java/lang/String");
		ClassField valueField = context.getField("java/lang/String.value.[C");
		ClassField offsetField = context.getField("java/lang/String.offset.I");
		ClassField countField = context.getField("java/lang/String.count.I");
		int handle = allocate(stringClass);
		putFieldHandle(handle, valueField, charHandle);
		putFieldInt(handle, offsetField, 0);
		putFieldInt(handle, countField, content.length());
		for (int i = 0, max = content.length(); i < max; i++) {
			putArrayChar(charHandle, i, content.charAt(i));
		}
		return handle;
	}

	public void arrayCopy(int srcHandle, int srcOfs, int dstHandle, int dstOfs,
			int count) throws VMException {
		AbstractClass srcClass = context.getClass(srcHandle);
		AbstractClass dstClass = context.getClass(dstHandle);
		if (srcHandle == 0 || dstHandle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert isArray(srcHandle) && isArray(dstHandle);
		if (!srcClass.canCastTo(dstClass)) {
			throw new VMException("java/lang/ArrayStoreException", "");
		}
		int size = getSizeForArray(srcClass.getName());
		/*
		 * ByteBuffer srcbb = getArrayObj(srcHandle, srcOfs + count); ByteBuffer
		 * dstbb = getArrayObj(dstHandle, dstOfs + count); ByteBuffer d =
		 * dstbb.duplicate(); ByteBuffer s = srcbb.duplicate();
		 * d.position(dstOfs); s.position(srcOfs); s.limit(srcOfs + count *
		 * size); d.put(s);
		 */
		for (int i = 0; i < count; i++) {
			switch (size) {
			case 1:
				putArrayByte(dstHandle, i + dstOfs,
						getArrayByte(srcHandle, i + srcOfs));
				break;
			case 4:
				putArrayInt(dstHandle, i + dstOfs,
						getArrayInt(srcHandle, i + srcOfs));
				break;
			case 8:
				putArrayLong(dstHandle, i + dstOfs,
						getArrayLong(srcHandle, i + srcOfs));
				break;
			}
		}
	}

	public boolean isHandleValid(int handle) {
		if (classId[handle] < 0 || objects[handle] == null) {
			return false;
		} else {
			return true;
		}
	}

	public BitSet scanHeap() throws VMCriticalException {

		BitSet used = new BitSet(MAX_OBJECTS);
		try {
			// BitSet touched = new BitSet(MAX_OBJECTS);
			Set<Integer> pending = new HashSet<Integer>();
			Set<Integer> processing = new HashSet<Integer>();
			// Class object
			// Class static
			// Threads holding
			// String lit.
			for (AbstractClass clazz : context.getClasses()) {
				if (clazz == null) {
					continue;
				}
				int handle = context.getClassObjectHandleForClass(clazz);
				// Class object
				assert isHandleValid(handle) : "handle " + handle
						+ " is invalid";
				// context.getConsole().info(
				// "SCAN INITC->" + handle + " " + clazz.getName());
				pending.add(handle);
				if (clazz instanceof ClassBase) {
					ClassBase cb = (ClassBase) clazz;
					// Class static
					for (ClassField field : cb.getFields()) {
						char t = field.getDescriptor().charAt(0);
						if ((t == 'L' || t == '[')
								&& ((field.getAccessFlags() & AbstractClass.ACC_STATIC) > 0)) {
							// is a static ref
							int handleSR = getStaticInt(field);

							if (handleSR == 0) {
								// context.getConsole().info(
								// "SCAN INITS->" + handleSR + " "
								// + clazz.getName() + " "
								// + field.getUniqueName() + " "
								// + field.getAbsPos() + " NULL");
								continue;
							}
							assert isHandleValid(handleSR);
							// context.getConsole().info(
							// "SCAN INITS->" + handleSR + " "
							// + clazz.getName() + " "
							// + field.getAbsPos() + " "
							// + field.getUniqueName());
							pending.add(handleSR);
						}
					}
				}
			}
			for (Integer slh : literals.values()) {
				assert isHandleValid(slh);
				// context.getConsole().info("SCAN INITL->" + slh);
				pending.add(slh);
			}

			for (IThread thread : context.getThreadManager().getThreads()) {
				thread.fillUsedHandles(pending);
			}

			ArrayList<ClassField> fields = new ArrayList<ClassField>();
			while (!pending.isEmpty()) {
				processing.clear();
				processing.addAll(pending);
				pending.clear();
				for (int handle : processing) {
					if (!used.get(handle)) {
						used.set(handle);
						AbstractClass ac = context.getClass(handle);
						if (ac instanceof ClassBase) {
							fields.clear();
							ClassBase clazz = (ClassBase) ac;
							ClassBase cb = clazz;
							do {
								Collections.addAll(fields, cb.getFields());
							} while ((cb = cb.getSuperClass()) != null);
							for (ClassField field : fields) {
								char type = field.getDescriptor().charAt(0);
								if ((type == 'L' || type == '[')
										&& ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0)) {
									int toadd = getFieldHandle(handle, field);
									if (toadd > 0) {
										assert isHandleValid(toadd);
										// context.getConsole()
										// .info(
										// "SCAN " + handle + "->"
										// + toadd);
										pending.add(toadd);
									}
								}
							}
						} else if (ac instanceof ClassArray) {
							ClassArray clazz = (ClassArray) ac;
							if (getArrayLength(handle) > 0) {
								char type = ac.getName().charAt(1);

								if (type == 'L' || type == '[') {
									int length = getArrayLength(handle);
									ByteBuffer obj = getArrayObj(handle, 0);
									for (int i = 1; i <= length; i++) {
										int toadd = obj.getInt(i << 2);
										if (toadd > 0) {
											assert isHandleValid(toadd);
											// context.getConsole().info(
											// "SCAN " + handle + "->"
											// + toadd);
											pending.add(toadd);
										}
									}
								}
							}
						}
					}
				}
			}
		} catch (VMException e) {
			throw new VMCriticalException(e);
		}
		return used;
	}

	public void gc() throws VMCriticalException {
		long t1 = System.nanoTime();
		BitSet used = scanHeap();
		int rl = 0;
		for (int i = 1; i < MAX_OBJECTS; i++) {
			if (classId[i] >= 0 && !used.get(i)) {
				releaseHandle(i);
				rl++;
			}
		}
		long t2 = System.nanoTime();
	}

	public void loadData(Element data) throws VMCriticalException {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			Element lis = data.element("literals");
			for (Element li : (List<Element>) lis.elements("literal")) {
				literals.put(li.getText(),
						Integer.valueOf(li.attributeValue("handle")));
			}

			Element statics = data.element("statics");
			{
				Element content = statics.element("content");
				baos.reset();
				Base64.decode(content.getText(), baos);
				staticCount = baos.size();
				staticArea.clear();
				staticArea.put(baos.toByteArray());
				staticArea.clear();
				Element map = statics.element("map");
				for (Element st : (List<Element>) map.elements("static")) {
					int cid = Integer.parseInt(st.attributeValue("cid"));
					int pos = Integer.parseInt(st.attributeValue("pos"));
					classStatic[cid] = pos;
				}
			}

			Element objs = data.element("objects");
			handleCount = Integer.parseInt(objs.attributeValue("handleCount"));
			nextHandle = Integer.parseInt(objs.attributeValue("nextHandle"));
			for (Element st : (List<Element>) objs.elements("object")) {
				int handle = Integer.parseInt(st.attributeValue("handle"));
				int cid = Integer.parseInt(st.attributeValue("cid"));
				String text = st.getText();
				baos.reset();
				Base64.decode(text, baos);
				ByteBuffer bb = ByteBuffer.allocateDirect(baos.size());
				bb.put(baos.toByteArray());
				bb.clear();
				objects[handle] = bb;
				classId[handle] = cid;
			}
		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	public void saveData(Element data) {

		// private HashMap<String, Integer> literals = new HashMap<String,
		// Integer>();
		Element li = data.addElement("literals");
		for (Entry<String, Integer> ls : literals.entrySet()) {
			li.addElement("literal")
					.addAttribute("handle", ls.getValue().toString())
					.addText(ls.getKey());
		}

		// private ByteBuffer staticArea = ByteBuffer.allocate(MAX_STATIC);
		// private int staticCount = 0;
		// class id-> pointer in staticArea
		// private int[] classStatic = new int[VMContext.MAX_CLASSES];
		Element cs = data.addElement("statics");
		{
			Element content = cs.addElement("content");
			Element map = cs.addElement("map");
			byte[] buf = new byte[staticCount << 2];
			staticArea.clear();
			staticArea.get(buf, 0, staticCount << 2);
			staticArea.clear();
			content.addText(Base64.encode(buf));
			for (int cid : context.getClassIds()) {
				map.addElement("static")
						.addAttribute("cid", String.valueOf(cid))
						.addAttribute("pos", String.valueOf(classStatic[cid]));

			}
		}

		// private ByteBuffer[] objects = new ByteBuffer[MAX_OBJECTS];
		// handles[handle*2] --> object id
		// handles[handle*2+1] --> class id
		// private int handles[] = new int[MAX_OBJECTS * 2];
		// private int nextHandle;
		// private int handleCount;
		Element objs = data.addElement("objects");
		{
			objs.addAttribute("handleCount", String.valueOf(handleCount));
			objs.addAttribute("nextHandle", String.valueOf(nextHandle));
			for (int i = 1; i < MAX_OBJECTS; i++) {
				int cid = classId[i];
				if (cid == -1) {
					continue;
				}
				ByteBuffer obj = objects[i];
				obj.clear();
				int size = obj.capacity();
				byte[] buf = new byte[size];
				obj.get(buf);
				obj.clear();
				objs.addElement("object")
						.addAttribute("handle", String.valueOf(i))
						.addAttribute("cid", String.valueOf(cid))
						.addText(Base64.encode(buf));
			}
		}

	}
}
