package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.VMContext;

public class ExceptionTest extends TestCase {

	public void testException() throws Exception {
		doTest(TestInitializer.getContext());
	}

	public void testExceptionFast() throws Exception {
		doTest(TestInitializer.getFastContext());
	}
	
	public void testExceptionArray() throws Exception {
		doTest(TestInitializer.getArrayContext());
	}

	private void doTest(VMContext context) throws Exception {
		context.bootFromClass("EXCLUDE/fisce/test/ExceptionTester");
		context.start();
		context.waitTillStopped(0);
	}

}
