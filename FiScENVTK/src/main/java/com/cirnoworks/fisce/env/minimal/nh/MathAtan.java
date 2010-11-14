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
package com.cirnoworks.fisce.env.minimal.nh;

import com.cirnoworks.fisce.util.TypeUtil;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;

/**
 * @author Cloudee
 * 
 */
public class MathAtan extends NativeHandlerTemplate {

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#dealNative(int[],
	 * com.cirnoworks.fisce.vm.IThread)
	 */
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		double param1 = Double.longBitsToDouble(TypeUtil.intToLong(args[0],
				args[1]));
		thread.pushDouble(Math.atan(param1));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#getUniqueName()
	 */
	public String getUniqueName() {
		return "java/lang/Math.atan.(D)D";
	}

}
