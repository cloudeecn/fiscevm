package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.VMFactory;

public class CFiScEFactory implements VMFactory {

	@Override
	public FiScEVM getVM() {
		return new FYContext();
	}

}
