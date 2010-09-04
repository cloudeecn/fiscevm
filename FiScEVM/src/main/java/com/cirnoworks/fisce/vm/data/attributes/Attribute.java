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
package com.cirnoworks.fisce.vm.data.attributes;

import com.cirnoworks.fisce.vm.data.IAttributesHolder;
import com.cirnoworks.fisce.vm.data.IConstantHolder;

/**
 *
 * @author cloudee
 */
public abstract class Attribute implements IConstantHolder {

    public char attributeNameIndex;
    public String attributeName;
    public int attributeLength;

    public static Attribute getAttributeByName(IAttributesHolder holder,
            String name) {
        for (int i = 0, max = holder.countAttribute(); i < max; i++) {
            Attribute a = holder.getAttribute(i);
            if (a.getAttributeName().equals(name)) {
                return a;
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

    public char getAttributeNameIndex() {
        return attributeNameIndex;
    }

    public void setAttributeNameIndex(char attributeNameIndex) {
        this.attributeNameIndex = attributeNameIndex;
    }

    public abstract void fillConstants();
}
