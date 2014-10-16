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

import java.lang.reflect.Method;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.util.HashMap;

import org.apache.harmony.luni.internal.nls.Messages;

/**
 * A specialized {@link InputStream} that is able to read (deserialize) Java
 * objects as well as primitive data types (int, byte, char etc.). The data has
 * typically been saved using an ObjectOutputStream.
 * 
 * @see ObjectOutputStream
 * @see ObjectInput
 * @see Serializable
 * @see Externalizable
 */
public class ObjectInputStream extends InputStream implements ObjectInput,
		ObjectStreamConstants {

	private InputStream emptyStream = new ByteArrayInputStream(new byte[0]);

	// If the receiver has already read & not consumed a TC code
	private boolean hasPushbackTC;

	// Push back TC code if the variable above is true
	private byte pushbackTC;

	// Where we read from
	private DataInputStream input;

	// Where we read primitive types from
	private DataInputStream primitiveTypes;

	// Where we keep primitive type data
	private InputStream primitiveData = emptyStream;

	// Resolve object is a mechanism for replacement
	private boolean enableResolve;

	// Original caller's class loader, used to perform class lookups
	private ClassLoader callerClassLoader;

	private static final HashMap<String, Class<?>> PRIMITIVE_CLASSES = new HashMap<String, Class<?>>();

	static {
		PRIMITIVE_CLASSES.put("byte", byte.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("short", short.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("int", int.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("long", long.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("boolean", boolean.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("char", char.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("float", float.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("double", double.class); //$NON-NLS-1$
		PRIMITIVE_CLASSES.put("void", void.class); //$NON-NLS-1$
	}

	/**
	 * GetField is an inner class that provides access to the persistent fields
	 * read from the source stream.
	 */
	public abstract static class GetField {
		/**
		 * Gets the ObjectStreamClass that describes a field.
		 *
		 * @return the descriptor class for a serialized field.
		 */
		public abstract ObjectStreamClass getObjectStreamClass();

		/**
		 * Indicates if the field identified by {@code name} is defaulted. This
		 * means that it has no value in this stream.
		 *
		 * @param name
		 *            the name of the field to check.
		 * @return {@code true} if the field is defaulted, {@code false}
		 *         otherwise.
		 * @throws IllegalArgumentException
		 *             if {@code name} does not identify a serializable field.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 */
		public abstract boolean defaulted(String name) throws IOException,
				IllegalArgumentException;

		/**
		 * Gets the value of the boolean field identified by {@code name} from
		 * the persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code boolean}.
		 */
		public abstract boolean get(String name, boolean defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the character field identified by {@code name} from
		 * the persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code char}.
		 */
		public abstract char get(String name, char defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the byte field identified by {@code name} from the
		 * persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code byte}.
		 */
		public abstract byte get(String name, byte defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the short field identified by {@code name} from the
		 * persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code short}.
		 */
		public abstract short get(String name, short defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the integer field identified by {@code name} from
		 * the persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code int}.
		 */
		public abstract int get(String name, int defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the long field identified by {@code name} from the
		 * persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code long}.
		 */
		public abstract long get(String name, long defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the float field identified by {@code name} from the
		 * persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code float} is
		 *             not {@code char}.
		 */
		public abstract float get(String name, float defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the double field identified by {@code name} from
		 * the persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code double}.
		 */
		public abstract double get(String name, double defaultValue)
				throws IOException, IllegalArgumentException;

		/**
		 * Gets the value of the object field identified by {@code name} from
		 * the persistent field.
		 *
		 * @param name
		 *            the name of the field to get.
		 * @param defaultValue
		 *            the default value that is used if the field does not have
		 *            a value when read from the source stream.
		 * @return the value of the field identified by {@code name}.
		 * @throws IOException
		 *             if an error occurs while reading from the source input
		 *             stream.
		 * @throws IllegalArgumentException
		 *             if the type of the field identified by {@code name} is
		 *             not {@code Object}.
		 */
		public abstract Object get(String name, Object defaultValue)
				throws IOException, IllegalArgumentException;
	}

	/**
	 * Constructs a new ObjectInputStream. This default constructor can be used
	 * by subclasses that do not want to use the public constructor if it
	 * allocates unneeded data.
	 * 
	 * @throws IOException
	 *             if an error occurs when creating this stream.
	 * @throws SecurityException
	 *             if a security manager is installed and it denies subclassing
	 *             this class.
	 * @see SecurityManager#checkPermission(java.security.Permission)
	 */
	protected ObjectInputStream() throws IOException, SecurityException {
		super();
		SecurityManager currentManager = System.getSecurityManager();
		if (currentManager != null) {
			currentManager.checkPermission(SUBCLASS_IMPLEMENTATION_PERMISSION);
		}
		// WARNING - we should throw IOException if not called from a subclass
		// according to the JavaDoc. Add the test.
	}

	/**
	 * Constructs a new ObjectInputStream that reads from the InputStream
	 * {@code input}.
	 * 
	 * @param input
	 *            the non-null source InputStream to filter reads on.
	 * @throws IOException
	 *             if an error occurs while reading the stream header.
	 * @throws StreamCorruptedException
	 *             if the source stream does not contain serialized objects that
	 *             can be read.
	 * @throws SecurityException
	 *             if a security manager is installed and it denies subclassing
	 *             this class.
	 */
	public ObjectInputStream(InputStream input)
			throws StreamCorruptedException, IOException {
		final Class<?> implementationClass = getClass();
		final Class<?> thisClass = ObjectInputStream.class;
		SecurityManager sm = System.getSecurityManager();
		if (sm != null && implementationClass != thisClass) {
			boolean mustCheck = (AccessController
					.doPrivileged(new PrivilegedAction<Boolean>() {
						public Boolean run() {
							try {
								Method method = implementationClass
										.getMethod("readFields");
								if (method.getDeclaringClass() != thisClass) {
									return Boolean.TRUE;
								}
							} catch (NoSuchMethodException e) {
							}
							try {
								Method method = implementationClass.getMethod(
										"readUnshared", Object.class);
								if (method.getDeclaringClass() != thisClass) {
									return Boolean.TRUE;
								}
							} catch (NoSuchMethodException e) {
							}
							return Boolean.FALSE;
						}
					})).booleanValue();
			if (mustCheck) {
				sm.checkPermission(ObjectStreamConstants.SUBCLASS_IMPLEMENTATION_PERMISSION);
			}
		}
		this.input = (input instanceof DataInputStream) ? (DataInputStream) input
				: new DataInputStream(input);
		primitiveTypes = new DataInputStream(this);
		enableResolve = false;
		resetState();
		// So read...() methods can be used by
		// subclasses during readStreamHeader()
		primitiveData = this.input;
		// Has to be done here according to the specification
		readStreamHeader();
		primitiveData = emptyStream;
	}

	/**
	 * Returns the number of bytes of primitive data that can be read from this
	 * stream without blocking. This method should not be used at any arbitrary
	 * position; just when reading primitive data types (int, char etc).
	 * 
	 * @return the number of available primitive data bytes.
	 * @throws IOException
	 *             if any I/O problem occurs while computing the available
	 *             bytes.
	 */
	@Override
	public int available() throws IOException {
		// returns 0 if next data is an object, or N if reading primitive types
		checkReadPrimitiveTypes();
		return primitiveData.available();
	}

	/**
	 * Checks to if it is ok to read primitive types from this stream at this
	 * point. One is not supposed to read primitive types when about to read an
	 * object, for example, so an exception has to be thrown.
	 * 
	 * @throws IOException
	 *             If any IO problem occurred when trying to read primitive type
	 *             or if it is illegal to read primitive types
	 */
	private void checkReadPrimitiveTypes() throws IOException {
		// If we still have primitive data, it is ok to read primitive data
		if (primitiveData == input || primitiveData.available() > 0) {
			return;
		}

		// If we got here either we had no Stream previously created or
		// we no longer have data in that one, so get more bytes
		do {
			int next = 0;
			if (hasPushbackTC) {
				hasPushbackTC = false;
			} else {
				next = input.read();
				pushbackTC = (byte) next;
			}
			switch (pushbackTC) {
			case TC_BLOCKDATA:
				primitiveData = new ByteArrayInputStream(readBlockData());
				return;
			case TC_BLOCKDATALONG:
				primitiveData = new ByteArrayInputStream(readBlockDataLong());
				return;
			case TC_RESET:
				resetState();
				break;
			default:
				if (next != -1) {
					pushbackTC();
				}
				return;
			}
			// Only TC_RESET falls through
		} while (true);
	}

	/**
	 * Closes this stream. This implementation closes the source stream.
	 * 
	 * @throws IOException
	 *             if an error occurs while closing this stream.
	 */
	@Override
	public void close() throws IOException {
		input.close();
	}

	/**
	 * Default method to read objects from this stream. Serializable fields
	 * defined in the object's class and superclasses are read from the source
	 * stream.
	 * 
	 * @throws ClassNotFoundException
	 *             if the object's class cannot be found.
	 * @throws IOException
	 *             if an I/O error occurs while reading the object data.
	 * @throws NotActiveException
	 *             if this method is not called from {@code readObject()}.
	 * @see ObjectOutputStream#defaultWriteObject
	 */
	public void defaultReadObject() throws IOException, ClassNotFoundException,
			NotActiveException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Enables object replacement for this stream. By default this is not
	 * enabled. Only trusted subclasses (loaded with system class loader) are
	 * allowed to change this status.
	 * 
	 * @param enable
	 *            {@code true} to enable object replacement; {@code false} to
	 *            disable it.
	 * @return the previous setting.
	 * @throws SecurityException
	 *             if a security manager is installed and it denies enabling
	 *             object replacement for this stream.
	 * @see #resolveObject
	 * @see ObjectOutputStream#enableReplaceObject
	 */
	protected boolean enableResolveObject(boolean enable)
			throws SecurityException {
		if (enable) {
			// The Stream has to be trusted for this feature to be enabled.
			// trusted means the stream's classloader has to be null
			SecurityManager currentManager = System.getSecurityManager();
			if (currentManager != null) {
				currentManager.checkPermission(SUBSTITUTION_PERMISSION);
			}
		}
		boolean originalValue = enableResolve;
		enableResolve = enable;
		return originalValue;
	}

	/**
	 * Pushes back the last TC code read
	 */
	private void pushbackTC() {
		hasPushbackTC = true;
	}

	/**
	 * Reads a single byte from the source stream and returns it as an integer
	 * in the range from 0 to 255. Returns -1 if the end of the source stream
	 * has been reached. Blocks if no input is available.
	 * 
	 * @return the byte read or -1 if the end of the source stream has been
	 *         reached.
	 * @throws IOException
	 *             if an error occurs while reading from this stream.
	 */
	@Override
	public int read() throws IOException {
		checkReadPrimitiveTypes();
		return primitiveData.read();
	}

	/**
	 * Reads at most {@code length} bytes from the source stream and stores them
	 * in byte array {@code buffer} starting at offset {@code count}. Blocks
	 * until {@code count} bytes have been read, the end of the source stream is
	 * detected or an exception is thrown.
	 * 
	 * @param buffer
	 *            the array in which to store the bytes read.
	 * @param offset
	 *            the initial position in {@code buffer} to store the bytes read
	 *            from the source stream.
	 * @param length
	 *            the maximum number of bytes to store in {@code buffer}.
	 * @return the number of bytes read or -1 if the end of the source input
	 *         stream has been reached.
	 * @throws IndexOutOfBoundsException
	 *             if {@code offset < 0} or {@code length < 0}, or if
	 *             {@code offset + length} is greater than the length of
	 *             {@code buffer}.
	 * @throws IOException
	 *             if an error occurs while reading from this stream.
	 * @throws NullPointerException
	 *             if {@code buffer} is {@code null}.
	 */
	@Override
	public int read(byte[] buffer, int offset, int length) throws IOException {
		// Force buffer null check first!
		if (offset > buffer.length || offset < 0) {
			// luni.12=Offset out of bounds \: {0}
			throw new ArrayIndexOutOfBoundsException(Messages.getString(
					"luni.12", offset)); //$NON-NLS-1$
		}
		if (length < 0 || length > buffer.length - offset) {
			// luni.18=Length out of bounds \: {0}
			throw new ArrayIndexOutOfBoundsException(Messages.getString(
					"luni.18", length)); //$NON-NLS-1$
		}
		if (length == 0) {
			return 0;
		}
		checkReadPrimitiveTypes();
		return primitiveData.read(buffer, offset, length);
	}

	/**
	 * Reads and returns an array of raw bytes with primitive data. The array
	 * will have up to 255 bytes. The primitive data will be in the format
	 * described by {@code DataOutputStream}.
	 * 
	 * @return The primitive data read, as raw bytes
	 * 
	 * @throws IOException
	 *             If an IO exception happened when reading the primitive data.
	 */
	private byte[] readBlockData() throws IOException {
		byte[] result = new byte[input.readByte() & 0xff];
		input.readFully(result);
		return result;
	}

	/**
	 * Reads and returns an array of raw bytes with primitive data. The array
	 * will have more than 255 bytes. The primitive data will be in the format
	 * described by {@code DataOutputStream}.
	 * 
	 * @return The primitive data read, as raw bytes
	 * 
	 * @throws IOException
	 *             If an IO exception happened when reading the primitive data.
	 */
	private byte[] readBlockDataLong() throws IOException {
		byte[] result = new byte[input.readInt()];
		input.readFully(result);
		return result;
	}

	/**
	 * Reads a boolean from the source stream.
	 * 
	 * @return the boolean value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public boolean readBoolean() throws IOException {
		return primitiveTypes.readBoolean();
	}

	/**
	 * Reads a byte (8 bit) from the source stream.
	 * 
	 * @return the byte value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public byte readByte() throws IOException {
		return primitiveTypes.readByte();
	}

	/**
	 * Reads a character (16 bit) from the source stream.
	 * 
	 * @return the char value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public char readChar() throws IOException {
		return primitiveTypes.readChar();
	}

	/**
	 * Reads a double (64 bit) from the source stream.
	 * 
	 * @return the double value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public double readDouble() throws IOException {
		return primitiveTypes.readDouble();
	}

	/**
	 * Reads the persistent fields of the object that is currently being read
	 * from the source stream. The values read are stored in a GetField object
	 * that provides access to the persistent fields. This GetField object is
	 * then returned.
	 * 
	 * @return the GetField object from which persistent fields can be accessed
	 *         by name.
	 * @throws ClassNotFoundException
	 *             if the class of an object being deserialized can not be
	 *             found.
	 * @throws IOException
	 *             if an error occurs while reading from this stream.
	 * @throws NotActiveException
	 *             if this stream is currently not reading an object.
	 */
	public GetField readFields() throws IOException, ClassNotFoundException,
			NotActiveException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Reads a float (32 bit) from the source stream.
	 * 
	 * @return the float value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public float readFloat() throws IOException {
		return primitiveTypes.readFloat();
	}

	/**
	 * Reads bytes from the source stream into the byte array {@code buffer}.
	 * This method will block until {@code buffer.length} bytes have been read.
	 * 
	 * @param buffer
	 *            the array in which to store the bytes read.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public void readFully(byte[] buffer) throws IOException {
		primitiveTypes.readFully(buffer);
	}

	/**
	 * Reads bytes from the source stream into the byte array {@code buffer}.
	 * This method will block until {@code length} number of bytes have been
	 * read.
	 * 
	 * @param buffer
	 *            the byte array in which to store the bytes read.
	 * @param offset
	 *            the initial position in {@code buffer} to store the bytes read
	 *            from the source stream.
	 * @param length
	 *            the maximum number of bytes to store in {@code buffer}.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public void readFully(byte[] buffer, int offset, int length)
			throws IOException {
		primitiveTypes.readFully(buffer, offset, length);
	}

	/**
	 * Reads an integer (32 bit) from the source stream.
	 * 
	 * @return the integer value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public int readInt() throws IOException {
		return primitiveTypes.readInt();
	}

	/**
	 * Reads the next line from the source stream. Lines are terminated by
	 * {@code '\r'}, {@code '\n'}, {@code "\r\n"} or an {@code EOF}.
	 * 
	 * @return the string read from the source stream.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @deprecated Use {@link BufferedReader}
	 */
	@Deprecated
	public String readLine() throws IOException {
		return primitiveTypes.readLine();
	}

	/**
	 * Reads a long (64 bit) from the source stream.
	 * 
	 * @return the long value read from the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public long readLong() throws IOException {
		return primitiveTypes.readLong();
	}

	/**
	 * Reads a class descriptor from the source stream.
	 * 
	 * @return the class descriptor read from the source stream.
	 * @throws ClassNotFoundException
	 *             if a class for one of the objects cannot be found.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	protected ObjectStreamClass readClassDescriptor() throws IOException,
			ClassNotFoundException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Creates the proxy class that implements the interfaces specified in
	 * {@code interfaceNames}.
	 * 
	 * @param interfaceNames
	 *            the interfaces used to create the proxy class.
	 * @return the proxy class.
	 * @throws ClassNotFoundException
	 *             if the proxy class or any of the specified interfaces cannot
	 *             be created.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @see ObjectOutputStream#annotateProxyClass(Class)
	 */
	protected Class<?> resolveProxyClass(String[] interfaceNames)
			throws IOException, ClassNotFoundException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Reads the next object from the source stream.
	 * 
	 * @return the object read from the source stream.
	 * @throws ClassNotFoundException
	 *             if the class of one of the objects in the object graph cannot
	 *             be found.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @throws OptionalDataException
	 *             if primitive data types were found instead of an object.
	 * @see ObjectOutputStream#writeObject(Object)
	 */
	public final Object readObject() throws OptionalDataException,
			ClassNotFoundException, IOException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Reads the next unshared object from the source stream.
	 * 
	 * @return the new object read.
	 * @throws ClassNotFoundException
	 *             if the class of one of the objects in the object graph cannot
	 *             be found.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @see ObjectOutputStream#writeUnshared
	 */
	public Object readUnshared() throws IOException, ClassNotFoundException {
		throw new UnsupportedOperationException();
	}

	/**
	 * Method to be overriden by subclasses to read the next object from the
	 * source stream.
	 * 
	 * @return the object read from the source stream.
	 * @throws ClassNotFoundException
	 *             if the class of one of the objects in the object graph cannot
	 *             be found.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @throws OptionalDataException
	 *             if primitive data types were found instead of an object.
	 * @see ObjectOutputStream#writeObjectOverride
	 */
	protected Object readObjectOverride() throws OptionalDataException,
			ClassNotFoundException, IOException {
		if (input == null) {
			return null;
		}
		// Subclasses must override.
		throw new IOException();
	}

	/**
	 * Reads a short (16 bit) from the source stream.
	 * 
	 * @return the short value read from the source stream.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public short readShort() throws IOException {
		return primitiveTypes.readShort();
	}

	/**
	 * Reads and validates the ObjectInputStream header from the source stream.
	 * 
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 * @throws StreamCorruptedException
	 *             if the source stream does not contain readable serialized
	 *             objects.
	 */
	protected void readStreamHeader() throws IOException,
			StreamCorruptedException {
		if (input.readShort() == STREAM_MAGIC
				&& input.readShort() == STREAM_VERSION) {
			return;
		}
		throw new StreamCorruptedException();
	}

	/**
	 * Reads an unsigned byte (8 bit) from the source stream.
	 * 
	 * @return the unsigned byte value read from the source stream packaged in
	 *         an integer.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public int readUnsignedByte() throws IOException {
		return primitiveTypes.readUnsignedByte();
	}

	/**
	 * Reads an unsigned short (16 bit) from the source stream.
	 * 
	 * @return the unsigned short value read from the source stream packaged in
	 *         an integer.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public int readUnsignedShort() throws IOException {
		return primitiveTypes.readUnsignedShort();
	}

	/**
	 * Reads a string encoded in {@link DataInput modified UTF-8} from the
	 * source stream.
	 * 
	 * @return the string encoded in {@link DataInput modified UTF-8} read from
	 *         the source stream.
	 * @throws EOFException
	 *             if the end of the input is reached before the read request
	 *             can be satisfied.
	 * @throws IOException
	 *             if an error occurs while reading from the source stream.
	 */
	public String readUTF() throws IOException {
		return primitiveTypes.readUTF();
	}

	/**
	 * Reset the collection of objects already loaded by the receiver.
	 */
	private void resetSeenObjects() {
		primitiveData = emptyStream;
	}

	/**
	 * Reset the receiver. The collection of objects already read by the
	 * receiver is reset, and internal structures are also reset so that the
	 * receiver knows it is in a fresh clean state.
	 */
	private void resetState() {
		resetSeenObjects();
		hasPushbackTC = false;
		pushbackTC = 0;
		// nestedLevels = 0;
	}

	/**
	 * Loads the Java class corresponding to the class descriptor
	 * {@code osClass} that has just been read from the source stream.
	 * 
	 * @param osClass
	 *            an ObjectStreamClass read from the source stream.
	 * @return a Class corresponding to the descriptor {@code osClass}.
	 * @throws ClassNotFoundException
	 *             if the class for an object cannot be found.
	 * @throws IOException
	 *             if an I/O error occurs while creating the class.
	 * @see ObjectOutputStream#annotateClass(Class)
	 */
	protected Class<?> resolveClass(ObjectStreamClass osClass)
			throws IOException, ClassNotFoundException {
		// fastpath: obtain cached value
		Class<?> cls = osClass.forClass();
		if (null == cls) {
			// slowpath: resolve the class
			String className = osClass.getName();

			// if it is primitive class, for example, long.class
			cls = PRIMITIVE_CLASSES.get(className);

			if (null == cls) {
				// not primitive class
				// Use the first non-null ClassLoader on the stack. If null, use
				// the system class loader
				cls = Class.forName(className, true, callerClassLoader);
			}
		}
		return cls;
	}

	/**
	 * Allows trusted subclasses to substitute the specified original
	 * {@code object} with a new object. Object substitution has to be activated
	 * first with calling {@code enableResolveObject(true)}. This implementation
	 * just returns {@code object}.
	 * 
	 * @param object
	 *            the original object for which a replacement may be defined.
	 * @return the replacement object for {@code object}.
	 * @throws IOException
	 *             if any I/O error occurs while creating the replacement
	 *             object.
	 * @see #enableResolveObject
	 * @see ObjectOutputStream#enableReplaceObject
	 * @see ObjectOutputStream#replaceObject
	 */
	protected Object resolveObject(Object object) throws IOException {
		// By default no object replacement. Subclasses can override
		return object;
	}

	/**
	 * Skips {@code length} bytes on the source stream. This method should not
	 * be used to skip bytes at any arbitrary position, just when reading
	 * primitive data types (int, char etc).
	 *
	 * @param length
	 *            the number of bytes to skip.
	 * @return the number of bytes actually skipped.
	 * @throws IOException
	 *             if an error occurs while skipping bytes on the source stream.
	 * @throws NullPointerException
	 *             if the source stream is {@code null}.
	 */
	public int skipBytes(int length) throws IOException {
		// To be used with available. Ok to call if reading primitive buffer
		if (input == null) {
			throw new NullPointerException();
		}

		int offset = 0;
		while (offset < length) {
			checkReadPrimitiveTypes();
			long skipped = primitiveData.skip(length - offset);
			if (skipped == 0) {
				return offset;
			}
			offset += (int) skipped;
		}
		return length;
	}

}
