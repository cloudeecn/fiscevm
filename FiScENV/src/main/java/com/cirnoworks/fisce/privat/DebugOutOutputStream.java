package com.cirnoworks.fisce.privat;

import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;

public class DebugOutOutputStream extends OutputStream {

	private final String enc;
	private byte[] buf = new byte[512];
	private int pos = 0;

	public DebugOutOutputStream(String enc) {
		this.enc = enc;
	}

	public DebugOutOutputStream() {
		this("utf-8");
	}

	public void flush() {
		if (pos > 0) {
			try {
				FiScEVM.debugOut(new String(buf, 0, pos, enc));
			} catch (Exception e) {
			}
			pos = 0;
		}
	}

	@Override
	public void write(int b) throws IOException {
		if (b == '\n' || b == '\r') {
			flush();
			return;
		}
		if (pos == buf.length) {
			flush();
		}
		buf[pos++] = (byte) b;
	}

}
