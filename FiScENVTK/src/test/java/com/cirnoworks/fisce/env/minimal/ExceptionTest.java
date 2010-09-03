package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;

public class ExceptionTest extends TestCase {

	public void testException() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestInitializer.getContext();
		context.bootFromClass("com/cirnoworks/fisce/test/ExceptionTester");
		context.start();
		// context.requestStop();
		context.waitTillStopped(0);
	}

}
