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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.NullConsole;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.ArrayThreadManager;
import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.vm.default_impl.DefaultHeap;

/**
 * @author cloudee
 * 
 */
public class FiScEAndroidTest extends Activity {

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		VMContext vm = new VMContext();
		vm.setClassLoader(new DefaultClassLoader());
		vm.setHeap(new DefaultHeap());
		vm.setThreadManager(new ArrayThreadManager());
		vm.setConsole(new NullConsole());
		vm.addToolkit(new BaseToolkit());
		try {
			vm.bootFromClass("com/cirnoworks/fisce/privat/Profile");
			vm.registerNativeHandler(new SystemOutStreamWrite0());
			vm.start();
			vm.waitTillStopped(0);
		} catch (Exception e) {
			Log.e("###", "", e);
		}
	}
}
