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
package fisce.util;

import java.util.Iterator;

/**
 * 
 * @author yuxuanhuang
 */
public class SimpleList<T> implements Iterable<T> {

	protected int size;
	protected Object[] container;

	public SimpleList(int initSize) {
		container = new Object[initSize];
	}

	public SimpleList() {
		this(16);
	}

	public final void ensureCap(int cap) {
		int oldCap = container.length;
		int newCap = oldCap;
		while (newCap < cap) {
			newCap <<= 1;
		}
		if (oldCap != newCap) {
			Object[] cs = new Object[newCap];
			System.arraycopy(container, 0, cs, 0, size);
			container = cs;
		}
	}

	public final void ensureSize(int newSize) {
		if (newSize > size) {
			ensureCap(newSize);
			size = newSize;
		}
	}

	public final int add(T c) {
		if (container.length <= size) {
			Object[] cs = new Object[container.length << 1];
			System.arraycopy(container, 0, cs, 0, size);
			container = cs;
		}
		container[size] = c;
		return size++;
	}

	public final boolean remove(T c) {
		for (int i = 0; i < size; i++) {
			if (container[i] == c) {
				remove(i);
				return true;
			}
		}
		return false;
	}

	public final void clear() {
		for (int i = 0, max = size; i < max; i++) {
			container[i] = null;
		}
		size = 0;
	}

	@SuppressWarnings("unchecked")
	public final T get(int idx) {
		if (idx >= size) {
			throw new ArrayIndexOutOfBoundsException();
		}
		return (T) container[idx];
	}

	public final void set(int idx, T c) {
		if (idx >= size) {
			throw new ArrayIndexOutOfBoundsException(idx + ">=" + size);
		}
		container[idx] = c;
	}

	public final int size() {
		return size;
	}

	public final void trimToSize(int size) {
		if (this.size < size) {
			throw new IndexOutOfBoundsException();
		} else if (this.size == size) {
			return;
		} else {
			for (int i = size, max = this.size; i < max; i++) {
				container[i] = null;
			}
			this.size = size;
		}
	}

	public final T remove(int index) {
		if (index < 0 || index >= size) {
			throw new IndexOutOfBoundsException();
		}
		@SuppressWarnings("unchecked")
		T ret = (T) container[index];
		for (int i = index + 1; i < size; i++) {
			container[i - 1] = container[i];
		}
		size--;
		container[size] = null;
		return ret;
	}

	public final T pop() {
		if (size == 0) {
			throw new IndexOutOfBoundsException("Pop from empty list");
		}
		size--;
		@SuppressWarnings("unchecked")
		T ret = (T) container[size];
		container[size] = null;
		return ret;
	}

	public final class SimpleListIterator implements Iterator<T> {

		private int pos;

		/*
		 * (non-Javadoc)
		 * 
		 * @see java.util.Iterator#hasNext()
		 */
		public boolean hasNext() {
			return pos < size;
		}

		/*
		 * (non-Javadoc)
		 * 
		 * @see java.util.Iterator#next()
		 */
		public T next() {
			T ret = get(pos);
			pos++;
			return ret;
		}

		/*
		 * (non-Javadoc)
		 * 
		 * @see java.util.Iterator#remove()
		 */
		public void remove() {
			if (pos == 0) {
				throw new IllegalArgumentException();
			}
			pos--;
			SimpleList.this.remove(pos);
		}

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Iterable#iterator()
	 */
	public final Iterator<T> iterator() {
		return new SimpleListIterator();
	}
}
