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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package java.io;

public abstract class InputStream {

	private static final int SKIP_BUFFER_SIZE = 2048;
	private static byte[] skipBuffer;

	public abstract int read() throws IOException;

	public int read(byte b[]) throws IOException {
		return read(b, 0, b.length);
	}

	public int read(byte b[], int off, int len) throws IOException {
		if (b == null) {
			throw new NullPointerException();
		} else if ((off < 0) || (off > b.length) || (len < 0)
				|| ((off + len) > b.length) || ((off + len) < 0)) {
			throw new IndexOutOfBoundsException();
		} else if (len == 0) {
			return 0;
		}

		int c = read();
		if (c == -1) {
			return -1;
		}
		b[off] = (byte) c;

		int i = 1;
		try {
			for (; i < len; i++) {
				c = read();
				if (c == -1) {
					break;
				}
				if (b != null) {
					b[off + i] = (byte) c;
				}
			}
		} catch (IOException ee) {
		}
		return i;
	}

	public long skip(long n) throws IOException {

		long remaining = n;
		int nr;
		if (skipBuffer == null)
			skipBuffer = new byte[SKIP_BUFFER_SIZE];

		byte[] localSkipBuffer = skipBuffer;

		if (n <= 0) {
			return 0;
		}

		while (remaining > 0) {
			nr = read(localSkipBuffer, 0,
					(int) (SKIP_BUFFER_SIZE < remaining ? SKIP_BUFFER_SIZE : remaining));
			if (nr < 0) {
				break;
			}
			remaining -= nr;
		}

		return n - remaining;
	}

	public int available() throws IOException {
		return 0;
	}

	public void close() throws IOException {
	}

	public synchronized void mark(int readlimit) {
	}

	public synchronized void reset() throws IOException {
		throw new IOException("mark/reset not supported");
	}

	public boolean markSupported() {
		return false;
	}

}
