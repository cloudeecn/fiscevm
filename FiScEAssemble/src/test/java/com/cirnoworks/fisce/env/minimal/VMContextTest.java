package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassMethod;

public class VMContextTest extends TestCase {
	public void testLookupMethodInterface() throws Exception {
		VMContext context = TestInitializer.getContext();
		context.bootFromClass("com/cirnoworks/fisce/test/Dummy");

		ClassBase it = (ClassBase) context
				.getClass("com/cirnoworks/fisce/test/ITester");
		ClassBase t = (ClassBase) context
				.getClass("com/cirnoworks/fisce/test/Tester");
		ClassBase tc = (ClassBase) context
				.getClass("com/cirnoworks/fisce/test/TesterChild");
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
		ClassMethod lookup = context.lookupMethodVirtual(t,
				target.getMethodName());
		assert lookup.getOwner() == t;
		ClassMethod lookup1 = context.lookupMethodVirtual(tc,
				target.getMethodName());
		assert lookup1.getOwner() == t;
	}
}
