package java.io;

public class FileOutputStream extends OutputStream {
	public FileOutputStream(String name) throws FileNotFoundException {
		this(name != null ? new File(name) : null, false);
	}

	public FileOutputStream(String name, boolean append)
			throws FileNotFoundException {
		this(name != null ? new File(name) : null, append);
	}

	public FileOutputStream(File file) throws FileNotFoundException {
		this(file, false);
	}

	public FileOutputStream(File file, boolean append)
			throws FileNotFoundException {
		throw new FileNotFoundException(file.getAbsolutePath());
	}

	public FileOutputStream(FileDescriptor fdObj) {
		throw new UnsupportedOperationException();
	}

	@Override
	public void write(int oneByte) throws IOException {
		throw new UnsupportedOperationException();
	}
}
