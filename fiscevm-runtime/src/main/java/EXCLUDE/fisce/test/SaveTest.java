package EXCLUDE.fisce.test;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Arrays;

import com.cirnoworks.fisce.privat.FiScEVM;

public class SaveTest extends Thread {

	int lastValue;
	int value;

	public static void main(String[] args) {
		new SaveTest().m();
	}

	boolean running = true;

	public void m() {
		int v = 0;

		Class<?> clazz = Integer.class;
		Constructor<?>[] constructors = clazz.getDeclaredConstructors();
		Method[] methods = clazz.getDeclaredMethods();
		Field[] fields = clazz.getDeclaredFields();
		String sClazz = clazz.getName();
		String sConstructorss = Arrays.toString(constructors);
		String sMethods = Arrays.toString(methods);
		String sFields = Arrays.toString(fields);

		start();
		while (running) {
			lastValue = value;
			v++;
			value++;
			System.out.println(lastValue + " " + v + " " + value);
		}
		System.out.println("**" + lastValue + " " + v + " " + value);
		if (value != lastValue + 1) {
			TestService.fail("Invalid value!");
		}
		System.out.println("Verifying saved reflected objects...");
		TestService.assertTrue(clazz.getName().equals(sClazz), "Class");
		TestService.assertTrue(
				Arrays.toString(constructors).equals(sConstructorss),
				"Construtors");
		TestService.assertTrue(Arrays.toString(methods).equals(sMethods),
				"Methods");
		TestService.assertTrue(Arrays.toString(fields).equals(sFields),
				"Fields");
	}

	@Override
	public void run() {
		try {
			Thread.sleep(300);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.gc();
		FiScEVM.infoOut("Saveing...");
		FiScEVM.save();
		FiScEVM.infoOut("\nSaved\n");
		running = false;
	}
}
