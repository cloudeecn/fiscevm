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
package com.cirnoworks.fisce.vm.default_impl;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map.Entry;
import java.util.Set;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IClassArray;
import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.intf.idata.IField;
import com.cirnoworks.fisce.util.Base64;
import com.cirnoworks.fisce.util.BufferUtil;
import com.cirnoworks.fisce.util.DOMHelper;
import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.JHeap;
import com.cirnoworks.fisce.vm.JThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;

public final class DefaultHeap implements JHeap {

	// not persist
	private VMContext context;

	// persist
	// String -> String handle
	private HashMap<String, Integer> literals = new HashMap<String, Integer>();
	private ByteBuffer staticArea = BufferUtil.createBuffer(MAX_STATIC);
	private int staticCount = 0;
	// class id-> pointer in staticArea
	private int[] classStatic = new int[VMContext.MAX_CLASSES];
	// objects[handle] -> data
	private ByteBuffer[] objects = new ByteBuffer[MAX_OBJECTS];
	// handles[handle] --> class id
	private int classId[] = new int[MAX_OBJECTS];
	private int nextHandle;
	private ArrayList<Integer> toFinalize = new ArrayList<Integer>();
	private BitSet finalized = new BitSet(MAX_OBJECTS);
	// no presist
	private int handleCount;
	// temp var for loading
	private int[] literalHandles;

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

	private void releaseHandle(int handle) throws VMException {
		assert isArray(handle) || finalized.get(handle);
		objects[handle] = null;
		classId[handle] = -1;
		handleCount--;
		finalized.set(handle, false);
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

		assert !finalized.get(h);
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

	private ByteBuffer getArrayObj(int handle) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert isArray(handle);
		ByteBuffer obj = objects[handle];
		// if (index >= obj.getInt(0)) {
		// throw new VMException("java/lang/ArrayIndexOutOfBoundsException",
		// String.valueOf(index));
		// }
		return obj;
	}

