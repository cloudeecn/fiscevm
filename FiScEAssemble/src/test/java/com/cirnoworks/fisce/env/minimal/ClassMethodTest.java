package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassMethod;

public class ClassMethodTest extends TestCase {
	public void testGetParamCount() throws Exception {
		VMContext context = TestInitializer.getContext();
		context.bootFromClass("EXCLUDE/fisce/test/Dummy");
		context.getClass("com/cirnoworks/fisce/privat/StringUtils");
		ClassBase it = (ClassBase) context
				.getClass("EXCLUDE/fisce/test/ITester");
		ClassBase t = (ClassBase) context.getClass("EXCLUDE/fisce/test/Tester");
		ClassBase tc = (ClassBase) context
				.getClass("EXCLUDE/fisce/test/TesterChild");
		ClassMethod[] methods = t.getMethods();
		ClassMethod target = null;
		for (int i = 0, max = methods.length; i < max; i++) {
			ClassMethod method = methods[i];
			if (method.getName().equals("complex")) {
				target = method;
				break;
			}
		}
		assert target != null;
		assert target.getParamCount() == 8 : target.getParamCount();

	}
}
