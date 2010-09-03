package com.cirnoworks.fisce.vm;

import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.vm.default_impl.DefaultHeap;
import com.cirnoworks.fisce.vm.default_impl.DefaultThreadManager;

public final class TestStatics {
	private TestStatics() {

	}

	public static VMContext prepareContext(String name) {
		
		VMContext context = new VMContext();
		//context.addToolkit(new Toolkit());
		context.setConsole(com.cirnoworks.fisce.vm.Log4JConsole.getConsole());
		context.setHeap(new DefaultHeap());
		context.setThreadManager(new DefaultThreadManager());
		DefaultClassLoader loader = new DefaultClassLoader();

		context.setClassLoader(loader);

		System.setProperty("microedition.encoding", "utf-8");

		context.getConsole().debug("");
		context.getConsole().debug("");
		context.getConsole().debug("#1#######################1#");
		context.getConsole().debug("#1#######################1#");
		context.getConsole().debug(name);
		return context;
	}
}
