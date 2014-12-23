package EXCLUDE.fisce.test;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Arrays;

interface Interface1 {
	void method1();
}

interface Interface2 {
	void method2();
}

class Handler implements InvocationHandler {

	@Override
	public Object invoke(Object proxy, Method method, Object[] args)
			throws Throwable {
		if (Object.class == method.getDeclaringClass()) {
			String name = method.getName();
			if ("equals".equals(name)) {
				return proxy == args[0];
			} else if ("hashCode".equals(name)) {
				return System.identityHashCode(proxy);
			} else if ("toString".equals(name)) {
				return proxy.getClass().getName() + "@"
						+ Integer.toHexString(System.identityHashCode(proxy))
						+ ", with InvocationHandler " + this;
			} else {
				throw new IllegalStateException(String.valueOf(method));
			}
		}
		System.out.println(method.toString() + " invoked for " + proxy
				+ " with arguments " + Arrays.toString(args));
		return null;
	}

}

public class ProxyTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("begin");
		try {
			Object o = Proxy
					.newProxyInstance(
							ClassLoader.getSystemClassLoader(),
							new Class<?>[] { Interface1.class, Interface2.class },
							new Handler());
			((Interface1) o).method1();
			((Interface2) o).method2();
		} catch (Throwable t) {
			t.printStackTrace();
			TestService.fail("failed");
		}
	}

}
