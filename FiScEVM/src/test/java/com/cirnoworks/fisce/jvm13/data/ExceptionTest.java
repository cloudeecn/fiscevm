package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.FiScEVM;
import com.cirnoworks.fisce.jvm13.TestStatics;

public class ExceptionTest extends TestCase {

	public void testException() throws Exception {
		FiScEVM context = TestStatics.prepareContext(getClass().getName());
		context.bootFromClass("ox/cirnoworks/test/ExceptionTester");
		context.start();
//		context.requestStop();
		context.waitTillStopped(0);
	}

}
