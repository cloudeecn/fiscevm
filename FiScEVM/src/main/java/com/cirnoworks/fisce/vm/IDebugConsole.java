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

public interface IDebugConsole {

	/**
	 * 输出debug级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @return true
	 */
	boolean debug(String msg);

	/**
	 * 输出debug级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @param t
	 *            和信息一起输出的异常
	 * @return true
	 */
	boolean debug(String msg, Throwable t);

	/**
	 * 输出info级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @return true
	 */
	boolean info(String msg);

	/**
	 * 输出info级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @param t
	 *            和信息一起输出的异常
	 * @return true
	 */
	boolean info(String msg, Throwable t);

	/**
	 * 输出warn级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @return true
	 */
	boolean warn(String msg);

	/**
	 * 输出warn级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @param t
	 *            和信息一起输出的异常
	 * @return true
	 */
	boolean warn(String msg, Throwable t);

	/**
	 * 输出error级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @return true
	 */
	boolean error(String msg);

	/**
	 * 输出error级别的信息，永远返回true
	 * 
	 * @param msg
	 *            要输出的信息
	 * @param t
	 *            和信息一起输出的异常
	 * @return true
	 */
	boolean error(String msg, Throwable t);

}
