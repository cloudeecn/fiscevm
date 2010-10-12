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

import java.io.UnsupportedEncodingException;

import com.cirnoworks.fisce.privat.FiScEVM;

public final class String implements Comparable, CharSequence {
	private char value[];

	private int offset;

	private int count;

	private int hash = 0;

	private static final long serialVersionUID = -6849794470754667710L;

	public String() {
		value = new char[0];
	}

	public String(String original) {
		this.count = original.count;
		if (original.value.length > this.count) {
			// The array representing the String is bigger than the new
			// String itself. Perhaps this constructor is being called
			// in order to trim the baggage, so make a copy of the array.
			this.value = new char[this.count];
			System.arraycopy(original.value, original.offset, this.value, 0,
					this.count);
		} else {
			// The array representing the String is the same
			// size as the String, so no point in making a copy.
			this.value = original.value;
		}
	}

	public String(char value[]) {
		this.count = value.length;
		this.value = new char[count];
		System.arraycopy(value, 0, this.value, 0, count);
	}

	public String(char value[], int offset, int count) {
		if (offset < 0) {
			throw new StringIndexOutOfBoundsException(offset);
		}
		if (count < 0) {
			throw new StringIndexOutOfBoundsException(count);
		}
		// Note: offset or count might be near -1>>>1.
		if (offset > value.length - count) {
			throw new StringIndexOutOfBoundsException(offset + count);
		}

		this.value = new char[count];
		this.count = count;
		System.arraycopy(value, offset, this.value, 0, count);
	}

	public String(byte ascii[], int hibyte, int offset, int count) {
		checkBounds(ascii, offset, count);

		char value[] = new char[count];
		this.count = count;
		this.value = value;

		if (hibyte == 0) {
			for (int i = count; i-- > 0;) {
				value[i] = (char) (ascii[i + offset] & 0xff);
			}
		} else {
			hibyte <<= 8;
			for (int i = count; i-- > 0;) {
				value[i] = (char) (hibyte | (ascii[i + offset] & 0xff));
			}
		}
	}

	public String(byte ascii[], int hibyte) {
		this(ascii, hibyte, 0, ascii.length);
	}

	private static void checkBounds(byte[] bytes, int offset, int length) {
		if (length < 0)
			throw new StringIndexOutOfBoundsException(length);
		if (offset < 0)
			throw new StringIndexOutOfBoundsException(offset);
		if (offset > bytes.length - length)
			throw new StringIndexOutOfBoundsException(offset + length);
	}

	public String(byte bytes[], int offset, int length, String charsetName)
			throws UnsupportedEncodingException {
		if (charsetName == null)
			throw new NullPointerException("charsetName");
		checkBounds(bytes, offset, length);
		value = FiScEVM.decode(charsetName, bytes, offset, length);
		count = value.length;
	}

	public String(byte bytes[], String charsetName)
			throws UnsupportedEncodingException {
		this(bytes, 0, bytes.length, charsetName);
	}

	public String(byte bytes[], int offset, int length) {
		checkBounds(bytes, offset, length);
		value = FiScEVM.decode(bytes, offset, length);
		count = value.length;
	}

	public String(byte bytes[]) {
		this(bytes, 0, bytes.length);
	}

	public String(StringBuffer buffer) {
		synchronized (buffer) {
			buffer.setShared();
			this.value = buffer.getValue();
			this.offset = 0;
			this.count = buffer.length();
		}
	}

	String(int offset, int count, char value[]) {
		this.value = value;
		this.offset = offset;
		this.count = count;
	}

	public int length() {
		return count;
	}

	public char charAt(int index) {
		if ((index < 0) || (index >= count)) {
			throw new StringIndexOutOfBoundsException(index);
		}
		return value[index + offset];
	}

