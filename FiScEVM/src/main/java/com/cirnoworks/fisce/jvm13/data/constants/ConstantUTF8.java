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

import com.cirnoworks.fisce.jvm13.VMContext;
import com.cirnoworks.fisce.jvm13.data.ClassBase;
import java.io.DataInputStream;
import java.io.IOException;

/**
 *
 * @author cloudee
 */
public class ConstantUTF8 extends Constant {

    private byte[] data;
    //cache
    private String string;

    public ConstantUTF8(VMContext context, ClassBase owner, DataInputStream dis)
            throws IOException {
        super(1, context, owner);
        char len = dis.readChar();
        data = new byte[len];
        dis.readFully(data);
        string = new String(data, "utf-8");
    }

    public void fillConstants(){

    }

    public byte[] getData() {
        return data;
    }

    public String getString() {
        return string;
    }

    public String toString() {
        return "ConstantUTF8:" + string;
    }
}
