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
package java.io;

import com.cirnoworks.fisce.privat.FiScEVM;

public class PrintStream {

	private boolean autoFlush = false;
	private boolean trouble = false;
	private OutputStream out;

	public PrintStream(OutputStream out) {
		this(out, false);
	}

	public PrintStream(OutputStream out, boolean autoFlush) {
		this.out = out;
		if (out == null)
			throw new NullPointerException("Null output stream");
		this.autoFlush = autoFlush;
	}

	private void ensureOpen() throws IOException {
		if (out == null)
			throw new IOException("Stream closed");
	}

	public void flush() {
		synchronized (this) {
			try {
				ensureOpen();
				out.flush();
			} catch (IOException x) {
				trouble = true;
			}
		}
	}

	private boolean closing = false;

	public void close() {
		synchronized (this) {
			if (!closing) {
				closing = true;
				try {
					out.close();
				} catch (IOException x) {
					trouble = true;
				}
				out = null;
			}
		}
	}

	public boolean checkError() {
		if (out != null)
			flush();
		return trouble;
	}

	protected void setError() {
		trouble = true;
	}

	public void write(int b) {
		try {
			synchronized (this) {
				ensureOpen();
				out.write(b);
				if ((b == '\n') && autoFlush)
					out.flush();
			}
		} catch (IOException x) {
			trouble = true;
		}
	}

	public void write(byte buf[], int off, int len) {
		try {
			synchronized (this) {
				ensureOpen();
				out.write(buf, off, len);
				if (autoFlush)
					out.flush();
			}
		} catch (IOException x) {
			trouble = true;
		}
	}

	public void write(byte[] buf) {
		write(buf, 0, buf.length);
	}

	private void write(char buf[]) {
		try {
			synchronized (this) {
				ensureOpen();
				out.write(FiScEVM.encode(buf, 0, buf.length));
				if (autoFlush) {
					for (int i = 0; i < buf.length; i++) {
						if (buf[i] == '\n') {
							out.flush();
							break;
						}
					}
				}
			}
		} catch (IOException x) {
			trouble = true;
		}
	}

	private void write(String s) {
		write(s.toCharArray());
	}

	private void newLine() {
		write('\n');
	}

	public void print(boolean b) {
		write(b ? "true" : "false");
	}

	public void print(char c) {
		write(String.valueOf(c));
	}

	public void print(int i) {
		write(String.valueOf(i));
	}

	public void print(long l) {
		write(String.valueOf(l));
	}

	public void print(float f) {
		write(String.valueOf(f));
	}

	public void print(double d) {
		write(String.valueOf(d));
	}

	public void print(char s[]) {
		write(s);
	}

	public void print(String s) {
		if (s == null) {
			s = "null";
		}
		write(s);
	}

	public void print(Object obj) {
		write(String.valueOf(obj));
	}

	public void println() {
		newLine();
	}

	public void println(boolean x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(char x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(int x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(long x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(float x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(double x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(char x[]) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(String x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}

	public void println(Object x) {
		synchronized (this) {
			print(x);
			newLine();
		}
	}
}
