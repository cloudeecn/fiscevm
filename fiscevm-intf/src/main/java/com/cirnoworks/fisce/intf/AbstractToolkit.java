package com.cirnoworks.fisce.intf;

public abstract class AbstractToolkit implements IToolkit {
	private float priority;

	public float getPriority() {
		return priority;
	}

	public void setPriority(float priority) {
		this.priority = priority;
	}

}
