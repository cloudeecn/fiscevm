package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.NullConsole;

public class BenchmarkTest extends TestCase {

	public void testBenchmark() throws Exception {
		FiScEVM context = TestInitializer.getContext();
		context.bootFromClass("com/cirnoworks/fisce/test/Benchmark");
		context.setConsole(new NullConsole()); 
		context.start();
		context.waitTillStopped(0);
	}
	
	public void testBenchmarkFast() throws Exception {
		FiScEVM context = TestInitializer.getFastContext();
		context.bootFromClass("com/cirnoworks/fisce/test/Benchmark");
		context.setConsole(new NullConsole()); 
		context.start();
		context.waitTillStopped(0);
	}
}
