/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

	@Override
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

	private StackTraceElement[] ourStackTrace;

	private synchronized StackTraceElement[] getOurStackTrace() {
		if (ourStackTrace == null && stackTrace != null) {
			int pos = -1;
			Class cl = this.getClass();
			String cn = cl.getName();
			for (int i = 0, max = stackTrace.length; i < max; i++) {
				FiScEVM.debug.println("$$$$" + stackTrace[i].getClassName()
						+ " " + cn);
				if (stackTrace[i] != null
						&& cn.equals(stackTrace[i].getClassName())) {
					pos = i;
				} else {
					try {
						Class clazz = Class.forName(stackTrace[i]
								.getClassName());
						if (!clazz.isAssignableFrom(cl)) {
							break;
						}
					} catch (Throwable t) {

					}
				}
			}
			FiScEVM.debug.println("pos=" + pos);
			if (pos >= 0 && pos < stackTrace.length - 1) {
				int len = stackTrace.length - 1 - pos;
				ourStackTrace = new StackTraceElement[len];
				System.arraycopy(stackTrace, pos + 1, ourStackTrace, 0, len);
			} else {
				ourStackTrace = stackTrace;
			}
		}
		return ourStackTrace;
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
