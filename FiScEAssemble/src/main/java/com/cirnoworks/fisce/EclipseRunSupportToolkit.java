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
package com.cirnoworks.fisce;

import java.io.InputStream;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.vm.IToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;

/**
 * @author Cloudee
 *
 */
public class EclipseRunSupportToolkit implements IToolkit{

	/* (non-Javadoc)
	 * @see com.cirnoworks.fisce.vm.IToolkit#setContext(com.cirnoworks.fisce.vm.VMContext)
	 */
	public void setContext(VMContext context) {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.cirnoworks.fisce.vm.IToolkit#setupContext()
	 */
	public void setupContext() {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.cirnoworks.fisce.vm.IToolkit#getResourceByClassName(java.lang.String)
	 */
	public InputStream getResourceByClassName(String className) {
		String path = "/classes/" + className + ".class";
		return getClass().getResourceAsStream(path);
	}

	/* (non-Javadoc)
	 * @see com.cirnoworks.fisce.vm.IToolkit#saveData(org.w3c.dom.Element)
	 */
	public void saveData(Element data) throws VMCriticalException {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.cirnoworks.fisce.vm.IToolkit#loadData(org.w3c.dom.Element)
	 */
	public void loadData(Element data) throws VMCriticalException {
		// TODO Auto-generated method stub
		
	}

}
