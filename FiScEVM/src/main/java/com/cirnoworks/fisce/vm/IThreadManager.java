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

package com.cirnoworks.fisce.vm;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.vm.data.ClassBase;

public interface IThreadManager {
	static final int MAX_THREADS = 256;

	static final int STATE_NEW = 0;
	static final int STATE_BOOT_PENDING = 1;
	/**
	 * In this state we are save to save,start etc.
	 */
	static final int STATE_STOP = 2;
	static final int STATE_RUN_PENDING = 3;
	static final int STATE_RUNNING = 4;
	static final int STATE_STOP_PENDING = 5;
	static final int STATE_DEAD_PENDING = 6;
	static final int STATE_DEAD = 7;

	/**
	 * 绑定这个线程调度器到虚拟机
	 * @param context
	 */
	void setContext(VMContext context);

	/**
	 * 保存数据
	 * @param data
	 * @throws VMCriticalException
	 */
	void saveData(Element data) throws VMCriticalException;

	/**
	 * 载入数据
	 * @param data
	 * @throws VMCriticalException
	 */
	void loadData(Element data) throws VMCriticalException;

	/**
	 * 从一个类的main方法开始初始化并执行。状态必须是NEW，否错会出错<br />
	 * @param clazz 要执行的类
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void bootFromMain(ClassBase clazz) throws VMException, VMCriticalException;

	/**
	 * 用一个新的线程开始执行java/lang/Thread的run()方法
	 * @param threadHandle Thread对象的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void pushThread(int threadHandle) throws VMException, VMCriticalException;

	/**
	 * 启动线程调度器，必须在STOP的状态来执行
	 * @throws VMException
	 */
	void start() throws VMException;

	/**
	 * 请求线程调度器停止执行。线程调度器将在一个安全（可以完整保存当前状态）的时点停止自己和所有线程的运行
	 */
	void requestStop();

	/**
	 * 等待线程调度器停止
	 * @param waitTime 等待的时间，0的话表示一直等下去
	 * @return 返回码
	 * @throws InterruptedException 在规定时间内线程调度器没有停止
	 */
	int waitTillStopped(long waitTime) throws InterruptedException;

	/**
	 * 设置线程的优先级
	 * @param threadHandle 线程对象的句柄
	 * @param priority 优先级，1-10
	 * @throws VMException
	 */
	void setPriority(int threadHandle, int priority) throws VMException;

	/**
	 * 让某个线程休眠一段时间
	 * @param thread 发出指令的线程（也就是要休眠的线程）
	 * @param time 时间
	 * @throws VMException
	 */
	void sleep(IThread thread, long time) throws VMException;

	/**
	 * 中止某个线程正在进行的sleep或者wait等待
	 * @param targetHandle 要中止的线程对象的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void interrupt(int targetHandle) throws VMException, VMCriticalException;

	/**
	 * 返回某个线程的中断标志
	 * @param targetHandle 线程对象的句柄
	 * @param clear 是否清除线程的中断标志
	 * @return 线程的中断标志
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	boolean isInterrupted(int targetHandle, boolean clear) throws VMException,
			VMCriticalException;

	/**
	 * 让制定线程释放持有的对象的锁并等待
	 * @param thread 发出指令的线程（也就是要等待的线程）
	 * @param monitorId 监视器的对象句柄
	 * @param time 等待时间
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void wait(IThread thread, int monitorId, long time) throws VMException,
			VMCriticalException;

	/**
	 * 唤醒正在等待的线程
	 * @param thread 发出指令的线程（当前的锁持有者）
	 * @param monitorId 要唤醒的对象的句柄（object.notify()中的object）
	 * @param all 是否唤醒所有等待者？（notify()还是notifyAll()）
	 * @throws VMException
	 */
	void notify(IThread thread, int monitorId, boolean all) throws VMException;

	/**
	 * 线程是否活着
	 * @param threadHandle 线程对象的句柄
	 * @return 线程是否活着
	 */
	boolean isAlive(int threadHandle);

	/**
	 * 退出，中止所有线程，不可逆的退出
	 * @param exitCode 返回码
	 */
	void exit(int exitCode);

	/**
	 * 返回包含所有线程的数组
	 * @return 包含所有线程的数组
	 * @throws VMException
	 */
	IThread[] getThreads() throws VMException;

	// IThread getThread(int threadHandle);

}