package com.cirnoworks.fisce.vm.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.NullConsole;
import com.cirnoworks.fisce.vm.TestStatics;

public class BenchmarkTest extends TestCase {

	public void testBenchmark() throws Exception {
		if (true) {
			return;
		}
		FiScEVM context = TestStatics.prepareContext(getClass().getName());
		context.bootFromClass("ox/cirnoworks/test/Benchmark");
		context.setConsole(new NullConsole());
		context.start();
		context.waitTillStopped(0);
	}
}
