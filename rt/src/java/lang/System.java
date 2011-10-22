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
