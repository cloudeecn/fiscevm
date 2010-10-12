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
package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

import fisce.util.HashMap;

/**
 * @author cloudee
 * 
 */
public class HashMapTest {

	public static void main(String[] args) {
		try {
			HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
			for (int i = 0; i < 500; i++) {
				map.put(i, i * 2);
			}
			if (map.size() != 500) {
				FiScEVM.throwOut(new RuntimeException("size is wrong! "
						+ map.size()));
			}
			if (map.contains(null)) {
				FiScEVM.throwOut(new RuntimeException(
						"Null exists before add null! "));
			}
			if (map.get(null) != null) {
				FiScEVM.throwOut(new RuntimeException(
						"get(null)!=null before add null! "));
			}
			map.put(null, -1);
			if (map.size() != 501) {
				FiScEVM.throwOut(new RuntimeException(
						"size is wrong after add null! " + map.size()));
			}
			if (!map.contains(null)) {
				FiScEVM.throwOut(new RuntimeException(
						"Null not exist after add null! "));
			}
			if (map.get(null) == null) {
				FiScEVM.throwOut(new RuntimeException(
						"get(null)==null after add null! "));
			}
			map.remove(null);
			if (map.size() != 500) {
				FiScEVM.throwOut(new RuntimeException(
						"size is wrong after add and remove null! "
								+ map.size()));
			}
			if (map.contains(null)) {
				FiScEVM.throwOut(new RuntimeException(
						"Null exists after add and remove null! "));
			}
			if (map.get(null) != null) {
				FiScEVM.throwOut(new RuntimeException(
						"get(null)!=null after add and remove null! "));
			}
			for (int i = 0; i < 500; i++) {
				if (!map.contains(i)) {
					FiScEVM.throwOut(new RuntimeException("key " + i
							+ " not exist!"));
				}
				int value = map.get(i);
				if (value != i * 2) {
					FiScEVM.throwOut(new RuntimeException("key " + i
							+ "'s value is " + value));
				}
			}
			for (int i = 501; i < 600; i++) {
				Integer key = new Integer(i);
				if (map.contains(key)) {
					FiScEVM.throwOut(new RuntimeException("key " + i
							+ " should not exist!"));
				}
				if (map.get(key) != null) {
					FiScEVM.throwOut(new RuntimeException("key " + i
							+ " should be null!"));
				}
			}
			for (int i = 0; i < 500; i += 2) {
				Integer key = new Integer(i);
				Integer value = (Integer) map.remove(key);
				if (value == null) {
					FiScEVM.throwOut(new RuntimeException(
							"remove error, remove exist non null should not return null"
									+ key));
				}
			}
			if (map.size() != 250) {
				FiScEVM.throwOut(new RuntimeException(
						"size is wrong after remove 2500"));
			}
			for (int i = 501; i < 600; i++) {
				Integer key = new Integer(i);
				Integer value = (Integer) map.remove(key);
				if (value != null) {
					FiScEVM.throwOut(new RuntimeException(
							"remove error, remove not exist should return null"
									+ key));
				}
			}
			if (map.size() != 250) {
				FiScEVM.throwOut(new RuntimeException(
						"size is wrong after remove non exists"));
			}
			for (int i = 1; i < 500; i += 2) {
				Integer key = new Integer(i);
				if (!map.contains(key)) {
					FiScEVM.throwOut(new RuntimeException("key " + i
							+ " not exist2!"));
				}
				Integer value = (Integer) map.get(key);
				if (value == null) {
					FiScEVM.throwOut(new RuntimeException("remove error, get("
							+ key + ") should not return null"));
				}
				if (value.intValue() != i * 2) {
					FiScEVM.throwOut(new RuntimeException("remove error, get("
							+ key + ") with wrong value " + value));
				}
			}
		} catch (Throwable t) {
			FiScEVM.throwOut(t);
		}
	}
}
