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


public abstract class Enum<E extends Enum<E>> implements Comparable<E> {

	private final String name;

	public final String name() {
		return name;
	}

	private final int ordinal;

	public final int ordinal() {
		return ordinal;
	}

	protected Enum(String name, int ordinal) {
		this.name = name;
		this.ordinal = ordinal;
	}

	@Override
	public String toString() {
		return name;
	}

	@Override
	public final boolean equals(Object other) {
		return this == other;
	}

	@Override
	public final int hashCode() {
		return super.hashCode();
	}

	@Override
	protected final Object clone() throws CloneNotSupportedException {
		throw new CloneNotSupportedException();
	}

	public final int compareTo(E o) {
		Enum other = (Enum) o;
		Enum self = this;
		if (self.getClass() != other.getClass() && // optimization
				self.getDeclaringClass() != other.getDeclaringClass())
			throw new ClassCastException();
		return self.ordinal - other.ordinal;
	}

	public final Class<E> getDeclaringClass() {
		Class clazz = getClass();
		Class zuper = clazz.getSuperclass();
		return (zuper == Enum.class) ? clazz : zuper;
	}

	public static <T extends Enum<T>> T valueOf(Class<T> enumType, String name) {
		T result = enumType.enumConstantDirectory().get(name);
		if (result != null)
			return result;
		if (name == null)
			throw new NullPointerException("Name is null");
		throw new IllegalArgumentException("No enum const " + enumType + "."
				+ name);
	}

	/**
	 * enum classes cannot have finalize methods.
	@Override
	protected final void finalize() {
	}
	 */
}
