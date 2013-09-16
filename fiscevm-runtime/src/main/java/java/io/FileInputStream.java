package java.io;

public class FileInputStream extends InputStream {
	private InputStream is;

	public FileInputStream(String name) throws FileNotFoundException {
		is = getClass().getResourceAsStream(name);
		if (is == null) {
			throw new FileNotFoundException(name);
		}
	}

	public FileInputStream(File file) throws FileNotFoundException {
		this(file.getAbsolutePath());
	}

	public FileInputStream(FileDescriptor fdObj) {
		throw new UnsupportedOperationException();
	}

	public int hashCode() {
		return is.hashCode();
	}

	public boolean equals(Object obj) {
		return is.equals(obj);
	}

	public String toString() {
		return is.toString();
	}

	public int available() throws IOException {
		return is.available();
	}

	public void close() throws IOException {
		is.close();
	}

	public void mark(int readlimit) {
		is.mark(readlimit);
	}

	public boolean markSupported() {
		return is.markSupported();
	}

	public int read() throws IOException {
		return is.read();
	}

	public int read(byte[] b) throws IOException {
		return is.read(b);
	}

	public int read(byte[] b, int offset, int length) throws IOException {
		return is.read(b, offset, length);
	}

	public void reset() throws IOException {
		is.reset();
	}

	public long skip(long n) throws IOException {
		return is.skip(n);
	}

}
