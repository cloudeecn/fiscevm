package com.cirnoworks.libfisce.shell;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

public class NativeLoader {
	private static final HashMap<String, String[]> files = new HashMap<String, String[]>();
	static {
		files.put("macosx", new String[] { "libfyjni.jnilib" });
		files.put("macosx64", new String[] { "libfyjni64.jnilib" });
		files.put("linux", new String[] { "libfyjni.so" });
		files.put("linux64", new String[] { "libfyjni64.so" });
		files.put("windows", new String[] { "fyjni.dll" });
		files.put("windows64", new String[] { "fyjni64.dll" });
		try {
			ResourceBundle bundle = ResourceBundle.getBundle("platforms");
			Enumeration<String> names = bundle.getKeys();
			while (names.hasMoreElements()) {
				String key = names.nextElement();
				String value = bundle.getString(key);
				String[] fileNames = value.split("\\,");
				for (int i = 0, max = fileNames.length; i < max; i++) {
					fileNames[i] = fileNames[i].trim();
				}
				files.put(key, fileNames);
			}
		} catch (MissingResourceException e) {
		}
	}

	public static void extract(File dir, String os) {
		try {
			byte[] buffer = new byte[16384];
			String[] names = files.get(os);
			if (names == null) {
				throw new RuntimeException(
						"Can't find native libraries for os: " + os);
			}
			File overrideDir = new File("override");
			if (!overrideDir.isDirectory()) {
				overrideDir = null;
			}
			for (String name : names) {
				InputStream is = null;
				URL url = null;
				if (overrideDir != null) {
					File overrideFile = new File(overrideDir, name);
					if (overrideFile.isFile()) {
						System.out.println("Using override native library: "
								+ overrideFile.getAbsolutePath());
						is = new FileInputStream(overrideFile);
						url = overrideFile.toURI().toURL();
					}
				}
				if (is == null) {
					System.out.println("Using bundled native library: " + name);
					is = NativeLoader.class.getResourceAsStream("/" + name);
					url = NativeLoader.class.getResource("/" + name);
				}
				if (is == null) {
					throw new IOException("Can't find native library: " + name);
				}
				OutputStream out = null;
				try {
					File output = new File(dir, name);
					output.deleteOnExit();
					out = new FileOutputStream(output);
					System.out.println("Extract " + url.toString() + " to "
							+ output);
					int read = 0;
					while ((read = is.read(buffer)) >= 0) {
						out.write(buffer, 0, read);
					}
					System.load(output.getAbsolutePath());
				} finally {
					try {
						is.close();
					} catch (Exception e) {
					}
					try {
						out.close();
					} catch (Exception e) {
					}
				}
			}
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	public static String getOSPostfix() {
		String osArch = System.getProperty("os.arch");
		return ("amd64".equals(osArch) || "x86_64".equals(osArch)) ? "64" : "";
	}

	public static String getOS() {
		String osName = System.getProperty("os.name");
		if (osName.startsWith("Windows")) {
			return "windows";
		} else if ((osName.startsWith("Linux"))
				|| (osName.startsWith("FreeBSD"))
				|| (osName.startsWith("SunOS")) || (osName.startsWith("Unix"))) {
			return "linux";
		} else if ((osName.startsWith("Mac OS X"))
				|| (osName.startsWith("Darwin"))) {
			return "macosx";
		} else {
			return null;
		}
	}
}
