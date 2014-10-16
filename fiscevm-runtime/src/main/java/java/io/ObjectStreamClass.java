/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

package java.io;


/**
 * Represents a descriptor for identifying a class during serialization and
 * deserialization. Information contained in the descriptor includes the name
 * and SUID of the class as well as field names and types. Information inherited
 * from the superclasses is also taken into account.
 * 
 * @see ObjectOutputStream
 * @see ObjectInputStream
 * @see java.lang.Class
 */
public class ObjectStreamClass implements Serializable {

	/*
	 * If an ObjectStreamClass describes an Externalizable class, it (the
	 * descriptor) should not have field descriptors (ObjectStreamField) at all.
	 * The ObjectStreamClass that gets saved should simply have no field info.
	 * This is a footnote in page 1511 (class Serializable) of "The Java Class
	 * Libraries, Second Edition, Vol. I".
	 */

	/**
	 * Constructs a new instance of this class.
	 */
	ObjectStreamClass() {
		super();
	}

	/**
	 * Returns the class (java.lang.Class) for this descriptor.
	 * 
	 * @return the class in the local VM that this descriptor represents;
	 *         {@code null} if there is no corresponding class.
	 */
	public Class<?> forClass() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets a field descriptor of the class represented by this class
	 * descriptor.
	 * 
	 * @param name
	 *            the name of the desired field.
	 * @return the field identified by {@code name} or {@code null} if there is
	 *         no such field.
	 */
	public ObjectStreamField getField(String name) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns a collection of field descriptors for the serialized fields of
	 * the class represented by this class descriptor.
	 * 
	 * @return an array of field descriptors or an array of length zero if there
	 *         are no fields in this descriptor's class.
	 */
	public ObjectStreamField[] getFields() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns the name of the class represented by this descriptor.
	 * 
	 * @return the fully qualified name of the class this descriptor represents.
	 */
	public String getName() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns the Serial Version User ID of the class represented by this
	 * descriptor.
	 * 
	 * @return the SUID for the class represented by this descriptor.
	 */
	public long getSerialVersionUID() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns the descriptor (ObjectStreamClass) of the superclass of the class
	 * represented by the receiver.
	 * 
	 * @return an ObjectStreamClass representing the superclass of the class
	 *         represented by the receiver.
	 */
	ObjectStreamClass getSuperclass() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns the descriptor corresponding to the class {@code cl}. If the
	 * class is not serializable or externalizable then {@code null} is
	 * returned.
	 * 
	 * @param cl
	 *            a java.langClass for which to obtain the corresponding
	 *            descriptor
	 * @return the corresponding descriptor if the {@code cl} is serializable or
	 *         externalizable; {@code null} otherwise.
	 */
	public static ObjectStreamClass lookup(Class<?> cl) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Return the descriptor (ObjectStreamClass) corresponding to the class
	 * <code>cl</code>. This method does not check whether the class implements
	 * Serializable or Externalizable.
	 * 
	 * @param cl
	 *            a java.lang.Class for which to obtain the corresponding
	 *            descriptor
	 * @return The corresponding descriptor for the specified class
	 * @since 1.6
	 */
	public static ObjectStreamClass lookupAny(Class<?> cl) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Returns a string containing a concise, human-readable description of this
	 * descriptor.
	 * 
	 * @return a printable representation of this descriptor.
	 */
	@Override
	public String toString() {
		throw new UnsupportedOperationException();
	}


}
