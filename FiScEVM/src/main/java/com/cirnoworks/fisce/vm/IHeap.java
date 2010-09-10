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

	/**
	 * 最大创建的对象的数量（个数）
	 */
	public static final int MAX_OBJECTS = 1048576;
	/**
	 * 最大的静态区的大小（字节）
	 */
	public static final int MAX_STATIC = 1048576;

	/**
	 * 由VMContext调用，将堆管理器绑定到虚拟机上
	 * 
	 * @param context
	 *            要绑定的虚拟机
	 */
	void setContext(VMContext context);

	/**
	 * 保存数据
	 * 
	 * @param data
	 *            保存的目标节点
	 * @throws VMCriticalException
	 *             保存的时候虚拟机出现异常
	 */
	void saveData(Element data) throws VMCriticalException;

	/**
	 * 读取数据
	 * 
	 * @param data
	 *            存放待读取数据的节点
	 * @throws VMCriticalException
	 *             读取的时候虚拟机出现异常
	 */
	void loadData(Element data) throws VMCriticalException;

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
	int allocate(ClassBase clazz) throws VMException, VMCriticalException;

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
	int allocate(ClassArray clazz, int length) throws VMException,
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
	 */
	int getArrayLength(int handle) throws VMException;

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
	boolean getFieldBoolean(int handle, ClassField field) throws VMException;

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
	byte getFieldByte(int handle, ClassField field) throws VMException;

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
	short getFieldShort(int handle, ClassField field) throws VMException;

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
	char getFieldChar(int handle, ClassField field) throws VMException;

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
	int getFieldInt(int handle, ClassField field) throws VMException;

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
	long getFieldLong(int handle, ClassField field) throws VMException;

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
	float getFieldFloat(int handle, ClassField field) throws VMException;

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
	double getFieldDouble(int handle, ClassField field) throws VMException;

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
	int getFieldHandle(int handle, ClassField field) throws VMException;

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
	void putFieldBoolean(int handle, ClassField field, boolean value)
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
	void putFieldByte(int handle, ClassField field, byte value)
			throws VMException;

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
	void putFieldShort(int handle, ClassField field, short value)
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
	void putFieldChar(int handle, ClassField field, char value)
			throws VMException;

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
	void putFieldInt(int handle, ClassField field, int value)
			throws VMException;

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
	void putFieldLong(int handle, ClassField field, long value)
			throws VMException;

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
	void putFieldFloat(int handle, ClassField field, float value)
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
	void putFieldDouble(int handle, ClassField field, double value)
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
	void putFieldHandle(int handle, ClassField field, int value)
			throws VMException;

	/**
	 * 取boolean数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	boolean getArrayBoolean(int handle, int index) throws VMException;

	/**
	 * 取byte数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	byte getArrayByte(int handle, int index) throws VMException;

	/**
	 * 取short数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	short getArrayShort(int handle, int index) throws VMException;

	/**
	 * 取char数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	char getArrayChar(int handle, int index) throws VMException;

	/**
	 * 取int数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	int getArrayInt(int handle, int index) throws VMException;

	/**
	 * 取long数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	long getArrayLong(int handle, int index) throws VMException;

	/**
	 * 取float数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	float getArrayFloat(int handle, int index) throws VMException;

	/**
	 * 取double数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
	double getArrayDouble(int handle, int index) throws VMException;

	/**
	 * 取Object数组中的值
	 * 
	 * @param handle
	 *            数组的句柄
	 * @param index
	 *            位置
	 * @return 值
	 * @throws VMException
	 */
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

	/**
	 * 复制数组的内容
	 * @param srcHandle 源数组
	 * @param srcOfs 源数组的起始点
	 * @param destHandle 目标数组
	 * @param destOfs 目标数组的起始点
	 * @param count 要复制的长度
	 * @throws VMException
	 */
	void arrayCopy(int srcHandle, int srcOfs, int destHandle, int destOfs,
			int count) throws VMException;

	/**
	 * 给类分配静态空间（载入类的时候由VM调用）
	 * @param clazz 要分配空间的类
	 * @throws VMException
	 */
	void initStaticAreaForClass(AbstractClass clazz) throws VMException;

	/**
	 * 取类静态空间的首地址
	 * @param clazz 类
	 * @return 类静态空间的首地址
	 * @throws VMException
	 */
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

	/**
	 * 取字符串常量对象的句柄 <br />
	 * 如果字符串常量不存在就创建一个
	 * @param str 字符串
	 * @return 对应的字符串常量的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public int getLiteral(String str) throws VMException, VMCriticalException;

	/**
	 * 从字符串对象中取得字符串
	 * @param handle 字符串对象的句柄
	 * @return 字符串
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public String getString(int handle) throws VMException, VMCriticalException;

	/**
	 * 给字符串在虚拟机中分配一个对象
	 * @param content 字符串
	 * @return 分配的对象句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public int putString(String content) throws VMException,
			VMCriticalException;

	/**
	 * 句柄是否合法
	 * @param handle 句柄
	 * @return 句柄是否合法
	 */
	boolean isHandleValid(int handle);

	/**
	 * 扫描堆内容，返回一个句柄是否被用到的位集
	 * @return 位集，每一位表示一个对应的句柄是不是被用到
	 * @throws VMCriticalException
	 */
	BitSet scanHeap() throws VMCriticalException;

	/**
	 * 垃圾收集，释放掉不使用的句柄和对应的内存空间
	 * @throws VMCriticalException
	 */
	void gc() throws VMCriticalException;
}
