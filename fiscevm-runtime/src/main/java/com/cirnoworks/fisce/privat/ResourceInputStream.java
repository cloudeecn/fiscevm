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

	@Override
	public int read() throws IOException {
		return read0(name, pos++);
	}

	@Override
	public int read(byte[] b, int off, int len) throws IOException {
		int ret = read0(name, pos, b, off, len);
		pos += len;
		return ret;
	}

	@Override
	public void close() throws IOException {
		close0(name);
	}

	private native int read0(String name, int pos) throws IOException;

	private native int read0(String name, int pos, byte[] b, int off, int len)
			throws IOException;

	private native void close0(String name) throws IOException;
}
