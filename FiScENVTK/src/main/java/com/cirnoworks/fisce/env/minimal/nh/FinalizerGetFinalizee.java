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

import com.cirnoworks.fisce.vm.IClassLoader;
import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.NativeHandlerTemplate;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
import com.cirnoworks.fisce.vm.data.ClassArray;

/**
 * @author Cloudee
 * 
 */
public class FinalizerGetFinalizee extends NativeHandlerTemplate {

	private IHeap heap;
	private ClassArray array;

	public void doInit() throws VMException, VMCriticalException {
		heap = context.getHeap();
		array = (ClassArray) context.getClass("[L" + IClassLoader.TOP_CLASS
				+ ";");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#dealNative(int[],
	 * com.cirnoworks.fisce.vm.IThread)
	 */
	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		int[] handles = heap.getToFinialize();
		int len = handles.length;
		int ret = heap.allocate(array, len);
		heap.fillArrayInt(ret, 0, handles, 0, len);
		thread.pushHandle(ret);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.cirnoworks.fisce.vm.INativeHandler#getUniqueName()
	 */
	public String getUniqueName() {
		return "java/lang/Finalizer.getFinalizee.()[Ljava/lang/Object;";
	}

}
