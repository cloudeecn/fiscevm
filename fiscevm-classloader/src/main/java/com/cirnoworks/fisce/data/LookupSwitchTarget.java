package com.cirnoworks.fisce.data;

public class LookupSwitchTarget {
	private int defaultTarget;
	private int[] keys;
	private int[] targets;

	public int getDefaultTarget() {
		return defaultTarget;
	}

	public void setDefaultTarget(int defaultTarget) {
		this.defaultTarget = defaultTarget;
	}

	public int[] getKeys() {
		return keys;
	}

	public void setKeys(int[] keys) {
		this.keys = keys;
	}

	public int[] getTargets() {
		return targets;
	}

	public void setTargets(int[] targets) {
		this.targets = targets;
	}

}
