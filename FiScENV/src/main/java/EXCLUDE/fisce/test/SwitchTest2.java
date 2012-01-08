package EXCLUDE.fisce.test;

public class SwitchTest2 extends TestService {
	public static void main(String[] args) {
		int[] values = { -1000, 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000,
				8000, 9000 };
		for (int value : values) {
			switch (value) {
			case -1000:
				if (value != -1000) {
					fail("Illegal switch: value=" + value + " case=" + -1000);
				}
				break;
			case 0:
				if (value != 0) {
					fail("Illegal switch: value=" + value + " case=" + 0);
				}
				break;
			case 1000:
				if (value != 1000) {
					fail("Illegal switch: value=" + value + " case=" + 1000);
				}
				break;
			case 2000:
				if (value != 2000) {
					fail("Illegal switch: value=" + value + " case=" + 2000);
				}
				break;
			case 3000:
				if (value != 3000) {
					fail("Illegal switch: value=" + value + " case=" + 3000);
				}
				break;
			case 4000:
				if (value != 4000) {
					fail("Illegal switch: value=" + value + " case=" + 4000);
				}
				break;
			case 5000:
				if (value != 5000) {
					fail("Illegal switch: value=" + value + " case=" + 5000);
				}
				break;
			case 6000:
				if (value != 6000) {
					fail("Illegal switch: value=" + value + " case=" + 6000);
				}
				break;
			case 7000:
				if (value != 7000) {
					fail("Illegal switch: value=" + value + " case=" + 7000);
				}
				break;
			case 8000:
				if (value != 8000) {
					fail("Illegal switch: value=" + value + " case=" + 8000);
				}
				break;
			case 9000:
				if (value != 9000) {
					fail("Illegal switch: value=" + value + " case=" + 9000);
				}
				break;
			default:
				fail("Unexpected drop to default, value=" + value);
			}
		}
	}
}
