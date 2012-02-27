package EXCLUDE.fisce.test;

public class ArrayToByteBufferTest extends TestService {
	private static native boolean test(int[] array);

	public static void main(String[] args) {
		int[] data = { 1, 2, 3, 4, 5 };
		boolean result=test(data);
		if(!result){
			fail("ArrayToByteBufferTest failed");
		}
	}

}
