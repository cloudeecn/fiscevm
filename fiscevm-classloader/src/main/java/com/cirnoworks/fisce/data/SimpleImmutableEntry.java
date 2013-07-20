package com.cirnoworks.fisce.data;

import java.util.Map.Entry;

public final class SimpleImmutableEntry<K, V> implements Entry<K, V> {

	private final K key;
	private final V value;

	public SimpleImmutableEntry(K key, V value) {
		super();
		this.key = key;
		this.value = value;
	}

	@Override
	public K getKey() {
		return key;
	}

	@Override
	public V getValue() {
		return value;
	}

	@Override
	public V setValue(V value) {
		throw new UnsupportedOperationException("immutable");
	}

}
