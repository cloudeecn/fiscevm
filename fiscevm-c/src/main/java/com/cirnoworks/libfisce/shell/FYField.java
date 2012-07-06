package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.idata.IClass;
import com.cirnoworks.fisce.intf.idata.IField;

public class FYField implements IField {
	private FYContext context;
	private int id;

	public FYField(FYContext context, int id) {
		super();
		this.context = context;
		this.id = id;
	}

	
	public IClass getOwner() {
		return context.getClassById(FisceService.getFieldOwner(
				context.getContext(), id));
	}

	public int getId() {
		return id;
	}
}
