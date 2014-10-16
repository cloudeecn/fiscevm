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

/**
 * A specialized {@link OutputStream} that is able to write (serialize) Java
 * objects as well as primitive data types (int, byte, char etc.). The data can
 * later be loaded using an ObjectInputStream.
 * 
 * @see ObjectInputStream
 * @see ObjectOutput
 * @see Serializable
 * @see Externalizable
 */
public class ObjectOutputStream extends OutputStream implements ObjectOutput,
        ObjectStreamConstants {

    /*
     * Where we write to
     */
    private DataOutputStream output;

    /*
     * Where we write primitive types to
     */
    private DataOutputStream primitiveTypes;

    /*
     * Where the write primitive types are actually written to
     */
    private ByteArrayOutputStream primitiveTypesBuffer;

    /*
     * Used to detect nested exception when saving an exception due to an error
     */
    private StreamCorruptedException nestedException;

    /*
     * Allows the receiver to decide if it needs to call writeObjectOverride
     */
    private boolean subclassOverridingImplementation;
    

    /**
     * PutField is an inner class to provide access to the persistent fields
     * that are written to the target stream.
     */
    public static abstract class PutField {
        /**
         * Puts the value of the boolean field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, boolean value);

        /**
         * Puts the value of the character field identified by {@code name} to
         * the persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, char value);

        /**
         * Puts the value of the byte field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, byte value);

        /**
         * Puts the value of the short field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, short value);

        /**
         * Puts the value of the integer field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, int value);

        /**
         * Puts the value of the long field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, long value);

        /**
         * Puts the value of the float field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, float value);

        /**
         * Puts the value of the double field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, double value);

        /**
         * Puts the value of the Object field identified by {@code name} to the
         * persistent field.
         *
         * @param name
         *            the name of the field to serialize.
         * @param value
         *            the value that is put to the persistent field.
         */
        public abstract void put(String name, Object value);

        /**
         * Writes the fields to the target stream {@code out}.
         *
         * @param out
         *            the target stream
         * @throws IOException
         *             if an error occurs while writing to the target stream.
         * @deprecated This method is unsafe and may corrupt the target stream.
         *             Use ObjectOutputStream#writeFields() instead.
         */
        @Deprecated
        public abstract void write(ObjectOutput out) throws IOException;
    }
    

    /**
     * Constructs a new {@code ObjectOutputStream}. This default constructor can
     * be used by subclasses that do not want to use the public constructor if
     * it allocates unneeded data.
     * 
     * @throws IOException
     *             if an error occurs when creating this stream.
     * @throws SecurityException
     *             if a security manager is installed and it denies subclassing
     *             this class.
     * @see SecurityManager#checkPermission(java.security.Permission)
     */
    protected ObjectOutputStream() throws IOException, SecurityException {
        super();
        SecurityManager currentManager = System.getSecurityManager();
        if (currentManager != null) {
            currentManager.checkPermission(SUBCLASS_IMPLEMENTATION_PERMISSION);
        }
        /*
         * WARNING - we should throw IOException if not called from a subclass
         * according to the JavaDoc. Add the test.
         */
        this.subclassOverridingImplementation = true;
    }

    /**
     * Constructs a new ObjectOutputStream that writes to the OutputStream
     * {@code output}.
     * 
     * @param output
     *            the non-null OutputStream to filter writes on.
     * 
     * @throws IOException
     *             if an error occurs while writing the object stream
     *             header
     * @throws SecurityException
     *             if a security manager is installed and it denies subclassing
     *             this class.
     */
    public ObjectOutputStream(OutputStream output) throws IOException {
        Class<?> implementationClass = getClass();
        Class<?> thisClass = ObjectOutputStream.class;
        if (implementationClass != thisClass) {
            boolean mustCheck = false;
            try {
                Method method = implementationClass.getMethod("putFields");
                mustCheck = method.getDeclaringClass() != thisClass;
            } catch (NoSuchMethodException e) {
            }
            if (!mustCheck) {
                try {
                    Method method = implementationClass.getMethod(
                            "writeUnshared", //$NON-NLS-1$
                            Object.class);
                    mustCheck = method.getDeclaringClass() != thisClass;
                } catch (NoSuchMethodException e) {
                }
            }
            if (mustCheck) {
                SecurityManager sm = System.getSecurityManager();
                if (sm != null) {
                    sm
                            .checkPermission(ObjectStreamConstants.SUBCLASS_IMPLEMENTATION_PERMISSION);
                }
            }
        }
        this.output = (output instanceof DataOutputStream) ? (DataOutputStream) output
                : new DataOutputStream(output);
        this.subclassOverridingImplementation = false;

        resetState();
        this.nestedException = new StreamCorruptedException();
        // So write...() methods can be used by
        // subclasses during writeStreamHeader()
        primitiveTypes = this.output;
        // Has to be done here according to the specification
        writeStreamHeader();
        primitiveTypes = null;
    }

    /**
     * Writes optional information for class {@code aClass} to the output
     * stream. This optional data can be read when deserializing the class
     * descriptor (ObjectStreamClass) for this class from an input stream. By
     * default, no extra data is saved.
     * 
     * @param aClass
     *            the class to annotate.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see ObjectInputStream#resolveClass(ObjectStreamClass)
     */
    protected void annotateClass(Class<?> aClass) throws IOException {
        // By default no extra info is saved. Subclasses can override
    }

    /**
     * Writes optional information for a proxy class to the target stream. This
     * optional data can be read when deserializing the proxy class from an
     * input stream. By default, no extra data is saved.
     * 
     * @param aClass
     *            the proxy class to annotate.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see ObjectInputStream#resolveProxyClass(String[])
     */
    protected void annotateProxyClass(Class<?> aClass) throws IOException {
        // By default no extra info is saved. Subclasses can override
    }

    /**
     * Do the necessary work to see if the receiver can be used to write
     * primitive types like int, char, etc.
     */
    private void checkWritePrimitiveTypes() {
        if (primitiveTypes == null) {
            // If we got here we have no Stream previously created
            // WARNING - if the stream does not grow, this code is wrong
            primitiveTypesBuffer = new ByteArrayOutputStream(128);
            primitiveTypes = new DataOutputStream(primitiveTypesBuffer);
        }
    }

    /**
     * Closes this stream. Any buffered data is flushed. This implementation
     * closes the target stream.
     * 
     * @throws IOException
     *             if an error occurs while closing this stream.
     */
    @Override
    public void close() throws IOException {
        // First flush what is needed (primitive data, etc)
        flush();
        output.close();
    }

    /**
     * Default method to write objects to this stream. Serializable fields
     * defined in the object's class and superclasses are written to the output
     * stream.
     * 
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @throws NotActiveException
     *             if this method is not called from {@code writeObject()}.
     * @see ObjectInputStream#defaultReadObject
     */
    public void defaultWriteObject() throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Writes buffered data to the target stream. This is similar to {@code
     * flush} but the flush is not propagated to the target stream.
     * 
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    protected void drain() throws IOException {
        if (primitiveTypes == null || primitiveTypesBuffer == null) {
            return;
        }

        // If we got here we have a Stream previously created
        int offset = 0;
        byte[] written = primitiveTypesBuffer.toByteArray();
        // Normalize the primitive data
        while (offset < written.length) {
            int toWrite = written.length - offset > 1024 ? 1024
                    : written.length - offset;
            if (toWrite < 256) {
                output.writeByte(TC_BLOCKDATA);
                output.writeByte((byte) toWrite);
            } else {
                output.writeByte(TC_BLOCKDATALONG);
                output.writeInt(toWrite);
            }

            // write primitive types we had and the marker of end-of-buffer
            output.write(written, offset, toWrite);
            offset += toWrite;
        }

        // and now we're clean to a state where we can write an object
        primitiveTypes = null;
        primitiveTypesBuffer = null;
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
     * @see #replaceObject
     * @see ObjectInputStream#enableResolveObject
     */
    protected boolean enableReplaceObject(boolean enable)
            throws SecurityException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Writes buffered data to the target stream and calls the {@code flush}
     * method of the target stream.
     * 
     * @throws IOException
     *             if an error occurs while writing to or flushing the output
     *             stream.
     */
    @Override
    public void flush() throws IOException {
        drain();
        output.flush();
    }


    /**
     * Gets this stream's {@code PutField} object. This object provides access
     * to the persistent fields that are eventually written to the output
     * stream. It is used to transfer the values from the fields of the object
     * that is currently being written to the persistent fields.
     * 
     * @return the PutField object from which persistent fields can be accessed
     *         by name.
     * @throws IOException
     *             if an I/O error occurs.
     * @throws NotActiveException
     *             if this method is not called from {@code writeObject()}.
     * @see ObjectInputStream#defaultReadObject
     */
    public PutField putFields() throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Allows trusted subclasses to substitute the specified original {@code
     * object} with a new object. Object substitution has to be activated first
     * with calling {@code enableReplaceObject(true)}. This implementation just
     * returns {@code object}.
     *
     * @param object
     *            the original object for which a replacement may be defined.
     * @return the replacement object for {@code object}.
     * @throws IOException
     *             if any I/O error occurs while creating the replacement
     *             object.
     * @see #enableReplaceObject
     * @see ObjectInputStream#enableResolveObject
     * @see ObjectInputStream#resolveObject
     */
    protected Object replaceObject(Object object) throws IOException {
        // By default no object replacement. Subclasses can override
        return object;
    }

    /**
     * Resets the state of this stream. A marker is written to the stream, so
     * that the corresponding input stream will also perform a reset at the same
     * point. Objects previously written are no longer remembered, so they will
     * be written again (instead of a cyclical reference) if found in the object
     * graph.
     * 
     * @throws IOException
     *             if {@code reset()} is called during the serialization of an
     *             object.
     */
    public void reset() throws IOException {
        // First we flush what we have
        drain();
        /*
         * And dump a reset marker, so that the ObjectInputStream can reset
         * itself at the same point
         */
        output.writeByte(TC_RESET);
        // Now we reset ourselves
        resetState();
    }

    /**
     * Reset the collection of objects already dumped by the receiver. If the
     * objects are found again in the object graph, the receiver will dump them
     * again, instead of a handle (cyclic reference).
     * 
     */
    private void resetSeenObjects() {
    }

    /**
     * Reset the receiver. The collection of objects already dumped by the
     * receiver is reset, and internal structures are also reset so that the
     * receiver knows it is in a fresh clean state.
     * 
     */
    private void resetState() {
        resetSeenObjects();
    }

    /**
     * Sets the specified protocol version to be used by this stream.
     * 
     * @param version
     *            the protocol version to be used. Use a {@code
     *            PROTOCOL_VERSION_x} constant from {@code
     *            java.io.ObjectStreamConstants}.
     * @throws IllegalArgumentException
     *             if an invalid {@code version} is specified.
     * @throws IOException
     *             if an I/O error occurs.
     * @see ObjectStreamConstants#PROTOCOL_VERSION_1
     * @see ObjectStreamConstants#PROTOCOL_VERSION_2
     */
    public void useProtocolVersion(int version) throws IOException {
    }

    /**
     * Writes the entire contents of the byte array {@code buffer} to the output
     * stream. Blocks until all bytes are written.
     * 
     * @param buffer
     *            the buffer to write.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    @Override
    public void write(byte[] buffer) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.write(buffer);
    }

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * offset {@code index} to the target stream. Blocks until all bytes are
     * written.
     * 
     * @param buffer
     *            the buffer to write.
     * @param offset
     *            the index of the first byte in {@code buffer} to write.
     * @param length
     *            the number of bytes from {@code buffer} to write to the output
     *            stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    @Override
    public void write(byte[] buffer, int offset, int length) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.write(buffer, offset, length);
    }

    /**
     * Writes a single byte to the target stream. Only the least significant
     * byte of the integer {@code value} is written to the stream. Blocks until
     * the byte is actually written.
     * 
     * @param value
     *            the byte to write.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    @Override
    public void write(int value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.write(value);
    }

    /**
     * Writes a boolean to the target stream.
     * 
     * @param value
     *            the boolean value to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeBoolean(boolean value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeBoolean(value);
    }

    /**
     * Writes a byte (8 bit) to the target stream.
     * 
     * @param value
     *            the byte to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeByte(int value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeByte(value);
    }

    /**
     * Writes the string {@code value} as a sequence of bytes to the target
     * stream. Only the least significant byte of each character in the string
     * is written.
     * 
     * @param value
     *            the string to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeBytes(String value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeBytes(value);
    }

    /**
     * Writes a character (16 bit) to the target stream.
     * 
     * @param value
     *            the character to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeChar(int value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeChar(value);
    }

    /**
     * Writes the string {@code value} as a sequence of characters to the target
     * stream.
     * 
     * @param value
     *            the string to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeChars(String value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeChars(value);
    }

    /**
     * Writes a double (64 bit) to the target stream.
     * 
     * @param value
     *            the double to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeDouble(double value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeDouble(value);
    }


    /**
     * Writes the fields of the object currently being written to the target
     * stream. The field values are buffered in the currently active {@code
     * PutField} object, which can be accessed by calling {@code putFields()}.
     * 
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @throws NotActiveException
     *             if there are no fields to write to the target stream.
     * @see #putFields
     */
    public void writeFields() throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Writes a float (32 bit) to the target stream.
     * 
     * @param value
     *            the float to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeFloat(float value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeFloat(value);
    }

    /**
     * Writes an integer (32 bit) to the target stream.
     * 
     * @param value
     *            the integer to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeInt(int value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeInt(value);
    }

    /**
     * Writes a long (64 bit) to the target stream.
     * 
     * @param value
     *            the long to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeLong(long value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeLong(value);
    }

    /**
     * Writes a class descriptor to the target stream.
     * 
     * @param classDesc
     *            the class descriptor to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    protected void writeClassDescriptor(ObjectStreamClass classDesc)
            throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Writes an object to the target stream.
     * 
     * @param object
     *            the object to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see ObjectInputStream#readObject()
     */
    public final void writeObject(Object object) throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Writes an unshared object to the target stream. This method is identical
     * to {@code writeObject}, except that it always writes a new object to the
     * stream versus the use of back-referencing for identical objects by
     * {@code writeObject}.
     * 
     * @param object
     *            the object to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see ObjectInputStream#readUnshared()
     */
    public void writeUnshared(Object object) throws IOException {
    	throw new UnsupportedOperationException();
    }

    /**
     * Method to be overridden by subclasses to write {@code object} to the
     * target stream.
     * 
     * @param object
     *            the object to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    protected void writeObjectOverride(Object object) throws IOException {
        if (!subclassOverridingImplementation) {
            // Subclasses must override.
            throw new IOException();
        }
    }

    /**
     * Writes a short (16 bit) to the target stream.
     * 
     * @param value
     *            the short to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeShort(int value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeShort(value);
    }

    /**
     * Writes the {@link ObjectOutputStream} header to the target stream.
     * 
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    protected void writeStreamHeader() throws IOException {
        output.writeShort(STREAM_MAGIC);
        output.writeShort(STREAM_VERSION);
    }

    /**
     * Writes a string encoded with {@link DataInput modified UTF-8} to the
     * target stream.
     * 
     * @param value
     *            the string to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     */
    public void writeUTF(String value) throws IOException {
        checkWritePrimitiveTypes();
        primitiveTypes.writeUTF(value);
    }
}
