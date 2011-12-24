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

package com.cirnoworks.fisce.intf;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.idata.IClassBase;

public interface IThreadManager {

	/**
	 * 返回包含所有线程的数组
	 * 
	 * @return 包含所有线程的数组
	 * @throws VMException
	 */
	IThread[] getThreads() throws VMException;

	// IThread getThread(int threadHandle);

}