package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.TestStatics;
import com.cirnoworks.fisce.jvm13.VMContext;

public class VMContextTest extends TestCase {
	public void testLookupMethodInterface() throws Exception {
		VMContext context = TestStatics.prepareContext(getClass().getName());
		context.initialize();

		ClassBase it = (ClassBase) context
				.getClass("ox/cirnoworks/test/ITester");
		ClassBase t = (ClassBase) context.getClass("ox/cirnoworks/test/Tester");
		ClassBase tc = (ClassBase) context
				.getClass("ox/cirnoworks/test/TesterChild");
		ClassMethod[] methods = it.getMethods();
		ClassMethod target = null;
		for (int i = 0, max = methods.length; i < max; i++) {
			ClassMethod method = methods[i];
			if (method.getName().equals("testInterface")) {
				target = method;
				break;
			}
		}
		assert target != null;
		ClassMethod lookup = context.lookupMethodVirtual(t, target
				.getMethodName());
		assert lookup.getOwner() == t;
		ClassMethod lookup1 = context.lookupMethodVirtual(tc, target
				.getMethodName());
		assert lookup1.getOwner() == t;
	}
}
