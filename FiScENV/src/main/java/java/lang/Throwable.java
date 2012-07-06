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

import java.io.PrintStream;

import com.cirnoworks.fisce.privat.FiScEVM;

public class Throwable {

	private String detailMessage;

	private Throwable cause = this;

	private StackTraceElement[] stackTrace;

	public Throwable() {
		fillInStackTrace();
	}

	public Throwable(String message) {
		fillInStackTrace();
		detailMessage = message;
	}

	public Throwable(String message, Throwable cause) {
		fillInStackTrace();
		detailMessage = message;
		this.cause = cause;
	}

	public Throwable(Throwable cause) {
		fillInStackTrace();
		detailMessage = (cause == null ? null : cause.toString());
		this.cause = cause;
	}

	public String getMessage() {
		return detailMessage;
	}

	public String getLocalizedMessage() {
		return getMessage();
	}

	public Throwable getCause() {
		return (cause == this ? null : cause);
	}

	public synchronized Throwable initCause(Throwable cause) {
		if (this.cause != this)
			throw new IllegalStateException("Can't overwrite cause");
		if (cause == this)
			throw new IllegalArgumentException("Self-causation not permitted");
		this.cause = cause;
		return this;
	}

	public String toString() {
		String s = getClass().getName();
		String message = getLocalizedMessage();
		return (message != null) ? (s + ": " + message) : s;
	}

	public void printStackTrace() {
		printStackTrace(FiScEVM.debug);
	}

	public void printStackTrace(PrintStream s) {
		synchronized (s) {
			s.println(this);
			StackTraceElement[] trace = getOurStackTrace();
			for (int i = 0; i < trace.length; i++)
				s.println("\tat " + trace[i]);

			Throwable ourCause = getCause();
			if (ourCause != null)
				ourCause.printStackTraceAsCause(s, trace);
		}
	}

	private void printStackTraceAsCause(PrintStream s,
			StackTraceElement[] causedTrace) {
		// assert Thread.holdsLock(s);

		// Compute number of frames in common between this and caused
		StackTraceElement[] trace = getOurStackTrace();
		int m = trace.length - 1, n = causedTrace.length - 1;
		while (m >= 0 && n >= 0 && trace[m].equals(causedTrace[n])) {
			m--;
			n--;
		}
		int framesInCommon = trace.length - 1 - m;

		s.println("Caused by: " + this);
		for (int i = 0; i <= m; i++)
			s.println("\tat " + trace[i]);
		if (framesInCommon != 0)
			s.println("\t... " + framesInCommon + " more");

		// Recurse if we have a cause
		Throwable ourCause = getCause();
		if (ourCause != null)
			ourCause.printStackTraceAsCause(s, trace);
	}

	public synchronized Throwable fillInStackTrace() {
		fillInStackTrace0();
		return this;
	}

	public native void fillInStackTrace0();

	public StackTraceElement[] getStackTrace() {
		return (StackTraceElement[]) getOurStackTrace().clone();
	}

	private synchronized StackTraceElement[] getOurStackTrace() {
		return stackTrace;
	}

	public void setStackTrace(StackTraceElement[] stackTrace) {
		StackTraceElement[] defensiveCopy = (StackTraceElement[]) stackTrace
				.clone();
		for (int i = 0; i < defensiveCopy.length; i++)
			if (defensiveCopy[i] == null)
				throw new NullPointerException("stackTrace[" + i + "]");

		this.stackTrace = defensiveCopy;
	}

}
