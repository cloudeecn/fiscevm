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

import com.cirnoworks.fisce.jvm13.data.constants.ConstantClass;

public class ExceptionHandler {

	public char startPc;
	public char endPc;
	public char handlerPc;
	public char catchType;
	public ConstantClass catchClass;

	public String toString() {
		return (int) startPc + "\t" + (int) endPc + "\t" + (int) handlerPc
				+ "\t" + (catchClass == null ? null : catchClass.getName());
	}
}
