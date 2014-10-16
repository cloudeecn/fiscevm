package java.lang;

public class ClassLoaderHelper {

	public static Class<?> defineClass(ClassLoader loader, String className,
			byte[] classRep, int offset, int length) {
		return loader.defineClass(className, classRep, offset, length);
	}
}
