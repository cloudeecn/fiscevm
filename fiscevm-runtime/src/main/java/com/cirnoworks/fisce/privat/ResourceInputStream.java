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
package com.cirnoworks.fisce.privat;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public final class ResourceInputStream extends InputStream {
	private final String name;
	private int pos;

	public ResourceInputStream(String name) throws FileNotFoundException,
			IOException {
		if (name == null) {
			throw new NullPointerException();
		}
		bind0(name, 0);
		this.name = name;
	}

	@Override
	public int read() throws IOException {
		bind0(name, pos);
		return read0();
	}

	@Override
	public int read(byte[] b, int off, int len) throws IOException {
		bind0(name, pos);
		int ret = read0(b, off, len);
		pos += ret;
		return ret;
	}

	@Override
	public void close() throws IOException {
		close0();
	}
	
	private native void bind0(String name, int pos)
			throws FileNotFoundException, IOException;

	private native int read0() throws IOException;

	private native int read0(byte[] b, int off, int len) throws IOException;

	private native void close0() throws IOException;

}
