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
import com.cirnoworks.fisce.vm.data.constants.ConstantUTF8;

public class InnerClass {

    public char innerClassInfoIndex;
    public ConstantClass innerClassInfo;
    public char outerClassInfoIndex;
    public ConstantClass outerClassInfo;
    public char innerNameIndex;
    public ConstantUTF8 innerName;
    public char innerClassAccessFlage;

    public String toString() {
        return innerClassInfo + "\t\t" + outerClassInfo + "\t\t" + innerName +
                "\t\t" + (int)innerClassAccessFlage;
    }
}
