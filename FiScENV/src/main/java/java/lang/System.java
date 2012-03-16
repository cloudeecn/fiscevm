/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package java.lang;

import java.io.InputStream;
import java.io.PrintStream;

import com.cirnoworks.fisce.privat.SystemInputStream;
import com.cirnoworks.fisce.privat.SystemOutputStream;

public final class System {

	private System() {
	}

	public final static InputStream in = new SystemInputStream();

	public final static PrintStream out = new PrintStream(
			new SystemOutputStream("info"));

	public final static PrintStream err = new PrintStream(
			new SystemOutputStream("error"));

	public static void setIn(InputStream in) {
		setIn0(in);
	}

	public static void setOut(PrintStream out) {
		setOut0(out);
	}

	public static void setErr(PrintStream err) {
		setErr0(err);
	}

	private static native void setIn0(InputStream in);

	private static native void setOut0(PrintStream out);

	private static native void setErr0(PrintStream err);

	public static native long currentTimeMillis();

	public static native void arraycopy(Object src, int srcPos, Object dest,
			int destPos, int length);

	public static native int identityHashCode(Object x);

	public static native String getProperty(String key);

	public static String getProperty(String key, String def) {
		String ret = getProperty(key);
		return ret == null ? def : ret;
	}

	public static native String setProperty(String key, String value);

	public static native void exit(int status);

	public static native void gc();

	public static native long nanoTime();
}
