package com.cirnoworks.fisce.vm;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.data.AbstractClass;

public interface JHeap extends IHeap {
	/**
	 * 最大创建的对象的数量（个数）
	 */
	public static final int MAX_OBJECTS = 65536;
	/**
	 * 最大的静态区的大小（字节）
	 */
	public static final int MAX_STATIC = 131072;

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
	 * 重建字符串常量表 在读取的最后阶段调用
	 * 
	 * @throws VMCriticalException
	 *             虚拟机异常
	 * @throws VMException
	 *             虚拟机异常
	 */
	void rebuildLiteral() throws VMCriticalException, VMException;

	/**
	 * 复制数组的内容
	 * 
	 * @param srcHandle
	 *            源数组
	 * @param srcOfs
	 *            源数组的起始点
	 * @param destHandle
	 *            目标数组
	 * @param destOfs
	 *            目标数组的起始点
	 * @param count
	 *            要复制的长度
	 * @throws VMException
	 */
	void arrayCopy(int srcHandle, int srcOfs, int destHandle, int destOfs,
			int count) throws VMException;

	/**
	 * 给类分配静态空间（载入类的时候由VM调用）
	 * 
	 * @param clazz
	 *            要分配空间的类
	 * @throws VMException
	 */
	void initStaticAreaForClass(AbstractClass clazz) throws VMException;

	/**
	 * 取类静态空间的首地址
	 * 
	 * @param clazz
	 *            类
	 * @return 类静态空间的首地址
	 * @throws VMException
	 */
	int getClazzStaticPos(AbstractClass clazz) throws VMException;

	/**
	 * 句柄是否合法
	 * 
	 * @param handle
	 *            句柄
	 * @return 句柄是否合法
	 */
	boolean isHandleValid(int handle);

	/**
	 * 垃圾收集，释放掉不使用的句柄和对应的内存空间
	 * 
	 * @throws VMCriticalException
	 * @throws VMException
	 */
	void gc() throws VMCriticalException, VMException;

	/**
	 * 获得需要执行finialize的对象句柄数组并清空等待队列里面的对象
	 * 
	 * @return 要执行finialize的对象句柄数组
	 */
	int[] getToFinialize();

	int getFieldAbs(int handle, int pos) throws VMException;

	long getFieldAbsWide(int handle, int pos) throws VMException;

	void putFieldAbs(int handle, int pos, int value)
			throws VMCriticalException, VMException;

	void putFieldAbsWide(int handle, int pos, long value)
			throws VMCriticalException, VMException;
}
