package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.NullConsole;

public class BenchmarkTest extends TestCase {

	public void testBenchmark() throws Exception {
		System.out.println("Default");
		FiScEVM context = TestInitializer.getContext();
		context.bootFromClass("EXCLUDE/fisce/test/Benchmark");
		context.setConsole(new NullConsole()); 
		context.start();
		context.waitTillStopped(0);
	}
	
	public void testBenchmarkFast() throws Exception {
		System.out.println("Fast");
		FiScEVM context = TestInitializer.getFastContext();
		context.bootFromClass("EXCLUDE/fisce/test/Benchmark");
		context.setConsole(new NullConsole()); 
		context.start();
		context.waitTillStopped(0);
	}
	
	public void testBenchmarkArray() throws Exception {
		System.out.println("Array");
		FiScEVM context = TestInitializer.getArrayContext();
		context.bootFromClass("EXCLUDE/fisce/test/Benchmark");
		context.setConsole(new NullConsole()); 
		context.start();
		context.waitTillStopped(0);
	}
}
