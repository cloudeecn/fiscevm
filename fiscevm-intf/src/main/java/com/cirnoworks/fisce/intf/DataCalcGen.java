package com.cirnoworks.fisce.intf;

import java.io.PrintStream;

public class DataCalcGen {

	private static long[] longData = { 1l, -1l, 0x11l, -0x11l, 0xF000F000l,
			0x0F000F00l, 0xF000F000F000F000l, 0x0F000F000F000F00l };
	private static int[] intData = { 1, -1, 0x11, -0x11, 0xF000F000, 0x0F000F00 };

	public static void main(String[] args) {
		// long
		PrintStream ps = System.out;
		int max = longData.length;
		for (int i = 0; i < max; i++) {
			ps.println("assertEqual(\"" + ((int) longData[i])
					+ "\", String.valueOf((int)longData[" + i + "]), \"l2i "
					+ longData[i] + "l\");");
			ps.println("assertEqual(\"" + ((float) longData[i])
					+ "\", String.valueOf((float)longData[" + i + "]), \"l2f "
					+ longData[i] + "l\");");
			ps.println("assertEqual(\"" + ((double) longData[i])
					+ "\", String.valueOf((double)longData[" + i + "]), \"l2d "
					+ longData[i] + "l\");");
			ps.println("assertEqual(\"" + (-longData[i])
					+ "\", String.valueOf(-longData[" + i + "]), \"lneg "
					+ longData[i] + "l\");");
			for (int j = 0; j < 64; j++) {
				ps.println("assertEqual(\"" + (longData[i] >> j)
						+ "\", String.valueOf(longData[" + i + "] >> " + j
						+ "), \"" + longData[i] + "l >> " + j + "\"); ");
				ps.println("assertEqual(\"" + (longData[i] >>> j)
						+ "\", String.valueOf(longData[" + i + "] >>> " + j
						+ "), \"" + longData[i] + "l >>> " + j + "\"); ");
				ps.println("assertEqual(\"" + (longData[i] << j)
						+ "\", String.valueOf(longData[" + i + "] << " + j
						+ "), \"" + longData[i] + "l << " + j + "\"); ");
			}
			for (int j = 0; j < max; j++) {
				ps.println("assertEqual(\"" + (longData[i] + longData[j])
						+ "\", String.valueOf(longData[" + i + "] + longData["
						+ j + "]), \"" + longData[i] + "l + " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] - longData[j])
						+ "\", String.valueOf(longData[" + i + "] - longData["
						+ j + "]), \"" + longData[i] + "l - " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] * longData[j])
						+ "\", String.valueOf(longData[" + i + "] * longData["
						+ j + "]), \"" + longData[i] + "l * " + longData[j]
						+ "l\"); ");
				if (longData[j] != 0) {
					ps.println("assertEqual(\"" + (longData[i] / longData[j])
							+ "\", String.valueOf(longData[" + i
							+ "] / longData[" + j + "]), \"" + longData[i]
							+ "l / " + longData[j] + "l\"); ");
					ps.println("assertEqual(\"" + (longData[i] % longData[j])
							+ "\", String.valueOf(longData[" + i
							+ "] % longData[" + j + "]), \"" + longData[i]
							+ "l % " + longData[j] + "l\"); ");
				}
				ps.println("assertEqual(\"" + (longData[i] & longData[j])
						+ "\", String.valueOf(longData[" + i + "] & longData["
						+ j + "]), \"" + longData[i] + "l & " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] | longData[j])
						+ "\", String.valueOf(longData[" + i + "] | longData["
						+ j + "]), \"" + longData[i] + "l | " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] ^ longData[j])
						+ "\", String.valueOf(longData[" + i + "] ^ longData["
						+ j + "]), \"" + longData[i] + "l ^ " + longData[j]
						+ "l\"); ");

				ps.println("assertEqual(\"" + (longData[i] > longData[j])
						+ "\", String.valueOf(longData[" + i + "] > longData["
						+ j + "]), \"" + longData[i] + "l > " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] < longData[j])
						+ "\", String.valueOf(longData[" + i + "] < longData["
						+ j + "]), \"" + longData[i] + "l < " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] == longData[j])
						+ "\", String.valueOf(longData[" + i + "] == longData["
						+ j + "]), \"" + longData[i] + "l == " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] >= longData[j])
						+ "\", String.valueOf(longData[" + i + "] >= longData["
						+ j + "]), \"" + longData[i] + "l >= " + longData[j]
						+ "l\"); ");
				ps.println("assertEqual(\"" + (longData[i] <= longData[j])
						+ "\", String.valueOf(longData[" + i + "] <= longData["
						+ j + "]), \"" + longData[i] + "l <= " + longData[j]
						+ "l\"); ");

			}
		}
		
		ps.println();
		ps.println();
		ps.println();
		ps.println();

		max = intData.length;
		for (int i = 0; i < max; i++) {
			ps.println("assertEqual(\"" + ((long) intData[i])
					+ "\", String.valueOf((long)intData[" + i + "]), \"i2l "
					+ intData[i] + "\");");
			ps.println("assertEqual(\"" + ((float) intData[i])
					+ "\", String.valueOf((float)intData[" + i + "]), \"i2f "
					+ intData[i] + "\");");
			ps.println("assertEqual(\"" + ((double) intData[i])
					+ "\", String.valueOf((double)intData[" + i + "]), \"i2d "
					+ intData[i] + "\");");
			ps.println("assertEqual(\"" + ((byte) intData[i])
					+ "\", String.valueOf((byte)intData[" + i + "]), \"i2b "
					+ intData[i] + "\");");
			ps.println("assertEqual(\"" + ((short) intData[i])
					+ "\", String.valueOf((short)intData[" + i + "]), \"i2s "
					+ intData[i] + "\");");
			ps.println("assertEqual(\"" + (-intData[i])
					+ "\", String.valueOf(-intData[" + i + "]), \"ineg "
					+ intData[i] + "\");");
			for (int j = 0; j < 32; j++) {
				ps.println("assertEqual(\"" + (intData[i] >> j)
						+ "\", String.valueOf(intData[" + i + "] >> " + j
						+ "), \"" + intData[i] + " >> " + j + "\"); ");
				ps.println("assertEqual(\"" + (intData[i] >>> j)
						+ "\", String.valueOf(intData[" + i + "] >>> " + j
						+ "), \"" + intData[i] + " >>> " + j + "\"); ");
				ps.println("assertEqual(\"" + (intData[i] << j)
						+ "\", String.valueOf(intData[" + i + "] << " + j
						+ "), \"" + intData[i] + " << " + j + "\"); ");
			}
			for (int j = 0; j < max; j++) {
				ps.println("assertEqual(\"" + (intData[i] + intData[j])
						+ "\", String.valueOf(intData[" + i + "] + intData["
						+ j + "]), \"" + intData[i] + " + " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] - intData[j])
						+ "\", String.valueOf(intData[" + i + "] - intData["
						+ j + "]), \"" + intData[i] + " - " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] * intData[j])
						+ "\", String.valueOf(intData[" + i + "] * intData["
						+ j + "]), \"" + intData[i] + " * " + intData[j]
						+ "\"); ");
				if (intData[j] != 0) {
					ps.println("assertEqual(\"" + (intData[i] / intData[j])
							+ "\", String.valueOf(intData[" + i
							+ "] / intData[" + j + "]), \"" + intData[i]
							+ " / " + intData[j] + "\"); ");
					ps.println("assertEqual(\"" + (intData[i] % intData[j])
							+ "\", String.valueOf(intData[" + i
							+ "] % intData[" + j + "]), \"" + intData[i]
							+ " % " + intData[j] + "\"); ");
				}
				ps.println("assertEqual(\"" + (intData[i] & intData[j])
						+ "\", String.valueOf(intData[" + i + "] & intData["
						+ j + "]), \"" + intData[i] + " & " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] | intData[j])
						+ "\", String.valueOf(intData[" + i + "] | intData["
						+ j + "]), \"" + intData[i] + " | " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] ^ intData[j])
						+ "\", String.valueOf(intData[" + i + "] ^ intData["
						+ j + "]), \"" + intData[i] + " ^ " + intData[j]
						+ "\"); ");

				ps.println("assertEqual(\"" + (intData[i] > intData[j])
						+ "\", String.valueOf(intData[" + i + "] > intData["
						+ j + "]), \"" + intData[i] + " > " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] < intData[j])
						+ "\", String.valueOf(intData[" + i + "] < intData["
						+ j + "]), \"" + intData[i] + " < " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] == intData[j])
						+ "\", String.valueOf(intData[" + i + "] == intData["
						+ j + "]), \"" + intData[i] + " == " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] >= intData[j])
						+ "\", String.valueOf(intData[" + i + "] >= intData["
						+ j + "]), \"" + intData[i] + " >= " + intData[j]
						+ "\"); ");
				ps.println("assertEqual(\"" + (intData[i] <= intData[j])
						+ "\", String.valueOf(intData[" + i + "] <= intData["
						+ j + "]), \"" + intData[i] + " <= " + intData[j]
						+ "\"); ");

			}
		}
	}

}
