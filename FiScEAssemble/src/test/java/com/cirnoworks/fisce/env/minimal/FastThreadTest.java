package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;

public class FastThreadTest extends TestCase {
	public void testRun() throws Exception {
		FiScEVM context = TestInitializer.getFastContext();
		context.bootFromClass("EXCLUDE/fisce/test/Tester");
		context.start();
		// context.requestStop();
		context.waitTillStopped(0);
	}

	public void testStop() throws Exception {
		FiScEVM context = TestInitializer.getFastContext();
		// context.setConsole(new NullConsole());
		context.bootFromClass("EXCLUDE/fisce/test/RunnerTester");
		context.start();
		try {
			context.waitTillStopped(200);
		} catch (InterruptedException e) {

		}
		context.requestStop();
		context.waitTillStopped(0);
		// System.out.println(3);
	}
}
