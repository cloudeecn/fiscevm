package EXCLUDE.fisce.test;

import java.lang.reflect.Method;
import java.util.Arrays;

import com.cirnoworks.fisce.privat.FiScEVM;

public class ReflectionTest extends RTParent implements RTInterface3,
		RTInterface4 {

	public ReflectionTest() {
		super(0);
	}

	private enum RTEnum {
		AAA, BBB, CCC;
	}

	private RTEnum e1 = RTEnum.AAA;

	public void testClass() {
		try {
			FiScEVM.debug.println(Arrays.toString(getClass().getInterfaces()));

			Class<ReflectionTest> clazz = ReflectionTest.class;
			assertTrue(clazz.getSuperclass() == RTParent.class);
			assertTrue(clazz.getName().equals(
					"EXCLUDE.fisce.test.ReflectionTest"));
			assertTrue(clazz.getSimpleName().equals("ReflectionTest"));
			FiScEVM.debug.println(clazz.getSimpleName());
			FiScEVM.debug.println("getMethods: "
					+ Arrays.toString(clazz.getMethods()));
			FiScEVM.debug.println("getDeclaredMethods: "
					+ Arrays.toString(clazz.getDeclaredMethods()));
			FiScEVM.debug.println("pud: "
					+ clazz.getMethod("pud", String.class));
			assertTrue(RTParent.class.isAssignableFrom(clazz.getMethod("pud",
					String.class).getDeclaringClass()));
			assertTrue(clazz.getMethod("wait") != null);
			assertTrue(RTInterface1.class.isInterface());
			assertTrue(RTInterface1.class.isInstance(this));
			assertTrue(!getClass().isInterface());
			assertTrue(RTEnum.class.isEnum());
			assertTrue(Integer.TYPE.isPrimitive());
			FiScEVM.debug.println("getFields"
					+ Arrays.toString(clazz.getFields()));
			FiScEVM.debug.println("getDeclaredFields"
					+ Arrays.toString(clazz.getDeclaredFields()));
			FiScEVM.debug.println("get Field k1: " + clazz.getField("k1"));
		} catch (Exception e) {
			e.printStackTrace(FiScEVM.debug);
			fail(e.toString());
		}
	}

	public void testMethod() throws Exception {
		Method m1 = getClass().getMethod("test", String.class, Integer.TYPE,
				Double.TYPE);
		Method m2 = getClass().getMethod("test", String[].class);
		this.base = "a";
		FiScEVM.debug.println(m1.toString());
		String a = (String) m1.invoke(this, "a", 2, 3);
		FiScEVM.debug.println(a);
		assertTrue(a.equals("aa23"));
	}

	public void testField() {

	}

	public static void main(String[] args) {
		try {
			ReflectionTest tester = new ReflectionTest();
			tester.testClass();
			tester.testMethod();
		} catch (Exception e) {
			e.printStackTrace(FiScEVM.debug);
			fail(e.toString());
		}
	}

	@Override
	public void pua() {

	}

	@Override
	public String pub() {
		return null;
	}

	@Override
	public String puc() {
		return null;
	}

}

interface RTInterface1 {
	String k1 = "111";

}

interface RTInterface2 {
	String k2 = "111";
	String k1 = "222";
}

interface RTInterface3 {

}

interface RTInterface4 {

}

class RTParent extends TestService implements RTInterface1, RTInterface2 {
	protected String base = "1";

	public RTParent() {

	}

	public RTParent(int a) {

	}

	public RTParent(int a, float b) {

	}

	private String pa() {
		return null;
	}

	private String pb() {
		return null;
	}

	protected String ppa() {
		return null;
	}

	public void pua() {

	}

	public Object pub() {
		return null;
	}

	public String puc() {
		return null;
	}

	public String test(String a, int b, double c) {
		return base + a + b + c;
	}

	public String test(String... a) {
		return base + Arrays.toString(a);
	}

	public Object pud(String a) {
		return a + "aaa";
	}

}