	public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) {
		if (srcBegin < 0) {
			throw new StringIndexOutOfBoundsException(srcBegin);
		}
		if (srcEnd > count) {
			throw new StringIndexOutOfBoundsException(srcEnd);
		}
		if (srcBegin > srcEnd) {
			throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);
		}
		System.arraycopy(value, offset + srcBegin, dst, dstBegin, srcEnd
				- srcBegin);
	}

	public void getBytes(int srcBegin, int srcEnd, byte dst[], int dstBegin) {
		if (srcBegin < 0) {
			throw new StringIndexOutOfBoundsException(srcBegin);
		}
		if (srcEnd > count) {
			throw new StringIndexOutOfBoundsException(srcEnd);
		}
		if (srcBegin > srcEnd) {
			throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);
		}
		int j = dstBegin;
		int n = offset + srcEnd;
		int i = offset + srcBegin;
		char[] val = value; /* avoid getfield opcode */

		while (i < n) {
			dst[j++] = (byte) val[i++];
		}
	}

	public byte[] getBytes(String charsetName)
			throws UnsupportedEncodingException {
		return FiScEVM.encode(charsetName, value, offset, count);
	}

	public byte[] getBytes() {
		return FiScEVM.encode(value, offset, count);
	}

	public boolean equals(Object anObject) {
		if (this == anObject) {
			return true;
		}
		if (anObject instanceof String) {
			String anotherString = (String) anObject;
			int n = count;
			if (n == anotherString.count) {
				char v1[] = value;
				char v2[] = anotherString.value;
				int i = offset;
				int j = anotherString.offset;
				while (n-- != 0) {
					if (v1[i++] != v2[j++])
						return false;
				}
				return true;
			}
		}
		return false;
	}

	public boolean contentEquals(StringBuffer sb) {
		synchronized (sb) {
			if (count != sb.length())
				return false;
			char v1[] = value;
			char v2[] = sb.getValue();
			int i = offset;
			int j = 0;
			int n = count;
			while (n-- != 0) {
				if (v1[i++] != v2[j++])
					return false;
			}
		}
		return true;
	}

	public boolean equalsIgnoreCase(String anotherString) {
		return (this == anotherString) ? true : (anotherString != null)
				&& (anotherString.count == count)
				&& regionMatches(true, 0, anotherString, 0, count);
	}

	public int compareTo(String anotherString) {
		int len1 = count;
		int len2 = anotherString.count;
		int n = len1 < len2 ? len1 : len2;
		char v1[] = value;
		char v2[] = anotherString.value;
		int i = offset;
		int j = anotherString.offset;

		if (i == j) {
			int k = i;
			int lim = n + i;
			while (k < lim) {
				char c1 = v1[k];
				char c2 = v2[k];
				if (c1 != c2) {
					return c1 - c2;
				}
				k++;
			}
		} else {
			while (n-- != 0) {
				char c1 = v1[i++];
				char c2 = v2[j++];
				if (c1 != c2) {
					return c1 - c2;
				}
			}
		}
		return len1 - len2;
	}

	public int compareTo(Object o) {
		return compareTo((String) o);
	}

	public boolean regionMatches(int toffset, String other, int ooffset, int len) {
		char ta[] = value;
		int to = offset + toffset;
		char pa[] = other.value;
		int po = other.offset + ooffset;
		// Note: toffset, ooffset, or len might be near -1>>>1.
		if ((ooffset < 0) || (toffset < 0) || (toffset > (long) count - len)
				|| (ooffset > (long) other.count - len)) {
			return false;
		}
		while (len-- > 0) {
			if (ta[to++] != pa[po++]) {
				return false;
			}
		}
		return true;
	}

	public boolean regionMatches(boolean ignoreCase, int toffset, String other,
			int ooffset, int len) {
		char ta[] = value;
		int to = offset + toffset;
		char pa[] = other.value;
		int po = other.offset + ooffset;
		// Note: toffset, ooffset, or len might be near -1>>>1.
		if ((ooffset < 0) || (toffset < 0) || (toffset > (long) count - len)
				|| (ooffset > (long) other.count - len)) {
			return false;
		}
		while (len-- > 0) {
			char c1 = ta[to++];
			char c2 = pa[po++];
			if (c1 == c2) {
				continue;
			}
			if (ignoreCase) {
				// If characters don't match but case may be ignored,
				// try converting both characters to uppercase.
				// If the results match, then the comparison scan should
				// continue.
				char u1 = Character.toUpperCase(c1);
				char u2 = Character.toUpperCase(c2);
				if (u1 == u2) {
					continue;
				}
				// Unfortunately, conversion to uppercase does not work properly
				// for the Georgian alphabet, which has strange rules about case
				// conversion. So we need to make one last check before
				// exiting.
				if (Character.toLowerCase(u1) == Character.toLowerCase(u2)) {
					continue;
				}
			}
			return false;
		}
		return true;
	}

	public boolean startsWith(String prefix, int toffset) {
		char ta[] = value;
		int to = offset + toffset;
		char pa[] = prefix.value;
		int po = prefix.offset;
		int pc = prefix.count;
		// Note: toffset might be near -1>>>1.
		if ((toffset < 0) || (toffset > count - pc)) {
			return false;
		}
		while (--pc >= 0) {
			if (ta[to++] != pa[po++]) {
				return false;
			}
		}
		return true;
	}

	public boolean startsWith(String prefix) {
		return startsWith(prefix, 0);
	}

	public boolean endsWith(String suffix) {
		return startsWith(suffix, count - suffix.count);
	}

	public int hashCode() {
		int h = hash;
		if (h == 0) {
			int off = offset;
			char val[] = value;
			int len = count;

			for (int i = 0; i < len; i++) {
				h = 31 * h + val[off++];
			}
			hash = h;
		}
		return h;
	}

	public int indexOf(int ch) {
		return indexOf(ch, 0);
	}

	public int indexOf(int ch, int fromIndex) {
		int max = offset + count;
		char v[] = value;

		if (fromIndex < 0) {
			fromIndex = 0;
		} else if (fromIndex >= count) {
			// Note: fromIndex might be near -1>>>1.
			return -1;
		}
		for (int i = offset + fromIndex; i < max; i++) {
			if (v[i] == ch) {
				return i - offset;
			}
		}
		return -1;
	}

	public int lastIndexOf(int ch) {
		return lastIndexOf(ch, count - 1);
	}

	public int lastIndexOf(int ch, int fromIndex) {
		int min = offset;
		char v[] = value;

		for (int i = offset + ((fromIndex >= count) ? count - 1 : fromIndex); i >= min; i--) {
			if (v[i] == ch) {
				return i - offset;
			}
		}
		return -1;
	}

	public int indexOf(String str) {
		return indexOf(str, 0);
	}

	public int indexOf(String str, int fromIndex) {
		return indexOf(value, offset, count, str.value, str.offset, str.count,
				fromIndex);
	}

	static int indexOf(char[] source, int sourceOffset, int sourceCount,
			char[] target, int targetOffset, int targetCount, int fromIndex) {
		if (fromIndex >= sourceCount) {
			return (targetCount == 0 ? sourceCount : -1);
		}
		if (fromIndex < 0) {
			fromIndex = 0;
		}
		if (targetCount == 0) {
			return fromIndex;
		}

		char first = target[targetOffset];
		int i = sourceOffset + fromIndex;
		int max = sourceOffset + (sourceCount - targetCount);

		startSearchForFirstChar: while (true) {
			/* Look for first character. */
			while (i <= max && source[i] != first) {
				i++;
			}
			if (i > max) {
				return -1;
			}

			/* Found first character, now look at the rest of v2 */
			int j = i + 1;
			int end = j + targetCount - 1;
			int k = targetOffset + 1;
			while (j < end) {
				if (source[j++] != target[k++]) {
					i++;
					/* Look for str's first char again. */
					continue startSearchForFirstChar;
				}
			}
			return i - sourceOffset; /* Found whole string. */
		}
	}

	public int lastIndexOf(String str) {
		return lastIndexOf(str, count);
	}

	public int lastIndexOf(String str, int fromIndex) {
		return lastIndexOf(value, offset, count, str.value, str.offset,
				str.count, fromIndex);
	}

	static int lastIndexOf(char[] source, int sourceOffset, int sourceCount,
			char[] target, int targetOffset, int targetCount, int fromIndex) {
		/*
		 * Check arguments; return immediately where possible. For consistency,
		 * don't check for null str.
		 */
		int rightIndex = sourceCount - targetCount;
		if (fromIndex < 0) {
			return -1;
		}
		if (fromIndex > rightIndex) {
			fromIndex = rightIndex;
		}
		/* Empty string always matches. */
		if (targetCount == 0) {
			return fromIndex;
		}

		int strLastIndex = targetOffset + targetCount - 1;
		char strLastChar = target[strLastIndex];
		int min = sourceOffset + targetCount - 1;
		int i = min + fromIndex;

		startSearchForLastChar: while (true) {
			while (i >= min && source[i] != strLastChar) {
				i--;
			}
			if (i < min) {
				return -1;
			}
			int j = i - 1;
			int start = j - (targetCount - 1);
			int k = strLastIndex - 1;

			while (j > start) {
				if (source[j--] != target[k--]) {
					i--;
					continue startSearchForLastChar;
				}
			}
			return start - sourceOffset + 1;
		}
	}

	public String substring(int beginIndex) {
		return substring(beginIndex, count);
	}

	public String substring(int beginIndex, int endIndex) {
		if (beginIndex < 0) {
			throw new StringIndexOutOfBoundsException(beginIndex);
		}
		if (endIndex > count) {
			throw new StringIndexOutOfBoundsException(endIndex);
		}
		if (beginIndex > endIndex) {
			throw new StringIndexOutOfBoundsException(endIndex - beginIndex);
		}
		return ((beginIndex == 0) && (endIndex == count)) ? this : new String(
				offset + beginIndex, endIndex - beginIndex, value);
	}

	public CharSequence subSequence(int beginIndex, int endIndex) {
		return this.substring(beginIndex, endIndex);
	}

	public String concat(String str) {
		int otherLen = str.length();
		if (otherLen == 0) {
			return this;
		}
		char buf[] = new char[count + otherLen];
		getChars(0, count, buf, 0);
		str.getChars(0, otherLen, buf, count);
		return new String(0, count + otherLen, buf);
	}

	public String replace(char oldChar, char newChar) {
		if (oldChar != newChar) {
			int len = count;
			int i = -1;
			char[] val = value; /* avoid getfield opcode */
			int off = offset; /* avoid getfield opcode */

			while (++i < len) {
				if (val[off + i] == oldChar) {
					break;
				}
			}
			if (i < len) {
				char buf[] = new char[len];
				for (int j = 0; j < i; j++) {
					buf[j] = val[off + j];
				}
				while (i < len) {
					char c = val[off + i];
					buf[i] = (c == oldChar) ? newChar : c;
					i++;
				}
				return new String(0, len, buf);
			}
		}
		return this;
	}

	public String toLowerCase() {
		char[] target = new char[count];
		for (int i = 0, max = count; i < max; i++) {
			target[i] = Character.toLowerCase(value[i + offset]);
		}
		return new String(0, count, target);
	}

	public String toUpperCase() {
		char[] target = new char[count];
		for (int i = 0, max = count; i < max; i++) {
			target[i] = Character.toUpperCase(value[i + offset]);
		}
		return new String(0, count, target);
	}

	public String trim() {
		int len = count;
		int st = 0;
		int off = offset;
		char[] val = value;

		while ((st < len) && (val[off + st] <= ' ')) {
			st++;
		}
		while ((st < len) && (val[off + len - 1] <= ' ')) {
			len--;
		}
		return ((st > 0) || (len < count)) ? substring(st, len) : this;
	}

	public String toString() {
		return this;
	}

	public char[] toCharArray() {
		char result[] = new char[count];
		getChars(0, count, result, 0);
		return result;
	}

	public static String valueOf(Object obj) {
		return (obj == null) ? "null" : obj.toString();
	}

	public static String valueOf(char data[]) {
		return new String(data);
	}

	public static String valueOf(char data[], int offset, int count) {
		return new String(data, offset, count);
	}

	public static String copyValueOf(char data[], int offset, int count) {
		return new String(data, offset, count);
	}

	public static String copyValueOf(char data[]) {
		return copyValueOf(data, 0, data.length);
	}

	public static String valueOf(boolean b) {
		return b ? "true" : "false";
	}

	public static String valueOf(char c) {
		char data[] = { c };
		return new String(0, 1, data);
	}

	public static String valueOf(int i) {
		return Integer.toString(i, 10);
	}

	public static String valueOf(long l) {
		return Long.toString(l, 10);
	}

	public static String valueOf(float f) {
		return Float.toString(f);
	}

	public static String valueOf(double d) {
		return Double.toString(d);
	}

	public native String intern();

}
