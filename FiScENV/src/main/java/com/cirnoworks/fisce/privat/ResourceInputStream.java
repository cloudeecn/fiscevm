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
package com.cirnoworks.fisce.privat;

import java.io.IOException;
import java.io.InputStream;

public class ResourceInputStream extends InputStream {
	private String name;
	private int pos;

	public ResourceInputStream(String name) {
		if (name == null) {
			throw new NullPointerException();
		}
		this.name = name;
	}

	public int read() throws IOException {
		return read0(name, pos++);
	}

	public int read(byte[] b, int off, int len) throws IOException {
		int ret = read0(name, pos, b, off, len);
		pos += len;
		return ret;
	}

	public void close() throws IOException {
		close0(name);
	}

	private native int read0(String name, int pos) throws IOException;

	private native int read0(String name, int pos, byte[] b, int off, int len)
			throws IOException;

	private native void close0(String name) throws IOException;
}
