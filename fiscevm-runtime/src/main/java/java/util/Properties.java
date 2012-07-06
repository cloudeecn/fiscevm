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

package java.util;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.Writer;

/**
 * A {@code Properties} object is a {@code Hashtable} where the keys and values
 * must be {@code String}s. Each property can have a default {@code Properties}
 * list which specifies the default values to be used when a given key is not
 * found in this {@code Properties} instance.
 *
 * @see Hashtable
 * @see java.lang.System#getProperties
 */
public class Properties extends Hashtable<Object, Object> {

	/**
	 * The default values for keys not found in this {@code Properties}
	 * instance.
	 */
	protected Properties defaults;

	private static final int NONE = 0, SLASH = 1, UNICODE = 2, CONTINUE = 3,
			KEY_DONE = 4, IGNORE = 5;

	/**
	 * Constructs a new {@code Properties} object.
	 */
	public Properties() {
		super();
	}

	/**
	 * Constructs a new {@code Properties} object using the specified default
	 * {@code Properties}.
	 *
	 * @param properties
	 *            the default {@code Properties}.
	 */
	public Properties(Properties properties) {
		defaults = properties;
	}

	private void dumpString(StringBuilder buffer, String string, boolean isKey) {
		int index = 0, length = string.length();
		if (!isKey && index < length && string.charAt(index) == ' ') {
			buffer.append("\\ "); //$NON-NLS-1$
			index++;
		}

		for (; index < length; index++) {
			char ch = string.charAt(index);
			switch (ch) {
			case '\t':
				buffer.append("\\t"); //$NON-NLS-1$
				break;
			case '\n':
				buffer.append("\\n"); //$NON-NLS-1$
				break;
			case '\f':
				buffer.append("\\f"); //$NON-NLS-1$
				break;
			case '\r':
				buffer.append("\\r"); //$NON-NLS-1$
				break;
			default:
				if ("\\#!=:".indexOf(ch) >= 0 || (isKey && ch == ' ')) {
					buffer.append('\\');
				}
				if (ch >= ' ' && ch <= '~') {
					buffer.append(ch);
				} else {
					buffer.append(toHexaDecimal(ch));
				}
			}
		}
	}

	private char[] toHexaDecimal(final int ch) {
		char[] hexChars = { '\\', 'u', '0', '0', '0', '0' };
		int hexChar, index = hexChars.length, copyOfCh = ch;
		do {
			hexChar = copyOfCh & 15;
			if (hexChar > 9) {
				hexChar = hexChar - 10 + 'A';
			} else {
				hexChar += '0';
			}
			hexChars[--index] = (char) hexChar;
		} while ((copyOfCh >>>= 4) != 0);
		return hexChars;
	}

	/**
	 * Searches for the property with the specified name. If the property is not
	 * found, the default {@code Properties} are checked. If the property is not
	 * found in the default {@code Properties}, {@code null} is returned.
	 *
	 * @param name
	 *            the name of the property to find.
	 * @return the named property value, or {@code null} if it can't be found.
	 */
	public String getProperty(String name) {
		Object result = super.get(name);
		String property = result instanceof String ? (String) result : null;
		if (property == null && defaults != null) {
			property = defaults.getProperty(name);
		}
		return property;
	}

	/**
	 * Searches for the property with the specified name. If the property is not
	 * found, it looks in the default {@code Properties}. If the property is not
	 * found in the default {@code Properties}, it returns the specified
	 * default.
	 *
	 * @param name
	 *            the name of the property to find.
	 * @param defaultValue
	 *            the default value.
	 * @return the named property value.
	 */
	public String getProperty(String name, String defaultValue) {
		Object result = super.get(name);
		String property = result instanceof String ? (String) result : null;
		if (property == null && defaults != null) {
			property = defaults.getProperty(name);
		}
		if (property == null) {
			return defaultValue;
		}
		return property;
	}

