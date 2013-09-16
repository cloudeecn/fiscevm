package java.io;

public final class FileDescriptor {

	private int fd = -1;

	public FileDescriptor() {
	}

	public static final FileDescriptor in = new FileDescriptor();
	public static final FileDescriptor out = new FileDescriptor();
	public static final FileDescriptor err = new FileDescriptor();

	public boolean valid() {
		return fd != -1;
	}

	public void sync() {
	};
}
