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
 * @author cloudee
 *
 */
public class HashMap<K, V> {

	private SimpleList<Entry<K, V>>[] content;
	private Entry<K, V> entryNull;
	private int size;
	private int cap;
	private final float loadFactor;
	private int max;

	@SuppressWarnings("unchecked")
	private void enlarge() {
		while (size > max) {
			cap *= 2;
			max = (int) (cap * loadFactor);
		}
		SimpleList<Entry<K, V>>[] oldContent = content;
		content = new SimpleList[cap];
		for (int i = 0, max = oldContent.length; i < max; i++) {
			SimpleList<Entry<K, V>> list = oldContent[i];
			if (list != null) {
				for (int j = 0, maxj = list.size(); j < maxj; j++) {
					Entry<K, V> entry = list.get(j);
					int index = (entry.getKey().hashCode() & 0x7fffffff) % cap;
					SimpleList<Entry<K, V>> newList = content[index];
					if (newList == null) {
						newList = new SimpleList<Entry<K, V>>(4);
						content[index] = newList;
					}
					newList.add(entry);
				}
			}
		}
	}

	public V remove(K key) {
		if (key == null) {
			if (entryNull != null) {
				V ret = entryNull.getValue();
				entryNull = null;
				size--;
				return ret;
			}
			return null;
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList<Entry<K, V>> list = content[index];
		if (list == null) {
			return null;
		}
		for (int i = 0, max = list.size(); i < max; i++) {
			Entry<K, V> entry = list.get(i);
			if (key.equals(entry.getKey())) {
				size--;
				list.remove(i);
				return entry.getValue();
			}
		}
		return null;
	}

	public void put(K key, V value) {
		if (size > max) {
			enlarge();
		}
		if (key == null) {
			if (entryNull == null) {
				entryNull = new Entry<K, V>();
				size++;
			}
			entryNull.setValue(value);
		} else {
			int index = (key.hashCode() & 0x7fffffff) % cap;
			SimpleList<Entry<K, V>> list = content[index];
			if (list == null) {
				list = new SimpleList<Entry<K, V>>();
				list.add(new Entry<K, V>(key, value));
				content[index] = list;
				size++;
			} else {
				for (int i = 0, max = list.size(); i < max; i++) {
					Entry<K, V> e = list.get(i);
					if (key.equals(e.getKey())) {
						e.setValue(value);
						return;
					}
				}
				list.add(new Entry<K, V>(key, value));
				size++;
			}
		}
	}

	public boolean contains(Object key) {
		if (key == null) {
			return entryNull != null;
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList<Entry<K, V>> list = content[index];
		if (list == null) {
			return false;
		}

		for (int i = 0, max = list.size(); i < max; i++) {
			Entry<K, V> entry = list.get(i);
			if (key.equals(entry.getKey())) {
				return true;
			}
		}
		return false;
	}

	public V get(K key) {
		if (key == null) {
			return entryNull == null ? null : entryNull.getValue();
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList<Entry<K, V>> list = content[index];
		if (list == null) {
			return null;
		}

		for (int i = 0, max = list.size(); i < max; i++) {
			Entry<K, V> entry = list.get(i);
			if (key.equals(entry.getKey())) {
				return entry.getValue();
			}
		}
		return null;
	}

	public int size() {
		return size;
	}

	@SuppressWarnings("unchecked")
	public HashMap(int cap, float loadFactor) {
		if (loadFactor > 0.99) {
			throw new IllegalArgumentException(
					"Please select a smaller loader factor (must < 0.99 and 0.7-0.8 is recommanded");
		}
		this.cap = cap;
		this.loadFactor = loadFactor;
		content = new SimpleList[cap];
		max = (int) (cap * loadFactor);
	}

	public HashMap() {
		this(17, 0.8f);
	}

	public HashMap(int cap) {
		this(cap, 0.8f);
	}
}