	private ByteBuffer getObj(int handle, ClassField field) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert validate(handle, field);
		return objects[handle];
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
		return objects[handle].capacity() >> ((ClassArray) context
				.getClazzById(classId[handle])).getSizeShift();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#allocate(com.cirnoworks.fisce.vm.
	 * data.ClassBase)
	 */
	public int allocate(IClassBase clazz) throws VMException,
			VMCriticalException {
		ByteBuffer obj = BufferUtil.createBuffer(((ClassBase) clazz)
				.getTotalSizeInHeap() * 4);
		return allocate(context.getClazzId(clazz), obj);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#allocate(com.cirnoworks.fisce.vm.
	 * data.ClassArray, int)
	 */
	public int allocate(IClassArray clazz, int length) throws VMException,
			VMCriticalException {
		assert clazz.getName().startsWith("[");
		int size = length << ((ClassArray) clazz).getSizeShift();
		ByteBuffer obj = BufferUtil.createBuffer(size);
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
		ByteBuffer clone = BufferUtil.createBuffer(target.capacity());
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
	public boolean getFieldBoolean(int handle, IField iField)
			throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getInt(field.getAbsPos() << 2) > 0;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldByte(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public byte getFieldByte(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return (byte) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldShort(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public short getFieldShort(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return (short) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldChar(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public char getFieldChar(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return (char) getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldInt(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public int getFieldInt(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldLong(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public long getFieldLong(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getLong(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldFloat(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public float getFieldFloat(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getFloat(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldDouble(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public double getFieldDouble(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getDouble(field.getAbsPos() << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#getFieldHandle(int,
	 * com.cirnoworks.fisce.vm.data.ClassField)
	 */
	public int getFieldHandle(int handle, IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		return getObj(handle, field).getInt(field.getAbsPos() << 2);
	}

	public int getFieldAbs(int handle, int pos) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		return objects[handle].getInt(pos << 2);
	}

	public long getFieldAbsWide(int handle, int pos) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		return objects[handle].getLong(pos << 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldBoolean(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, boolean)
	 */
	public void putFieldBoolean(int handle, IField iField, boolean value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putInt(field.getAbsPos() << 2, value ? 1 : 0);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldByte(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, byte)
	 */
	public void putFieldByte(int handle, IField iField, byte value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldShort(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, char)
	 */
	public void putFieldShort(int handle, IField iField, short value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldChar(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, char)
	 */
	public void putFieldChar(int handle, IField iField, char value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldInt(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, int)
	 */
	public void putFieldInt(int handle, IField iField, int value)
			throws VMException {
		ClassField field = (ClassField) iField;
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
	public void putFieldLong(int handle, IField iField, long value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putLong(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldFloat(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, float)
	 */
	public void putFieldFloat(int handle, IField iField, float value)
			throws VMException {
		ClassField field = (ClassField) iField;
		assert validate(handle, field);
		getObj(handle, field).putFloat(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldDouble(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, double)
	 */
	public void putFieldDouble(int handle, IField iField, double value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putDouble(field.getAbsPos() << 2, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.IHeap#setFieldHandle(int,
	 * com.cirnoworks.fisce.vm.data.ClassField, int)
	 */
	public void putFieldHandle(int handle, IField iField, int value)
			throws VMException {
		ClassField field = (ClassField) iField;
		getObj(handle, field).putInt(field.getAbsPos() << 2, value);
	}

	public void putFieldAbs(int handle, int pos, int value) throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		objects[handle].putInt(pos << 2, value);
	}

	public void putFieldAbsWide(int handle, int pos, long value)
			throws VMException {
		if (handle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		objects[handle].putLong(pos << 2, value);
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
		return getArrayObj(handle).get(index) > 0;
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
		return (byte) getArrayObj(handle).get(index);
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
		return (short) getArrayObj(handle).getInt((index << 2));
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
		return (char) getArrayObj(handle).getInt((index << 2));
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
		return getArrayObj(handle).getInt((index << 2));
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
		return getArrayObj(handle).getLong((index << 3));
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
		return getArrayObj(handle).getFloat((index << 2));
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
		return getArrayObj(handle).getDouble((index << 3));
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
		return getArrayObj(handle).getInt((index << 2));
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
		getArrayObj(handle).put(index, (byte) (value ? 1 : 0));

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
		getArrayObj(handle).put(index, value);
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
		getArrayObj(handle).putInt((index << 2), value);
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
		getArrayObj(handle).putInt((index << 2), value);
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
		getArrayObj(handle).putInt((index << 2), value);
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
		getArrayObj(handle).putLong((index << 3), value);
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
		getArrayObj(handle).putFloat((index << 2), value);
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
		getArrayObj(handle).putDouble((index << 3), value);
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
		getArrayObj(handle).putInt((index << 2), value);
	}

	private void validateFillArray(int handle, int dstPos, int srcSize,
			int srcPos, int len) throws VMException {
		assert isArray(handle);
		int dstSize = getArrayLength(handle);
		if (dstPos < 0 || dstPos + len > dstSize) {
			throw new VMException("java/lang/IndexOutOfBoundsException",
					"fill:dst");
		}
		if (srcPos < 0 || srcPos + len > srcSize) {
			throw new VMException("java/lang/IndexOutOfBoundsException",
					"fill:src");
		}
	}

	public void fillArrayBoolean(int handle, int dstPos, boolean[] src,
			int srcPos, int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			obj.put(dstPos + i, (byte) (src[srcPos + i] ? 1 : 0));
		}
	}

	public void getArrayBoolean(boolean[] dst, int dstPos, int handle,
			int srcPos, int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			dst[dstPos + i] = obj.get(srcPos + i) > 0;
		}
	}

	public void fillArrayByte(int handle, int dstPos, byte[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		ByteBuffer obj = getArrayObj(handle);
		obj = obj.duplicate();
		obj.position(dstPos);
		obj.put(src, srcPos, len);
	}

	public void getArrayByte(byte[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		ByteBuffer obj = getArrayObj(handle);
		obj = obj.duplicate();
		obj.position(srcPos);
		obj.get(dst, dstPos, len);
	}

	public void fillArrayShort(int handle, int dstPos, short[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			obj.putInt((dstPos + i) << 2, src[srcPos + i]);
		}
	}

	public void getArrayShort(short[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			dst[dstPos + i] = (short) obj.getInt((srcPos + i) << 2);

		}
	}

	public void fillArrayChar(int handle, int dstPos, char[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			obj.putInt((dstPos + i) << 2, src[srcPos + i]);
		}
	}

	public void getArrayChar(char[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		ByteBuffer obj = getArrayObj(handle);
		for (int i = 0; i < len; i++) {
			dst[dstPos + i] = (char) obj.getInt((srcPos + i) << 2);
		}
	}

	public void fillArrayInt(int handle, int dstPos, int[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		IntBuffer obj = getArrayObj(handle).asIntBuffer();
		obj.position(dstPos);
		obj.put(src, srcPos, len);
	}

	public void getArrayInt(int[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		IntBuffer obj = getArrayObj(handle).asIntBuffer();
		obj.position(srcPos);
		obj.get(dst, dstPos, len);
	}

	public void fillArrayLong(int handle, int dstPos, long[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		LongBuffer obj = getArrayObj(handle).asLongBuffer();
		obj.position(dstPos);
		obj.put(src, srcPos, len);
	}

	public void getArrayLong(long[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		LongBuffer obj = getArrayObj(handle).asLongBuffer();
		obj.position(srcPos);
		obj.get(dst, dstPos, len);
	}

	public void fillArrayFloat(int handle, int dstPos, float[] src, int srcPos,
			int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		FloatBuffer obj = getArrayObj(handle).asFloatBuffer();
		obj.position(dstPos);
		obj.put(src, srcPos, len);
	}

	public void getArrayFloat(float[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		FloatBuffer obj = getArrayObj(handle).asFloatBuffer();
		obj.position(srcPos);
		obj.get(dst, dstPos, len);
	}

	public void fillArrayDouble(int handle, int dstPos, double[] src,
			int srcPos, int len) throws VMException {
		validateFillArray(handle, dstPos, src.length, srcPos, len);
		DoubleBuffer obj = getArrayObj(handle).asDoubleBuffer();
		obj.position(dstPos);
		obj.put(src, srcPos, len);
	}

	public void getArrayDouble(double[] dst, int dstPos, int handle,
			int srcPos, int len) throws VMException, VMCriticalException {
		validateFillArray(handle, srcPos, dst.length, dstPos, len);
		DoubleBuffer obj = getArrayObj(handle).asDoubleBuffer();
		obj.position(srcPos);
		obj.get(dst, dstPos, len);
	}

	/*
	 * Integer> classMap = new HashMap<String, Integer>(); // Class name ->
	 * Class o
	 */
	public boolean getStaticBoolean(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getInt(field.getAbsPos() << 2) > 0;
	}

	public byte getStaticByte(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (byte) staticArea.getInt(field.getAbsPos() << 2);
	}

	public short getStaticShort(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (short) staticArea.getInt(field.getAbsPos() << 2);
	}

	public char getStaticChar(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return (char) staticArea.getInt(field.getAbsPos() << 2);
	}

	public int getStaticInt(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getInt(field.getAbsPos() << 2);
	}

	public long getStaticLong(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getLong(field.getAbsPos() << 2);
	}

	public float getStaticFloat(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getFloat(field.getAbsPos() << 2);
	}

	public double getStaticDouble(IField iField) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		return staticArea.getDouble(field.getAbsPos() << 2);
	}

	public int getStaticAbs(int pos) throws VMException {
		return staticArea.getInt(pos << 2);
	}

	public long getStaticAbsWide(int pos) throws VMException {
		return staticArea.getLong(pos << 2);
	}

	public void setStaticBoolean(IField iField, boolean value)
			throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value ? 1 : 0);
	}

	public void setStaticByte(IField iField, byte value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticShort(IField iField, short value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticChar(IField iField, char value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticInt(IField iField, int value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putInt(field.getAbsPos() << 2, value);
	}

	public void setStaticLong(IField iField, long value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putLong(field.getAbsPos() << 2, value);
	}

	public void setStaticFloat(IField iField, float value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putFloat(field.getAbsPos() << 2, value);
	}

	public void setStaticDouble(IField iField, double value) throws VMException {
		ClassField field = (ClassField) iField;
		if ((field.getAccessFlags() & AbstractClass.ACC_STATIC) == 0) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"get/set static field is not static!");
		}
		staticArea.putDouble(field.getAbsPos() << 2, value);
	}

	public void setStaticAbs(int pos, int value) {
		staticArea.putInt(pos << 2, value);
	}

	public void setStaticAbsWide(int pos, long value) {
		staticArea.putLong(pos << 2, value);
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
		AbstractClass srcClass = (AbstractClass) context.getClass(srcHandle);
		AbstractClass dstClass = (AbstractClass) context.getClass(dstHandle);
		if (srcHandle == 0 || dstHandle == 0) {
			throw new VMException("java/lang/NullPointerException", "");
		}
		assert isArray(srcHandle) && isArray(dstHandle);
		// TODO more study in arrayCopy
		// if (!srcClass.canCastTo(dstClass)) {
		// throw new VMException("java/lang/ArrayStoreException", "");
		// }
		int sizeShift = ((ClassArray) srcClass).getSizeShift();
		/*
		 * ByteBuffer srcbb = getArrayObj(srcHandle, srcOfs + count); ByteBuffer
		 * dstbb = getArrayObj(dstHandle, dstOfs + count); ByteBuffer d =
		 * dstbb.duplicate(); ByteBuffer s = srcbb.duplicate();
		 * d.position(dstOfs); s.position(srcOfs); s.limit(srcOfs + count *
		 * size); d.put(s);
		 */
		for (int i = 0; i < count; i++) {
			switch (sizeShift) {
			case 0:
				putArrayByte(dstHandle, i + dstOfs,
						getArrayByte(srcHandle, i + srcOfs));
				break;
			case 2:
				putArrayInt(dstHandle, i + dstOfs,
						getArrayInt(srcHandle, i + srcOfs));
				break;
			case 3:
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

	public void scanInit(BitSet used) throws VMException, VMCriticalException {
		// BitSet touched = new BitSet(MAX_OBJECTS);
		// Set<Integer> pending = new HashSet<Integer>();
		// Class object
		// Class static
		// Threads holding
		// String lit.
		// toFinialized
		for (AbstractClass clazz : context.getClasses()) {
			if (clazz == null) {
				continue;
			}
			int handle = context.getClassObjectHandleForClass(clazz);
			// Class object
			assert isHandleValid(handle) : "handle " + handle + " is invalid";
			assert context.getConsole().info(
					"SCAN INITC->" + handle + " " + clazz.getName());
			// pending.add(handle);
			used.set(handle);
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
							assert context.getConsole().info(
									"SCAN INITS->" + handleSR + " "
											+ clazz.getName() + " "
											+ field.getUniqueName() + " "
											+ field.getAbsPos() + " NULL");
							continue;
						}
						assert isHandleValid(handleSR);
						assert context.getConsole().info(
								"SCAN INITS->" + handleSR + " "
										+ clazz.getName() + " "
										+ field.getAbsPos() + " "
										+ field.getUniqueName());
						// pending.add(handleSR);
						used.set(handleSR);
					}
				}
			}
		}
		for (Integer slh : literals.values()) {
			assert isHandleValid(slh);
			assert context.getConsole().info("SCAN INITL->" + slh);
			used.set(slh);
			// pending.add(slh);
		}

		for (IThread thread : context.getThreadManager().getThreads()) {
			((JThread) thread).fillUsedHandles(used);
		}

		for (Integer in : toFinalize) {
			assert isHandleValid(in) : "handle " + in + " is invalid";
			assert context.getConsole().info("SCAN INITF->" + in);
			used.set(in);
		}

	}

	private void scanRef(BitSet used, BitSet init) throws VMException,
			VMCriticalException {
		// BitSet used = new BitSet(MAX_OBJECTS);
		Set<Integer> pending = new HashSet<Integer>();
		for (int i = 0; i < MAX_OBJECTS; i++) {
			if (init.get(i)) {
				pending.add(i);
			}
		}
		Set<Integer> processing = new HashSet<Integer>();
		ArrayList<ClassField> fields = new ArrayList<ClassField>();
		while (!pending.isEmpty()) {
			processing.clear();
			processing.addAll(pending);
			pending.clear();
			for (int handle : processing) {
				if (!used.get(handle)) {
					used.set(handle);
					AbstractClass ac = (AbstractClass) context.getClass(handle);
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
									assert isHandleValid(toadd) : "invalid handle "
											+ toadd
											+ " in class "
											+ ac.getName()
											+ "."
											+ field.getName() + " " + handle;
									assert context.getConsole().info(
											"SCAN " + handle + "->" + toadd);
									pending.add(toadd);
								}
							}
						}
					} else if (ac instanceof ClassArray) {
						// ClassArray clazz = (ClassArray) ac;
						if (getArrayLength(handle) > 0) {
							char type = ac.getName().charAt(1);

							if (type == 'L' || type == '[') {
								int length = getArrayLength(handle);
								// int[] obj = getArrayObjInt(handle);
								for (int i = 0; i < length; i++) {
									int toadd = getArrayHandle(handle, i);
									if (toadd > 0) {
										assert isHandleValid(toadd) : "invalid handle "
												+ toadd
												+ " in array"
												+ ac.getName() + " " + handle;
										assert context.getConsole()
												.info("SCAN " + handle + "->"
														+ toadd);
										pending.add(toadd);
									}
								}
							}
						}
					} else {
						throw new VMException("java/lang/Error",
								"Unsupported class type");
					}
				}
			}
		}
	}

	public void gc() throws VMCriticalException, VMException {
		long t1 = System.nanoTime();

		int rl = 0, fi = 0;
		BitSet used = new BitSet(MAX_OBJECTS);
		BitSet add = new BitSet(MAX_OBJECTS);
		scanInit(add);
		scanRef(used, add);
		add.clear();
		for (int i = 1; i < MAX_OBJECTS; i++) {
			if (classId[i] >= 0 && !used.get(i) && !finalized.get(i)
					&& !isArray(i)) {
				toFinalize.add(i);
				finalized.set(i);
				// used.set(i);
				add.set(i);
				fi++;
			}
		}
		scanRef(used, add);
		for (int i = 1; i < MAX_OBJECTS; i++) {
			if (classId[i] >= 0 && !used.get(i)) {
				rl++;
				releaseHandle(i);
			}
		}
		long t2 = System.nanoTime();
		System.out.println("#GC: to finalize " + fi + "objects, release " + rl
				+ " objects in " + (t2 - t1) / 1000000f + "ms, " + handleCount
				+ " objects in use");
	}

	public int[] getToFinialize() {
		int size = toFinalize.size();
		int[] ret = new int[size];
		for (int i = 0; i < size; i++) {
			ret[i] = toFinalize.get(i);
		}
		toFinalize.clear();
		return ret;
	}

	public void loadData(Element data) throws VMCriticalException {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			NodeList lis = ((Element) data.getElementsByTagName("literals")
					.item(0)).getElementsByTagName("literal");
			literalHandles = new int[lis.getLength()];
			for (int i = 0, max = lis.getLength(); i < max; i++) {
				Element li = (Element) lis.item(i);
				literalHandles[i] = Integer.valueOf(li.getAttribute("handle"));
			}

			Element statics = (Element) data.getElementsByTagName("statics")
					.item(0);
			{
				Element content = (Element) statics.getElementsByTagName(
						"content").item(0);
				baos.reset();
				Base64.decode(DOMHelper.getTextContent(content), baos);
				staticCount = baos.size() / 4;
				staticArea.clear();
				staticArea.put(baos.toByteArray());
				staticArea.clear();
				NodeList map = ((Element) statics.getElementsByTagName("map")
						.item(0)).getElementsByTagName("static");
				for (int i = 0, max = map.getLength(); i < max; i++) {
					Element st = (Element) map.item(i);
					int cid = Integer.parseInt(st.getAttribute("cid"));
					int pos = Integer.parseInt(st.getAttribute("pos"));
					classStatic[cid] = pos;
				}
			}

			Element objs = (Element) data.getElementsByTagName("objects").item(
					0);
			handleCount = Integer.parseInt(objs.getAttribute("handleCount"));
			nextHandle = Integer.parseInt(objs.getAttribute("nextHandle"));
			NodeList objectElements = objs.getElementsByTagName("object");
			for (int i = 0, max = objectElements.getLength(); i < max; i++) {
				Element st = (Element) objectElements.item(i);
				int handle = Integer.parseInt(st.getAttribute("handle"));
				int cid = Integer.parseInt(st.getAttribute("cid"));
				String text = DOMHelper.getTextContent(st);
				baos.reset();
				Base64.decode(text, baos);
				ByteBuffer bb = BufferUtil.createBuffer(baos.size());
				bb.put(baos.toByteArray());
				bb.clear();
				objects[handle] = bb;
				classId[handle] = cid;
			}

			Element finialize = (Element) data
					.getElementsByTagName("finialize").item(0);
			toFinalize.clear();
			List<Element> pendings = DOMHelper.elements(finialize, "pending");
			for (Element pending : pendings) {
				String shandle = pending.getAttribute("handle");
				Integer handle = Integer.valueOf(shandle);
				toFinalize.add(handle);
			}
			finalized.clear();
			List<Element> finializeds = DOMHelper.elements(finialize,
					"finialized");
			for (Element finalizedElement : finializeds) {
				String shandle = finalizedElement.getAttribute("handle");
				int handle = Integer.parseInt(shandle);
				finalized.set(handle);
			}

		} catch (Exception e) {
			throw new VMCriticalException(e);
		}
	}

	public void rebuildLiteral() throws VMException, VMCriticalException {
		for (int handle : literalHandles) {
			String key = getString(handle);
			literals.put(key, handle);
		}
		literalHandles = null;
	}

	public void saveData(Element data) {

		// private HashMap<String, Integer> literals = new HashMap<String,
		// Integer>();
		Document document = data.getOwnerDocument();
		Element li = document.createElement("literals");
		data.appendChild(li);
		for (Entry<String, Integer> ls : literals.entrySet()) {
			Element lit = document.createElement("literal");
			li.appendChild(lit);

			lit.setAttribute("handle", ls.getValue().toString());
			// DOMHelper.setTextContent(lit, ls.getKey());
		}

		// private ByteBuffer staticArea = BufferUtil.createBuffer(MAX_STATIC);
		// private int staticCount = 0;
		// class id-> pointer in staticArea
		// private int[] classStatic = new int[VMContext.MAX_CLASSES];
		Element cs = document.createElement("statics");
		data.appendChild(cs);
		{
			Element content = document.createElement("content");// cs.addElement("content");
			Element map = document.createElement("map");// cs.addElement("map");
			cs.appendChild(content);
			cs.appendChild(map);
			byte[] buf = new byte[staticCount << 2];
			staticArea.clear();
			staticArea.get(buf, 0, staticCount << 2);
			staticArea.clear();
			DOMHelper.setTextContent(content, Base64.encode(buf));
			for (int cid : context.getClassIds()) {
				Element st = document.createElement("static");
				map.appendChild(st);
				st.setAttribute("cid", String.valueOf(cid));
				st.setAttribute("pos", String.valueOf(classStatic[cid]));

			}
		}

		// private ByteBuffer[] objects = new ByteBuffer[MAX_OBJECTS];
		// handles[handle*2] --> object id
		// handles[handle*2+1] --> class id
		// private int handles[] = new int[MAX_OBJECTS * 2];
		// private int nextHandle;
		// private int handleCount;
		Element objs = document.createElement("objects");
		data.appendChild(objs);
		{
			objs.setAttribute("handleCount", String.valueOf(handleCount));
			objs.setAttribute("nextHandle", String.valueOf(nextHandle));
			for (int i = 1; i < MAX_OBJECTS; i++) {
				int cid = classId[i];
				if (cid == -1) {
					continue;
				}
				AbstractClass clazz = context.getClazzById(cid);
				char type;
				if (clazz instanceof ClassBase) {
					type = IClassLoader.TYPE_INT;
				} else if (clazz instanceof ClassArray) {
					type = clazz.getName().charAt(1);
				} else {
					type = 0;
				}
				ByteBuffer obj = objects[i];
				obj.clear();
				int size = obj.capacity();
				byte[] buf = new byte[size];
				obj.get(buf);
				obj.clear();
				Element ele = document.createElement("object");
				objs.appendChild(ele);
				ele.setAttribute("handle", String.valueOf(i));
				ele.setAttribute("cid", String.valueOf(cid));
				ele.setAttribute("type", String.valueOf(type));
				DOMHelper.setTextContent(ele, Base64.encode(buf));
			}
		}
		Element finialize = document.createElement("finialize");
		data.appendChild(finialize);
		{
			for (Integer in : toFinalize) {
				Element ele = document.createElement("pending");
				finialize.appendChild(ele);
				ele.setAttribute("handle", in.toString());
			}
			for (int i = 0; i < MAX_OBJECTS; i++) {
				if (finalized.get(i)) {
					Element ele = document.createElement("finialized");
					finialize.appendChild(ele);
					ele.setAttribute("handle", String.valueOf(i));
				}
			}
		}
	}

}
