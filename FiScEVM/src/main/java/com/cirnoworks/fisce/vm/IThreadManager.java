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

package com.cirnoworks.fisce.jvm13;

import java.io.InputStream;
import java.io.OutputStream;

import org.dom4j.Element;

import com.cirnoworks.fisce.jvm13.data.ClassBase;

public interface IThreadManager {
	static final int MAX_THREADS = 256;

	static final int STATE_NEW = 0;
	static final int STATE_BOOT_PENDING = 1;
	/**
	 * In this state we are save to save,start etc.
	 */
	static final int STATE_STOP = 2;
	static final int STATE_RUN_PENDING = 3;
	static final int STATE_RUNNING = 4;
	static final int STATE_STOP_PENDING = 5;
	static final int STATE_DEAD_PENDING = 6;
	static final int STATE_DEAD = 7;

	void setContext(VMContext context);

	void saveData(Element data) throws VMCriticalException;

	void loadData(Element data) throws VMCriticalException;

	void bootFromMain(ClassBase clazz) throws VMException, VMCriticalException;

	void pushThread(int threadHandle) throws VMException, VMCriticalException;

	void start() throws VMException;

	void requestStop();

	int waitTillStopped(long waitTime) throws InterruptedException;

	void setPriority(int threadHandle, int priority) throws VMException;

	void sleep(IThread thread, long time) throws VMException;

	void interrupt(int targetHandle) throws VMException, VMCriticalException;

	boolean isInterrupted(int targetHandle, boolean clear) throws VMException,
			VMCriticalException;

	void wait(IThread thread, int monitorId, long time) throws VMException,
			VMCriticalException;

	void notify(IThread thread, int monitorId, boolean all) throws VMException;

	boolean isAlive(int threadHandle);

	void exit(int exitCode);

	IThread[] getThreads() throws VMException;

	// IThread getThread(int threadHandle);

}