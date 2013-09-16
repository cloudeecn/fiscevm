package java.util.concurrent;

import java.util.HashMap;
import java.util.Map;

public class ConcurrentHashMap<K, V> extends HashMap<K, V> {

	public ConcurrentHashMap() {
		super();
	}

	public ConcurrentHashMap(int capacity, float loadFactor) {
		super(capacity, loadFactor);
	}

	public ConcurrentHashMap(int capacity) {
		super(capacity);
	}

	public ConcurrentHashMap(Map<? extends K, ? extends V> map) {
		super(map);
	}

}
