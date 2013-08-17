package EXCLUDE.fisce.test;

public class HelloWorld {
	public static void main(String[] args) {
		try {
			throw new RuntimeException();
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println("Hello world!");
	}
}
