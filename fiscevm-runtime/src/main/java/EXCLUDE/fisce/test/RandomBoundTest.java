package EXCLUDE.fisce.test;

import java.util.Random;

/**
 * Test if the basic architecture of the VM works well
 * 
 * @author cloudee
 * 
 */
public class RandomBoundTest implements IF1 {

	public static void main(String[] args) {
		long seed = 1408780899893L;
		// long multip = 0x5deece66dL;
		// long result1 = multip * seed;
		// long result2 = result1 + 0x0bl;
		// long result3 = 1L << 48;
		// long result4 = result3 - 1;
		// long result5 = result2 & result4;
		// long result6 = result5 >> 46;
		// int result7 = (int) result6;
		//
		// FiScEVM.infoOut("r1=" + result1);
		// FiScEVM.infoOut("r2=" + result2);
		// FiScEVM.infoOut("r3=" + result3);
		// FiScEVM.infoOut("r4=" + result4);
		// FiScEVM.infoOut("r5=" + result5);
		// FiScEVM.infoOut("r6=" + result6);
		// FiScEVM.infoOut("r7=" + result7);

//		long sd = System.currentTimeMillis();
		Random rand = new Random(seed);
		boolean failed = false;
		for (int i = 0; i < 50000; i++) {
			int ir = rand.nextInt(4);
			if (ir < 0 | ir >= 4) {
				failed = true;
				TestService.fail("Random int result out of bound " + ir
						+ "/[0-7), seed=" + seed);
			}
			float fr = rand.nextFloat();
			if (fr < 0.0f || fr > 1.0f) {
				failed = true;
				TestService.fail("Random float result out of bound " + ir
						+ "/(0.0-1.0), seed=" + seed);
			}
			if (failed) {
				throw new RuntimeException("failed");
			}
		}
	}
}
