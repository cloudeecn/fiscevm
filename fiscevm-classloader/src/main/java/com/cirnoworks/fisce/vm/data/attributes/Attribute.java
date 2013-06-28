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
package com.cirnoworks.fisce.vm.data.attributes;

/**
 * 
 * @author cloudee
 */
public abstract class Attribute {

	public String attributeName;
	public int attributeLength;

	public static Attribute getAttributeByName(Attribute[] attributes,
			String name) {
		for (Attribute attribute : attributes) {
			if (name.equals(attribute.getAttributeName())) {
				return attribute;
			}
		}
		return null;
	}

	public int getAttributeLength() {
		return attributeLength;
	}

	public void setAttributeLength(int attributeLength) {
		this.attributeLength = attributeLength;
	}

	public String getAttributeName() {
		return attributeName;
	}

	public void setAttributeName(String attributeName) {
		this.attributeName = attributeName;
	}

}
