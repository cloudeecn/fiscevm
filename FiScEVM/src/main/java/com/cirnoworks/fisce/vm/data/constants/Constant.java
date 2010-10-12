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
package com.cirnoworks.fisce.vm.data.constants;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.IConstantHolder;

/**
 * 
 * @author cloudee
 */
public abstract class Constant implements IConstantHolder {

	public static final int CONSTANT_CLASS = 7;
	public static final int CONSTANT_FIELDREF = 9;
	public static final int CONSTANT_METHODREF = 10;
	public static final int CONSTANT_INTERFACE_METHODREF = 11;
	public static final int CONSTANT_STRING = 8;
	public static final int CONSTANT_INTEGER = 3;
	public static final int CONSTANT_FLOAT = 4;
	public static final int CONSTANT_LONG = 5;
	public static final int CONSTANT_DOUBLE = 6;
	public static final int CONSTANT_NAME_AND_TYPE = 12;
	public static final int CONSTANT_UTF8 = 1;
	protected byte tag;
	protected VMContext context;
	protected ClassBase owner;

	public Constant(int tag, VMContext context, ClassBase owner) {
		this.tag = (byte) tag;
		this.context = context;
		this.owner = owner;
	}

	public VMContext getContext() {
		return context;
	}

	public ClassBase getOwner() {
		return owner;
	}

	public byte getTag() {
		return tag;
	}
}
