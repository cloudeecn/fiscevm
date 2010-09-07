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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package fisce.util;

/**
 * @author cloudee
 * 
 */
public class HashMap {

	private SimpleList[] content;
	private Entry entryNull;
	private int size;
	private int cap;
	private final float loadFactor;
	private int max;

	private void enlarge() {
		while (size > max) {
			cap *= 2;
			max = (int) (cap * loadFactor);
		}
		SimpleList[] oldContent = content;
		content = new SimpleList[cap];
		for (int i = 0, max = oldContent.length; i < max; i++) {
			SimpleList list = oldContent[i];
			if (list != null) {
				for (int j = 0, maxj = list.size(); j < maxj; j++) {
					Entry entry = (Entry) list.get(j);
					int index = (entry.getKey().hashCode() & 0x7fffffff) % cap;
					SimpleList newList = content[index];
					if (newList == null) {
						newList = new SimpleList(4);
						content[index] = newList;
					}
					newList.add(entry);
				}
			}
		}
	}

	public Object remove(Object key) {
		if (key == null) {
			if (entryNull != null) {
				Object ret = entryNull.getValue();
				entryNull = null;
				size--;
				return ret;
			}
			return null;
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList list = content[index];
		if (list == null) {
			return null;
		}
		for (int i = 0, max = list.size(); i < max; i++) {
			Entry entry = (Entry) list.get(i);
			if (key.equals(entry.getKey())) {
				size--;
				list.remove(i);
				return entry.getValue();
			}
		}
		return null;
	}

	public void put(Object key, Object value) {
		if (size > max) {
			enlarge();
		}
		if (key == null) {
			if (entryNull == null) {
				entryNull = new Entry();
				size++;
			}
			entryNull.setValue(value);
		} else {
			int index = (key.hashCode() & 0x7fffffff) % cap;
			SimpleList list = content[index];
			if (list == null) {
				list = new SimpleList();
				list.add(new Entry(key, value));
				content[index] = list;
				size++;
			} else {
				for (int i = 0, max = list.size(); i < max; i++) {
					Entry e = (Entry) list.get(i);
					if (key.equals(e.getKey())) {
						e.setValue(value);
						return;
					}
				}
				list.add(new Entry(key, value));
				size++;
			}
		}
	}

	public boolean contains(Object key) {
		if (key == null) {
			return entryNull != null;
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList list = content[index];
		if (list == null) {
			return false;
		}

		for (int i = 0, max = list.size(); i < max; i++) {
			Entry entry = (Entry) list.get(i);
			if (key.equals(entry.getKey())) {
				return true;
			}
		}
		return false;
	}

	public Object get(Object key) {
		if (key == null) {
			return entryNull == null ? null : entryNull.getValue();
		}
		int index = (key.hashCode() & 0x7fffffff) % cap;
		SimpleList list = content[index];
		if (list == null) {
			return null;
		}

		for (int i = 0, max = list.size(); i < max; i++) {
			Entry entry = (Entry) list.get(i);
			if (key.equals(entry.getKey())) {
				return entry.getValue();
			}
		}
		return null;
	}

	public int size() {
		return size;
	}

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
