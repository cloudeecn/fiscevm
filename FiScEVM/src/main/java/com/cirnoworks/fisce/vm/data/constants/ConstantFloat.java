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
package com.cirnoworks.fisce.vm.data.constants;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;
import java.io.DataInputStream;
import java.io.IOException;

/**
 *
 * @author cloudee
 */
public class ConstantFloat extends Constant {

    public float data;

    public ConstantFloat(VMContext context, ClassBase owner, DataInputStream dis)
            throws IOException {
        super(4, context, owner);
        data = dis.readFloat();
    }

    public float getData() {
        return data;
    }

    public void fillConstants() {
    }

    public String toString() {
        return "ConstantFloat:" + data;
    }
}
