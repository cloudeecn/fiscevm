package com.cirnoworks.fisce.jvm13.data;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.TestStatics;
import com.cirnoworks.fisce.jvm13.VMContext;

public class AbstractClassTest extends TestCase {
	public void testCanCastTo() throws Exception {

		VMContext context = TestStatics.prepareContext(getClass().getName());
		context.initialize();
		

		AbstractClass class0 = context.getClass("java/lang/Object");
		AbstractClass class1 = context.getClass("[[B");
		AbstractClass class2 = context.getClass("[[C");
		AbstractClass class3 = context.getClass("[[[B");
		AbstractClass class4 = context.getClass("[[Ljava/lang/Object;");
		AbstractClass class5 = context
				.getClass("[[Lox/cirnoworks/test/ITester;");
		AbstractClass class6 = context
				.getClass("[[Lox/cirnoworks/test/Tester;");
		AbstractClass class7 = context
				.getClass("[[Lox/cirnoworks/test/TesterChild;");
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
