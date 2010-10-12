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

public interface IThrower {
	/**
	 * 将一个VMException异常向虚拟机内抛出
	 * @param e 准备抛出的VMException异常
	 * @param thread 抛出异常的线程
	 * @return 异常对象的句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	int prepareThrowable(VMException e, IThread thread) throws VMException,
			VMCriticalException;

	/**
	 * 填充虚拟机内异常的堆栈跟踪信息
	 * @param context 虚拟机
	 * @param thread 用于堆栈跟踪的线程
	 * @param throwableHandle 用于填充堆栈跟踪信息的Throwable对象句柄
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	void fillInStackTrace(VMContext context, IThread thread, int throwableHandle)
			throws VMException, VMCriticalException;
}
