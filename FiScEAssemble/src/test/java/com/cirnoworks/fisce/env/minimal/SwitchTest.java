package com.cirnoworks.fisce.env.minimal;

import com.cirnoworks.fisce.vm.VMContext;

public class SwitchTest {
	public void testSwitch() throws Exception {
		doTest(TestInitializer.getContext());
	}

	public void testSwitchFast() throws Exception {
		doTest(TestInitializer.getFastContext());
	}
	
	public void testSwitchArray() throws Exception {
		doTest(TestInitializer.getArrayContext());
	}
	
	public void testSwitch2() throws Exception {
		doTest2(TestInitializer.getContext());
	}

	public void testSwitch2Fast() throws Exception {
		doTest2(TestInitializer.getFastContext());
	}
	
	public void testSwitch2Array() throws Exception {
		doTest2(TestInitializer.getArrayContext());
	}

	private void doTest(VMContext context) throws Exception {
		context.bootFromClass("EXCLUDE/fisce/test/SwitchTest");
		context.start();
		context.waitTillStopped(0);
	}
	
	private void doTest2(VMContext context) throws Exception {
		context.bootFromClass("EXCLUDE/fisce/test/SwitchTest2");
		context.start();
		context.waitTillStopped(0);
	}
}
