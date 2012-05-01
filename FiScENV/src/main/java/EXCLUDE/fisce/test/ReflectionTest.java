package EXCLUDE.fisce.test;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
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

	private RTEnum e1 = RTEnum.CCC;
	Class<ReflectionTest> clazz = ReflectionTest.class;

	public ReflectionTest(int a, float b) {
		super(a, b);
		// TODO Auto-generated constructor stub
	}

	public ReflectionTest(int a) {
		super(a);
		// TODO Auto-generated constructor stub
	}

	public void testClass() {
		try {
			// FiScEVM.debug.println(Arrays.toString(getClass().getInterfaces()));

			assertTrue(clazz.getSuperclass() == RTParent.class);
			assertTrue(clazz.getName().equals(
					"EXCLUDE.fisce.test.ReflectionTest"));
			assertTrue(clazz.getSimpleName().equals("ReflectionTest"));
			FiScEVM.debug.println(clazz.getSimpleName());

		} catch (Exception e) {
			e.printStackTrace(FiScEVM.debug);
			fail(e.toString());
		}
	}

	public void testMethod() throws Exception {
		FiScEVM.debug.println("getMethods: "
				+ Arrays.toString(clazz.getMethods()));
		FiScEVM.debug.println("getDeclaredMethods: "
				+ Arrays.toString(clazz.getDeclaredMethods()));
		FiScEVM.debug.println("pud: " + clazz.getMethod("pud", String.class));
		assertTrue(RTParent.class.isAssignableFrom(clazz.getMethod("pud",
				String.class).getDeclaringClass()));
		assertTrue(clazz.getMethod("wait") != null);
		assertTrue(RTInterface1.class.isInterface());
		assertTrue(RTInterface1.class.isInstance(this));
		assertTrue(!getClass().isInterface());
		assertTrue(RTEnum.class.isEnum());
		assertTrue(Integer.TYPE.isPrimitive());

		Method m1 = getClass().getMethod("test", String.class, Integer.TYPE,
				Double.TYPE);
		Method m2 = getClass().getMethod("test", String[].class);
		this.base = "a";
		FiScEVM.debug.println(m1.toString());
		System.out.println("This=" + System.identityHashCode(this));
		String a = (String) m1.invoke(this, "a", 2, 3.0);
		FiScEVM.debug.println(a);
		assertTrue(a.startsWith("aa23"));
		boolean thrown = false;
		a = (String) m1.invoke(this, "a", 2, 3);
		assertTrue(a.startsWith("aa23"));
	}

	public void testField() {
		try {
			FiScEVM.debug.println("getFields"
					+ Arrays.toString(clazz.getFields()));
			FiScEVM.debug.println("getDeclaredFields"
					+ Arrays.toString(clazz.getDeclaredFields()));
			FiScEVM.debug.println("get Field k1: " + clazz.getField("k1"));

			Class<RTInterface1> ci1 = RTInterface1.class;
			Class<RTInterface2> ci2 = RTInterface2.class;
			Class<RTParent> crp = RTParent.class;
			Class<ReflectionTest> crt = ReflectionTest.class;
			Field fk1 = crt.getField("k1");
			Field fk2 = crt.getField("k2");
			Field fk3 = crt.getField("k3");
			Field ffk1 = RTInterface2.class.getField("k1");
			Field fbase = crp.getDeclaredField("base2");
			Field fe1 = crt.getDeclaredField("e1");
			assertTrue(ci1.getDeclaredField("k1").equals(fk1), fk1
					+ " mismatches " + ci2.getDeclaredField("k1"));
			assertTrue(ci2.getDeclaredField("k1").equals(ffk1), ffk1
					+ " mismatches " + ci2.getDeclaredField("k1"));
			assertTrue(ci2.getDeclaredField("k2").equals(fk2), fk2
					+ " mismatches " + ci2.getDeclaredField("k2"));
			assertTrue(ci1.getDeclaredField("k3").equals(fk3), fk3
					+ " mismatches " + ci1.getDeclaredField("k3"));
			System.out.println(RTInterface2.k1);
			assertTrue(fk1.get(null).equals(111), fk1.get(null)
					+ " mismatches Integer[111]");
			assertTrue(fk3.get(null).equals("333"), fk3.get(null)
					+ " mismatches String[333]");
			assertTrue(fk2.get(null).equals(1111L), fk2.get(null)
					+ " mismatches Long[111]");
			assertTrue(ffk1.get(null).equals(2222.0), ffk1.get(null)
					+ " mismatches Double[2222]");
			assertTrue(fk1.getInt(null) == 111, fk1.get(null)
					+ " mismatches Integer[111]");
			assertTrue(fk1.getLong(null) == 111, fk1.get(null)
					+ " mismatches Integer[111]");

			fbase.set(this, 123);
			assertTrue(fbase.getInt(this) == 123);

			fbase.set(this, (short) 123);
			assertTrue(fbase.getLong(this) == 123);

			fbase.setShort(this, (short) 123);
			assertTrue(fbase.getLong(this) == 123);

		} catch (Exception e) {
			e.printStackTrace(FiScEVM.debug);
			fail("Exception thrown:" + e.toString());
		}
	}

	public void testArray() {
		int[][][] ii = (int[][][]) Array.newInstance(int.class, new int[] { 2,
				3, 5 });
		assertTrue(ii.length == 2);
		assertTrue(ii[1].length == 3);
		assertTrue(ii[1][2].length == 5);
	}

	public void testConstructor() {
		try {
			ReflectionTest rt = clazz.newInstance();
			assertTrue(rt.e1 == RTEnum.CCC);
			FiScEVM.debug.println("getConstructors"
					+ Arrays.toString(clazz.getConstructors()));

			rt = clazz.getConstructor(int.class, float.class).newInstance(200,
					200.0f);
			assertTrue(rt.e1 == RTEnum.CCC);
			assertTrue(rt.a == 200);
			assertTrue(rt.b == 200.0f);

		} catch (Exception e) {
			e.printStackTrace(FiScEVM.debug);
			fail("Exception thrown:" + e.toString());
		}
	}

	public static void main(String[] args) {
		try {
			// FiScEVM.debug = System.err;
			// TestService.nativeFail = false;
			ReflectionTest tester = new ReflectionTest();
			tester.testClass();
			tester.testMethod();
			tester.testField();
			tester.testArray();
			tester.testConstructor();
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
	int k1 = 111;
	String k3 = "333";

}

interface RTInterface2 extends RTInterface1 {
	long k2 = 1111;
	double k1 = 2222;
}

interface RTInterface3 {

}

interface RTInterface4 {

}

class Temp {
	static int c = 5;
	static final int d = 8;
	static final String e = "aaa";
	static final Object f = System.out;
}

class RTParent extends TestService implements RTInterface1, RTInterface2 {
	protected String base = "1";
	protected int base2 = 1;
	protected int a = 0;
	protected float b = 0;

	public RTParent() {

	}

	public RTParent(int a) {
		this.a = a;
	}

	public RTParent(int a, float b) {
		this.a = a;
		this.b = b;
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