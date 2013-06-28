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

import com.cirnoworks.fisce.classloader.utils.SimpleJSONUtil;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;

/**
 * 
 * @author cloudee
 */
public class ConstantString extends Constant {

	private char stringIndex;
	private String str;
	private int handle;

	public ConstantString(VMContext context, ClassBase owner,
			DataInputStream dis) throws IOException {
		super(8, context, owner);
		stringIndex = dis.readChar();
	}

	/**
	 * the object handle in VM that refer to the inner String object.
	 * 
	 * @return
	 * @throws VMException
	 * @throws VMCriticalException
	 */
	public int getHandle() throws VMException, VMCriticalException {
		if (handle == 0) {
			handle = context.getHeap().getLiteral(str);
		}
		return handle;
	}

	public void fillConstants() {
		str = owner.getStringFromUTF8Constant(stringIndex);
	}

	public String toString() {
		return "ConstantString:" + (int) stringIndex + ":"
				+ owner.getStringFromUTF8Constant(stringIndex);
	}

	@Override
	public void appendJSON(StringBuilder sb, int baseIndent, boolean addComma) {
		SimpleJSONUtil.add(sb, baseIndent, "{", false);
		SimpleJSONUtil.add(sb, baseIndent + 1, "value",
				SimpleJSONUtil.escapeString(str, true), false);
		SimpleJSONUtil.add(sb, baseIndent, "}", addComma);
	}
}
