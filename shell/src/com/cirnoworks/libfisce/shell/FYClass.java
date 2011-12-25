package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.idata.IClass;
import com.cirnoworks.fisce.intf.idata.IClassArray;
import com.cirnoworks.fisce.intf.idata.IClassBase;

public class FYClass implements IClass, IClassBase, IClassArray {

	private FYContext context;
	private int id;

	public FYClass(FYContext context, int id) {
		super();
		this.context = context;
		this.id = id;
	}

	@Override
	public String getName() {
		return FisceService.getClassName(context.getContext(), id);
	}

	@Override
	public IClassBase getSuperClass() {
		int superId = FisceService.getClassSuper(context.getContext(), id);
		return context.getClassById(superId);
	}

	public int getId() {
		return id;
	}

}
