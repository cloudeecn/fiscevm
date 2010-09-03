package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.VMContext;

public class ClassInfoPrinterTest extends TestCase {

	public void testClassInfo() throws Exception {
		if (true) {
			return;
		}
		VMContext context = TestInitializer.getContext();
		context.initialize();
		// AbstractClass clazz = context.getClass("java/io/OutputStreamWriter");
		// context.println(clazz.toString());
	}
}
