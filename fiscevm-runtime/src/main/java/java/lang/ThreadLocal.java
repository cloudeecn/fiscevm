package java.lang;

public class ThreadLocal<V> {

	private V value;
	private boolean inited;

	protected V initialValue() {
		return (null);
	}

	public void set(V value) {
		inited = true;
		this.value = value;
	}

	public V get() {
		if (!inited) {
			value = initialValue();
		}
		return value;
	}
}
