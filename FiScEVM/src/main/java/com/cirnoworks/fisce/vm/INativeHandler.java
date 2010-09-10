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

public interface INativeHandler {
	/**
	 * 处理native方法，由虚拟机调用
	 * @param args 参数，如果不是静态方法，args[0]表示this，如果方法中有long或者double的参数，这个参数占据两个位置
	 * @param context 虚拟机
	 * @param thread 调用这个方法的线程
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void dealNative(int[] args, VMContext context, IThread thread)
			throws VMException, VMCriticalException;

	/**
	 * 返回这个native方法的签名
	 * @return 这个native方法的签名
	 */
	String getUniqueName();
}
