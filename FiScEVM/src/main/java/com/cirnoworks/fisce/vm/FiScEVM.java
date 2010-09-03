/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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
package com.cirnoworks.fisce.jvm13;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface FiScEVM {

	void bootFromData(InputStream is) throws VMCriticalException, IOException;

	void saveData(OutputStream is) throws VMCriticalException, IOException;

	void bootFromClass(String name) throws VMException, VMCriticalException;

	void requestStop();

	int waitTillStopped(long waitTime) throws InterruptedException;

	void start() throws VMException;

	void setClassLoader(IClassLoader classLoader);

	void setThrower(IThrower thrower);

	void registerNativeHandler(INativeHandler inh);

	void addStateListener(IStateListener isl);

	void addToolkit(IToolkit toolkit);

	void setThreadManager(IThreadManager threadManager);

	void setHeap(IHeap heap);

	void setConsole(IDebugConsole console);

	void exit(int code);

}