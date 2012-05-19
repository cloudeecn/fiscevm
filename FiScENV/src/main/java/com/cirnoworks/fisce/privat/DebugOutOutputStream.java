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
import java.io.OutputStream;

public class DebugOutOutputStream extends OutputStream {

	private final String enc;
	private byte[] buf = new byte[512];
	private int pos = 0;

	public DebugOutOutputStream(String enc) {
		this.enc = enc;
	}

	public DebugOutOutputStream() {
		this("utf-8");
	}

	@Override
	public void flush() {
		if (pos > 0) {
			try {
				FiScEVM.infoOut(new String(buf, 0, pos, enc));
			} catch (Exception e) {
			}
			pos = 0;
		}
	}

	@Override
	public void write(int b) throws IOException {
		if (b == '\n' || b == '\r') {
			flush();
			return;
		}
		if (pos == buf.length) {
			flush();
		}
		buf[pos++] = (byte) b;
	}

}