	/**
	 * Lists the mappings in this {@code Properties} to the specified
	 * {@code PrintStream} in a human readable form.
	 *
	 * @param out
	 *            the {@code PrintStream} to write the content to in human
	 *            readable form.
	 */
	public void list(PrintStream out) {
		if (out == null) {
			throw new NullPointerException();
		}
		StringBuilder buffer = new StringBuilder(80);
		Enumeration<?> keys = propertyNames();
		while (keys.hasMoreElements()) {
			String key = (String) keys.nextElement();
			buffer.append(key);
			buffer.append('=');
			String property = (String) super.get(key);
			Properties def = defaults;
			while (property == null) {
				property = (String) def.get(key);
				def = def.defaults;
			}
			if (property.length() > 40) {
				buffer.append(property.substring(0, 37));
				buffer.append("..."); //$NON-NLS-1$
			} else {
				buffer.append(property);
			}
			out.println(buffer.toString());
			buffer.setLength(0);
		}
	}

	/**
	 * Lists the mappings in this {@code Properties} to the specified
	 * {@code PrintWriter} in a human readable form.
	 *
	 * @param writer
	 *            the {@code PrintWriter} to write the content to in human
	 *            readable form.
	 */
	public void list(PrintWriter writer) {
		if (writer == null) {
			throw new NullPointerException();
		}
		StringBuilder buffer = new StringBuilder(80);
		Enumeration<?> keys = propertyNames();
		while (keys.hasMoreElements()) {
			String key = (String) keys.nextElement();
			buffer.append(key);
			buffer.append('=');
			String property = (String) super.get(key);
			Properties def = defaults;
			while (property == null) {
				property = (String) def.get(key);
				def = def.defaults;
			}
			if (property.length() > 40) {
				buffer.append(property.substring(0, 37));
				buffer.append("..."); //$NON-NLS-1$
			} else {
				buffer.append(property);
			}
			writer.println(buffer.toString());
			buffer.setLength(0);
		}
	}

