package EXCLUDE.fisce.test;

class Parent {
	public final static int MESSAGE_DEAD = -1;
	public final static int MESSAGE_PASS_COMPLETED = 0;
	public final static int MESSAGE_STAGE_OVER = 1;

	protected boolean loaded;
	protected int[] message;
}

class Child extends Parent {
	protected long score;
	protected int graze;
	protected int point;
	protected String stageId;
	protected String bgmInBattle;
	protected String bgmBoss;
	protected boolean doNormalBattle = true;

	public void test() {
		message = new int[] { MESSAGE_PASS_COMPLETED };
		test(message);
		message = new int[] { MESSAGE_DEAD };
		test(message);
	}

	public void test(int[] message) {
		switch (message[0]) {
		case MESSAGE_DEAD:
			if (message[0] != MESSAGE_DEAD) {
				TestService.fail("xxx1" + message[0] + " MESSAGE_DEAD="
						+ MESSAGE_DEAD + " MESSAGE_PASS_COMPLETED="
						+ MESSAGE_PASS_COMPLETED);
			}
			break;
		case MESSAGE_PASS_COMPLETED:
			if (message[0] != MESSAGE_PASS_COMPLETED) {
				TestService.fail("xxx2" + message[0] + " MESSAGE_DEAD="
						+ MESSAGE_DEAD + " MESSAGE_PASS_COMPLETED="
						+ MESSAGE_PASS_COMPLETED);
			}
			break;
		default:
			TestService.fail("xxx3" + message[0] + " MESSAGE_DEAD="
					+ MESSAGE_DEAD + " MESSAGE_PASS_COMPLETED="
					+ MESSAGE_PASS_COMPLETED);
		}
	}
}

public class SwitchTest extends TestService {
	public static void main(String[] args) {
		int[] values = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		for (int value : values) {
			switch (value) {
			case -1:
				if (value != -1) {
					fail("Illegal switch: value=" + value + " case=" + -1);
				}
				break;
			case 0:
				if (value != 0) {
					fail("Illegal switch: value=" + value + " case=" + 0);
				}
				break;
			case 1:
				if (value != 1) {
					fail("Illegal switch: value=" + value + " case=" + 1);
				}
				break;
			case 2:
				if (value != 2) {
					fail("Illegal switch: value=" + value + " case=" + 2);
				}
				break;
			case 3:
				if (value != 3) {
					fail("Illegal switch: value=" + value + " case=" + 3);
				}
				break;
			case 4:
				if (value != 4) {
					fail("Illegal switch: value=" + value + " case=" + 4);
				}
				break;
			case 5:
				if (value != 5) {
					fail("Illegal switch: value=" + value + " case=" + 5);
				}
				break;
			case 6:
				if (value != 6) {
					fail("Illegal switch: value=" + value + " case=" + 6);
				}
				break;
			case 7:
				if (value != 7) {
					fail("Illegal switch: value=" + value + " case=" + 7);
				}
				break;
			case 8:
				if (value != 8) {
					fail("Illegal switch: value=" + value + " case=" + 8);
				}
				break;
			case 9:
				if (value != 9) {
					fail("Illegal switch: value=" + value + " case=" + 9);
				}
				break;
			default:
				fail("Unexpected drop to default, value=" + value);
			}
		}
		new Child().test();
	}
}
