/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/**
 * @author Serguei S.Zapreyev
 *
 */

/**
 * ###############################################################################
 * ###############################################################################
 * TODO LIST:
 * 1. Provide correct processing the case if process isn't started because of some
 *    reason
 * 2. Clean and develop the native support
 * 3. Think of the default/undefault buffering
 * 3. Runtime.SubProcess.SubInputStream.read(b, off, len) and
 *    Runtime.SubProcess.SubErrorStream.read(b, off, len) should be effectively
 *    reimplemented on the native side.
 * ###############################################################################
 * ###############################################################################
 */

package java.lang;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

/**
 * @com.intel.drl.spec_ref
 */
public class Runtime {

	// //////////////////////////////////////////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////// RUNTIME BODY
	// ////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * "Every Java application has a single instance of class Runtime ..."
	 */
	private static Runtime thisApplicationRuntime = new Runtime();

	private static ArrayList<Thread> hooksList = new ArrayList<Thread>();

	/**
	 * 0 - normal work 1 - being shutdown sequence running 2 - being finalizing
	 */
	private static int VMState = 0;

	static boolean finalizeOnExit = false;

	/**
	 * An application cannot create its own instance of this class.
	 */
	private Runtime() {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public static Runtime getRuntime() {
		return thisApplicationRuntime;
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void exit(int status) {
		System.exit(status);
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void addShutdownHook(Thread hook) {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public boolean removeShutdownHook(Thread hook) {
		return false;
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void halt(int status) {
		System.exit(status);
	}

	/**
	 * @com.intel.drl.spec_ref
	 * @deprecated
	 */
	public static void runFinalizersOnExit(boolean value) {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native long freeMemory();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native long totalMemory();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native long maxMemory();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void gc() {
		System.gc();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void runFinalization() {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void traceInstructions(boolean on) {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void traceMethodCalls(boolean on) {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void load(String filename) throws SecurityException,
			UnsatisfiedLinkError {
	}

	void load0(String filename, ClassLoader cL, boolean check)
			throws SecurityException, UnsatisfiedLinkError {
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void loadLibrary(String libname) throws SecurityException,
			UnsatisfiedLinkError {
	}

	void loadLibrary0(String libname, ClassLoader cL, boolean check)
			throws SecurityException, UnsatisfiedLinkError {
	}

	/**
	 * @com.intel.drl.spec_ref
	 * @deprecated
	 */
	public InputStream getLocalizedInputStream(InputStream in) {
		// XXX: return new BufferedInputStream( (InputStream) (Object) new
		// InputStreamReader( in ) );
		return in;
	}

	/**
	 * @com.intel.drl.spec_ref
	 * @deprecated
	 */
	public OutputStream getLocalizedOutputStream(OutputStream out) {
		// XXX: return new BufferedOutputStream( (OutputStream) (Object) new
		// OutputStreamWriter( out ) );
		return out;
	}
}
