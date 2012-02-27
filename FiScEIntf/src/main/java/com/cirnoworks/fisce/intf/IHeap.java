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
package com.cirnoworks.fisce.intf;

import java.nio.ByteBuffer;

import com.cirnoworks.fisce.intf.idata.IClassArray;
import com.cirnoworks.fisce.intf.idata.IClassBase;
import com.cirnoworks.fisce.intf.idata.IField;

/**
 * 
 * @author cloudee
 */
public interface IHeap {

	/**
	 * 分配一个对象
	 * 
	 * @param clazz
	 *            对象的类
	 * @return 对象的句柄
	 * @throws VMException
	 *             发生了可以由虚拟机内部处理的异常
	 * @throws VMCriticalException
	 *             发生了虚拟机内部无法处理的异常
	 */
	int allocate(IClassBase clazz) throws VMException, VMCriticalException;

	/**
	 * 分配一个数组
	 * 
	 * @param clazz
	 *            数组的类
	 * @param length
	 *            数组的大小
	 * @return 数组的句柄
	 * @throws VMException
	 *             发生了可以由虚拟机内部处理的异常
	 * @throws VMCriticalException
	 *             发生了虚拟机内部无法处理的异常
	 */
	int allocate(IClassArray clazz, int length) throws VMException,
			VMCriticalException;

	/**
	 * 克隆一个对象
	 * 
	 * @param handle
	 *            被克隆的对象的句柄
	 * @return 新对象的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int clone(int handle) throws VMException, VMCriticalException;

	/**
	 * 返回字符串所对应的字符串常量的句柄。<br />
	 * 如果常量池中没有对应的字符串常量就在常量池中创建一个。
	 * 
	 * @param content
	 *            字符串的内容
	 * @return 常量池中的字符串句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int getInternString(String content) throws VMException, VMCriticalException;

	/**
	 * 获得一个对象所在的类的class id。(class id将在com.cirnoworks.fisce.vm.VMContext中被映射到类)
	 * 
	 * @param handle
	 *            对象的句柄
	 * @return 类的class id。
	 */
	int getClass(int handle) throws VMException;

