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
/**
 * @author Evgueni Brevnov, Serguei S. Zapreyev, Alexey V. Varlamov
 */

package java.lang.reflect;

import java.lang.annotation.Annotation;

/**
 * @com.intel.drl.spec_ref
 */
public final class Field extends AccessibleObject implements Member {

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public Annotation[] getDeclaredAnnotations() {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	@SuppressWarnings("unchecked")
	public <T extends Annotation> T getAnnotation(Class<T> annotationClass) {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Type getGenericType() throws GenericSignatureFormatError,
			TypeNotPresentException, MalformedParameterizedTypeException {
		throw new RuntimeException(
				"Generic and annotations in reflection is not currently supported.");
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public String toGenericString() {
		return getUniqueName();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native boolean isSynthetic();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native boolean isEnumConstant();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public Object get(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type.isPrimitive()) {
			if (type == Boolean.TYPE) {
				return getBoolean0(obj);
			} else if (type == Byte.TYPE) {
				return getByte0(obj);
			} else if (type == Short.TYPE) {
				return getShort0(obj);
			} else if (type == Character.TYPE) {
				return getChar0(obj);
			} else if (type == Integer.TYPE) {
				return getInt0(obj);
			} else if (type == Float.TYPE) {
				return getFloat0(obj);
			} else if (type == Long.TYPE) {
				return getLong0(obj);
			} else if (type == Double.TYPE) {
				return getDouble0(obj);
			} else {
				throw new IllegalArgumentException();
			}
		} else {
			return getObject0(obj);
		}
	}

	public boolean getBoolean(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Boolean.TYPE) {
			return getBoolean0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public byte getByte(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Byte.TYPE) {
			return getByte0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public char getChar(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Character.TYPE) {
			return getChar0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public short getShort(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Short.TYPE) {
			return getShort0(obj);
		} else if (type == Byte.TYPE) {
			return getByte0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public int getInt(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Integer.TYPE) {
			return getInt0(obj);
		} else if (type == Byte.TYPE) {
			return getByte0(obj);
		} else if (type == Character.TYPE) {
			return getChar0(obj);
		} else if (type == Short.TYPE) {
			return getShort0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public float getFloat(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Float.TYPE) {
			return getFloat0(obj);
		} else if (type == Long.TYPE) {
			return getLong0(obj);
		} else if (type == Byte.TYPE) {
			return getByte0(obj);
		} else if (type == Character.TYPE) {
			return getChar0(obj);
		} else if (type == Short.TYPE) {
			return getShort0(obj);
		} else if (type == Integer.TYPE) {
			return getInt0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public long getLong(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Long.TYPE) {
			return getLong0(obj);
		} else if (type == Byte.TYPE) {
			return getByte0(obj);
		} else if (type == Character.TYPE) {
			return getChar0(obj);
		} else if (type == Short.TYPE) {
			return getShort0(obj);
		} else if (type == Integer.TYPE) {
			return getInt0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	public double getDouble(Object obj) throws IllegalArgumentException,
			IllegalAccessException {
		Class<?> type = getType();
		if (type == Double.TYPE) {
			return getDouble0(obj);
		} else if (type == Float.TYPE) {
			return getFloat0(obj);
		} else if (type == Long.TYPE) {
			return getLong0(obj);
		} else if (type == Byte.TYPE) {
			return getByte0(obj);
		} else if (type == Character.TYPE) {
			return getChar0(obj);
		} else if (type == Short.TYPE) {
			return getShort0(obj);
		} else if (type == Integer.TYPE) {
			return getInt0(obj);
		} else {
			throw new IllegalArgumentException("Can't convert " + type
					+ " to boolean");
		}
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native boolean getBoolean0(Object obj)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native byte getByte0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native char getChar0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?> getDeclaringClass();

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native double getDouble0(Object obj)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native float getFloat0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native int getInt0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native long getLong0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native int getModifiers();

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native String getName();

	/**
	 * @com.intel.drl.spec_ref
	 */
	private native short getShort0(Object obj) throws IllegalArgumentException,
			IllegalAccessException;

	private native Object getObject0(Object obj)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native Class<?> getType();

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public int hashCode() {
		return getDeclaringClass().getName().hashCode() ^ getName().hashCode();
	}

	/**
	 * @com.intel.drl.spec_ref
	 */
	public void set(Object obj, Object value) throws IllegalArgumentException,
			IllegalAccessException {
		if (getType().isPrimitive()) {
			if (value == null) {
				throw new NullPointerException("value");
			}
			if (getType() == Boolean.TYPE) {
				if (value instanceof Boolean) {
					setBoolean(obj, ((Boolean) value).booleanValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to boolean");
				}
			} else if (getType() == Byte.TYPE) {
				if (value instanceof Byte) {
					setBoolean(obj, ((Boolean) value).booleanValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to byte");
				}
			} else if (getType() == Short.TYPE) {
				if (value instanceof Number) {
					setByte(obj, ((Number) value).byteValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to short");
				}

			} else if (getType() == Integer.TYPE) {
				if (value instanceof Number) {
					setInt(obj, ((Number) value).intValue());
				} else if (value instanceof Character) {
					setInt(obj, ((Character) value).charValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to int");
				}
			} else if (getType() == Float.TYPE) {
				if (value instanceof Number) {
					setFloat(obj, ((Number) value).floatValue());
				} else if (value instanceof Character) {
					setFloat(obj, ((Character) value).charValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to int");
				}
			} else if (getType() == Long.TYPE) {
				if (value instanceof Number) {
					setLong(obj, ((Number) value).longValue());
				} else if (value instanceof Character) {
					setLong(obj, ((Character) value).charValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to int");
				}
			} else if (getType() == Double.TYPE) {
				if (value instanceof Number) {
					setDouble(obj, ((Number) value).doubleValue());
				} else if (value instanceof Character) {
					setDouble(obj, ((Character) value).charValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to int");
				}
			} else if (getType() == Character.TYPE) {
				if (value instanceof Character) {
					setChar(obj, ((Character) value).charValue());
				} else {
					throw new IllegalArgumentException("Can't convert "
							+ value.getClass() + " to int");
				}
			}
		} else {
			setObject(obj, value);
		}
	}

	private native void setObject(Object obj, Object value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setBoolean(Object obj, boolean value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setByte(Object obj, byte value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setChar(Object obj, char value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setDouble(Object obj, double value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setFloat(Object obj, float value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setInt(Object obj, int value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setLong(Object obj, long value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	public native void setShort(Object obj, short value)
			throws IllegalArgumentException, IllegalAccessException;

	/**
	 * @com.intel.drl.spec_ref
	 */
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder(80);
		// append modifiers if any
		int modifier = getModifiers();
		if (modifier != 0) {
			sb.append(Modifier.toString(modifier)).append(' ');
		}
		// append return type
		appendArrayType(sb, getType());
		sb.append(' ');
		// append full field name
		sb.append(getDeclaringClass().getName()).append('.').append(getName());
		return sb.toString();
	}

	/* NON API SECTION */

	native String getUniqueName();

}
