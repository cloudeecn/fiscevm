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
package com.cirnoworks.fisce.data;

import org.objectweb.asm.tree.FieldNode;

/**
 * 
 * @author cloudee
 */
public final class FieldData {

	public final FieldNode node;

	public FieldData(FieldNode node) {
		this.node = node;
	}

	/**
	 * Position in heap memory of this class. filled in the load phase 1. After
	 * the constant table loaded.
	 */
	public int position = -1;
	/**
	 * Length of the class. set after the constant table filled.
	 */
	public int length;
	public char constantValueIndex;

	public int getPosition() {
		return position;
	}

	public void setPosition(int position) {
		this.position = position;
	}

	public int getLength() {
		return length;
	}

	public void setLength(int length) {
		this.length = length;
	}

	public int getAccessFlags() {
		return node.access;
	}

	public String getName() {
		return node.name;
	}

	public String getDescriptor() {
		return node.desc;
	}

	public char getConstantValueIndex() {
		return constantValueIndex;
	}

	public void setConstantValueIndex(char constantValueIndex) {
		this.constantValueIndex = constantValueIndex;
	}

}
