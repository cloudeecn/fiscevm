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

package java.io;

import com.cirnoworks.fisce.privat.UTF8;

/**
 * A class for turning a byte stream into a character stream. Data read from the
 * source input stream is converted into characters by either a default or a
 * provided character converter. The default encoding is taken from the
 * "file.encoding" system property. {@code InputStreamReader} contains a buffer
 * of bytes read from the source stream and converts these into characters as
 * needed. The buffer size is 8K.
 *
 * @see OutputStreamWriter
 */
public class InputStreamReader extends Reader {
	private InputStream in;

	private static final int BUFFER_SIZE = 8192;

	private byte[] buffer = new byte[BUFFER_SIZE];
	private int bufferPos = 0;
	private boolean over = false;

	/**
	 * Constructs a new {@code InputStreamReader} on the {@link InputStream}
	 * {@code in}. This constructor sets the character converter to the encoding
	 * specified in the "file.encoding" property and falls back to ISO 8859_1
	 * (ISO-Latin-1) if the property doesn't exist.
	 *
	 * @param in
	 *            the input stream from which to read characters.
	 */
	public InputStreamReader(InputStream in) {
		super(in);
		this.in = in;
	}

	public InputStreamReader(InputStream in, Object o) {
		super(in);
		this.in = in;

	}

	/**
	 * Closes this reader. This implementation closes the source InputStream and
	 * releases all local storage.
	 *
	 * @throws IOException
	 *             if an error occurs attempting to close this reader.
	 */
	@Override
	public void close() throws IOException {
		synchronized (lock) {
			if (in != null) {
				in.close();
				in = null;
			}
		}
	}

	/**
	 * Returns the name of the encoding used to convert bytes into characters.
	 * The value {@code null} is returned if this reader has been closed.
	 *
	 * @return the name of the character converter or {@code null} if this
	 *         reader is closed.
	 */
	public String getEncoding() {
		if (!isOpen()) {
			return null;
		}
		return "utf-8";
	}

	/**
	 * Reads at most {@code length} characters from this reader and stores them
	 * at position {@code offset} in the character array {@code buf}. Returns
	 * the number of characters actually read or -1 if the end of the reader has
	 * been reached. The bytes are either obtained from converting bytes in this
	 * reader's buffer or by first filling the buffer from the source
	 * InputStream and then reading from the buffer.
	 *
	 * @param buf
	 *            the array to store the characters read.
	 * @param offset
	 *            the initial position in {@code buf} to store the characters
	 *            read from this reader.
	 * @param length
	 *            the maximum number of characters to read.
	 * @return the number of characters read or -1 if the end of the reader has
	 *         been reached.
	 * @throws IndexOutOfBoundsException
	 *             if {@code offset < 0} or {@code length < 0}, or if
	 *             {@code offset + length} is greater than the length of
	 *             {@code buf}.
	 * @throws IOException
	 *             if this reader is closed or some other I/O error occurs.
	 */
	@Override
	public int read(char[] buf, int offset, int length) throws IOException {
		synchronized (lock) {
			if (!isOpen()) {
				throw new IOException("InputStreamReader is closed."); //$NON-NLS-1$
			}
			if (offset < 0 || offset > buf.length - length || length < 0) {
				throw new IndexOutOfBoundsException();
			}
			if (length == 0) {
				return 0;
			}

			if (bufferPos < 3 && !over) {
				int read = in.read(buffer, bufferPos, BUFFER_SIZE - bufferPos);
				if (read < 0) {
					over = true;
				} else {
					bufferPos += read;
				}
			}

			int pos = 0;
			int posC = 0;
			all: while (pos < bufferPos && posC < length) {
				int x, y, z;
				x = buffer[pos++];
				switch (UTF8.utf8SizeU((byte) x)) {
				case 0:
				default:
					buf[offset + posC] = '?';
					posC++;
					break;
				case 1:
					buf[offset + posC] = (char) x;
					posC++;
					break;
				case 2:
					if (pos == bufferPos) {
						if (over) {
							// It's really end, write a QM and
							buf[offset + posC] = '?';
							posC++;
							break all;
						} else {
							// Push back and quit
							pos--;
							break all;
						}
					}
					y = buffer[pos++];
					buf[offset + posC] = (char) (((x & 0x1f) << 6) + (y & 0x3f));
					break;
				case 3:
					if (pos == bufferPos) {
						if (over) {
							// It's really end, write a QM and
							buf[offset + posC] = '?';
							posC++;
							break all;
						} else {
							// Push back and quit
							pos--;
							break all;
						}
					}
					y = buffer[pos++];
					if (pos == bufferPos) {
						if (over) {
							// It's really end, write a QM and
							buf[offset + posC] = '?';
							posC++;
							break;
						} else {
							// Push back and quit
							pos -= 2;
							break;
						}
					}
					z = buffer[pos++];
					buf[offset + posC] = (char) (((x & 0xf) << 12)
							+ ((y & 0x3f) << 6) + (z & 0x3f));
					break;
				}
			}
			bufferPos -= pos;
			if (bufferPos > 0) {
				// move
				System.arraycopy(buffer, pos, buffer, 0, bufferPos - pos);
			}
			if (posC == 0 & over) {
				return -1;
			}

			return posC;
		}
	}

	/*
	 * Answer a boolean indicating whether or not this InputStreamReader is
	 * open.
	 */
	private boolean isOpen() {
		return in != null;
	}

	/**
	 * Indicates whether this reader is ready to be read without blocking. If
	 * the result is {@code true}, the next {@code read()} will not block. If
	 * the result is {@code false} then this reader may or may not block when
	 * {@code read()} is called. This implementation returns {@code true} if
	 * there are bytes available in the buffer or the source stream has bytes
	 * available.
	 *
	 * @return {@code true} if the receiver will not block when {@code read()}
	 *         is called, {@code false} if unknown or blocking will occur.
	 * @throws IOException
	 *             if this reader is closed or some other I/O error occurs.
	 */
	@Override
	public boolean ready() throws IOException {
		synchronized (lock) {
			if (in == null) {
				// luni.BA=InputStreamReader is closed.
				throw new IOException("Already closed"); //$NON-NLS-1$
			}
			try {
				return bufferPos + in.available() > 3;
			} catch (IOException e) {
				return false;
			}
		}
	}
}
