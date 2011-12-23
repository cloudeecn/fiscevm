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

import java.io.DataInputStream;
import java.io.IOException;

import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassBase;

/**
 * 
 * @author cloudee
 */
public class ConstantClass extends Constant {

	private char nameIndex;
	// cache
	private String name;
	private AbstractClass clazz;

	public ConstantClass(VMContext context, ClassBase owner, DataInputStream dis)
			throws IOException {
		super(7, context, owner);
		nameIndex = dis.readChar();
	}

	public String getName() {
		assert owner.isConstantsLoaded();
		return name;
	}

	public void fillConstants() {
		assert name == null;
		name = owner.getStringFromUTF8Constant(nameIndex);
	}

	public synchronized AbstractClass getClazz() throws VMException, VMCriticalException {
		assert owner.isConstantsLoaded();
		if (clazz == null) {
			clazz = context.getClass(name);
		}

		return clazz;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final ConstantClass other = (ConstantClass) obj;
		if ((this.name == null) ? (other.name != null) : !this.name
				.equals(other.name)) {
			return false;
		}
		return true;
	}

	@Override
	public int hashCode() {
		int hash = 3;
		hash = 19 * hash + (this.name != null ? this.name.hashCode() : 0);
		return hash;
	}

	@Override
	public String toString() {
		return "ConstantClass:" + (int) nameIndex + ":" + getName();
	}
}
