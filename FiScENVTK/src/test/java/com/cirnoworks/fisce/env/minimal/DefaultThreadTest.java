package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;

public class DefaultThreadTest extends TestCase {
	public void testRun() throws Exception {
		FiScEVM context = TestInitializer.getContext();
		context.bootFromClass("com/cirnoworks/fisce/test/Tester");
		context.start();
		// context.requestStop();
		context.waitTillStopped(0);
	}

	public void testStop() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestInitializer.getContext();
		// context.setConsole(new NullConsole());
		context.bootFromClass("com/cirnoworks/fisce/test/RunnerTester");
		context.start();
		try {
			context.waitTillStopped(1000);
		} catch (InterruptedException e) {

		}
		context.requestStop();
		context.waitTillStopped(0);
		// System.out.println(3);
	}
}
