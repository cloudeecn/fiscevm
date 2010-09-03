package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.FiScEVM;
import com.cirnoworks.fisce.jvm13.NullConsole;
import com.cirnoworks.fisce.jvm13.TestStatics;

public class BenchmarkTest extends TestCase {

	public void testBenchmark() throws Exception {
		FiScEVM context = TestStatics.prepareContext(getClass().getName());
		context.bootFromClass("ox/cirnoworks/test/Benchmark");
		context.setConsole(new NullConsole());
		context.start();
		context.waitTillStopped(0);
	}
}
