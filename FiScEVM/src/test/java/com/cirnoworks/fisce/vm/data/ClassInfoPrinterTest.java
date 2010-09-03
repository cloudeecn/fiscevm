package com.cirnoworks.fisce.vm.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.TestStatics;
import com.cirnoworks.fisce.vm.VMContext;

public class ClassInfoPrinterTest extends TestCase {

	public void testClassInfo() throws Exception {
		if (true) {
			return;
		}
		VMContext context = TestStatics.prepareContext(getClass().getName());
		context.initialize();
		// AbstractClass clazz = context.getClass("java/io/OutputStreamWriter");
		// context.println(clazz.toString());
	}
}
