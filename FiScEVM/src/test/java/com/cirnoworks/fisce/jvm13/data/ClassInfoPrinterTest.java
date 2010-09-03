package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.TestStatics;
import com.cirnoworks.fisce.jvm13.VMContext;

public class ClassInfoPrinterTest extends TestCase {

	public void testClassInfo() throws Exception {

		VMContext context = TestStatics.prepareContext(getClass().getName());
		context.initialize();
		// AbstractClass clazz = context.getClass("java/io/OutputStreamWriter");
		// context.println(clazz.toString());
	}
}
