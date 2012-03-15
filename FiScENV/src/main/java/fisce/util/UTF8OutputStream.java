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
package fisce.util;

import java.io.IOException;
import java.io.OutputStream;

import com.cirnoworks.fisce.privat.UTF8;

/**
 * Consumes utf-8 bytes and convert to unicode characters to Appendable
 *
 * @author Cloudee
 *
 */
public class UTF8OutputStream extends OutputStream {

	private final Appendable out;
	private final byte[] buf = new byte[3];
	private int pos = 0;
	private int size = 0;

	public UTF8OutputStream(Appendable out) {
		super();
		this.out = out;
	}

	@Override
	public void write(int oneByte) throws IOException {
		byte b = buf[pos++] = (byte) oneByte;
		if (pos == 1) {
			size = UTF8.utf8SizeU(b);
			switch (size) {
			case 0:
			default:
				out.append('?');
				pos = 0;
				size = 0;
				break;
			case 1:
				out.append((char) b);
				pos = 0;
				size = 0;
				break;
			case 2:
			case 3:
				// no append,wait for other
				break;
			}
		} else {
			if ((oneByte & 0xc0)/* 0b 11000000 */!= 0x80) {
				// error
				out.append('?');
				pos = 0;
				size = 0;
			} else {
				// ok
				if (pos >= size) {
					switch (size) {
					case 2:
						out.append((char) (((buf[0] & 0x1f) << 6) + (buf[1] & 0x3f)));
						break;
					case 3:
						out.append((char) (((buf[0] & 0xf) << 12)
								+ ((buf[1] & 0x3f) << 6) + (buf[2] & 0x3f)));
						break;
					}
					pos = 0;
					size = 0;
				}
			}
		}
	}
}
