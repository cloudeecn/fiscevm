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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * 虚拟机的对外接口，里面是供外界使用的虚拟机的方法。<br />
 * 推荐用这个接口来代替@see com.cirnoworks.fisce.vm.VMContext来进行控制以免不小心影响到虚拟机的正常运行。
 * 
 * @author Cloudee
 * 
 */
public interface FiScEVM {

	/**
	 * Initialize the VM from saved data. After this the VM will be at "STOP"
	 * status.<br />
	 * 
	 * 从保存的数据中初始化虚拟机。 初始化后的虚拟机将处于停止的状态，需要调用start方法来启动。
	 * 
	 * @param is
	 *            The inputstream with the data.
	 * @throws VMCriticalException
	 *             If something wrong happened while class loading etc.
	 * @throws IOException
	 *             If IO errors happened when reading data.
	 */
	void bootFromData(InputStream is) throws VMCriticalException, IOException;

	/**
	 * 保存当前的虚拟机状态。 需要在虚拟机停止的时候运行。
	 * 
	 * @param os
	 *            用于保存数据的输出流
	 * @throws VMCriticalException
	 *             发生了虚拟机相关的严重错误
	 * @throws IOException
	 *             发生了IO错误
	 */
	void saveData(OutputStream os) throws VMCriticalException, IOException;

	/**
	 * 初始化一个新的虚拟机，并且启动指定类的public static void main(String[] args)方法。
	 * 
	 * @param name
	 *            启动类的全名。
	 * @throws VMException
	 *             发生了虚拟机相关的错误
	 * @throws VMCriticalException
	 *             发生了虚拟机相关的严重错误
	 */
	void bootFromClass(String name) throws VMException, VMCriticalException;

	/**
	 * 向虚拟机发送停止请求。 虚拟机将在最近的安全停止点停止运行。
	 */
	void requestStop();

	/**
	 * 等待虚拟机停止运行。
	 * 
	 * @param waitTime
	 *            等待时间，0表示不限时。
	 * @return 返回码（如果虚拟机里面的程序用System.exit退出的话）
	 * @throws InterruptedException
	 *             线程在等待过程中被中断了
	 */
	int waitTillStopped(long waitTime) throws InterruptedException;

	/**
	 * 启动虚拟机，必须先用两种boot中的一种来初始化
	 * 
	 * @throws VMException
	 */
	void start() throws VMException;

	/**
	 * 设置虚拟机用的ClassLoader<br />
	 * 可以用自带的 @see com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader
	 * 
	 * @param classLoader
	 */
	void setClassLoader(IClassLoader classLoader);

	/**
	 * 给虚拟机增加状态监听器
	 * 
	 * @param isl
	 *            状态监听器，虚拟机状态改变时将会回调状态监听器的方法
	 */
	void addStateListener(IStateListener isl);

	/**
	 * 给虚拟机增加工具包<br />
	 * TODO 工具包
	 * 
	 * @param toolkit
	 *            工具包
	 */
	void addToolkit(IToolkit toolkit);

	/**
	 * 设置虚拟机的线程管理器<br />
	 * 线程管理器负责多线程的调度和创建/销毁线程<br />
	 * 参考@see com.cirnoworks.fisce.vm.default_impl.DefaultThreadManager
	 * 
	 * @param threadManager
	 *            线程管理器
	 */
	void setThreadManager(IThreadManager threadManager);

	/**
	 * 设置虚拟机的堆管理器<br />
	 * 堆管理器负责管理虚拟机的堆空间、静态空间、字符串常量空间，创建对象，垃圾收集<br />
	 * 参考@see com.cirnoworks.fisce.vm.default_impl.DefaultHeap
	 * 
	 * @param heap
	 *            堆管理器
	 */
	void setHeap(IHeap heap);

	/**
	 * 设置虚拟机的控制台输出。<br />
	 * 默认的@see com.cirnoworks.fisce.vm.NullConsole不进行任何输出
	 * 
	 * @param console
	 *            控制台输出
	 */
	void setConsole(IDebugConsole console);
}