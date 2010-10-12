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

import com.cirnoworks.fisce.vm.data.constants.ConstantClass;
import java.io.PrintWriter;
import java.io.StringWriter;

/**
 *
 * @author cloudee
 */
public class AttributeExceptions extends Attribute{
    public char[] exceptionIndexTable;
    public ConstantClass[] exceptions;

    public String toString(){
        StringWriter sw=new StringWriter();
        PrintWriter out=new PrintWriter(sw);
        out.println("$$$$$AttributeExceptions");
        for(ConstantClass e:exceptions){
            out.println(e.toString());
        }
        out.println("$$$$$/AttributeExceptions");
        return sw.toString();
    }

	@Override
	public void fillConstants() {
	}
}
