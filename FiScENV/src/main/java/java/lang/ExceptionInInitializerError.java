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
package java.lang;

public class ExceptionInInitializerError extends LinkageError {

	private Throwable exception;

	public ExceptionInInitializerError() {
		initCause(null);
	}

	public ExceptionInInitializerError(Throwable thrown) {
		initCause(null);
		this.exception = thrown;
	}

	public ExceptionInInitializerError(String s) {
		super(s);
		initCause(null);
	}

	public Throwable getException() {
		return exception;
	}

	public Throwable getCause() {
		return exception;
	}
}
