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
