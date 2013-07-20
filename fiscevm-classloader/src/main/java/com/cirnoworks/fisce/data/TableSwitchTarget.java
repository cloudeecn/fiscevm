package com.cirnoworks.fisce.data;

public class TableSwitchTarget {
	private int defaultTarget;
	private int min;
	private int max;
	private int[] targets;
	public int getDefaultTarget() {
		return defaultTarget;
	}
	public void setDefaultTarget(int defaultTarget) {
		this.defaultTarget = defaultTarget;
	}
	public int getMin() {
		return min;
	}
	public void setMin(int min) {
		this.min = min;
	}
	public int getMax() {
		return max;
	}
	public void setMax(int max) {
		this.max = max;
	}
	public int[] getTargets() {
		return targets;
	}
	public void setTargets(int[] targets) {
		this.targets = targets;
	}
	
	
}
