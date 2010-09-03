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
package com.cirnoworks.fisce.vm;

import java.util.BitSet;

import org.dom4j.Element;

import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;

/**
 * 
 * @author cloudee
 */
public interface IHeap {

	public static final int MAX_OBJECTS = 65536;
	public static final int MAX_STATIC = 1048576;

	void setContext(VMContext context);

	void saveData(Element data) throws VMCriticalException;

	void loadData(Element data) throws VMCriticalException;

	int allocate(ClassBase clazz) throws VMException, VMCriticalException;

	int allocate(ClassArray clazz, int length) throws VMException,
			VMCriticalException;
	
	int clone(int handle) throws VMException, VMCriticalException;

	int getInternString(String content) throws VMException, VMCriticalException;

	/**
	 * get class id of an object
	 * 
	 * @param handle
	 *            handle of the object
	 * @return the class id
	 */
	int getClass(int handle) throws VMException;

	int getArrayLength(int handle) throws VMException;

	boolean getFieldBoolean(int handle, ClassField field) throws VMException;

	byte getFieldByte(int handle, ClassField field) throws VMException;

	short getFieldShort(int handle, ClassField field) throws VMException;

	char getFieldChar(int handle, ClassField field) throws VMException;

	int getFieldInt(int handle, ClassField field) throws VMException;

	long getFieldLong(int handle, ClassField field) throws VMException;

	float getFieldFloat(int handle, ClassField field) throws VMException;

	double getFieldDouble(int handle, ClassField field) throws VMException;

	int getFieldHandle(int handle, ClassField field) throws VMException;

	void putFieldBoolean(int handle, ClassField field, boolean value)
			throws VMException;

	void putFieldByte(int handle, ClassField field, byte value)
			throws VMException;

	void putFieldShort(int handle, ClassField field, short value)
			throws VMException;

	void putFieldChar(int handle, ClassField field, char value)
			throws VMException;

	void putFieldInt(int handle, ClassField field, int value)
			throws VMException;

	void putFieldLong(int handle, ClassField field, long value)
			throws VMException;

	void putFieldFloat(int handle, ClassField field, float value)
			throws VMException;

	void putFieldDouble(int handle, ClassField field, double value)
			throws VMException;

	void putFieldHandle(int handle, ClassField field, int value)
			throws VMException;

	boolean getArrayBoolean(int handle, int index) throws VMException;

	byte getArrayByte(int handle, int index) throws VMException;

	short getArrayShort(int handle, int index) throws VMException;

	char getArrayChar(int handle, int index) throws VMException;

	int getArrayInt(int handle, int index) throws VMException;

	long getArrayLong(int handle, int index) throws VMException;

	float getArrayFloat(int handle, int index) throws VMException;

	double getArrayDouble(int handle, int index) throws VMException;

	int getArrayHandle(int handle, int index) throws VMException;

	void putArrayBoolean(int handle, int index, boolean value)
			throws VMException;

	void putArrayByte(int handle, int index, byte value) throws VMException;

	void putArrayShort(int handle, int index, short value) throws VMException;

	void putArrayChar(int handle, int index, char value) throws VMException;

	void putArrayInt(int handle, int index, int value) throws VMException;

	void putArrayLong(int handle, int index, long value) throws VMException;

	void putArrayFloat(int handle, int index, float value) throws VMException;

	void putArrayDouble(int handle, int index, double value) throws VMException;

	void putArrayHandle(int handle, int index, int value) throws VMException;

	void arrayCopy(int srcHandle, int srcOfs, int destHandle, int destOfs,
			int count) throws VMException;

	void initStaticAreaForClass(AbstractClass clazz) throws VMException;

	int getClazzStaticPos(AbstractClass clazz) throws VMException;

	public boolean getStaticBoolean(ClassField field) throws VMException;

	public byte getStaticByte(ClassField field) throws VMException;

	public char getStaticShort(ClassField field) throws VMException;

	public char getStaticChar(ClassField field) throws VMException;

	public int getStaticInt(ClassField field) throws VMException;

	public long getStaticLong(ClassField field) throws VMException;

	public float getStaticFloat(ClassField field) throws VMException;

	public double getStaticDouble(ClassField field) throws VMException;

	public void setStaticBoolean(ClassField field, boolean value)
			throws VMException;

	public void setStaticByte(ClassField field, byte value) throws VMException;

	public void setStaticShort(ClassField field, char value) throws VMException;

	public void setStaticChar(ClassField field, char value) throws VMException;

	public void setStaticInt(ClassField field, int value) throws VMException;

	public void setStaticLong(ClassField field, long value) throws VMException;

	public void setStaticFloat(ClassField field, float value)
			throws VMException;

	public void setStaticDouble(ClassField field, double value)
			throws VMException;

	public int getLiteral(String str) throws VMException, VMCriticalException;

	public String getString(int handle) throws VMException, VMCriticalException;

	public int putString(String content) throws VMException,
			VMCriticalException;

	boolean isHandleValid(int handle);

	BitSet scanHeap() throws VMCriticalException;

	void gc() throws VMCriticalException;
}
