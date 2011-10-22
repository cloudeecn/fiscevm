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

/**
 * 
 * @author Cloudee
 */
final public class SingleLink {

	final static int MAX_POOL = 128;
	static int pool_size;
	static final SingleLinkContent[] contenters = new SingleLinkContent[MAX_POOL];

	final static private SingleLinkContent getContenter() {
		synchronized (contenters) {
			if (pool_size > 0) {
				return contenters[--pool_size];
			}
			return new SingleLinkContent();
		}
	}

	final static private void releaseContenter(SingleLinkContent c) {
		synchronized (contenters) {
			if (pool_size < MAX_POOL) {
				contenters[pool_size++] = c;
				c.next = null;
				c.content = null;
			}
		}
	}

	final SingleLinkContent first = new SingleLinkContent();
	SingleLinkContent now;
	SingleLinkContent before;
	int size;

	public SingleLink() {
		now = first;
	}

	public boolean hasNext() {
		return now.next != null;
	}

	public Object next() {
		before = now;
		now = now.next;
		return now.content;
	}

	public boolean add(Object o) {
		if (o == null) {
			throw new NullPointerException();
		}
		SingleLinkContent toAdd = getContenter();
		toAdd.content = o;
		toAdd.next = now.next;
		now.next = toAdd;
		size++;
		return true;
	}

	public boolean addFirst(Object o) {
		if (o == null) {
			throw new NullPointerException();
		}
		SingleLinkContent toAdd = getContenter();
		toAdd.content = o;
		toAdd.next = first.next;
		first.next = toAdd;
		size++;
		return true;
	}

	public void reset() {
		now = first;
		before = null;
	}

	public void remove() {
		if (before == null) {
			throw new java.lang.IllegalStateException();
		}
		before.next = now.next;
		releaseContenter(now);
		now = before;
		before = null;
		size--;
	}

	public void remove(Object o) {
		SingleLinkContent sc = first;
		SingleLinkContent rbefore;
		while (sc.next != null) {
			rbefore = sc;
			sc = sc.next;
			if (sc.content == o) {
				rbefore.next = sc.next;
				releaseContenter(sc);
				size--;
				break;
			}
		}
	}

	public int size() {
		return size;
	}

	public boolean isEmpty() {
		return first.next == null;
	}

	public void clear() {
		SingleLinkContent n = first, tmp;
		for (int i = 0, max = size() > (MAX_POOL - pool_size) ? MAX_POOL
				- pool_size : size(); i < max; i++) {
			tmp = n;
			n = n.next;
			releaseContenter(tmp);
		}
		now = first;
		first.next = null;
		before = null;
		size = 0;
	}

	public int psize() {
		return size;
	}
}

class SingleLinkContent {

	SingleLinkContent next;
	Object content;
}
