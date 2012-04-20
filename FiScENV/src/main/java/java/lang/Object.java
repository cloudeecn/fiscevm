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

public class Object {

	static {
		new Finalizer().start();
		Boolean.valueOf("true");
		Byte.valueOf("1");
		Short.valueOf("1");
		Character.valueOf('1');
		Integer.valueOf("1");
		Float.valueOf("1");
		Long.valueOf("1");
		Double.valueOf("1");
		// FiScEVM.throwOut(new RuntimeException());
	}

	public final native Class<? extends Object> getClass();

	public int hashCode() {
		return System.identityHashCode(this);
	}

	public boolean equals(Object obj) {
		return (this == obj);
	}

	protected native Object clone() throws CloneNotSupportedException;

	public String toString() {
		return getClass().getName() + "@" + Integer.toHexString(hashCode());
	}

	public final native void notify();

	public final native void notifyAll();

	public final native void wait(long timeout) throws InterruptedException;

	public final void wait(long timeout, int nanos) throws InterruptedException {
		if (timeout < 0) {
			throw new IllegalArgumentException("timeout value is negative");
		}

		if (nanos < 0 || nanos > 999999) {
			throw new IllegalArgumentException(
					"nanosecond timeout value out of range");
		}

		if (nanos >= 500000 || (nanos != 0 && timeout == 0)) {
			timeout++;
		}

		wait(timeout);
	}

	public final void wait() throws InterruptedException {
		wait(0);
	}

	protected void finalize() throws Throwable {
	}
}
