package com.cirnoworks.libfisce.shell.testers;

import com.cirnoworks.libfisce.shell.HLTestBase;

public class HLTest extends HLTestBase{

	public void testHelloWorld(){
		run("EXCLUDE/fisce/test/HelloWorld");
	}
	
	public void testUnicode() {
		run("EXCLUDE/fisce/test/UnicodeTest");
	}

	public void testThread() {
		run("EXCLUDE/fisce/test/Tester");
	}

	public void testArray() {
		run("EXCLUDE/fisce/test/ArrayTest");
	}

	public void testArch() {
		run("EXCLUDE/fisce/test/ArchitectureTest");
	}

	public void testHierarchy() {
		run("EXCLUDE/fisce/test/HierarchyTest");
	}

	public void testDebugPrintStream() {
		run("EXCLUDE/fisce/test/DebugPrintStreamTest");
	}

	public void testAutoBoxing() {
		run("EXCLUDE/fisce/test/AutoBoxingTest");
	}

	public void testThread2() {
		run("EXCLUDE/fisce/test/RunnerTester");
	}

	public void testEnum() {
		run("EXCLUDE/fisce/test/EnumTester");
	}

	public void testException() {
		run("EXCLUDE/fisce/test/ExceptionTester");
	}

	public void testForEach() {
		run("EXCLUDE/fisce/test/ForEachTest");
	}

	public void testHashMap() {
		run("EXCLUDE/fisce/test/HashMapTest");
	}

	public void testStatic() {
		run("EXCLUDE/fisce/test/StaticTest");
	}

	public void testProfile() {
		run("com/cirnoworks/fisce/privat/Profile");
	}

	public void testGC() {
		run("EXCLUDE/fisce/test/GCTest");
	}

	public void testComplex() {
		run("EXCLUDE/fisce/test/ComplexClassLayout");
	}

	public void testTableSwitch() {
		run("EXCLUDE/fisce/test/SwitchTest");
	}

	public void testLookupSwitch() {
		run("EXCLUDE/fisce/test/SwitchTest2");
	}

	public void testLinpack() {
		run("com/cirnoworks/fisce/privat/Linpack");
	}

	public void testStore() {
		run("EXCLUDE/fisce/test/ParamStoreTest");
	}

	public void testReflection() {
		run("EXCLUDE/fisce/test/ReflectionTest");
	}

	public void testProxy() {
		run("EXCLUDE/fisce/test/ProxyTest");
	}
}
