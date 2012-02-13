package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class ParamStoreTest extends TestService {

	public static void storeTestStatic(int a, int b, int c) {
		int[] ret = { 1, 2, 3, 4, 5 };
		FiScEVM.storeParamsToArray(0, ret, 1, 3);
		if (ret[0] != 1) {
			fail("0s");
		}
		if (ret[1] != 9) {
			fail("1s");
		}
		if (ret[2] != 8) {
			fail("2s");
		}
		if (ret[3] != 7) {
			fail("3s");
		}
		if (ret[4] != 5) {
			fail("4s");
		}
	}

	public void storeTest(int a, int b) {
		int[] ret = { 1, 2, 3, 4, 5 };
		FiScEVM.storeParamsToArray(0, ret, 1, 3);
		if (ret[0] != 1) {
			fail("0");
		}
		if (ret[1] != System.identityHashCode(this)) {
			fail("1");
		}
		if (ret[2] != 9) {
			fail("2");
		}
		if (ret[3] != 8) {
			fail("3");
		}
		if (ret[4] != 5) {
			fail("4");
		}
	}

	public static void main(String[] args) {
		storeTestStatic(9, 8, 7);
		ParamStoreTest pst = new ParamStoreTest();
		pst.storeTest(9, 8);
	}
}
