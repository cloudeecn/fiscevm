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
package com.cirnoworks.fisce.data.constants.internal;

import com.cirnoworks.fisce.data.constants.ConstantData;

/**
 * ClassLoader inner use only
 * 
 * @author cloudee
 */
public class ConstantNameTypeInfoData implements ConstantData {

	protected String name;
	protected String descriptor;

	public void setName(String name) {
		this.name = name;
	}

	public void setDescriptor(String descriptor) {
		this.descriptor = descriptor;
	}

	public String getName() {
		return name;
	}

	public String getDescriptor() {
		return descriptor;
	}

	public String toString() {
		return "ConstantNameType: " + name + "." + descriptor;
	}

	public void fillConstants(ConstantData[] constantPool) {
		
	}

}
