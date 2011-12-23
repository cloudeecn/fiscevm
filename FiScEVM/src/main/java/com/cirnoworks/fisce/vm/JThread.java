package com.cirnoworks.fisce.vm;

import java.util.BitSet;
import java.util.List;

import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IMethod;

public interface JThread extends IThread {
	/**
	 * 总线程栈的大小
	 */
	public static final int STACK_SIZE = 1048576;

	/**
	 * 线程可以被调度
	 */
	public static final int STAT_ALIVE = 0;
	/**
	 * 线程已经死亡
	 */
	public static final int STAT_DEAD = 1;

	/**
	 * 往当前执行栈中压入一个整形变量
	 * 
	 * @param value
	 *            值
	 */
	void pushInt(int value);

	/**
	 * 往当前执行栈中压入一个句柄
	 * 
	 * @param value
	 *            值
	 */
	void pushHandle(int handle);

	/**
	 * 往当前执行栈中压入一个长整形变量
	 * 
	 * @param value
	 *            值
	 */
	void pushLong(long value);

	/**
	 * 往当前执行栈中压入一个浮点变量
	 * 
	 * @param value
	 *            值
	 */
	void pushFloat(float value);

	/**
	 * 往当前执行栈中压入一个双精度浮点变量
	 * 
	 * @param value
	 *            值
	 */
	void pushDouble(double value);

	/**
	 * 从当前执行栈中弹出一个整形变量
	 * 
	 * @return 值
	 */
	int popInt();

	/**
	 * 从当前执行栈中弹出一个句柄
	 * 
	 * @return 值
	 */
	int popHandle();

	/**
	 * 从当前执行栈中弹出一个长整形变量
	 * 
	 * @return 值
	 */
	long popLong();

	/**
	 * 从当前执行栈中弹出一个浮点变量
	 * 
	 * @return 值
	 */
	float popFloat();

	/**
	 * 从当前执行栈中弹出一个双精度浮点变量
	 * 
	 * @return 值
	 */
	double popDouble();

	/**
	 * 从一个句柄型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	int getLocalHandle(int index);

	/**
	 * 给一个句柄型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalHandle(int index, int value);

	/**
	 * 从一个整型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	int getLocalInt(int index);

	/**
	 * 给一个整型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalInt(int index, int value);

	/**
	 * 从一个长整型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	long getLocalLong(int index);

	/**
	 * 给一个长整型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalLong(int index, long value);

	/**
	 * 从一个双精度浮点型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	double getLocalDouble(int index);

	/**
	 * 给一个双精度浮点型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalDouble(int index, double value);

	/**
	 * 压入线程栈，准备调用别的方法
	 * 
	 * @param mt
	 *            要调用的方法
	 */
	void pushFrame(IMethod mt);

	/**
	 * 直接调用某个方法
	 * 
	 * @param invoke
	 *            要调用的方法
	 * @param isStaticdumpStackTrace
	 *            是否为静态方法
	 * @param argsCount
	 *            参数的个数
	 * @param args
	 *            参数
	 * @param types
	 *            参数的类型
	 */
	void pushMethod(IMethod invoke, boolean isStatic, int argsCount,
			int[] args, byte[] types);

	/**
	 * 取得当前正在执行的方法
	 * 
	 * @return 当前正在执行的方法
	 */
	IMethod getCurrentMethod();

	/**
	 * 为线程导出调用栈
	 * 
	 * @param list
	 *            用于存放调用栈的列表，如果传入null的话就产生一个新的
	 * @return 到处后的调用栈列表
	 * @throws VMCriticalException
	 * @throws VMException
	 */
	List<com.cirnoworks.fisce.vm.data.StackTraceElement> dumpStackTrace(
			List<com.cirnoworks.fisce.vm.data.StackTraceElement> list)
			throws VMException, VMCriticalException;

	/**
	 * 取当前线程的状态
	 * 
	 * @return
	 */
	int getStatus();



	/**
	 * 设置执行完当前的指令后是否把控制权交回现成管理器
	 * 
	 * @param yield
	 */
	void setYield(boolean yield);

	/**
	 * 将这个线程使用到的句柄填充到集合中
	 * 
	 * @param tofill
	 *            待填充的集合
	 */
	void fillUsedHandles(BitSet tofill);
}
