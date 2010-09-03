/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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
package com.cirnoworks.fisce.privat;

import java.io.IOException;
import java.io.OutputStream;

public class SystemOutputStream extends OutputStream {
	private String channel;

	public SystemOutputStream(String channel) {
		this.channel = channel;
	}

	public void write(int b) throws IOException {
		write0(b, channel);
	}

	private native void write0(int b, String channel) throws IOException;

}
