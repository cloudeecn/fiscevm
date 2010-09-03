package com.cirnoworks.fisce.vm.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.TestStatics;

public class DefaultThreadTest extends TestCase {
	public void testRun() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestStatics.prepareContext(getClass().getName()
				+ ".testRun");
		context.bootFromClass("ox/cirnoworks/test/Tester");
		context.start();
		// context.requestStop();
		context.waitTillStopped(0);
	}

	public void testStop() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestStatics.prepareContext(getClass().getName()
				+ ".testStop");
		// context.setConsole(new NullConsole());
		context.bootFromClass("ox/cirnoworks/test/RunnerTester");
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
