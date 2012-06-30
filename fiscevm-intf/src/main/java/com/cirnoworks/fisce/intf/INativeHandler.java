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

/**
 * 本地方法处理器，在虚拟机启动之前由Toolkit注入到虚拟机里
 * 
 * @author yuxuanhuang
 * 
 */
public interface INativeHandler {

	/**
	 * 设置调用这个NativeHandler的虚拟机并初始化必要的信息<br>
	 * 一个NativeHandler实例只对应一个虚拟机<br>
	 * 建议：加上每个实例只能初始化一次的限制
	 * 
	 * @param context
	 * @throws VMCriticalException
	 * @throws VMException
	 */
	void init(FiScEVM context) throws VMCriticalException, VMException;

	/**
	 * 处理native方法，由虚拟机调用
	 * 
	 * @param args
	 *            参数，如果不是静态方法，args[0]表示this，如果方法中有long或者double的参数，这个参数占据两个位置
	 * @param context
	 *            虚拟机
	 * @param thread
	 *            调用这个方法的线程
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException;

	/**
	 * 返回本地方法的签名，这个签名将给虚拟机用于识别这个处理器是处理哪个本地方法的
	 * 
	 * @return 这个native方法的签名
	 */
	String getUniqueName();
}