	/**
	 * 获取数组的长度
	 * 
	 * @param handle
	 *            数组所在的句柄
	 * @return 数组的长度
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int getArrayLength(int handle) throws VMException, VMCriticalException;

	/**
	 * 获得对象的成员变量的布尔值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的布尔值
	 * @throws VMException
	 */
	boolean getFieldBoolean(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的byte值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的byte值
	 * @throws VMException
	 */
	byte getFieldByte(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的short值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的short值
	 * @throws VMException
	 */
	short getFieldShort(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的char值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的char值
	 * @throws VMException
	 */
	char getFieldChar(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的int值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的int值
	 * @throws VMException
	 */
	int getFieldInt(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的long值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的long值
	 * @throws VMException
	 */
	long getFieldLong(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的float值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的float值
	 * @throws VMException
	 */
	float getFieldFloat(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的double值
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的double值
	 * @throws VMException
	 */
	double getFieldDouble(int handle, IField field) throws VMException;

	/**
	 * 获得对象的成员变量的句柄
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @return 成员变量的句柄
	 * @throws VMException
	 */
	int getFieldHandle(int handle, IField field) throws VMException;

	/**
	 * 设定对象成员变量的值（boolean）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldBoolean(int handle, IField field, boolean value)
			throws VMException;

	/**
	 * 设定对象成员变量的值（byte）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldByte(int handle, IField field, byte value) throws VMException;

	/**
	 * 设定对象成员变量的值（short）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldShort(int handle, IField field, short value)
			throws VMException;

	/**
	 * 设定对象成员变量的值（char）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldChar(int handle, IField field, char value) throws VMException;

	/**
	 * 设定对象成员变量的值（int）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldInt(int handle, IField field, int value) throws VMException;

	/**
	 * 设定对象成员变量的值（long）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldLong(int handle, IField field, long value) throws VMException;

	/**
	 * 设定对象成员变量的值（float）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldFloat(int handle, IField field, float value)
			throws VMException;

	/**
	 * 设定对象成员变量的值（double）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldDouble(int handle, IField field, double value)
			throws VMException;

	/**
	 * 设定对象成员变量的值（handle）
	 * 
	 * @param handle
	 *            对象的句柄
	 * @param field
	 *            成员变量
	 * @param value
	 *            值
	 * @throws VMException
	 */
	void putFieldHandle(int handle, IField field, int value) throws VMException;

	/**
	 * 取boolean数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	boolean getArrayBoolean(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取byte数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	byte getArrayByte(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取short数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	short getArrayShort(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取char数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	char getArrayChar(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取int数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int getArrayInt(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取long数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	long getArrayLong(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取float数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	float getArrayFloat(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取double数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	double getArrayDouble(int handle, int index) throws VMException,
			VMCriticalException;

	/**
	 * 取Object数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int getArrayHandle(int handle, int index) throws VMException,
			VMCriticalException;

	void putArrayBoolean(int handle, int index, boolean value)
			throws VMException, VMCriticalException;

	void putArrayByte(int handle, int index, byte value) throws VMException,
			VMCriticalException;

	void putArrayShort(int handle, int index, short value) throws VMException,
			VMCriticalException;

	void putArrayChar(int handle, int index, char value) throws VMException,
			VMCriticalException;

	void putArrayInt(int handle, int index, int value) throws VMException,
			VMCriticalException;

	void putArrayLong(int handle, int index, long value) throws VMException,
			VMCriticalException;

	void putArrayFloat(int handle, int index, float value) throws VMException,
			VMCriticalException;

	void putArrayDouble(int handle, int index, double value)
			throws VMException, VMCriticalException;

	void putArrayHandle(int handle, int index, int value) throws VMException,
			VMCriticalException;

	void fillArrayBoolean(int handle, int dstPos, boolean[] src, int srcPos,
			int len) throws VMException, VMCriticalException;

	void fillArrayByte(int handle, int dstPos, byte[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayShort(int handle, int dstPos, short[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayChar(int handle, int dstPos, char[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayInt(int handle, int dstPos, int[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayLong(int handle, int dstPos, long[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayFloat(int handle, int dstPos, float[] src, int srcPos, int len)
			throws VMException, VMCriticalException;

	void fillArrayDouble(int handle, int dstPos, double[] src, int srcPos,
			int len) throws VMException, VMCriticalException;

	void getArrayBoolean(boolean[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException;

	void getArrayByteBuffer(ByteBuffer dst, int handle, int srcPos, int len);

	void getArrayByte(byte[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayShort(short[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayChar(char[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayInt(int[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayLong(long[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayFloat(float[] dst, int dstPos, int handle, int srcPos, int len)
			throws VMException, VMCriticalException;

	void getArrayDouble(double[] dst, int dstPos, int handle, int srcPos,
			int len) throws VMException, VMCriticalException;

	public boolean getStaticBoolean(IField field) throws VMException;

	public byte getStaticByte(IField field) throws VMException;

	public short getStaticShort(IField field) throws VMException;

	public char getStaticChar(IField field) throws VMException;

	public int getStaticInt(IField field) throws VMException;

	public long getStaticLong(IField field) throws VMException;

	public float getStaticFloat(IField field) throws VMException;

	public double getStaticDouble(IField field) throws VMException;

	public void setStaticBoolean(IField field, boolean value)
			throws VMException;

	public void setStaticByte(IField field, byte value) throws VMException;

	public void setStaticShort(IField field, short value) throws VMException;

	public void setStaticChar(IField field, char value) throws VMException;

	public void setStaticInt(IField field, int value) throws VMException;

	public void setStaticLong(IField field, long value) throws VMException;

	public void setStaticFloat(IField field, float value) throws VMException;

	public void setStaticDouble(IField field, double value) throws VMException;

	/**
	 * 取字符串常量对象的句柄 <br />
	 * 如果字符串常量不存在就创建一个
	 * 
	 * @param str
	 *            字符串
	 * @return 对应的字符串常量的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public int getLiteral(String str) throws VMException, VMCriticalException;

	/**
	 * 从字符串对象中取得字符串
	 * 
	 * @param handle
	 *            字符串对象的句柄
	 * @return 字符串
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public String getString(int handle) throws VMException, VMCriticalException;

	/**
	 * 给字符串在虚拟机中分配一个对象
	 * 
	 * @param content
	 *            字符串
	 * @return 分配的对象句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public int putString(String content) throws VMException,
			VMCriticalException;

}