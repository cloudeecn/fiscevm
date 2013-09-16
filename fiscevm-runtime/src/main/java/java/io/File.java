package java.io;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;

/**
 * mock
 */

public class File implements Serializable, Comparable<File> {
	private String path;

	public static final char separatorChar = '/';
	public static final String separator = "/";
	public static final char pathSeparatorChar = ':';
	public static final String pathSeparator = ":";

	/**
	 * Creates a new <code>File</code> instance by converting the given pathname
	 * string into an abstract pathname. If the given string is the empty
	 * string, then the result is the empty abstract pathname.
	 * 
	 * @param pathname
	 *            A pathname string
	 * @throws NullPointerException
	 *             If the <code>pathname</code> argument is <code>null</code>
	 */
	public File(String pathname) {
		if (pathname == null) {
			throw new NullPointerException();
		}
		if (!pathname.startsWith("/")) {
			pathname = "/" + pathname;
		}
		this.path = pathname;
		if (this.path.endsWith("/")) {
			this.path = this.path.substring(0, this.path.length() - 1);
		}
	}

	public File(String parent, String child) {
		if (child == null) {
			throw new NullPointerException();
		}
		if (parent != null) {
			if (!parent.startsWith("/")) {
				parent = "/" + parent;
			}
			if (parent.endsWith("/")) {
				this.path = parent + child;
			} else {
				this.path = parent + "/" + child;
			}
		} else {
			if (!child.startsWith("/")) {
				child = "/" + child;
			}
			this.path = child;
		}
		if (this.path.endsWith("/")) {
			this.path = this.path.substring(0, this.path.length() - 1);
		}
	}

	public File(File parent, String child) {
		this(parent.getAbsolutePath(), child);
	}

	public File(URI uri) {

		// Check our many preconditions
		if (!uri.isAbsolute())
			throw new IllegalArgumentException("URI is not absolute");
		if (uri.isOpaque())
			throw new IllegalArgumentException("URI is not hierarchical");
		String scheme = uri.getScheme();
		if ((scheme == null) || !scheme.equalsIgnoreCase("file"))
			throw new IllegalArgumentException("URI scheme is not \"file\"");
		if (uri.getAuthority() != null)
			throw new IllegalArgumentException("URI has an authority component");
		if (uri.getFragment() != null)
			throw new IllegalArgumentException("URI has a fragment component");
		if (uri.getQuery() != null)
			throw new IllegalArgumentException("URI has a query component");
		String p = uri.getPath();
		if (p.equals(""))
			throw new IllegalArgumentException("URI path component is empty");

		if (p == null) {
			throw new NullPointerException();
		}
		if (!p.startsWith("/")) {
			p = "/" + p;
		}
		this.path = p;
		if (this.path.endsWith("/")) {
			this.path = this.path.substring(0, this.path.length() - 1);
		}
	}

	public String getName() {
		int index = path.lastIndexOf(separatorChar);
		return path.substring(index + 1);
	}

	public String getParent() {
		int index = path.lastIndexOf(separatorChar);
		return path.substring(0, index);
	}

	public File getParentFile() {
		String p = this.getParent();
		if (p == null)
			return null;
		return new File(p);
	}

	public String getPath() {
		return path;
	}

	public boolean isAbsolute() {
		return true;
	}

	public String getAbsolutePath() {
		return path;
	}

	public File getAbsoluteFile() {
		String absPath = getAbsolutePath();
		return new File(absPath);
	}

	public String getCanonicalPath() throws IOException {
		return path;
	}

	public File getCanonicalFile() throws IOException {
		return this;
	}

	public URL toURL() throws MalformedURLException {
		return new URL("file", "", path);
	}

	public URI toURI() {
		try {
			File f = getAbsoluteFile();
			return new URI("file", null, path, null);
		} catch (URISyntaxException x) {
			throw new Error(x); // Can't happen
		}
	}

	public boolean canRead() {
		return true;
	}

	public boolean canWrite() {
		return false;
	}

	public boolean exists() {
		return true;
	}

	public boolean isDirectory() {
		return false;
	}

	public boolean isFile() {
		return true;
	}

	public boolean isHidden() {
		return false;
	}

	public long lastModified() {
		return 0l;
	}

	public long length() {
		return 0l;
	}

	public boolean createNewFile() throws IOException {
		return false;
	}

	public boolean delete() {
		return false;
	}

	public void deleteOnExit() {
	}

	public String[] list() {
		return new String[0];
	}

	public String[] list(FilenameFilter filter) {
		return new String[0];
	}

	public File[] listFiles() {
		return new File[0];
	}

	public File[] listFiles(FilenameFilter filter) {
		return new File[0];
	}

	public File[] listFiles(FileFilter filter) {
		return new File[0];
	}

	public boolean mkdir() {
		return false;
	}

	public boolean mkdirs() {
		return false;
	}

	public boolean renameTo(File dest) {
		return false;
	}

	public boolean setLastModified(long time) {
		return false;
	}

	public boolean setReadOnly() {
		return false;
	}

	public boolean setWritable(boolean writable, boolean ownerOnly) {
		return false;
	}

	public boolean setWritable(boolean writable) {
		return false;
	}

	public boolean setReadable(boolean readable, boolean ownerOnly) {
		return false;
	}

	public boolean setReadable(boolean readable) {
		return false;
	}

	public boolean setExecutable(boolean executable, boolean ownerOnly) {
		return false;
	}

	public boolean setExecutable(boolean executable) {
		return setExecutable(executable, true);
	}

	public boolean canExecute() {
		return false;
	}

	public static File[] listRoots() {
		return new File[] { new File("/") };
	}

	public long getTotalSpace() {
		return 0;
	}

	public long getFreeSpace() {
		return 0;
	}

	public long getUsableSpace() {
		return 0;
	}

	public static File createTempFile(String prefix, String suffix,
			File directory) throws IOException {
		throw new IOException("Can't create file in fisce");
	}

	public static File createTempFile(String prefix, String suffix)
			throws IOException {
		throw new IOException("Can't create file in fisce");
	}

	public int compareTo(File pathname) {
		return path.compareTo(pathname.path);
	}

	public boolean equals(Object obj) {
		if ((obj != null) && (obj instanceof File)) {
			return compareTo((File) obj) == 0;
		}
		return false;
	}

	public int hashCode() {
		return path.hashCode() + 1;
	}

	public String toString() {
		return getPath();
	}

}