	/**
	 * Loads properties from the specified {@code InputStream}. The encoding is
	 * ISO8859-1. The {@code Properties} file is interpreted according to the
	 * following rules:
	 * <ul>
	 * <li>Empty lines are ignored.</li>
	 * <li>Lines starting with either a "#" or a "!" are comment lines and are
	 * ignored.</li>
	 * <li>A backslash at the end of the line escapes the following newline
	 * character ("\r", "\n", "\r\n"). If there's a whitespace after the
	 * backslash it will just escape that whitespace instead of concatenating
	 * the lines. This does not apply to comment lines.</li>
	 * <li>A property line consists of the key, the space between the key and
	 * the value, and the value. The key goes up to the first whitespace, "=" or
	 * ":" that is not escaped. The space between the key and the value contains
	 * either one whitespace, one "=" or one ":" and any number of additional
	 * whitespaces before and after that character. The value starts with the
	 * first character after the space between the key and the value.</li>
	 * <li>Following escape sequences are recognized: "\ ", "\\", "\r", "\n",
	 * "\!", "\#", "\t", "\b", "\f", and "&#92;uXXXX" (unicode character).</li>
	 * </ul>
	 *
	 * @param in
	 *            the {@code InputStream}.
	 * @throws IOException
	 *             if error occurs during reading from the {@code InputStream}.
	 */
	@SuppressWarnings("fallthrough")
	public synchronized void load(InputStream in) throws IOException {
		if (in == null) {
			throw new NullPointerException();
		}
		int mode = NONE, unicode = 0, count = 0;
		char nextChar, buf[] = new char[40];
		int offset = 0, keyLength = -1, intVal;
		boolean firstChar = true;
		BufferedInputStream bis = new BufferedInputStream(in);

		while (true) {
			intVal = bis.read();
			if (intVal == -1) {
				// if mode is UNICODE but has less than 4 hex digits, should
				// throw an IllegalArgumentException
				// luni.08=Invalid Unicode sequence: expected format \\uxxxx
				if (mode == UNICODE && count < 4) {
					throw new IllegalArgumentException(
							"Invalid Unicode sequence: expected format \\uxxxx"); //$NON-NLS-1$
				}
				// if mode is SLASH and no data is read, should append '\u0000'
				// to buf
				if (mode == SLASH) {
					buf[offset++] = '\u0000';
				}
				break;
			}
			nextChar = (char) (intVal & 0xff);

			if (offset == buf.length) {
				char[] newBuf = new char[buf.length * 2];
				System.arraycopy(buf, 0, newBuf, 0, offset);
				buf = newBuf;
			}
			if (mode == UNICODE) {
				int digit = Character.digit(nextChar, 16);
				if (digit >= 0) {
					unicode = (unicode << 4) + digit;
					if (++count < 4) {
						continue;
					}
				} else if (count <= 4) {
					// luni.09=Invalid Unicode sequence: illegal character
					throw new IllegalArgumentException(
							"Invalid Unicode sequence: illegal character"); //$NON-NLS-1$
				}
				mode = NONE;
				buf[offset++] = (char) unicode;
				if (nextChar != '\n') {
					continue;
				}
			}
			if (mode == SLASH) {
				mode = NONE;
				switch (nextChar) {
				case '\r':
					mode = CONTINUE; // Look for a following \n
					continue;
				case '\n':
					mode = IGNORE; // Ignore whitespace on the next line
					continue;
				case 'b':
					nextChar = '\b';
					break;
				case 'f':
					nextChar = '\f';
					break;
				case 'n':
					nextChar = '\n';
					break;
				case 'r':
					nextChar = '\r';
					break;
				case 't':
					nextChar = '\t';
					break;
				case 'u':
					mode = UNICODE;
					unicode = count = 0;
					continue;
				}
			} else {
				switch (nextChar) {
				case '#':
				case '!':
					if (firstChar) {
						while (true) {
							intVal = bis.read();
							if (intVal == -1) {
								break;
							}
							// & 0xff not required
							nextChar = (char) intVal;
							if (nextChar == '\r' || nextChar == '\n') {
								break;
							}
						}
						continue;
					}
					break;
				case '\n':
					if (mode == CONTINUE) { // Part of a \r\n sequence
						mode = IGNORE; // Ignore whitespace on the next line
						continue;
					}
					// fall into the next case
				case '\r':
					mode = NONE;
					firstChar = true;
					if (offset > 0 || (offset == 0 && keyLength == 0)) {
						if (keyLength == -1) {
							keyLength = offset;
						}
						String temp = new String(buf, 0, offset);
						put(temp.substring(0, keyLength),
								temp.substring(keyLength));
					}
					keyLength = -1;
					offset = 0;
					continue;
				case '\\':
					if (mode == KEY_DONE) {
						keyLength = offset;
					}
					mode = SLASH;
					continue;
				case ':':
				case '=':
					if (keyLength == -1) { // if parsing the key
						mode = NONE;
						keyLength = offset;
						continue;
					}
					break;
				}
				if (nextChar < 256 && Character.isWhitespace(nextChar)) {
					if (mode == CONTINUE) {
						mode = IGNORE;
					}
					// if key length == 0 or value length == 0
					if (offset == 0 || offset == keyLength || mode == IGNORE) {
						continue;
					}
					if (keyLength == -1) { // if parsing the key
						mode = KEY_DONE;
						continue;
					}
				}
				if (mode == IGNORE || mode == CONTINUE) {
					mode = NONE;
				}
			}
			firstChar = false;
			if (mode == KEY_DONE) {
				keyLength = offset;
				mode = NONE;
			}
			buf[offset++] = nextChar;
		}
		if (keyLength == -1 && offset > 0) {
			keyLength = offset;
		}
		if (keyLength >= 0) {
			String temp = new String(buf, 0, offset);
			put(temp.substring(0, keyLength), temp.substring(keyLength));
		}
	}

