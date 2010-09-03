/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
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
package com.cirnoworks.fisce.jvm13.data.constants;

import java.io.DataInputStream;
import java.io.IOException;

import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.VMCriticalException;
import com.cirnoworks.fisce.jvm13.VMException;
import com.cirnoworks.fisce.jvm13.data.ClassBase;

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
}
