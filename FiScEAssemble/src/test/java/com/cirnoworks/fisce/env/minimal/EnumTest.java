package com.cirnoworks.fisce.env.minimal;

import com.cirnoworks.fisce.vm.VMContext;

public class EnumTest {

	public void testEnum() throws Exception {
		VMContext vm = TestInitializer.getContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}

	public void testEnumFast() throws Exception {
		VMContext vm = TestInitializer.getFastContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}
	
	public void testEnumArray() throws Exception {
		VMContext vm = TestInitializer.getArrayContext();
		// vm.setConsole(Log4JConsole.getConsole());
		doTest(vm);
	}

	private void doTest(VMContext context) throws Exception {
		context.bootFromClass("EXCLUDE/fisce/test/EnumTester");
		context.start();
		context.waitTillStopped(0);
	}
}
