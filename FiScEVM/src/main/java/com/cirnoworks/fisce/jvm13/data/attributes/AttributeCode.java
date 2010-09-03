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
package com.cirnoworks.fisce.jvm13.data.attributes;

import java.io.PrintWriter;
import java.io.StringWriter;

import com.cirnoworks.fisce.jvm13.data.IAttributesHolder;

/**
 *
 * @author cloudee
 */
public class AttributeCode extends Attribute implements IAttributesHolder {

    public char maxStack;
    public char maxLocals;
    public byte[] code;
    public ExceptionHandler[] exceptionTable;
    public Attribute[] attributes;

    public String toString() {
        StringWriter sw = new StringWriter();
        PrintWriter out = new PrintWriter(sw);
        out.println("$$$$$AttributeCode:");
        out.println("maxStack=" + maxStack);
        out.println("maxLocals=" + maxLocals);
        out.println("code length=" + code.length);
        out.println("Excepion Table:");
        for (ExceptionHandler et : exceptionTable) {
            out.println(et.toString());
        }
        out.println("Code Attributes:");
        for (Attribute a : attributes) {
            out.println(a.toString());
        }
        out.println("$$$$$/AttributeCode");
        return sw.toString();
    }

    public void createAttributes(int size) {
        attributes = new Attribute[size];
    }

    public void setAttribute(int idx, Attribute a) {
        attributes[idx] = a;
    }

    public Attribute getAttribute(int idx) {
        return attributes[idx];
    }

    public int countAttribute() {
        return attributes.length;
    }

	@Override
	public void fillConstants() {
	}
}
