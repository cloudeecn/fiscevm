package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.TestStatics;
import com.cirnoworks.fisce.jvm13.VMContext;

public class ClassMethodTest extends TestCase {
	public void testGetParamCount() throws Exception {
		VMContext context = TestStatics.prepareContext(getClass().getName());
		context.initialize();
		
		ClassBase it = (ClassBase) context
				.getClass("ox/cirnoworks/test/ITester");
		ClassBase t = (ClassBase) context.getClass("ox/cirnoworks/test/Tester");
		ClassBase tc = (ClassBase) context
				.getClass("ox/cirnoworks/test/TesterChild");
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
