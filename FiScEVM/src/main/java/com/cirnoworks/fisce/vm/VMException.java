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

/**
 * 发生了可以抛给虚拟机内部处理的异常
 */
public class VMException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = -6933484337768111121L;
	private String innerThrowableName;
	private String innerMessage;

	/**
	 * 构筑这个异常
	 * 
	 * @param innerThrowableName
	 *            虚拟机内部的异常类的名字，分隔符是"/"，比如“java/lang/Exception”
	 * @param message
	 *            传给虚拟机内部的异常的message
	 */
	public VMException(String innerThrowableName, String message) {
		super(innerThrowableName + ":" + message);
		this.innerThrowableName = innerThrowableName;
		this.innerMessage = message;
	}

	public String getInnerThrowableName() {
		return innerThrowableName;
	}

	public String getInnerMessage() {
		return innerMessage;
	}

}
