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
package com.cirnoworks.fisce.vm.data;

import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.VMContext;

/**
 * 
 * @author cloudee
 */
public final class ClassArray extends AbstractClass {

	private AbstractClass contentClass;
	private char pType;
	private int sizeShift;

	public AbstractClass getContentClass() {
		return contentClass;
	}

	public void setContentClass(AbstractClass contentClass) {
		this.contentClass = contentClass;
	}

	public char getpType() {
		return pType;
	}

	public void setpType(char pType) {
		this.pType = pType;
	}

	public int getSizeShift() {
		return sizeShift;
	}

	public void setSizeShift(int sizeShift) {
		this.sizeShift = sizeShift;
	}

	public ClassArray(VMContext context, IClassLoader loader) {
		super(context, loader);
	}

	public String toString() {
		if (getContentClass() == null) {
			return "[" + String.valueOf(pType);
		} else {
			return "[" + getContentClass().getName();
		}
	}
}
