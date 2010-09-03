package com.cirnoworks.fisce.env.minimal;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;

public class AbstractClassTest extends TestCase {
	public void testCanCastTo() throws Exception {

		if (true) {
			return;
		}

		VMContext context = TestInitializer.getContext();
		context.initialize();

		AbstractClass class0 = context.getClass("java/lang/Object");
		AbstractClass class1 = context.getClass("[[B");
		AbstractClass class2 = context.getClass("[[C");
		AbstractClass class3 = context.getClass("[[[B");
		AbstractClass class4 = context.getClass("[[Ljava/lang/Object;");
		AbstractClass class5 = context
				.getClass("[[Lcom/cirnoworks/fisce/test/ITester;");
		AbstractClass class6 = context
				.getClass("[[Lcom/cirnoworks/fisce/test/Tester;");
		AbstractClass class7 = context
				.getClass("[[Lcom/cirnoworks/fisce/test/TesterChild;");
		assert !class1.canCastTo(class2);
		assert !class1.canCastTo(class3);
		assert class1.canCastTo(class0);
		assert class3.canCastTo(class4);
		assert class6.canCastTo(class5);
		assert class6.canCastTo(class4);
		assert !class6.canCastTo(class3);
		assert !class6.canCastTo(class2);
	}
}