	/**
	 * Returns all of the property names that this {@code Properties} object
	 * contains.
	 *
	 * @return an {@code Enumeration} containing the names of all properties
	 *         that this {@code Properties} object contains.
	 */
	public Enumeration<?> propertyNames() {
		Hashtable<Object, Object> selected = new Hashtable<Object, Object>();
		selectProperties(selected);
		return selected.keys();
	}

	private void selectProperties(Hashtable<Object, Object> selected) {
		if (defaults != null) {
			defaults.selectProperties(selected);
		}
		selected.putAll(this);
	}

	/**
	 * Saves the mappings in this {@code Properties} to the specified
	 * {@code OutputStream}, putting the specified comment at the beginning. The
	 * output from this method is suitable for being read by the
	 * {@link #load(InputStream)} method.
	 *
	 * @param out
	 *            the {@code OutputStream} to write to.
	 * @param comment
	 *            the comment to add at the beginning.
	 * @throws ClassCastException
	 *             if the key or value of a mapping is not a String.
	 * @deprecated This method ignores any {@code IOException} thrown while
	 *             writing -- use {@link #store} instead for better exception
	 *             handling.
	 */
	public void save(OutputStream out, String comment) {
		try {
			store(out, comment);
		} catch (IOException e) {
		}
	}

	/**
	 * Maps the specified key to the specified value. If the key already exists,
	 * the old value is replaced. The key and value cannot be {@code null}.
	 *
	 * @param name
	 *            the key.
	 * @param value
	 *            the value.
	 * @return the old value mapped to the key, or {@code null}.
	 */
	public Object setProperty(String name, String value) {
		return put(name, value);
	}

	private static String lineSeparator;

	/**
	 * Stores the mappings in this {@code Properties} to the specified
	 * {@code OutputStream}, putting the specified comment at the beginning. The
	 * output from this method is suitable for being read by the
	 * {@link #load(InputStream)} method.
	 *
	 * @param out
	 *            the {@code OutputStream} to write to.
	 * @param comments
	 *            the comments to put at the beginning.
	 * @throws IOException
	 *             if an error occurs during the write to the
	 *             {@code OutputStream}.
	 * @throws ClassCastException
	 *             if the key or value of a mapping is not a {@code String}.
	 */
	public synchronized void store(OutputStream out, String comments)
			throws IOException {
		if (lineSeparator == null) {
			lineSeparator = "\n"; //$NON-NLS-1$
		}
		OutputStreamWriter writer = new OutputStreamWriter(out, "ISO8859_1"); //$NON-NLS-1$
		if (comments != null) {
			writeComments(writer, comments);
		}
		writer.write('#');
		writer.write(new Date().toString());
		writer.write(lineSeparator);

		StringBuilder buffer = new StringBuilder(200);
		for (Map.Entry<Object, Object> entry : entrySet()) {
			String key = (String) entry.getKey();
			dumpString(buffer, key, true);
			buffer.append('=');
			dumpString(buffer, (String) entry.getValue(), false);
			buffer.append(lineSeparator);
			writer.write(buffer.toString());
			buffer.setLength(0);
		}
		writer.flush();
	}

	private void writeComments(Writer writer, String comments)
			throws IOException {
		writer.write('#');
		char[] chars = comments.toCharArray();
		for (int index = 0; index < chars.length; index++) {
			if (chars[index] == '\r' || chars[index] == '\n') {
				int indexPlusOne = index + 1;
				if (chars[index] == '\r' && indexPlusOne < chars.length
						&& chars[indexPlusOne] == '\n') {
					// "\r\n"
					continue;
				}
				writer.write(lineSeparator);
				if (indexPlusOne < chars.length
						&& (chars[indexPlusOne] == '#' || chars[indexPlusOne] == '!')) {
					// return char with either '#' or '!' afterward
					continue;
				}
				writer.write('#');
			} else {
				writer.write(chars[index]);
			}
		}
		writer.write(lineSeparator);
	}

}