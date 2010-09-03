package com.cirnoworks.fisce.vm.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.TestStatics;

public class ExceptionTest extends TestCase {

	public void testException() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestStatics.prepareContext(getClass().getName());
		context.bootFromClass("ox/cirnoworks/test/ExceptionTester");
		context.start();
//		context.requestStop();
		context.waitTillStopped(0);
	}

}
