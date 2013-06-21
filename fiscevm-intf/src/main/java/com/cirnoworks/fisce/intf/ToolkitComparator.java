package com.cirnoworks.fisce.intf;

import java.util.Comparator;

public class ToolkitComparator implements Comparator<IToolkit> {

	public static final ToolkitComparator singleton = new ToolkitComparator();

	@Override
	public int compare(IToolkit o1, IToolkit o2) {
		return -Float.compare(o1.getPriority(), o2.getPriority());
	}

}
