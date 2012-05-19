package EXCLUDE.fisce.test;

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
