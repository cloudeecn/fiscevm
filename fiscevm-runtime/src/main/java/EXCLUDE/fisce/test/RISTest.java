package EXCLUDE.fisce.test;

import java.io.InputStream;

import com.cirnoworks.fisce.privat.FiScEVM;

import static EXCLUDE.fisce.test.TestService.*;

public class RISTest {
	private final static byte[] baseline = "1234567890abcdefghijklmnopqrstuvwxyz"
			.getBytes();

	public static void main(String[] args) {
		try {
			byte[] buf = new byte[1024];
			int read;
			int pos;

			InputStream is = RISTest.class
					.getResourceAsStream("/EXCLUDE/fisce/test/StreamTest1.txt");
			assertTrue(is == null, "File not found but stream got");
			is = RISTest.class
					.getResourceAsStream("/EXCLUDE/fisce/test/StreamTest.txt");
			assertTrue(is != null, "Stream not got");
			pos = 0;
			while ((read = is.read()) >= 0) {
				assertEqual(baseline[pos++], (byte) read);
				// buf[pos] = (byte) read;
			}
			is.close();

			for (int i = 0; i < 128; i++) {
				is = RISTest.class
						.getResourceAsStream("/EXCLUDE/fisce/test/StreamTest.txt");
				is.close();
			}

			is = RISTest.class
					.getResourceAsStream("/EXCLUDE/fisce/test/StreamTest.txt");
			pos = 0;
			while ((read = is.read(buf, pos, buf.length - pos)) >= 0) {
				pos += read;
				System.out.println(read);
			}
			assertEqual(baseline.length, pos);
			for (int i = 0; i < pos; i++) {
				assertEqual(baseline[i], buf[i]);
			}
			is.close();
			is = RISTest.class
					.getResourceAsStream("/EXCLUDE/fisce/test/StreamTest.txt");
			pos = 0;
			{
				int left = 10;
				while (left > 0 && (read = is.read(buf, pos, left)) >= 0) {
					left -= read;
					pos += read;
					System.out.println(read);
				}
				assertTrue(left == 0);
			}
			FiScEVM.save();
			while ((read = is.read(buf, pos, buf.length - pos)) >= 0) {
				pos += read;
				System.out.println(read);
			}
			assertEqual(baseline.length, pos);
			for (int i = 0; i < pos; i++) {
				assertEqual(baseline[i], buf[i]);
			}
			is.close();
		} catch (Throwable t) {
			fail("Unexpected exception thrown", t);
		}
	}
}
