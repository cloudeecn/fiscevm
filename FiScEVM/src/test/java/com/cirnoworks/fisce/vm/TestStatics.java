package com.cirnoworks.fisce.jvm13;

import com.cirnoworks.fisce.jvm13.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.jvm13.default_impl.DefaultHeap;
import com.cirnoworks.fisce.jvm13.default_impl.DefaultThreadManager;

public final class TestStatics {
	private TestStatics() {

	}

	public static VMContext prepareContext(String name) {
		
		VMContext context = new VMContext();
		//context.addToolkit(new Toolkit());
		context.setConsole(com.cirnoworks.fisce.jvm13.Log4JConsole.getConsole());
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
