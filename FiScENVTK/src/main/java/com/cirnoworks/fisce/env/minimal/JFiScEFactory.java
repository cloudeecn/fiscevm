package com.cirnoworks.fisce.env.minimal;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.NullConsole;
import com.cirnoworks.fisce.intf.VMFactory;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.ArrayHeap;
import com.cirnoworks.fisce.vm.default_impl.ArrayThreadManager;
import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;

public class JFiScEFactory implements VMFactory {

	public FiScEVM getVM() {
		VMContext context = new VMContext();
		context.setClassLoader(new DefaultClassLoader());
		context.setHeap(new ArrayHeap());
		context.setConsole(new NullConsole());
		context.setThreadManager(new ArrayThreadManager());
		context.addToolkit(new BaseToolkit());
		return context;
	}

}
