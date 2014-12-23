package EXCLUDE.fisce.test;

import java.util.ArrayList;
import java.util.HashMap;

import com.cirnoworks.fisce.privat.FiScEVM;

interface IF1 {
	final char[] c = { 'a', 'b', 'c' };
}

interface IF2 {
	final char[] c = { 'a', 'd', 'e' };
}

class P {
	final char[] c = { 'a', 'f', 'g' };
}

/**
 * Test if the basic architecture of the VM works well
 * 
 * @author cloudee
 * 
 */
public class ArchitectureTest extends TestService implements IF1 {
	private final char[] chars = { '<', 'i', 'n', 'i', 't', '>' };
	private static long[] longData = { 1l, -1l, 0x11l, -0x11l, 0xF000F000l,
			0x0F000F00l, 0xF000F000F000F000l, 0x0F000F000F000F00l };
	private static int[] intData = { 1, -1, 0x11, -0x11, 0xF000F000, 0x0F000F00 };

	public void run() {
		System.out.println(new String(c));
	}

	public static void strictTestLong() {
		assertEqual("1", String.valueOf((int) longData[0]), "l2i 1l");
		assertEqual("1.0", String.valueOf((float) longData[0]), "l2f 1l");
		assertEqual("1.0", String.valueOf((double) longData[0]), "l2d 1l");
		assertEqual("-1", String.valueOf(-longData[0]), "lneg 1l");
		assertEqual("1", String.valueOf(longData[0] >> 0), "1l >> 0");
		assertEqual("1", String.valueOf(longData[0] >>> 0), "1l >>> 0");
		assertEqual("1", String.valueOf(longData[0] << 0), "1l << 0");
		assertEqual("0", String.valueOf(longData[0] >> 1), "1l >> 1");
		assertEqual("0", String.valueOf(longData[0] >>> 1), "1l >>> 1");
		assertEqual("2", String.valueOf(longData[0] << 1), "1l << 1");
		assertEqual("0", String.valueOf(longData[0] >> 2), "1l >> 2");
		assertEqual("0", String.valueOf(longData[0] >>> 2), "1l >>> 2");
		assertEqual("4", String.valueOf(longData[0] << 2), "1l << 2");
		assertEqual("0", String.valueOf(longData[0] >> 3), "1l >> 3");
		assertEqual("0", String.valueOf(longData[0] >>> 3), "1l >>> 3");
		assertEqual("8", String.valueOf(longData[0] << 3), "1l << 3");
		assertEqual("0", String.valueOf(longData[0] >> 4), "1l >> 4");
		assertEqual("0", String.valueOf(longData[0] >>> 4), "1l >>> 4");
		assertEqual("16", String.valueOf(longData[0] << 4), "1l << 4");
		assertEqual("0", String.valueOf(longData[0] >> 5), "1l >> 5");
		assertEqual("0", String.valueOf(longData[0] >>> 5), "1l >>> 5");
		assertEqual("32", String.valueOf(longData[0] << 5), "1l << 5");
		assertEqual("0", String.valueOf(longData[0] >> 6), "1l >> 6");
		assertEqual("0", String.valueOf(longData[0] >>> 6), "1l >>> 6");
		assertEqual("64", String.valueOf(longData[0] << 6), "1l << 6");
		assertEqual("0", String.valueOf(longData[0] >> 7), "1l >> 7");
		assertEqual("0", String.valueOf(longData[0] >>> 7), "1l >>> 7");
		assertEqual("128", String.valueOf(longData[0] << 7), "1l << 7");
		assertEqual("0", String.valueOf(longData[0] >> 8), "1l >> 8");
		assertEqual("0", String.valueOf(longData[0] >>> 8), "1l >>> 8");
		assertEqual("256", String.valueOf(longData[0] << 8), "1l << 8");
		assertEqual("0", String.valueOf(longData[0] >> 9), "1l >> 9");
		assertEqual("0", String.valueOf(longData[0] >>> 9), "1l >>> 9");
		assertEqual("512", String.valueOf(longData[0] << 9), "1l << 9");
		assertEqual("0", String.valueOf(longData[0] >> 10), "1l >> 10");
		assertEqual("0", String.valueOf(longData[0] >>> 10), "1l >>> 10");
		assertEqual("1024", String.valueOf(longData[0] << 10), "1l << 10");
		assertEqual("0", String.valueOf(longData[0] >> 11), "1l >> 11");
		assertEqual("0", String.valueOf(longData[0] >>> 11), "1l >>> 11");
		assertEqual("2048", String.valueOf(longData[0] << 11), "1l << 11");
		assertEqual("0", String.valueOf(longData[0] >> 12), "1l >> 12");
		assertEqual("0", String.valueOf(longData[0] >>> 12), "1l >>> 12");
		assertEqual("4096", String.valueOf(longData[0] << 12), "1l << 12");
		assertEqual("0", String.valueOf(longData[0] >> 13), "1l >> 13");
		assertEqual("0", String.valueOf(longData[0] >>> 13), "1l >>> 13");
		assertEqual("8192", String.valueOf(longData[0] << 13), "1l << 13");
		assertEqual("0", String.valueOf(longData[0] >> 14), "1l >> 14");
		assertEqual("0", String.valueOf(longData[0] >>> 14), "1l >>> 14");
		assertEqual("16384", String.valueOf(longData[0] << 14), "1l << 14");
		assertEqual("0", String.valueOf(longData[0] >> 15), "1l >> 15");
		assertEqual("0", String.valueOf(longData[0] >>> 15), "1l >>> 15");
		assertEqual("32768", String.valueOf(longData[0] << 15), "1l << 15");
		assertEqual("0", String.valueOf(longData[0] >> 16), "1l >> 16");
		assertEqual("0", String.valueOf(longData[0] >>> 16), "1l >>> 16");
		assertEqual("65536", String.valueOf(longData[0] << 16), "1l << 16");
		assertEqual("0", String.valueOf(longData[0] >> 17), "1l >> 17");
		assertEqual("0", String.valueOf(longData[0] >>> 17), "1l >>> 17");
		assertEqual("131072", String.valueOf(longData[0] << 17), "1l << 17");
		assertEqual("0", String.valueOf(longData[0] >> 18), "1l >> 18");
		assertEqual("0", String.valueOf(longData[0] >>> 18), "1l >>> 18");
		assertEqual("262144", String.valueOf(longData[0] << 18), "1l << 18");
		assertEqual("0", String.valueOf(longData[0] >> 19), "1l >> 19");
		assertEqual("0", String.valueOf(longData[0] >>> 19), "1l >>> 19");
		assertEqual("524288", String.valueOf(longData[0] << 19), "1l << 19");
		assertEqual("0", String.valueOf(longData[0] >> 20), "1l >> 20");
		assertEqual("0", String.valueOf(longData[0] >>> 20), "1l >>> 20");
		assertEqual("1048576", String.valueOf(longData[0] << 20), "1l << 20");
		assertEqual("0", String.valueOf(longData[0] >> 21), "1l >> 21");
		assertEqual("0", String.valueOf(longData[0] >>> 21), "1l >>> 21");
		assertEqual("2097152", String.valueOf(longData[0] << 21), "1l << 21");
		assertEqual("0", String.valueOf(longData[0] >> 22), "1l >> 22");
		assertEqual("0", String.valueOf(longData[0] >>> 22), "1l >>> 22");
		assertEqual("4194304", String.valueOf(longData[0] << 22), "1l << 22");
		assertEqual("0", String.valueOf(longData[0] >> 23), "1l >> 23");
		assertEqual("0", String.valueOf(longData[0] >>> 23), "1l >>> 23");
		assertEqual("8388608", String.valueOf(longData[0] << 23), "1l << 23");
		assertEqual("0", String.valueOf(longData[0] >> 24), "1l >> 24");
		assertEqual("0", String.valueOf(longData[0] >>> 24), "1l >>> 24");
		assertEqual("16777216", String.valueOf(longData[0] << 24), "1l << 24");
		assertEqual("0", String.valueOf(longData[0] >> 25), "1l >> 25");
		assertEqual("0", String.valueOf(longData[0] >>> 25), "1l >>> 25");
		assertEqual("33554432", String.valueOf(longData[0] << 25), "1l << 25");
		assertEqual("0", String.valueOf(longData[0] >> 26), "1l >> 26");
		assertEqual("0", String.valueOf(longData[0] >>> 26), "1l >>> 26");
		assertEqual("67108864", String.valueOf(longData[0] << 26), "1l << 26");
		assertEqual("0", String.valueOf(longData[0] >> 27), "1l >> 27");
		assertEqual("0", String.valueOf(longData[0] >>> 27), "1l >>> 27");
		assertEqual("134217728", String.valueOf(longData[0] << 27), "1l << 27");
		assertEqual("0", String.valueOf(longData[0] >> 28), "1l >> 28");
		assertEqual("0", String.valueOf(longData[0] >>> 28), "1l >>> 28");
		assertEqual("268435456", String.valueOf(longData[0] << 28), "1l << 28");
		assertEqual("0", String.valueOf(longData[0] >> 29), "1l >> 29");
		assertEqual("0", String.valueOf(longData[0] >>> 29), "1l >>> 29");
		assertEqual("536870912", String.valueOf(longData[0] << 29), "1l << 29");
		assertEqual("0", String.valueOf(longData[0] >> 30), "1l >> 30");
		assertEqual("0", String.valueOf(longData[0] >>> 30), "1l >>> 30");
		assertEqual("1073741824", String.valueOf(longData[0] << 30), "1l << 30");
		assertEqual("0", String.valueOf(longData[0] >> 31), "1l >> 31");
		assertEqual("0", String.valueOf(longData[0] >>> 31), "1l >>> 31");
		assertEqual("2147483648", String.valueOf(longData[0] << 31), "1l << 31");
		assertEqual("0", String.valueOf(longData[0] >> 32), "1l >> 32");
		assertEqual("0", String.valueOf(longData[0] >>> 32), "1l >>> 32");
		assertEqual("4294967296", String.valueOf(longData[0] << 32), "1l << 32");
		assertEqual("0", String.valueOf(longData[0] >> 33), "1l >> 33");
		assertEqual("0", String.valueOf(longData[0] >>> 33), "1l >>> 33");
		assertEqual("8589934592", String.valueOf(longData[0] << 33), "1l << 33");
		assertEqual("0", String.valueOf(longData[0] >> 34), "1l >> 34");
		assertEqual("0", String.valueOf(longData[0] >>> 34), "1l >>> 34");
		assertEqual("17179869184", String.valueOf(longData[0] << 34),
				"1l << 34");
		assertEqual("0", String.valueOf(longData[0] >> 35), "1l >> 35");
		assertEqual("0", String.valueOf(longData[0] >>> 35), "1l >>> 35");
		assertEqual("34359738368", String.valueOf(longData[0] << 35),
				"1l << 35");
		assertEqual("0", String.valueOf(longData[0] >> 36), "1l >> 36");
		assertEqual("0", String.valueOf(longData[0] >>> 36), "1l >>> 36");
		assertEqual("68719476736", String.valueOf(longData[0] << 36),
				"1l << 36");
		assertEqual("0", String.valueOf(longData[0] >> 37), "1l >> 37");
		assertEqual("0", String.valueOf(longData[0] >>> 37), "1l >>> 37");
		assertEqual("137438953472", String.valueOf(longData[0] << 37),
				"1l << 37");
		assertEqual("0", String.valueOf(longData[0] >> 38), "1l >> 38");
		assertEqual("0", String.valueOf(longData[0] >>> 38), "1l >>> 38");
		assertEqual("274877906944", String.valueOf(longData[0] << 38),
				"1l << 38");
		assertEqual("0", String.valueOf(longData[0] >> 39), "1l >> 39");
		assertEqual("0", String.valueOf(longData[0] >>> 39), "1l >>> 39");
		assertEqual("549755813888", String.valueOf(longData[0] << 39),
				"1l << 39");
		assertEqual("0", String.valueOf(longData[0] >> 40), "1l >> 40");
		assertEqual("0", String.valueOf(longData[0] >>> 40), "1l >>> 40");
		assertEqual("1099511627776", String.valueOf(longData[0] << 40),
				"1l << 40");
		assertEqual("0", String.valueOf(longData[0] >> 41), "1l >> 41");
		assertEqual("0", String.valueOf(longData[0] >>> 41), "1l >>> 41");
		assertEqual("2199023255552", String.valueOf(longData[0] << 41),
				"1l << 41");
		assertEqual("0", String.valueOf(longData[0] >> 42), "1l >> 42");
		assertEqual("0", String.valueOf(longData[0] >>> 42), "1l >>> 42");
		assertEqual("4398046511104", String.valueOf(longData[0] << 42),
				"1l << 42");
		assertEqual("0", String.valueOf(longData[0] >> 43), "1l >> 43");
		assertEqual("0", String.valueOf(longData[0] >>> 43), "1l >>> 43");
		assertEqual("8796093022208", String.valueOf(longData[0] << 43),
				"1l << 43");
		assertEqual("0", String.valueOf(longData[0] >> 44), "1l >> 44");
		assertEqual("0", String.valueOf(longData[0] >>> 44), "1l >>> 44");
		assertEqual("17592186044416", String.valueOf(longData[0] << 44),
				"1l << 44");
		assertEqual("0", String.valueOf(longData[0] >> 45), "1l >> 45");
		assertEqual("0", String.valueOf(longData[0] >>> 45), "1l >>> 45");
		assertEqual("35184372088832", String.valueOf(longData[0] << 45),
				"1l << 45");
		assertEqual("0", String.valueOf(longData[0] >> 46), "1l >> 46");
		assertEqual("0", String.valueOf(longData[0] >>> 46), "1l >>> 46");
		assertEqual("70368744177664", String.valueOf(longData[0] << 46),
				"1l << 46");
		assertEqual("0", String.valueOf(longData[0] >> 47), "1l >> 47");
		assertEqual("0", String.valueOf(longData[0] >>> 47), "1l >>> 47");
		assertEqual("140737488355328", String.valueOf(longData[0] << 47),
				"1l << 47");
		assertEqual("0", String.valueOf(longData[0] >> 48), "1l >> 48");
		assertEqual("0", String.valueOf(longData[0] >>> 48), "1l >>> 48");
		assertEqual("281474976710656", String.valueOf(longData[0] << 48),
				"1l << 48");
		assertEqual("0", String.valueOf(longData[0] >> 49), "1l >> 49");
		assertEqual("0", String.valueOf(longData[0] >>> 49), "1l >>> 49");
		assertEqual("562949953421312", String.valueOf(longData[0] << 49),
				"1l << 49");
		assertEqual("0", String.valueOf(longData[0] >> 50), "1l >> 50");
		assertEqual("0", String.valueOf(longData[0] >>> 50), "1l >>> 50");
		assertEqual("1125899906842624", String.valueOf(longData[0] << 50),
				"1l << 50");
		assertEqual("0", String.valueOf(longData[0] >> 51), "1l >> 51");
		assertEqual("0", String.valueOf(longData[0] >>> 51), "1l >>> 51");
		assertEqual("2251799813685248", String.valueOf(longData[0] << 51),
				"1l << 51");
		assertEqual("0", String.valueOf(longData[0] >> 52), "1l >> 52");
		assertEqual("0", String.valueOf(longData[0] >>> 52), "1l >>> 52");
		assertEqual("4503599627370496", String.valueOf(longData[0] << 52),
				"1l << 52");
		assertEqual("0", String.valueOf(longData[0] >> 53), "1l >> 53");
		assertEqual("0", String.valueOf(longData[0] >>> 53), "1l >>> 53");
		assertEqual("9007199254740992", String.valueOf(longData[0] << 53),
				"1l << 53");
		assertEqual("0", String.valueOf(longData[0] >> 54), "1l >> 54");
		assertEqual("0", String.valueOf(longData[0] >>> 54), "1l >>> 54");
		assertEqual("18014398509481984", String.valueOf(longData[0] << 54),
				"1l << 54");
		assertEqual("0", String.valueOf(longData[0] >> 55), "1l >> 55");
		assertEqual("0", String.valueOf(longData[0] >>> 55), "1l >>> 55");
		assertEqual("36028797018963968", String.valueOf(longData[0] << 55),
				"1l << 55");
		assertEqual("0", String.valueOf(longData[0] >> 56), "1l >> 56");
		assertEqual("0", String.valueOf(longData[0] >>> 56), "1l >>> 56");
		assertEqual("72057594037927936", String.valueOf(longData[0] << 56),
				"1l << 56");
		assertEqual("0", String.valueOf(longData[0] >> 57), "1l >> 57");
		assertEqual("0", String.valueOf(longData[0] >>> 57), "1l >>> 57");
		assertEqual("144115188075855872", String.valueOf(longData[0] << 57),
				"1l << 57");
		assertEqual("0", String.valueOf(longData[0] >> 58), "1l >> 58");
		assertEqual("0", String.valueOf(longData[0] >>> 58), "1l >>> 58");
		assertEqual("288230376151711744", String.valueOf(longData[0] << 58),
				"1l << 58");
		assertEqual("0", String.valueOf(longData[0] >> 59), "1l >> 59");
		assertEqual("0", String.valueOf(longData[0] >>> 59), "1l >>> 59");
		assertEqual("576460752303423488", String.valueOf(longData[0] << 59),
				"1l << 59");
		assertEqual("0", String.valueOf(longData[0] >> 60), "1l >> 60");
		assertEqual("0", String.valueOf(longData[0] >>> 60), "1l >>> 60");
		assertEqual("1152921504606846976", String.valueOf(longData[0] << 60),
				"1l << 60");
		assertEqual("0", String.valueOf(longData[0] >> 61), "1l >> 61");
		assertEqual("0", String.valueOf(longData[0] >>> 61), "1l >>> 61");
		assertEqual("2305843009213693952", String.valueOf(longData[0] << 61),
				"1l << 61");
		assertEqual("0", String.valueOf(longData[0] >> 62), "1l >> 62");
		assertEqual("0", String.valueOf(longData[0] >>> 62), "1l >>> 62");
		assertEqual("4611686018427387904", String.valueOf(longData[0] << 62),
				"1l << 62");
		assertEqual("0", String.valueOf(longData[0] >> 63), "1l >> 63");
		assertEqual("0", String.valueOf(longData[0] >>> 63), "1l >>> 63");
		assertEqual("-9223372036854775808", String.valueOf(longData[0] << 63),
				"1l << 63");
		assertEqual("2", String.valueOf(longData[0] + longData[0]), "1l + 1l");
		assertEqual("0", String.valueOf(longData[0] - longData[0]), "1l - 1l");
		assertEqual("1", String.valueOf(longData[0] * longData[0]), "1l * 1l");
		assertEqual("1", String.valueOf(longData[0] / longData[0]), "1l / 1l");
		assertEqual("0", String.valueOf(longData[0] % longData[0]), "1l % 1l");
		assertEqual("1", String.valueOf(longData[0] & longData[0]), "1l & 1l");
		assertEqual("1", String.valueOf(longData[0] | longData[0]), "1l | 1l");
		assertEqual("0", String.valueOf(longData[0] ^ longData[0]), "1l ^ 1l");
		assertEqual("false", String.valueOf(longData[0] > longData[0]),
				"1l > 1l");
		assertEqual("false", String.valueOf(longData[0] < longData[0]),
				"1l < 1l");
		assertEqual("true", String.valueOf(longData[0] == longData[0]),
				"1l == 1l");
		assertEqual("true", String.valueOf(longData[0] >= longData[0]),
				"1l >= 1l");
		assertEqual("true", String.valueOf(longData[0] <= longData[0]),
				"1l <= 1l");
		assertEqual("0", String.valueOf(longData[0] + longData[1]), "1l + -1l");
		assertEqual("2", String.valueOf(longData[0] - longData[1]), "1l - -1l");
		assertEqual("-1", String.valueOf(longData[0] * longData[1]), "1l * -1l");
		assertEqual("-1", String.valueOf(longData[0] / longData[1]), "1l / -1l");
		assertEqual("0", String.valueOf(longData[0] % longData[1]), "1l % -1l");
		assertEqual("1", String.valueOf(longData[0] & longData[1]), "1l & -1l");
		assertEqual("-1", String.valueOf(longData[0] | longData[1]), "1l | -1l");
		assertEqual("-2", String.valueOf(longData[0] ^ longData[1]), "1l ^ -1l");
		assertEqual("true", String.valueOf(longData[0] > longData[1]),
				"1l > -1l");
		assertEqual("false", String.valueOf(longData[0] < longData[1]),
				"1l < -1l");
		assertEqual("false", String.valueOf(longData[0] == longData[1]),
				"1l == -1l");
		assertEqual("true", String.valueOf(longData[0] >= longData[1]),
				"1l >= -1l");
		assertEqual("false", String.valueOf(longData[0] <= longData[1]),
				"1l <= -1l");
		assertEqual("18", String.valueOf(longData[0] + longData[2]), "1l + 17l");
		assertEqual("-16", String.valueOf(longData[0] - longData[2]),
				"1l - 17l");
		assertEqual("17", String.valueOf(longData[0] * longData[2]), "1l * 17l");
		assertEqual("0", String.valueOf(longData[0] / longData[2]), "1l / 17l");
		assertEqual("1", String.valueOf(longData[0] % longData[2]), "1l % 17l");
		assertEqual("1", String.valueOf(longData[0] & longData[2]), "1l & 17l");
		assertEqual("17", String.valueOf(longData[0] | longData[2]), "1l | 17l");
		assertEqual("16", String.valueOf(longData[0] ^ longData[2]), "1l ^ 17l");
		assertEqual("false", String.valueOf(longData[0] > longData[2]),
				"1l > 17l");
		assertEqual("true", String.valueOf(longData[0] < longData[2]),
				"1l < 17l");
		assertEqual("false", String.valueOf(longData[0] == longData[2]),
				"1l == 17l");
		assertEqual("false", String.valueOf(longData[0] >= longData[2]),
				"1l >= 17l");
		assertEqual("true", String.valueOf(longData[0] <= longData[2]),
				"1l <= 17l");
		assertEqual("-16", String.valueOf(longData[0] + longData[3]),
				"1l + -17l");
		assertEqual("18", String.valueOf(longData[0] - longData[3]),
				"1l - -17l");
		assertEqual("-17", String.valueOf(longData[0] * longData[3]),
				"1l * -17l");
		assertEqual("0", String.valueOf(longData[0] / longData[3]), "1l / -17l");
		assertEqual("1", String.valueOf(longData[0] % longData[3]), "1l % -17l");
		assertEqual("1", String.valueOf(longData[0] & longData[3]), "1l & -17l");
		assertEqual("-17", String.valueOf(longData[0] | longData[3]),
				"1l | -17l");
		assertEqual("-18", String.valueOf(longData[0] ^ longData[3]),
				"1l ^ -17l");
		assertEqual("true", String.valueOf(longData[0] > longData[3]),
				"1l > -17l");
		assertEqual("false", String.valueOf(longData[0] < longData[3]),
				"1l < -17l");
		assertEqual("false", String.valueOf(longData[0] == longData[3]),
				"1l == -17l");
		assertEqual("true", String.valueOf(longData[0] >= longData[3]),
				"1l >= -17l");
		assertEqual("false", String.valueOf(longData[0] <= longData[3]),
				"1l <= -17l");
		assertEqual("4026593281", String.valueOf(longData[0] + longData[4]),
				"1l + 4026593280l");
		assertEqual("-4026593279", String.valueOf(longData[0] - longData[4]),
				"1l - 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[0] * longData[4]),
				"1l * 4026593280l");
		assertEqual("0", String.valueOf(longData[0] / longData[4]),
				"1l / 4026593280l");
		assertEqual("1", String.valueOf(longData[0] % longData[4]),
				"1l % 4026593280l");
		assertEqual("0", String.valueOf(longData[0] & longData[4]),
				"1l & 4026593280l");
		assertEqual("4026593281", String.valueOf(longData[0] | longData[4]),
				"1l | 4026593280l");
		assertEqual("4026593281", String.valueOf(longData[0] ^ longData[4]),
				"1l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[0] > longData[4]),
				"1l > 4026593280l");
		assertEqual("true", String.valueOf(longData[0] < longData[4]),
				"1l < 4026593280l");
		assertEqual("false", String.valueOf(longData[0] == longData[4]),
				"1l == 4026593280l");
		assertEqual("false", String.valueOf(longData[0] >= longData[4]),
				"1l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[0] <= longData[4]),
				"1l <= 4026593280l");
		assertEqual("251662081", String.valueOf(longData[0] + longData[5]),
				"1l + 251662080l");
		assertEqual("-251662079", String.valueOf(longData[0] - longData[5]),
				"1l - 251662080l");
		assertEqual("251662080", String.valueOf(longData[0] * longData[5]),
				"1l * 251662080l");
		assertEqual("0", String.valueOf(longData[0] / longData[5]),
				"1l / 251662080l");
		assertEqual("1", String.valueOf(longData[0] % longData[5]),
				"1l % 251662080l");
		assertEqual("0", String.valueOf(longData[0] & longData[5]),
				"1l & 251662080l");
		assertEqual("251662081", String.valueOf(longData[0] | longData[5]),
				"1l | 251662080l");
		assertEqual("251662081", String.valueOf(longData[0] ^ longData[5]),
				"1l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[0] > longData[5]),
				"1l > 251662080l");
		assertEqual("true", String.valueOf(longData[0] < longData[5]),
				"1l < 251662080l");
		assertEqual("false", String.valueOf(longData[0] == longData[5]),
				"1l == 251662080l");
		assertEqual("false", String.valueOf(longData[0] >= longData[5]),
				"1l >= 251662080l");
		assertEqual("true", String.valueOf(longData[0] <= longData[5]),
				"1l <= 251662080l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[0] + longData[6]),
				"1l + -1152657617789587456l");
		assertEqual("1152657617789587457",
				String.valueOf(longData[0] - longData[6]),
				"1l - -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[0] * longData[6]),
				"1l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[0] / longData[6]),
				"1l / -1152657617789587456l");
		assertEqual("1", String.valueOf(longData[0] % longData[6]),
				"1l % -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[0] & longData[6]),
				"1l & -1152657617789587456l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[0] | longData[6]),
				"1l | -1152657617789587456l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[0] ^ longData[6]),
				"1l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[0] > longData[6]),
				"1l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[0] < longData[6]),
				"1l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[0] == longData[6]),
				"1l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[0] >= longData[6]),
				"1l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[0] <= longData[6]),
				"1l <= -1152657617789587456l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[0] + longData[7]),
				"1l + 1080880403494997760l");
		assertEqual("-1080880403494997759",
				String.valueOf(longData[0] - longData[7]),
				"1l - 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[0] * longData[7]),
				"1l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[0] / longData[7]),
				"1l / 1080880403494997760l");
		assertEqual("1", String.valueOf(longData[0] % longData[7]),
				"1l % 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[0] & longData[7]),
				"1l & 1080880403494997760l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[0] | longData[7]),
				"1l | 1080880403494997760l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[0] ^ longData[7]),
				"1l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[0] > longData[7]),
				"1l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[0] < longData[7]),
				"1l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[0] == longData[7]),
				"1l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[0] >= longData[7]),
				"1l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[0] <= longData[7]),
				"1l <= 1080880403494997760l");
		assertEqual("-1", String.valueOf((int) longData[1]), "l2i -1l");
		assertEqual("-1.0", String.valueOf((float) longData[1]), "l2f -1l");
		assertEqual("-1.0", String.valueOf((double) longData[1]), "l2d -1l");
		assertEqual("1", String.valueOf(-longData[1]), "lneg -1l");
		assertEqual("-1", String.valueOf(longData[1] >> 0), "-1l >> 0");
		assertEqual("-1", String.valueOf(longData[1] >>> 0), "-1l >>> 0");
		assertEqual("-1", String.valueOf(longData[1] << 0), "-1l << 0");
		assertEqual("-1", String.valueOf(longData[1] >> 1), "-1l >> 1");
		assertEqual("9223372036854775807", String.valueOf(longData[1] >>> 1),
				"-1l >>> 1");
		assertEqual("-2", String.valueOf(longData[1] << 1), "-1l << 1");
		assertEqual("-1", String.valueOf(longData[1] >> 2), "-1l >> 2");
		assertEqual("4611686018427387903", String.valueOf(longData[1] >>> 2),
				"-1l >>> 2");
		assertEqual("-4", String.valueOf(longData[1] << 2), "-1l << 2");
		assertEqual("-1", String.valueOf(longData[1] >> 3), "-1l >> 3");
		assertEqual("2305843009213693951", String.valueOf(longData[1] >>> 3),
				"-1l >>> 3");
		assertEqual("-8", String.valueOf(longData[1] << 3), "-1l << 3");
		assertEqual("-1", String.valueOf(longData[1] >> 4), "-1l >> 4");
		assertEqual("1152921504606846975", String.valueOf(longData[1] >>> 4),
				"-1l >>> 4");
		assertEqual("-16", String.valueOf(longData[1] << 4), "-1l << 4");
		assertEqual("-1", String.valueOf(longData[1] >> 5), "-1l >> 5");
		assertEqual("576460752303423487", String.valueOf(longData[1] >>> 5),
				"-1l >>> 5");
		assertEqual("-32", String.valueOf(longData[1] << 5), "-1l << 5");
		assertEqual("-1", String.valueOf(longData[1] >> 6), "-1l >> 6");
		assertEqual("288230376151711743", String.valueOf(longData[1] >>> 6),
				"-1l >>> 6");
		assertEqual("-64", String.valueOf(longData[1] << 6), "-1l << 6");
		assertEqual("-1", String.valueOf(longData[1] >> 7), "-1l >> 7");
		assertEqual("144115188075855871", String.valueOf(longData[1] >>> 7),
				"-1l >>> 7");
		assertEqual("-128", String.valueOf(longData[1] << 7), "-1l << 7");
		assertEqual("-1", String.valueOf(longData[1] >> 8), "-1l >> 8");
		assertEqual("72057594037927935", String.valueOf(longData[1] >>> 8),
				"-1l >>> 8");
		assertEqual("-256", String.valueOf(longData[1] << 8), "-1l << 8");
		assertEqual("-1", String.valueOf(longData[1] >> 9), "-1l >> 9");
		assertEqual("36028797018963967", String.valueOf(longData[1] >>> 9),
				"-1l >>> 9");
		assertEqual("-512", String.valueOf(longData[1] << 9), "-1l << 9");
		assertEqual("-1", String.valueOf(longData[1] >> 10), "-1l >> 10");
		assertEqual("18014398509481983", String.valueOf(longData[1] >>> 10),
				"-1l >>> 10");
		assertEqual("-1024", String.valueOf(longData[1] << 10), "-1l << 10");
		assertEqual("-1", String.valueOf(longData[1] >> 11), "-1l >> 11");
		assertEqual("9007199254740991", String.valueOf(longData[1] >>> 11),
				"-1l >>> 11");
		assertEqual("-2048", String.valueOf(longData[1] << 11), "-1l << 11");
		assertEqual("-1", String.valueOf(longData[1] >> 12), "-1l >> 12");
		assertEqual("4503599627370495", String.valueOf(longData[1] >>> 12),
				"-1l >>> 12");
		assertEqual("-4096", String.valueOf(longData[1] << 12), "-1l << 12");
		assertEqual("-1", String.valueOf(longData[1] >> 13), "-1l >> 13");
		assertEqual("2251799813685247", String.valueOf(longData[1] >>> 13),
				"-1l >>> 13");
		assertEqual("-8192", String.valueOf(longData[1] << 13), "-1l << 13");
		assertEqual("-1", String.valueOf(longData[1] >> 14), "-1l >> 14");
		assertEqual("1125899906842623", String.valueOf(longData[1] >>> 14),
				"-1l >>> 14");
		assertEqual("-16384", String.valueOf(longData[1] << 14), "-1l << 14");
		assertEqual("-1", String.valueOf(longData[1] >> 15), "-1l >> 15");
		assertEqual("562949953421311", String.valueOf(longData[1] >>> 15),
				"-1l >>> 15");
		assertEqual("-32768", String.valueOf(longData[1] << 15), "-1l << 15");
		assertEqual("-1", String.valueOf(longData[1] >> 16), "-1l >> 16");
		assertEqual("281474976710655", String.valueOf(longData[1] >>> 16),
				"-1l >>> 16");
		assertEqual("-65536", String.valueOf(longData[1] << 16), "-1l << 16");
		assertEqual("-1", String.valueOf(longData[1] >> 17), "-1l >> 17");
		assertEqual("140737488355327", String.valueOf(longData[1] >>> 17),
				"-1l >>> 17");
		assertEqual("-131072", String.valueOf(longData[1] << 17), "-1l << 17");
		assertEqual("-1", String.valueOf(longData[1] >> 18), "-1l >> 18");
		assertEqual("70368744177663", String.valueOf(longData[1] >>> 18),
				"-1l >>> 18");
		assertEqual("-262144", String.valueOf(longData[1] << 18), "-1l << 18");
		assertEqual("-1", String.valueOf(longData[1] >> 19), "-1l >> 19");
		assertEqual("35184372088831", String.valueOf(longData[1] >>> 19),
				"-1l >>> 19");
		assertEqual("-524288", String.valueOf(longData[1] << 19), "-1l << 19");
		assertEqual("-1", String.valueOf(longData[1] >> 20), "-1l >> 20");
		assertEqual("17592186044415", String.valueOf(longData[1] >>> 20),
				"-1l >>> 20");
		assertEqual("-1048576", String.valueOf(longData[1] << 20), "-1l << 20");
		assertEqual("-1", String.valueOf(longData[1] >> 21), "-1l >> 21");
		assertEqual("8796093022207", String.valueOf(longData[1] >>> 21),
				"-1l >>> 21");
		assertEqual("-2097152", String.valueOf(longData[1] << 21), "-1l << 21");
		assertEqual("-1", String.valueOf(longData[1] >> 22), "-1l >> 22");
		assertEqual("4398046511103", String.valueOf(longData[1] >>> 22),
				"-1l >>> 22");
		assertEqual("-4194304", String.valueOf(longData[1] << 22), "-1l << 22");
		assertEqual("-1", String.valueOf(longData[1] >> 23), "-1l >> 23");
		assertEqual("2199023255551", String.valueOf(longData[1] >>> 23),
				"-1l >>> 23");
		assertEqual("-8388608", String.valueOf(longData[1] << 23), "-1l << 23");
		assertEqual("-1", String.valueOf(longData[1] >> 24), "-1l >> 24");
		assertEqual("1099511627775", String.valueOf(longData[1] >>> 24),
				"-1l >>> 24");
		assertEqual("-16777216", String.valueOf(longData[1] << 24), "-1l << 24");
		assertEqual("-1", String.valueOf(longData[1] >> 25), "-1l >> 25");
		assertEqual("549755813887", String.valueOf(longData[1] >>> 25),
				"-1l >>> 25");
		assertEqual("-33554432", String.valueOf(longData[1] << 25), "-1l << 25");
		assertEqual("-1", String.valueOf(longData[1] >> 26), "-1l >> 26");
		assertEqual("274877906943", String.valueOf(longData[1] >>> 26),
				"-1l >>> 26");
		assertEqual("-67108864", String.valueOf(longData[1] << 26), "-1l << 26");
		assertEqual("-1", String.valueOf(longData[1] >> 27), "-1l >> 27");
		assertEqual("137438953471", String.valueOf(longData[1] >>> 27),
				"-1l >>> 27");
		assertEqual("-134217728", String.valueOf(longData[1] << 27),
				"-1l << 27");
		assertEqual("-1", String.valueOf(longData[1] >> 28), "-1l >> 28");
		assertEqual("68719476735", String.valueOf(longData[1] >>> 28),
				"-1l >>> 28");
		assertEqual("-268435456", String.valueOf(longData[1] << 28),
				"-1l << 28");
		assertEqual("-1", String.valueOf(longData[1] >> 29), "-1l >> 29");
		assertEqual("34359738367", String.valueOf(longData[1] >>> 29),
				"-1l >>> 29");
		assertEqual("-536870912", String.valueOf(longData[1] << 29),
				"-1l << 29");
		assertEqual("-1", String.valueOf(longData[1] >> 30), "-1l >> 30");
		assertEqual("17179869183", String.valueOf(longData[1] >>> 30),
				"-1l >>> 30");
		assertEqual("-1073741824", String.valueOf(longData[1] << 30),
				"-1l << 30");
		assertEqual("-1", String.valueOf(longData[1] >> 31), "-1l >> 31");
		assertEqual("8589934591", String.valueOf(longData[1] >>> 31),
				"-1l >>> 31");
		assertEqual("-2147483648", String.valueOf(longData[1] << 31),
				"-1l << 31");
		assertEqual("-1", String.valueOf(longData[1] >> 32), "-1l >> 32");
		assertEqual("4294967295", String.valueOf(longData[1] >>> 32),
				"-1l >>> 32");
		assertEqual("-4294967296", String.valueOf(longData[1] << 32),
				"-1l << 32");
		assertEqual("-1", String.valueOf(longData[1] >> 33), "-1l >> 33");
		assertEqual("2147483647", String.valueOf(longData[1] >>> 33),
				"-1l >>> 33");
		assertEqual("-8589934592", String.valueOf(longData[1] << 33),
				"-1l << 33");
		assertEqual("-1", String.valueOf(longData[1] >> 34), "-1l >> 34");
		assertEqual("1073741823", String.valueOf(longData[1] >>> 34),
				"-1l >>> 34");
		assertEqual("-17179869184", String.valueOf(longData[1] << 34),
				"-1l << 34");
		assertEqual("-1", String.valueOf(longData[1] >> 35), "-1l >> 35");
		assertEqual("536870911", String.valueOf(longData[1] >>> 35),
				"-1l >>> 35");
		assertEqual("-34359738368", String.valueOf(longData[1] << 35),
				"-1l << 35");
		assertEqual("-1", String.valueOf(longData[1] >> 36), "-1l >> 36");
		assertEqual("268435455", String.valueOf(longData[1] >>> 36),
				"-1l >>> 36");
		assertEqual("-68719476736", String.valueOf(longData[1] << 36),
				"-1l << 36");
		assertEqual("-1", String.valueOf(longData[1] >> 37), "-1l >> 37");
		assertEqual("134217727", String.valueOf(longData[1] >>> 37),
				"-1l >>> 37");
		assertEqual("-137438953472", String.valueOf(longData[1] << 37),
				"-1l << 37");
		assertEqual("-1", String.valueOf(longData[1] >> 38), "-1l >> 38");
		assertEqual("67108863", String.valueOf(longData[1] >>> 38),
				"-1l >>> 38");
		assertEqual("-274877906944", String.valueOf(longData[1] << 38),
				"-1l << 38");
		assertEqual("-1", String.valueOf(longData[1] >> 39), "-1l >> 39");
		assertEqual("33554431", String.valueOf(longData[1] >>> 39),
				"-1l >>> 39");
		assertEqual("-549755813888", String.valueOf(longData[1] << 39),
				"-1l << 39");
		assertEqual("-1", String.valueOf(longData[1] >> 40), "-1l >> 40");
		assertEqual("16777215", String.valueOf(longData[1] >>> 40),
				"-1l >>> 40");
		assertEqual("-1099511627776", String.valueOf(longData[1] << 40),
				"-1l << 40");
		assertEqual("-1", String.valueOf(longData[1] >> 41), "-1l >> 41");
		assertEqual("8388607", String.valueOf(longData[1] >>> 41), "-1l >>> 41");
		assertEqual("-2199023255552", String.valueOf(longData[1] << 41),
				"-1l << 41");
		assertEqual("-1", String.valueOf(longData[1] >> 42), "-1l >> 42");
		assertEqual("4194303", String.valueOf(longData[1] >>> 42), "-1l >>> 42");
		assertEqual("-4398046511104", String.valueOf(longData[1] << 42),
				"-1l << 42");
		assertEqual("-1", String.valueOf(longData[1] >> 43), "-1l >> 43");
		assertEqual("2097151", String.valueOf(longData[1] >>> 43), "-1l >>> 43");
		assertEqual("-8796093022208", String.valueOf(longData[1] << 43),
				"-1l << 43");
		assertEqual("-1", String.valueOf(longData[1] >> 44), "-1l >> 44");
		assertEqual("1048575", String.valueOf(longData[1] >>> 44), "-1l >>> 44");
		assertEqual("-17592186044416", String.valueOf(longData[1] << 44),
				"-1l << 44");
		assertEqual("-1", String.valueOf(longData[1] >> 45), "-1l >> 45");
		assertEqual("524287", String.valueOf(longData[1] >>> 45), "-1l >>> 45");
		assertEqual("-35184372088832", String.valueOf(longData[1] << 45),
				"-1l << 45");
		assertEqual("-1", String.valueOf(longData[1] >> 46), "-1l >> 46");
		assertEqual("262143", String.valueOf(longData[1] >>> 46), "-1l >>> 46");
		assertEqual("-70368744177664", String.valueOf(longData[1] << 46),
				"-1l << 46");
		assertEqual("-1", String.valueOf(longData[1] >> 47), "-1l >> 47");
		assertEqual("131071", String.valueOf(longData[1] >>> 47), "-1l >>> 47");
		assertEqual("-140737488355328", String.valueOf(longData[1] << 47),
				"-1l << 47");
		assertEqual("-1", String.valueOf(longData[1] >> 48), "-1l >> 48");
		assertEqual("65535", String.valueOf(longData[1] >>> 48), "-1l >>> 48");
		assertEqual("-281474976710656", String.valueOf(longData[1] << 48),
				"-1l << 48");
		assertEqual("-1", String.valueOf(longData[1] >> 49), "-1l >> 49");
		assertEqual("32767", String.valueOf(longData[1] >>> 49), "-1l >>> 49");
		assertEqual("-562949953421312", String.valueOf(longData[1] << 49),
				"-1l << 49");
		assertEqual("-1", String.valueOf(longData[1] >> 50), "-1l >> 50");
		assertEqual("16383", String.valueOf(longData[1] >>> 50), "-1l >>> 50");
		assertEqual("-1125899906842624", String.valueOf(longData[1] << 50),
				"-1l << 50");
		assertEqual("-1", String.valueOf(longData[1] >> 51), "-1l >> 51");
		assertEqual("8191", String.valueOf(longData[1] >>> 51), "-1l >>> 51");
		assertEqual("-2251799813685248", String.valueOf(longData[1] << 51),
				"-1l << 51");
		assertEqual("-1", String.valueOf(longData[1] >> 52), "-1l >> 52");
		assertEqual("4095", String.valueOf(longData[1] >>> 52), "-1l >>> 52");
		assertEqual("-4503599627370496", String.valueOf(longData[1] << 52),
				"-1l << 52");
		assertEqual("-1", String.valueOf(longData[1] >> 53), "-1l >> 53");
		assertEqual("2047", String.valueOf(longData[1] >>> 53), "-1l >>> 53");
		assertEqual("-9007199254740992", String.valueOf(longData[1] << 53),
				"-1l << 53");
		assertEqual("-1", String.valueOf(longData[1] >> 54), "-1l >> 54");
		assertEqual("1023", String.valueOf(longData[1] >>> 54), "-1l >>> 54");
		assertEqual("-18014398509481984", String.valueOf(longData[1] << 54),
				"-1l << 54");
		assertEqual("-1", String.valueOf(longData[1] >> 55), "-1l >> 55");
		assertEqual("511", String.valueOf(longData[1] >>> 55), "-1l >>> 55");
		assertEqual("-36028797018963968", String.valueOf(longData[1] << 55),
				"-1l << 55");
		assertEqual("-1", String.valueOf(longData[1] >> 56), "-1l >> 56");
		assertEqual("255", String.valueOf(longData[1] >>> 56), "-1l >>> 56");
		assertEqual("-72057594037927936", String.valueOf(longData[1] << 56),
				"-1l << 56");
		assertEqual("-1", String.valueOf(longData[1] >> 57), "-1l >> 57");
		assertEqual("127", String.valueOf(longData[1] >>> 57), "-1l >>> 57");
		assertEqual("-144115188075855872", String.valueOf(longData[1] << 57),
				"-1l << 57");
		assertEqual("-1", String.valueOf(longData[1] >> 58), "-1l >> 58");
		assertEqual("63", String.valueOf(longData[1] >>> 58), "-1l >>> 58");
		assertEqual("-288230376151711744", String.valueOf(longData[1] << 58),
				"-1l << 58");
		assertEqual("-1", String.valueOf(longData[1] >> 59), "-1l >> 59");
		assertEqual("31", String.valueOf(longData[1] >>> 59), "-1l >>> 59");
		assertEqual("-576460752303423488", String.valueOf(longData[1] << 59),
				"-1l << 59");
		assertEqual("-1", String.valueOf(longData[1] >> 60), "-1l >> 60");
		assertEqual("15", String.valueOf(longData[1] >>> 60), "-1l >>> 60");
		assertEqual("-1152921504606846976", String.valueOf(longData[1] << 60),
				"-1l << 60");
		assertEqual("-1", String.valueOf(longData[1] >> 61), "-1l >> 61");
		assertEqual("7", String.valueOf(longData[1] >>> 61), "-1l >>> 61");
		assertEqual("-2305843009213693952", String.valueOf(longData[1] << 61),
				"-1l << 61");
		assertEqual("-1", String.valueOf(longData[1] >> 62), "-1l >> 62");
		assertEqual("3", String.valueOf(longData[1] >>> 62), "-1l >>> 62");
		assertEqual("-4611686018427387904", String.valueOf(longData[1] << 62),
				"-1l << 62");
		assertEqual("-1", String.valueOf(longData[1] >> 63), "-1l >> 63");
		assertEqual("1", String.valueOf(longData[1] >>> 63), "-1l >>> 63");
		assertEqual("-9223372036854775808", String.valueOf(longData[1] << 63),
				"-1l << 63");
		assertEqual("0", String.valueOf(longData[1] + longData[0]), "-1l + 1l");
		assertEqual("-2", String.valueOf(longData[1] - longData[0]), "-1l - 1l");
		assertEqual("-1", String.valueOf(longData[1] * longData[0]), "-1l * 1l");
		assertEqual("-1", String.valueOf(longData[1] / longData[0]), "-1l / 1l");
		assertEqual("0", String.valueOf(longData[1] % longData[0]), "-1l % 1l");
		assertEqual("1", String.valueOf(longData[1] & longData[0]), "-1l & 1l");
		assertEqual("-1", String.valueOf(longData[1] | longData[0]), "-1l | 1l");
		assertEqual("-2", String.valueOf(longData[1] ^ longData[0]), "-1l ^ 1l");
		assertEqual("false", String.valueOf(longData[1] > longData[0]),
				"-1l > 1l");
		assertEqual("true", String.valueOf(longData[1] < longData[0]),
				"-1l < 1l");
		assertEqual("false", String.valueOf(longData[1] == longData[0]),
				"-1l == 1l");
		assertEqual("false", String.valueOf(longData[1] >= longData[0]),
				"-1l >= 1l");
		assertEqual("true", String.valueOf(longData[1] <= longData[0]),
				"-1l <= 1l");
		assertEqual("-2", String.valueOf(longData[1] + longData[1]),
				"-1l + -1l");
		assertEqual("0", String.valueOf(longData[1] - longData[1]), "-1l - -1l");
		assertEqual("1", String.valueOf(longData[1] * longData[1]), "-1l * -1l");
		assertEqual("1", String.valueOf(longData[1] / longData[1]), "-1l / -1l");
		assertEqual("0", String.valueOf(longData[1] % longData[1]), "-1l % -1l");
		assertEqual("-1", String.valueOf(longData[1] & longData[1]),
				"-1l & -1l");
		assertEqual("-1", String.valueOf(longData[1] | longData[1]),
				"-1l | -1l");
		assertEqual("0", String.valueOf(longData[1] ^ longData[1]), "-1l ^ -1l");
		assertEqual("false", String.valueOf(longData[1] > longData[1]),
				"-1l > -1l");
		assertEqual("false", String.valueOf(longData[1] < longData[1]),
				"-1l < -1l");
		assertEqual("true", String.valueOf(longData[1] == longData[1]),
				"-1l == -1l");
		assertEqual("true", String.valueOf(longData[1] >= longData[1]),
				"-1l >= -1l");
		assertEqual("true", String.valueOf(longData[1] <= longData[1]),
				"-1l <= -1l");
		assertEqual("16", String.valueOf(longData[1] + longData[2]),
				"-1l + 17l");
		assertEqual("-18", String.valueOf(longData[1] - longData[2]),
				"-1l - 17l");
		assertEqual("-17", String.valueOf(longData[1] * longData[2]),
				"-1l * 17l");
		assertEqual("0", String.valueOf(longData[1] / longData[2]), "-1l / 17l");
		assertEqual("-1", String.valueOf(longData[1] % longData[2]),
				"-1l % 17l");
		assertEqual("17", String.valueOf(longData[1] & longData[2]),
				"-1l & 17l");
		assertEqual("-1", String.valueOf(longData[1] | longData[2]),
				"-1l | 17l");
		assertEqual("-18", String.valueOf(longData[1] ^ longData[2]),
				"-1l ^ 17l");
		assertEqual("false", String.valueOf(longData[1] > longData[2]),
				"-1l > 17l");
		assertEqual("true", String.valueOf(longData[1] < longData[2]),
				"-1l < 17l");
		assertEqual("false", String.valueOf(longData[1] == longData[2]),
				"-1l == 17l");
		assertEqual("false", String.valueOf(longData[1] >= longData[2]),
				"-1l >= 17l");
		assertEqual("true", String.valueOf(longData[1] <= longData[2]),
				"-1l <= 17l");
		assertEqual("-18", String.valueOf(longData[1] + longData[3]),
				"-1l + -17l");
		assertEqual("16", String.valueOf(longData[1] - longData[3]),
				"-1l - -17l");
		assertEqual("17", String.valueOf(longData[1] * longData[3]),
				"-1l * -17l");
		assertEqual("0", String.valueOf(longData[1] / longData[3]),
				"-1l / -17l");
		assertEqual("-1", String.valueOf(longData[1] % longData[3]),
				"-1l % -17l");
		assertEqual("-17", String.valueOf(longData[1] & longData[3]),
				"-1l & -17l");
		assertEqual("-1", String.valueOf(longData[1] | longData[3]),
				"-1l | -17l");
		assertEqual("16", String.valueOf(longData[1] ^ longData[3]),
				"-1l ^ -17l");
		assertEqual("true", String.valueOf(longData[1] > longData[3]),
				"-1l > -17l");
		assertEqual("false", String.valueOf(longData[1] < longData[3]),
				"-1l < -17l");
		assertEqual("false", String.valueOf(longData[1] == longData[3]),
				"-1l == -17l");
		assertEqual("true", String.valueOf(longData[1] >= longData[3]),
				"-1l >= -17l");
		assertEqual("false", String.valueOf(longData[1] <= longData[3]),
				"-1l <= -17l");
		assertEqual("4026593279", String.valueOf(longData[1] + longData[4]),
				"-1l + 4026593280l");
		assertEqual("-4026593281", String.valueOf(longData[1] - longData[4]),
				"-1l - 4026593280l");
		assertEqual("-4026593280", String.valueOf(longData[1] * longData[4]),
				"-1l * 4026593280l");
		assertEqual("0", String.valueOf(longData[1] / longData[4]),
				"-1l / 4026593280l");
		assertEqual("-1", String.valueOf(longData[1] % longData[4]),
				"-1l % 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[1] & longData[4]),
				"-1l & 4026593280l");
		assertEqual("-1", String.valueOf(longData[1] | longData[4]),
				"-1l | 4026593280l");
		assertEqual("-4026593281", String.valueOf(longData[1] ^ longData[4]),
				"-1l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[1] > longData[4]),
				"-1l > 4026593280l");
		assertEqual("true", String.valueOf(longData[1] < longData[4]),
				"-1l < 4026593280l");
		assertEqual("false", String.valueOf(longData[1] == longData[4]),
				"-1l == 4026593280l");
		assertEqual("false", String.valueOf(longData[1] >= longData[4]),
				"-1l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[1] <= longData[4]),
				"-1l <= 4026593280l");
		assertEqual("251662079", String.valueOf(longData[1] + longData[5]),
				"-1l + 251662080l");
		assertEqual("-251662081", String.valueOf(longData[1] - longData[5]),
				"-1l - 251662080l");
		assertEqual("-251662080", String.valueOf(longData[1] * longData[5]),
				"-1l * 251662080l");
		assertEqual("0", String.valueOf(longData[1] / longData[5]),
				"-1l / 251662080l");
		assertEqual("-1", String.valueOf(longData[1] % longData[5]),
				"-1l % 251662080l");
		assertEqual("251662080", String.valueOf(longData[1] & longData[5]),
				"-1l & 251662080l");
		assertEqual("-1", String.valueOf(longData[1] | longData[5]),
				"-1l | 251662080l");
		assertEqual("-251662081", String.valueOf(longData[1] ^ longData[5]),
				"-1l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[1] > longData[5]),
				"-1l > 251662080l");
		assertEqual("true", String.valueOf(longData[1] < longData[5]),
				"-1l < 251662080l");
		assertEqual("false", String.valueOf(longData[1] == longData[5]),
				"-1l == 251662080l");
		assertEqual("false", String.valueOf(longData[1] >= longData[5]),
				"-1l >= 251662080l");
		assertEqual("true", String.valueOf(longData[1] <= longData[5]),
				"-1l <= 251662080l");
		assertEqual("-1152657617789587457",
				String.valueOf(longData[1] + longData[6]),
				"-1l + -1152657617789587456l");
		assertEqual("1152657617789587455",
				String.valueOf(longData[1] - longData[6]),
				"-1l - -1152657617789587456l");
		assertEqual("1152657617789587456",
				String.valueOf(longData[1] * longData[6]),
				"-1l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[1] / longData[6]),
				"-1l / -1152657617789587456l");
		assertEqual("-1", String.valueOf(longData[1] % longData[6]),
				"-1l % -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[1] & longData[6]),
				"-1l & -1152657617789587456l");
		assertEqual("-1", String.valueOf(longData[1] | longData[6]),
				"-1l | -1152657617789587456l");
		assertEqual("1152657617789587455",
				String.valueOf(longData[1] ^ longData[6]),
				"-1l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[1] > longData[6]),
				"-1l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[1] < longData[6]),
				"-1l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[1] == longData[6]),
				"-1l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[1] >= longData[6]),
				"-1l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[1] <= longData[6]),
				"-1l <= -1152657617789587456l");
		assertEqual("1080880403494997759",
				String.valueOf(longData[1] + longData[7]),
				"-1l + 1080880403494997760l");
		assertEqual("-1080880403494997761",
				String.valueOf(longData[1] - longData[7]),
				"-1l - 1080880403494997760l");
		assertEqual("-1080880403494997760",
				String.valueOf(longData[1] * longData[7]),
				"-1l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[1] / longData[7]),
				"-1l / 1080880403494997760l");
		assertEqual("-1", String.valueOf(longData[1] % longData[7]),
				"-1l % 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[1] & longData[7]),
				"-1l & 1080880403494997760l");
		assertEqual("-1", String.valueOf(longData[1] | longData[7]),
				"-1l | 1080880403494997760l");
		assertEqual("-1080880403494997761",
				String.valueOf(longData[1] ^ longData[7]),
				"-1l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[1] > longData[7]),
				"-1l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[1] < longData[7]),
				"-1l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[1] == longData[7]),
				"-1l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[1] >= longData[7]),
				"-1l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[1] <= longData[7]),
				"-1l <= 1080880403494997760l");
		assertEqual("17", String.valueOf((int) longData[2]), "l2i 17l");
		assertEqual("17.0", String.valueOf((float) longData[2]), "l2f 17l");
		assertEqual("17.0", String.valueOf((double) longData[2]), "l2d 17l");
		assertEqual("-17", String.valueOf(-longData[2]), "lneg 17l");
		assertEqual("17", String.valueOf(longData[2] >> 0), "17l >> 0");
		assertEqual("17", String.valueOf(longData[2] >>> 0), "17l >>> 0");
		assertEqual("17", String.valueOf(longData[2] << 0), "17l << 0");
		assertEqual("8", String.valueOf(longData[2] >> 1), "17l >> 1");
		assertEqual("8", String.valueOf(longData[2] >>> 1), "17l >>> 1");
		assertEqual("34", String.valueOf(longData[2] << 1), "17l << 1");
		assertEqual("4", String.valueOf(longData[2] >> 2), "17l >> 2");
		assertEqual("4", String.valueOf(longData[2] >>> 2), "17l >>> 2");
		assertEqual("68", String.valueOf(longData[2] << 2), "17l << 2");
		assertEqual("2", String.valueOf(longData[2] >> 3), "17l >> 3");
		assertEqual("2", String.valueOf(longData[2] >>> 3), "17l >>> 3");
		assertEqual("136", String.valueOf(longData[2] << 3), "17l << 3");
		assertEqual("1", String.valueOf(longData[2] >> 4), "17l >> 4");
		assertEqual("1", String.valueOf(longData[2] >>> 4), "17l >>> 4");
		assertEqual("272", String.valueOf(longData[2] << 4), "17l << 4");
		assertEqual("0", String.valueOf(longData[2] >> 5), "17l >> 5");
		assertEqual("0", String.valueOf(longData[2] >>> 5), "17l >>> 5");
		assertEqual("544", String.valueOf(longData[2] << 5), "17l << 5");
		assertEqual("0", String.valueOf(longData[2] >> 6), "17l >> 6");
		assertEqual("0", String.valueOf(longData[2] >>> 6), "17l >>> 6");
		assertEqual("1088", String.valueOf(longData[2] << 6), "17l << 6");
		assertEqual("0", String.valueOf(longData[2] >> 7), "17l >> 7");
		assertEqual("0", String.valueOf(longData[2] >>> 7), "17l >>> 7");
		assertEqual("2176", String.valueOf(longData[2] << 7), "17l << 7");
		assertEqual("0", String.valueOf(longData[2] >> 8), "17l >> 8");
		assertEqual("0", String.valueOf(longData[2] >>> 8), "17l >>> 8");
		assertEqual("4352", String.valueOf(longData[2] << 8), "17l << 8");
		assertEqual("0", String.valueOf(longData[2] >> 9), "17l >> 9");
		assertEqual("0", String.valueOf(longData[2] >>> 9), "17l >>> 9");
		assertEqual("8704", String.valueOf(longData[2] << 9), "17l << 9");
		assertEqual("0", String.valueOf(longData[2] >> 10), "17l >> 10");
		assertEqual("0", String.valueOf(longData[2] >>> 10), "17l >>> 10");
		assertEqual("17408", String.valueOf(longData[2] << 10), "17l << 10");
		assertEqual("0", String.valueOf(longData[2] >> 11), "17l >> 11");
		assertEqual("0", String.valueOf(longData[2] >>> 11), "17l >>> 11");
		assertEqual("34816", String.valueOf(longData[2] << 11), "17l << 11");
		assertEqual("0", String.valueOf(longData[2] >> 12), "17l >> 12");
		assertEqual("0", String.valueOf(longData[2] >>> 12), "17l >>> 12");
		assertEqual("69632", String.valueOf(longData[2] << 12), "17l << 12");
		assertEqual("0", String.valueOf(longData[2] >> 13), "17l >> 13");
		assertEqual("0", String.valueOf(longData[2] >>> 13), "17l >>> 13");
		assertEqual("139264", String.valueOf(longData[2] << 13), "17l << 13");
		assertEqual("0", String.valueOf(longData[2] >> 14), "17l >> 14");
		assertEqual("0", String.valueOf(longData[2] >>> 14), "17l >>> 14");
		assertEqual("278528", String.valueOf(longData[2] << 14), "17l << 14");
		assertEqual("0", String.valueOf(longData[2] >> 15), "17l >> 15");
		assertEqual("0", String.valueOf(longData[2] >>> 15), "17l >>> 15");
		assertEqual("557056", String.valueOf(longData[2] << 15), "17l << 15");
		assertEqual("0", String.valueOf(longData[2] >> 16), "17l >> 16");
		assertEqual("0", String.valueOf(longData[2] >>> 16), "17l >>> 16");
		assertEqual("1114112", String.valueOf(longData[2] << 16), "17l << 16");
		assertEqual("0", String.valueOf(longData[2] >> 17), "17l >> 17");
		assertEqual("0", String.valueOf(longData[2] >>> 17), "17l >>> 17");
		assertEqual("2228224", String.valueOf(longData[2] << 17), "17l << 17");
		assertEqual("0", String.valueOf(longData[2] >> 18), "17l >> 18");
		assertEqual("0", String.valueOf(longData[2] >>> 18), "17l >>> 18");
		assertEqual("4456448", String.valueOf(longData[2] << 18), "17l << 18");
		assertEqual("0", String.valueOf(longData[2] >> 19), "17l >> 19");
		assertEqual("0", String.valueOf(longData[2] >>> 19), "17l >>> 19");
		assertEqual("8912896", String.valueOf(longData[2] << 19), "17l << 19");
		assertEqual("0", String.valueOf(longData[2] >> 20), "17l >> 20");
		assertEqual("0", String.valueOf(longData[2] >>> 20), "17l >>> 20");
		assertEqual("17825792", String.valueOf(longData[2] << 20), "17l << 20");
		assertEqual("0", String.valueOf(longData[2] >> 21), "17l >> 21");
		assertEqual("0", String.valueOf(longData[2] >>> 21), "17l >>> 21");
		assertEqual("35651584", String.valueOf(longData[2] << 21), "17l << 21");
		assertEqual("0", String.valueOf(longData[2] >> 22), "17l >> 22");
		assertEqual("0", String.valueOf(longData[2] >>> 22), "17l >>> 22");
		assertEqual("71303168", String.valueOf(longData[2] << 22), "17l << 22");
		assertEqual("0", String.valueOf(longData[2] >> 23), "17l >> 23");
		assertEqual("0", String.valueOf(longData[2] >>> 23), "17l >>> 23");
		assertEqual("142606336", String.valueOf(longData[2] << 23), "17l << 23");
		assertEqual("0", String.valueOf(longData[2] >> 24), "17l >> 24");
		assertEqual("0", String.valueOf(longData[2] >>> 24), "17l >>> 24");
		assertEqual("285212672", String.valueOf(longData[2] << 24), "17l << 24");
		assertEqual("0", String.valueOf(longData[2] >> 25), "17l >> 25");
		assertEqual("0", String.valueOf(longData[2] >>> 25), "17l >>> 25");
		assertEqual("570425344", String.valueOf(longData[2] << 25), "17l << 25");
		assertEqual("0", String.valueOf(longData[2] >> 26), "17l >> 26");
		assertEqual("0", String.valueOf(longData[2] >>> 26), "17l >>> 26");
		assertEqual("1140850688", String.valueOf(longData[2] << 26),
				"17l << 26");
		assertEqual("0", String.valueOf(longData[2] >> 27), "17l >> 27");
		assertEqual("0", String.valueOf(longData[2] >>> 27), "17l >>> 27");
		assertEqual("2281701376", String.valueOf(longData[2] << 27),
				"17l << 27");
		assertEqual("0", String.valueOf(longData[2] >> 28), "17l >> 28");
		assertEqual("0", String.valueOf(longData[2] >>> 28), "17l >>> 28");
		assertEqual("4563402752", String.valueOf(longData[2] << 28),
				"17l << 28");
		assertEqual("0", String.valueOf(longData[2] >> 29), "17l >> 29");
		assertEqual("0", String.valueOf(longData[2] >>> 29), "17l >>> 29");
		assertEqual("9126805504", String.valueOf(longData[2] << 29),
				"17l << 29");
		assertEqual("0", String.valueOf(longData[2] >> 30), "17l >> 30");
		assertEqual("0", String.valueOf(longData[2] >>> 30), "17l >>> 30");
		assertEqual("18253611008", String.valueOf(longData[2] << 30),
				"17l << 30");
		assertEqual("0", String.valueOf(longData[2] >> 31), "17l >> 31");
		assertEqual("0", String.valueOf(longData[2] >>> 31), "17l >>> 31");
		assertEqual("36507222016", String.valueOf(longData[2] << 31),
				"17l << 31");
		assertEqual("0", String.valueOf(longData[2] >> 32), "17l >> 32");
		assertEqual("0", String.valueOf(longData[2] >>> 32), "17l >>> 32");
		assertEqual("73014444032", String.valueOf(longData[2] << 32),
				"17l << 32");
		assertEqual("0", String.valueOf(longData[2] >> 33), "17l >> 33");
		assertEqual("0", String.valueOf(longData[2] >>> 33), "17l >>> 33");
		assertEqual("146028888064", String.valueOf(longData[2] << 33),
				"17l << 33");
		assertEqual("0", String.valueOf(longData[2] >> 34), "17l >> 34");
		assertEqual("0", String.valueOf(longData[2] >>> 34), "17l >>> 34");
		assertEqual("292057776128", String.valueOf(longData[2] << 34),
				"17l << 34");
		assertEqual("0", String.valueOf(longData[2] >> 35), "17l >> 35");
		assertEqual("0", String.valueOf(longData[2] >>> 35), "17l >>> 35");
		assertEqual("584115552256", String.valueOf(longData[2] << 35),
				"17l << 35");
		assertEqual("0", String.valueOf(longData[2] >> 36), "17l >> 36");
		assertEqual("0", String.valueOf(longData[2] >>> 36), "17l >>> 36");
		assertEqual("1168231104512", String.valueOf(longData[2] << 36),
				"17l << 36");
		assertEqual("0", String.valueOf(longData[2] >> 37), "17l >> 37");
		assertEqual("0", String.valueOf(longData[2] >>> 37), "17l >>> 37");
		assertEqual("2336462209024", String.valueOf(longData[2] << 37),
				"17l << 37");
		assertEqual("0", String.valueOf(longData[2] >> 38), "17l >> 38");
		assertEqual("0", String.valueOf(longData[2] >>> 38), "17l >>> 38");
		assertEqual("4672924418048", String.valueOf(longData[2] << 38),
				"17l << 38");
		assertEqual("0", String.valueOf(longData[2] >> 39), "17l >> 39");
		assertEqual("0", String.valueOf(longData[2] >>> 39), "17l >>> 39");
		assertEqual("9345848836096", String.valueOf(longData[2] << 39),
				"17l << 39");
		assertEqual("0", String.valueOf(longData[2] >> 40), "17l >> 40");
		assertEqual("0", String.valueOf(longData[2] >>> 40), "17l >>> 40");
		assertEqual("18691697672192", String.valueOf(longData[2] << 40),
				"17l << 40");
		assertEqual("0", String.valueOf(longData[2] >> 41), "17l >> 41");
		assertEqual("0", String.valueOf(longData[2] >>> 41), "17l >>> 41");
		assertEqual("37383395344384", String.valueOf(longData[2] << 41),
				"17l << 41");
		assertEqual("0", String.valueOf(longData[2] >> 42), "17l >> 42");
		assertEqual("0", String.valueOf(longData[2] >>> 42), "17l >>> 42");
		assertEqual("74766790688768", String.valueOf(longData[2] << 42),
				"17l << 42");
		assertEqual("0", String.valueOf(longData[2] >> 43), "17l >> 43");
		assertEqual("0", String.valueOf(longData[2] >>> 43), "17l >>> 43");
		assertEqual("149533581377536", String.valueOf(longData[2] << 43),
				"17l << 43");
		assertEqual("0", String.valueOf(longData[2] >> 44), "17l >> 44");
		assertEqual("0", String.valueOf(longData[2] >>> 44), "17l >>> 44");
		assertEqual("299067162755072", String.valueOf(longData[2] << 44),
				"17l << 44");
		assertEqual("0", String.valueOf(longData[2] >> 45), "17l >> 45");
		assertEqual("0", String.valueOf(longData[2] >>> 45), "17l >>> 45");
		assertEqual("598134325510144", String.valueOf(longData[2] << 45),
				"17l << 45");
		assertEqual("0", String.valueOf(longData[2] >> 46), "17l >> 46");
		assertEqual("0", String.valueOf(longData[2] >>> 46), "17l >>> 46");
		assertEqual("1196268651020288", String.valueOf(longData[2] << 46),
				"17l << 46");
		assertEqual("0", String.valueOf(longData[2] >> 47), "17l >> 47");
		assertEqual("0", String.valueOf(longData[2] >>> 47), "17l >>> 47");
		assertEqual("2392537302040576", String.valueOf(longData[2] << 47),
				"17l << 47");
		assertEqual("0", String.valueOf(longData[2] >> 48), "17l >> 48");
		assertEqual("0", String.valueOf(longData[2] >>> 48), "17l >>> 48");
		assertEqual("4785074604081152", String.valueOf(longData[2] << 48),
				"17l << 48");
		assertEqual("0", String.valueOf(longData[2] >> 49), "17l >> 49");
		assertEqual("0", String.valueOf(longData[2] >>> 49), "17l >>> 49");
		assertEqual("9570149208162304", String.valueOf(longData[2] << 49),
				"17l << 49");
		assertEqual("0", String.valueOf(longData[2] >> 50), "17l >> 50");
		assertEqual("0", String.valueOf(longData[2] >>> 50), "17l >>> 50");
		assertEqual("19140298416324608", String.valueOf(longData[2] << 50),
				"17l << 50");
		assertEqual("0", String.valueOf(longData[2] >> 51), "17l >> 51");
		assertEqual("0", String.valueOf(longData[2] >>> 51), "17l >>> 51");
		assertEqual("38280596832649216", String.valueOf(longData[2] << 51),
				"17l << 51");
		assertEqual("0", String.valueOf(longData[2] >> 52), "17l >> 52");
		assertEqual("0", String.valueOf(longData[2] >>> 52), "17l >>> 52");
		assertEqual("76561193665298432", String.valueOf(longData[2] << 52),
				"17l << 52");
		assertEqual("0", String.valueOf(longData[2] >> 53), "17l >> 53");
		assertEqual("0", String.valueOf(longData[2] >>> 53), "17l >>> 53");
		assertEqual("153122387330596864", String.valueOf(longData[2] << 53),
				"17l << 53");
		assertEqual("0", String.valueOf(longData[2] >> 54), "17l >> 54");
		assertEqual("0", String.valueOf(longData[2] >>> 54), "17l >>> 54");
		assertEqual("306244774661193728", String.valueOf(longData[2] << 54),
				"17l << 54");
		assertEqual("0", String.valueOf(longData[2] >> 55), "17l >> 55");
		assertEqual("0", String.valueOf(longData[2] >>> 55), "17l >>> 55");
		assertEqual("612489549322387456", String.valueOf(longData[2] << 55),
				"17l << 55");
		assertEqual("0", String.valueOf(longData[2] >> 56), "17l >> 56");
		assertEqual("0", String.valueOf(longData[2] >>> 56), "17l >>> 56");
		assertEqual("1224979098644774912", String.valueOf(longData[2] << 56),
				"17l << 56");
		assertEqual("0", String.valueOf(longData[2] >> 57), "17l >> 57");
		assertEqual("0", String.valueOf(longData[2] >>> 57), "17l >>> 57");
		assertEqual("2449958197289549824", String.valueOf(longData[2] << 57),
				"17l << 57");
		assertEqual("0", String.valueOf(longData[2] >> 58), "17l >> 58");
		assertEqual("0", String.valueOf(longData[2] >>> 58), "17l >>> 58");
		assertEqual("4899916394579099648", String.valueOf(longData[2] << 58),
				"17l << 58");
		assertEqual("0", String.valueOf(longData[2] >> 59), "17l >> 59");
		assertEqual("0", String.valueOf(longData[2] >>> 59), "17l >>> 59");
		assertEqual("-8646911284551352320", String.valueOf(longData[2] << 59),
				"17l << 59");
		assertEqual("0", String.valueOf(longData[2] >> 60), "17l >> 60");
		assertEqual("0", String.valueOf(longData[2] >>> 60), "17l >>> 60");
		assertEqual("1152921504606846976", String.valueOf(longData[2] << 60),
				"17l << 60");
		assertEqual("0", String.valueOf(longData[2] >> 61), "17l >> 61");
		assertEqual("0", String.valueOf(longData[2] >>> 61), "17l >>> 61");
		assertEqual("2305843009213693952", String.valueOf(longData[2] << 61),
				"17l << 61");
		assertEqual("0", String.valueOf(longData[2] >> 62), "17l >> 62");
		assertEqual("0", String.valueOf(longData[2] >>> 62), "17l >>> 62");
		assertEqual("4611686018427387904", String.valueOf(longData[2] << 62),
				"17l << 62");
		assertEqual("0", String.valueOf(longData[2] >> 63), "17l >> 63");
		assertEqual("0", String.valueOf(longData[2] >>> 63), "17l >>> 63");
		assertEqual("-9223372036854775808", String.valueOf(longData[2] << 63),
				"17l << 63");
		assertEqual("18", String.valueOf(longData[2] + longData[0]), "17l + 1l");
		assertEqual("16", String.valueOf(longData[2] - longData[0]), "17l - 1l");
		assertEqual("17", String.valueOf(longData[2] * longData[0]), "17l * 1l");
		assertEqual("17", String.valueOf(longData[2] / longData[0]), "17l / 1l");
		assertEqual("0", String.valueOf(longData[2] % longData[0]), "17l % 1l");
		assertEqual("1", String.valueOf(longData[2] & longData[0]), "17l & 1l");
		assertEqual("17", String.valueOf(longData[2] | longData[0]), "17l | 1l");
		assertEqual("16", String.valueOf(longData[2] ^ longData[0]), "17l ^ 1l");
		assertEqual("true", String.valueOf(longData[2] > longData[0]),
				"17l > 1l");
		assertEqual("false", String.valueOf(longData[2] < longData[0]),
				"17l < 1l");
		assertEqual("false", String.valueOf(longData[2] == longData[0]),
				"17l == 1l");
		assertEqual("true", String.valueOf(longData[2] >= longData[0]),
				"17l >= 1l");
		assertEqual("false", String.valueOf(longData[2] <= longData[0]),
				"17l <= 1l");
		assertEqual("16", String.valueOf(longData[2] + longData[1]),
				"17l + -1l");
		assertEqual("18", String.valueOf(longData[2] - longData[1]),
				"17l - -1l");
		assertEqual("-17", String.valueOf(longData[2] * longData[1]),
				"17l * -1l");
		assertEqual("-17", String.valueOf(longData[2] / longData[1]),
				"17l / -1l");
		assertEqual("0", String.valueOf(longData[2] % longData[1]), "17l % -1l");
		assertEqual("17", String.valueOf(longData[2] & longData[1]),
				"17l & -1l");
		assertEqual("-1", String.valueOf(longData[2] | longData[1]),
				"17l | -1l");
		assertEqual("-18", String.valueOf(longData[2] ^ longData[1]),
				"17l ^ -1l");
		assertEqual("true", String.valueOf(longData[2] > longData[1]),
				"17l > -1l");
		assertEqual("false", String.valueOf(longData[2] < longData[1]),
				"17l < -1l");
		assertEqual("false", String.valueOf(longData[2] == longData[1]),
				"17l == -1l");
		assertEqual("true", String.valueOf(longData[2] >= longData[1]),
				"17l >= -1l");
		assertEqual("false", String.valueOf(longData[2] <= longData[1]),
				"17l <= -1l");
		assertEqual("34", String.valueOf(longData[2] + longData[2]),
				"17l + 17l");
		assertEqual("0", String.valueOf(longData[2] - longData[2]), "17l - 17l");
		assertEqual("289", String.valueOf(longData[2] * longData[2]),
				"17l * 17l");
		assertEqual("1", String.valueOf(longData[2] / longData[2]), "17l / 17l");
		assertEqual("0", String.valueOf(longData[2] % longData[2]), "17l % 17l");
		assertEqual("17", String.valueOf(longData[2] & longData[2]),
				"17l & 17l");
		assertEqual("17", String.valueOf(longData[2] | longData[2]),
				"17l | 17l");
		assertEqual("0", String.valueOf(longData[2] ^ longData[2]), "17l ^ 17l");
		assertEqual("false", String.valueOf(longData[2] > longData[2]),
				"17l > 17l");
		assertEqual("false", String.valueOf(longData[2] < longData[2]),
				"17l < 17l");
		assertEqual("true", String.valueOf(longData[2] == longData[2]),
				"17l == 17l");
		assertEqual("true", String.valueOf(longData[2] >= longData[2]),
				"17l >= 17l");
		assertEqual("true", String.valueOf(longData[2] <= longData[2]),
				"17l <= 17l");
		assertEqual("0", String.valueOf(longData[2] + longData[3]),
				"17l + -17l");
		assertEqual("34", String.valueOf(longData[2] - longData[3]),
				"17l - -17l");
		assertEqual("-289", String.valueOf(longData[2] * longData[3]),
				"17l * -17l");
		assertEqual("-1", String.valueOf(longData[2] / longData[3]),
				"17l / -17l");
		assertEqual("0", String.valueOf(longData[2] % longData[3]),
				"17l % -17l");
		assertEqual("1", String.valueOf(longData[2] & longData[3]),
				"17l & -17l");
		assertEqual("-1", String.valueOf(longData[2] | longData[3]),
				"17l | -17l");
		assertEqual("-2", String.valueOf(longData[2] ^ longData[3]),
				"17l ^ -17l");
		assertEqual("true", String.valueOf(longData[2] > longData[3]),
				"17l > -17l");
		assertEqual("false", String.valueOf(longData[2] < longData[3]),
				"17l < -17l");
		assertEqual("false", String.valueOf(longData[2] == longData[3]),
				"17l == -17l");
		assertEqual("true", String.valueOf(longData[2] >= longData[3]),
				"17l >= -17l");
		assertEqual("false", String.valueOf(longData[2] <= longData[3]),
				"17l <= -17l");
		assertEqual("4026593297", String.valueOf(longData[2] + longData[4]),
				"17l + 4026593280l");
		assertEqual("-4026593263", String.valueOf(longData[2] - longData[4]),
				"17l - 4026593280l");
		assertEqual("68452085760", String.valueOf(longData[2] * longData[4]),
				"17l * 4026593280l");
		assertEqual("0", String.valueOf(longData[2] / longData[4]),
				"17l / 4026593280l");
		assertEqual("17", String.valueOf(longData[2] % longData[4]),
				"17l % 4026593280l");
		assertEqual("0", String.valueOf(longData[2] & longData[4]),
				"17l & 4026593280l");
		assertEqual("4026593297", String.valueOf(longData[2] | longData[4]),
				"17l | 4026593280l");
		assertEqual("4026593297", String.valueOf(longData[2] ^ longData[4]),
				"17l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[2] > longData[4]),
				"17l > 4026593280l");
		assertEqual("true", String.valueOf(longData[2] < longData[4]),
				"17l < 4026593280l");
		assertEqual("false", String.valueOf(longData[2] == longData[4]),
				"17l == 4026593280l");
		assertEqual("false", String.valueOf(longData[2] >= longData[4]),
				"17l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[2] <= longData[4]),
				"17l <= 4026593280l");
		assertEqual("251662097", String.valueOf(longData[2] + longData[5]),
				"17l + 251662080l");
		assertEqual("-251662063", String.valueOf(longData[2] - longData[5]),
				"17l - 251662080l");
		assertEqual("4278255360", String.valueOf(longData[2] * longData[5]),
				"17l * 251662080l");
		assertEqual("0", String.valueOf(longData[2] / longData[5]),
				"17l / 251662080l");
		assertEqual("17", String.valueOf(longData[2] % longData[5]),
				"17l % 251662080l");
		assertEqual("0", String.valueOf(longData[2] & longData[5]),
				"17l & 251662080l");
		assertEqual("251662097", String.valueOf(longData[2] | longData[5]),
				"17l | 251662080l");
		assertEqual("251662097", String.valueOf(longData[2] ^ longData[5]),
				"17l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[2] > longData[5]),
				"17l > 251662080l");
		assertEqual("true", String.valueOf(longData[2] < longData[5]),
				"17l < 251662080l");
		assertEqual("false", String.valueOf(longData[2] == longData[5]),
				"17l == 251662080l");
		assertEqual("false", String.valueOf(longData[2] >= longData[5]),
				"17l >= 251662080l");
		assertEqual("true", String.valueOf(longData[2] <= longData[5]),
				"17l <= 251662080l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[2] + longData[6]),
				"17l + -1152657617789587456l");
		assertEqual("1152657617789587473",
				String.valueOf(longData[2] - longData[6]),
				"17l - -1152657617789587456l");
		assertEqual("-1148435428713435136",
				String.valueOf(longData[2] * longData[6]),
				"17l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[2] / longData[6]),
				"17l / -1152657617789587456l");
		assertEqual("17", String.valueOf(longData[2] % longData[6]),
				"17l % -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[2] & longData[6]),
				"17l & -1152657617789587456l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[2] | longData[6]),
				"17l | -1152657617789587456l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[2] ^ longData[6]),
				"17l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[2] > longData[6]),
				"17l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[2] < longData[6]),
				"17l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[2] == longData[6]),
				"17l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[2] >= longData[6]),
				"17l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[2] <= longData[6]),
				"17l <= -1152657617789587456l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[2] + longData[7]),
				"17l + 1080880403494997760l");
		assertEqual("-1080880403494997743",
				String.valueOf(longData[2] - longData[7]),
				"17l - 1080880403494997760l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[2] * longData[7]),
				"17l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[2] / longData[7]),
				"17l / 1080880403494997760l");
		assertEqual("17", String.valueOf(longData[2] % longData[7]),
				"17l % 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[2] & longData[7]),
				"17l & 1080880403494997760l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[2] | longData[7]),
				"17l | 1080880403494997760l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[2] ^ longData[7]),
				"17l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[2] > longData[7]),
				"17l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[2] < longData[7]),
				"17l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[2] == longData[7]),
				"17l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[2] >= longData[7]),
				"17l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[2] <= longData[7]),
				"17l <= 1080880403494997760l");
		assertEqual("-17", String.valueOf((int) longData[3]), "l2i -17l");
		assertEqual("-17.0", String.valueOf((float) longData[3]), "l2f -17l");
		assertEqual("-17.0", String.valueOf((double) longData[3]), "l2d -17l");
		assertEqual("17", String.valueOf(-longData[3]), "lneg -17l");
		assertEqual("-17", String.valueOf(longData[3] >> 0), "-17l >> 0");
		assertEqual("-17", String.valueOf(longData[3] >>> 0), "-17l >>> 0");
		assertEqual("-17", String.valueOf(longData[3] << 0), "-17l << 0");
		assertEqual("-9", String.valueOf(longData[3] >> 1), "-17l >> 1");
		assertEqual("9223372036854775799", String.valueOf(longData[3] >>> 1),
				"-17l >>> 1");
		assertEqual("-34", String.valueOf(longData[3] << 1), "-17l << 1");
		assertEqual("-5", String.valueOf(longData[3] >> 2), "-17l >> 2");
		assertEqual("4611686018427387899", String.valueOf(longData[3] >>> 2),
				"-17l >>> 2");
		assertEqual("-68", String.valueOf(longData[3] << 2), "-17l << 2");
		assertEqual("-3", String.valueOf(longData[3] >> 3), "-17l >> 3");
		assertEqual("2305843009213693949", String.valueOf(longData[3] >>> 3),
				"-17l >>> 3");
		assertEqual("-136", String.valueOf(longData[3] << 3), "-17l << 3");
		assertEqual("-2", String.valueOf(longData[3] >> 4), "-17l >> 4");
		assertEqual("1152921504606846974", String.valueOf(longData[3] >>> 4),
				"-17l >>> 4");
		assertEqual("-272", String.valueOf(longData[3] << 4), "-17l << 4");
		assertEqual("-1", String.valueOf(longData[3] >> 5), "-17l >> 5");
		assertEqual("576460752303423487", String.valueOf(longData[3] >>> 5),
				"-17l >>> 5");
		assertEqual("-544", String.valueOf(longData[3] << 5), "-17l << 5");
		assertEqual("-1", String.valueOf(longData[3] >> 6), "-17l >> 6");
		assertEqual("288230376151711743", String.valueOf(longData[3] >>> 6),
				"-17l >>> 6");
		assertEqual("-1088", String.valueOf(longData[3] << 6), "-17l << 6");
		assertEqual("-1", String.valueOf(longData[3] >> 7), "-17l >> 7");
		assertEqual("144115188075855871", String.valueOf(longData[3] >>> 7),
				"-17l >>> 7");
		assertEqual("-2176", String.valueOf(longData[3] << 7), "-17l << 7");
		assertEqual("-1", String.valueOf(longData[3] >> 8), "-17l >> 8");
		assertEqual("72057594037927935", String.valueOf(longData[3] >>> 8),
				"-17l >>> 8");
		assertEqual("-4352", String.valueOf(longData[3] << 8), "-17l << 8");
		assertEqual("-1", String.valueOf(longData[3] >> 9), "-17l >> 9");
		assertEqual("36028797018963967", String.valueOf(longData[3] >>> 9),
				"-17l >>> 9");
		assertEqual("-8704", String.valueOf(longData[3] << 9), "-17l << 9");
		assertEqual("-1", String.valueOf(longData[3] >> 10), "-17l >> 10");
		assertEqual("18014398509481983", String.valueOf(longData[3] >>> 10),
				"-17l >>> 10");
		assertEqual("-17408", String.valueOf(longData[3] << 10), "-17l << 10");
		assertEqual("-1", String.valueOf(longData[3] >> 11), "-17l >> 11");
		assertEqual("9007199254740991", String.valueOf(longData[3] >>> 11),
				"-17l >>> 11");
		assertEqual("-34816", String.valueOf(longData[3] << 11), "-17l << 11");
		assertEqual("-1", String.valueOf(longData[3] >> 12), "-17l >> 12");
		assertEqual("4503599627370495", String.valueOf(longData[3] >>> 12),
				"-17l >>> 12");
		assertEqual("-69632", String.valueOf(longData[3] << 12), "-17l << 12");
		assertEqual("-1", String.valueOf(longData[3] >> 13), "-17l >> 13");
		assertEqual("2251799813685247", String.valueOf(longData[3] >>> 13),
				"-17l >>> 13");
		assertEqual("-139264", String.valueOf(longData[3] << 13), "-17l << 13");
		assertEqual("-1", String.valueOf(longData[3] >> 14), "-17l >> 14");
		assertEqual("1125899906842623", String.valueOf(longData[3] >>> 14),
				"-17l >>> 14");
		assertEqual("-278528", String.valueOf(longData[3] << 14), "-17l << 14");
		assertEqual("-1", String.valueOf(longData[3] >> 15), "-17l >> 15");
		assertEqual("562949953421311", String.valueOf(longData[3] >>> 15),
				"-17l >>> 15");
		assertEqual("-557056", String.valueOf(longData[3] << 15), "-17l << 15");
		assertEqual("-1", String.valueOf(longData[3] >> 16), "-17l >> 16");
		assertEqual("281474976710655", String.valueOf(longData[3] >>> 16),
				"-17l >>> 16");
		assertEqual("-1114112", String.valueOf(longData[3] << 16), "-17l << 16");
		assertEqual("-1", String.valueOf(longData[3] >> 17), "-17l >> 17");
		assertEqual("140737488355327", String.valueOf(longData[3] >>> 17),
				"-17l >>> 17");
		assertEqual("-2228224", String.valueOf(longData[3] << 17), "-17l << 17");
		assertEqual("-1", String.valueOf(longData[3] >> 18), "-17l >> 18");
		assertEqual("70368744177663", String.valueOf(longData[3] >>> 18),
				"-17l >>> 18");
		assertEqual("-4456448", String.valueOf(longData[3] << 18), "-17l << 18");
		assertEqual("-1", String.valueOf(longData[3] >> 19), "-17l >> 19");
		assertEqual("35184372088831", String.valueOf(longData[3] >>> 19),
				"-17l >>> 19");
		assertEqual("-8912896", String.valueOf(longData[3] << 19), "-17l << 19");
		assertEqual("-1", String.valueOf(longData[3] >> 20), "-17l >> 20");
		assertEqual("17592186044415", String.valueOf(longData[3] >>> 20),
				"-17l >>> 20");
		assertEqual("-17825792", String.valueOf(longData[3] << 20),
				"-17l << 20");
		assertEqual("-1", String.valueOf(longData[3] >> 21), "-17l >> 21");
		assertEqual("8796093022207", String.valueOf(longData[3] >>> 21),
				"-17l >>> 21");
		assertEqual("-35651584", String.valueOf(longData[3] << 21),
				"-17l << 21");
		assertEqual("-1", String.valueOf(longData[3] >> 22), "-17l >> 22");
		assertEqual("4398046511103", String.valueOf(longData[3] >>> 22),
				"-17l >>> 22");
		assertEqual("-71303168", String.valueOf(longData[3] << 22),
				"-17l << 22");
		assertEqual("-1", String.valueOf(longData[3] >> 23), "-17l >> 23");
		assertEqual("2199023255551", String.valueOf(longData[3] >>> 23),
				"-17l >>> 23");
		assertEqual("-142606336", String.valueOf(longData[3] << 23),
				"-17l << 23");
		assertEqual("-1", String.valueOf(longData[3] >> 24), "-17l >> 24");
		assertEqual("1099511627775", String.valueOf(longData[3] >>> 24),
				"-17l >>> 24");
		assertEqual("-285212672", String.valueOf(longData[3] << 24),
				"-17l << 24");
		assertEqual("-1", String.valueOf(longData[3] >> 25), "-17l >> 25");
		assertEqual("549755813887", String.valueOf(longData[3] >>> 25),
				"-17l >>> 25");
		assertEqual("-570425344", String.valueOf(longData[3] << 25),
				"-17l << 25");
		assertEqual("-1", String.valueOf(longData[3] >> 26), "-17l >> 26");
		assertEqual("274877906943", String.valueOf(longData[3] >>> 26),
				"-17l >>> 26");
		assertEqual("-1140850688", String.valueOf(longData[3] << 26),
				"-17l << 26");
		assertEqual("-1", String.valueOf(longData[3] >> 27), "-17l >> 27");
		assertEqual("137438953471", String.valueOf(longData[3] >>> 27),
				"-17l >>> 27");
		assertEqual("-2281701376", String.valueOf(longData[3] << 27),
				"-17l << 27");
		assertEqual("-1", String.valueOf(longData[3] >> 28), "-17l >> 28");
		assertEqual("68719476735", String.valueOf(longData[3] >>> 28),
				"-17l >>> 28");
		assertEqual("-4563402752", String.valueOf(longData[3] << 28),
				"-17l << 28");
		assertEqual("-1", String.valueOf(longData[3] >> 29), "-17l >> 29");
		assertEqual("34359738367", String.valueOf(longData[3] >>> 29),
				"-17l >>> 29");
		assertEqual("-9126805504", String.valueOf(longData[3] << 29),
				"-17l << 29");
		assertEqual("-1", String.valueOf(longData[3] >> 30), "-17l >> 30");
		assertEqual("17179869183", String.valueOf(longData[3] >>> 30),
				"-17l >>> 30");
		assertEqual("-18253611008", String.valueOf(longData[3] << 30),
				"-17l << 30");
		assertEqual("-1", String.valueOf(longData[3] >> 31), "-17l >> 31");
		assertEqual("8589934591", String.valueOf(longData[3] >>> 31),
				"-17l >>> 31");
		assertEqual("-36507222016", String.valueOf(longData[3] << 31),
				"-17l << 31");
		assertEqual("-1", String.valueOf(longData[3] >> 32), "-17l >> 32");
		assertEqual("4294967295", String.valueOf(longData[3] >>> 32),
				"-17l >>> 32");
		assertEqual("-73014444032", String.valueOf(longData[3] << 32),
				"-17l << 32");
		assertEqual("-1", String.valueOf(longData[3] >> 33), "-17l >> 33");
		assertEqual("2147483647", String.valueOf(longData[3] >>> 33),
				"-17l >>> 33");
		assertEqual("-146028888064", String.valueOf(longData[3] << 33),
				"-17l << 33");
		assertEqual("-1", String.valueOf(longData[3] >> 34), "-17l >> 34");
		assertEqual("1073741823", String.valueOf(longData[3] >>> 34),
				"-17l >>> 34");
		assertEqual("-292057776128", String.valueOf(longData[3] << 34),
				"-17l << 34");
		assertEqual("-1", String.valueOf(longData[3] >> 35), "-17l >> 35");
		assertEqual("536870911", String.valueOf(longData[3] >>> 35),
				"-17l >>> 35");
		assertEqual("-584115552256", String.valueOf(longData[3] << 35),
				"-17l << 35");
		assertEqual("-1", String.valueOf(longData[3] >> 36), "-17l >> 36");
		assertEqual("268435455", String.valueOf(longData[3] >>> 36),
				"-17l >>> 36");
		assertEqual("-1168231104512", String.valueOf(longData[3] << 36),
				"-17l << 36");
		assertEqual("-1", String.valueOf(longData[3] >> 37), "-17l >> 37");
		assertEqual("134217727", String.valueOf(longData[3] >>> 37),
				"-17l >>> 37");
		assertEqual("-2336462209024", String.valueOf(longData[3] << 37),
				"-17l << 37");
		assertEqual("-1", String.valueOf(longData[3] >> 38), "-17l >> 38");
		assertEqual("67108863", String.valueOf(longData[3] >>> 38),
				"-17l >>> 38");
		assertEqual("-4672924418048", String.valueOf(longData[3] << 38),
				"-17l << 38");
		assertEqual("-1", String.valueOf(longData[3] >> 39), "-17l >> 39");
		assertEqual("33554431", String.valueOf(longData[3] >>> 39),
				"-17l >>> 39");
		assertEqual("-9345848836096", String.valueOf(longData[3] << 39),
				"-17l << 39");
		assertEqual("-1", String.valueOf(longData[3] >> 40), "-17l >> 40");
		assertEqual("16777215", String.valueOf(longData[3] >>> 40),
				"-17l >>> 40");
		assertEqual("-18691697672192", String.valueOf(longData[3] << 40),
				"-17l << 40");
		assertEqual("-1", String.valueOf(longData[3] >> 41), "-17l >> 41");
		assertEqual("8388607", String.valueOf(longData[3] >>> 41),
				"-17l >>> 41");
		assertEqual("-37383395344384", String.valueOf(longData[3] << 41),
				"-17l << 41");
		assertEqual("-1", String.valueOf(longData[3] >> 42), "-17l >> 42");
		assertEqual("4194303", String.valueOf(longData[3] >>> 42),
				"-17l >>> 42");
		assertEqual("-74766790688768", String.valueOf(longData[3] << 42),
				"-17l << 42");
		assertEqual("-1", String.valueOf(longData[3] >> 43), "-17l >> 43");
		assertEqual("2097151", String.valueOf(longData[3] >>> 43),
				"-17l >>> 43");
		assertEqual("-149533581377536", String.valueOf(longData[3] << 43),
				"-17l << 43");
		assertEqual("-1", String.valueOf(longData[3] >> 44), "-17l >> 44");
		assertEqual("1048575", String.valueOf(longData[3] >>> 44),
				"-17l >>> 44");
		assertEqual("-299067162755072", String.valueOf(longData[3] << 44),
				"-17l << 44");
		assertEqual("-1", String.valueOf(longData[3] >> 45), "-17l >> 45");
		assertEqual("524287", String.valueOf(longData[3] >>> 45), "-17l >>> 45");
		assertEqual("-598134325510144", String.valueOf(longData[3] << 45),
				"-17l << 45");
		assertEqual("-1", String.valueOf(longData[3] >> 46), "-17l >> 46");
		assertEqual("262143", String.valueOf(longData[3] >>> 46), "-17l >>> 46");
		assertEqual("-1196268651020288", String.valueOf(longData[3] << 46),
				"-17l << 46");
		assertEqual("-1", String.valueOf(longData[3] >> 47), "-17l >> 47");
		assertEqual("131071", String.valueOf(longData[3] >>> 47), "-17l >>> 47");
		assertEqual("-2392537302040576", String.valueOf(longData[3] << 47),
				"-17l << 47");
		assertEqual("-1", String.valueOf(longData[3] >> 48), "-17l >> 48");
		assertEqual("65535", String.valueOf(longData[3] >>> 48), "-17l >>> 48");
		assertEqual("-4785074604081152", String.valueOf(longData[3] << 48),
				"-17l << 48");
		assertEqual("-1", String.valueOf(longData[3] >> 49), "-17l >> 49");
		assertEqual("32767", String.valueOf(longData[3] >>> 49), "-17l >>> 49");
		assertEqual("-9570149208162304", String.valueOf(longData[3] << 49),
				"-17l << 49");
		assertEqual("-1", String.valueOf(longData[3] >> 50), "-17l >> 50");
		assertEqual("16383", String.valueOf(longData[3] >>> 50), "-17l >>> 50");
		assertEqual("-19140298416324608", String.valueOf(longData[3] << 50),
				"-17l << 50");
		assertEqual("-1", String.valueOf(longData[3] >> 51), "-17l >> 51");
		assertEqual("8191", String.valueOf(longData[3] >>> 51), "-17l >>> 51");
		assertEqual("-38280596832649216", String.valueOf(longData[3] << 51),
				"-17l << 51");
		assertEqual("-1", String.valueOf(longData[3] >> 52), "-17l >> 52");
		assertEqual("4095", String.valueOf(longData[3] >>> 52), "-17l >>> 52");
		assertEqual("-76561193665298432", String.valueOf(longData[3] << 52),
				"-17l << 52");
		assertEqual("-1", String.valueOf(longData[3] >> 53), "-17l >> 53");
		assertEqual("2047", String.valueOf(longData[3] >>> 53), "-17l >>> 53");
		assertEqual("-153122387330596864", String.valueOf(longData[3] << 53),
				"-17l << 53");
		assertEqual("-1", String.valueOf(longData[3] >> 54), "-17l >> 54");
		assertEqual("1023", String.valueOf(longData[3] >>> 54), "-17l >>> 54");
		assertEqual("-306244774661193728", String.valueOf(longData[3] << 54),
				"-17l << 54");
		assertEqual("-1", String.valueOf(longData[3] >> 55), "-17l >> 55");
		assertEqual("511", String.valueOf(longData[3] >>> 55), "-17l >>> 55");
		assertEqual("-612489549322387456", String.valueOf(longData[3] << 55),
				"-17l << 55");
		assertEqual("-1", String.valueOf(longData[3] >> 56), "-17l >> 56");
		assertEqual("255", String.valueOf(longData[3] >>> 56), "-17l >>> 56");
		assertEqual("-1224979098644774912", String.valueOf(longData[3] << 56),
				"-17l << 56");
		assertEqual("-1", String.valueOf(longData[3] >> 57), "-17l >> 57");
		assertEqual("127", String.valueOf(longData[3] >>> 57), "-17l >>> 57");
		assertEqual("-2449958197289549824", String.valueOf(longData[3] << 57),
				"-17l << 57");
		assertEqual("-1", String.valueOf(longData[3] >> 58), "-17l >> 58");
		assertEqual("63", String.valueOf(longData[3] >>> 58), "-17l >>> 58");
		assertEqual("-4899916394579099648", String.valueOf(longData[3] << 58),
				"-17l << 58");
		assertEqual("-1", String.valueOf(longData[3] >> 59), "-17l >> 59");
		assertEqual("31", String.valueOf(longData[3] >>> 59), "-17l >>> 59");
		assertEqual("8646911284551352320", String.valueOf(longData[3] << 59),
				"-17l << 59");
		assertEqual("-1", String.valueOf(longData[3] >> 60), "-17l >> 60");
		assertEqual("15", String.valueOf(longData[3] >>> 60), "-17l >>> 60");
		assertEqual("-1152921504606846976", String.valueOf(longData[3] << 60),
				"-17l << 60");
		assertEqual("-1", String.valueOf(longData[3] >> 61), "-17l >> 61");
		assertEqual("7", String.valueOf(longData[3] >>> 61), "-17l >>> 61");
		assertEqual("-2305843009213693952", String.valueOf(longData[3] << 61),
				"-17l << 61");
		assertEqual("-1", String.valueOf(longData[3] >> 62), "-17l >> 62");
		assertEqual("3", String.valueOf(longData[3] >>> 62), "-17l >>> 62");
		assertEqual("-4611686018427387904", String.valueOf(longData[3] << 62),
				"-17l << 62");
		assertEqual("-1", String.valueOf(longData[3] >> 63), "-17l >> 63");
		assertEqual("1", String.valueOf(longData[3] >>> 63), "-17l >>> 63");
		assertEqual("-9223372036854775808", String.valueOf(longData[3] << 63),
				"-17l << 63");
		assertEqual("-16", String.valueOf(longData[3] + longData[0]),
				"-17l + 1l");
		assertEqual("-18", String.valueOf(longData[3] - longData[0]),
				"-17l - 1l");
		assertEqual("-17", String.valueOf(longData[3] * longData[0]),
				"-17l * 1l");
		assertEqual("-17", String.valueOf(longData[3] / longData[0]),
				"-17l / 1l");
		assertEqual("0", String.valueOf(longData[3] % longData[0]), "-17l % 1l");
		assertEqual("1", String.valueOf(longData[3] & longData[0]), "-17l & 1l");
		assertEqual("-17", String.valueOf(longData[3] | longData[0]),
				"-17l | 1l");
		assertEqual("-18", String.valueOf(longData[3] ^ longData[0]),
				"-17l ^ 1l");
		assertEqual("false", String.valueOf(longData[3] > longData[0]),
				"-17l > 1l");
		assertEqual("true", String.valueOf(longData[3] < longData[0]),
				"-17l < 1l");
		assertEqual("false", String.valueOf(longData[3] == longData[0]),
				"-17l == 1l");
		assertEqual("false", String.valueOf(longData[3] >= longData[0]),
				"-17l >= 1l");
		assertEqual("true", String.valueOf(longData[3] <= longData[0]),
				"-17l <= 1l");
		assertEqual("-18", String.valueOf(longData[3] + longData[1]),
				"-17l + -1l");
		assertEqual("-16", String.valueOf(longData[3] - longData[1]),
				"-17l - -1l");
		assertEqual("17", String.valueOf(longData[3] * longData[1]),
				"-17l * -1l");
		assertEqual("17", String.valueOf(longData[3] / longData[1]),
				"-17l / -1l");
		assertEqual("0", String.valueOf(longData[3] % longData[1]),
				"-17l % -1l");
		assertEqual("-17", String.valueOf(longData[3] & longData[1]),
				"-17l & -1l");
		assertEqual("-1", String.valueOf(longData[3] | longData[1]),
				"-17l | -1l");
		assertEqual("16", String.valueOf(longData[3] ^ longData[1]),
				"-17l ^ -1l");
		assertEqual("false", String.valueOf(longData[3] > longData[1]),
				"-17l > -1l");
		assertEqual("true", String.valueOf(longData[3] < longData[1]),
				"-17l < -1l");
		assertEqual("false", String.valueOf(longData[3] == longData[1]),
				"-17l == -1l");
		assertEqual("false", String.valueOf(longData[3] >= longData[1]),
				"-17l >= -1l");
		assertEqual("true", String.valueOf(longData[3] <= longData[1]),
				"-17l <= -1l");
		assertEqual("0", String.valueOf(longData[3] + longData[2]),
				"-17l + 17l");
		assertEqual("-34", String.valueOf(longData[3] - longData[2]),
				"-17l - 17l");
		assertEqual("-289", String.valueOf(longData[3] * longData[2]),
				"-17l * 17l");
		assertEqual("-1", String.valueOf(longData[3] / longData[2]),
				"-17l / 17l");
		assertEqual("0", String.valueOf(longData[3] % longData[2]),
				"-17l % 17l");
		assertEqual("1", String.valueOf(longData[3] & longData[2]),
				"-17l & 17l");
		assertEqual("-1", String.valueOf(longData[3] | longData[2]),
				"-17l | 17l");
		assertEqual("-2", String.valueOf(longData[3] ^ longData[2]),
				"-17l ^ 17l");
		assertEqual("false", String.valueOf(longData[3] > longData[2]),
				"-17l > 17l");
		assertEqual("true", String.valueOf(longData[3] < longData[2]),
				"-17l < 17l");
		assertEqual("false", String.valueOf(longData[3] == longData[2]),
				"-17l == 17l");
		assertEqual("false", String.valueOf(longData[3] >= longData[2]),
				"-17l >= 17l");
		assertEqual("true", String.valueOf(longData[3] <= longData[2]),
				"-17l <= 17l");
		assertEqual("-34", String.valueOf(longData[3] + longData[3]),
				"-17l + -17l");
		assertEqual("0", String.valueOf(longData[3] - longData[3]),
				"-17l - -17l");
		assertEqual("289", String.valueOf(longData[3] * longData[3]),
				"-17l * -17l");
		assertEqual("1", String.valueOf(longData[3] / longData[3]),
				"-17l / -17l");
		assertEqual("0", String.valueOf(longData[3] % longData[3]),
				"-17l % -17l");
		assertEqual("-17", String.valueOf(longData[3] & longData[3]),
				"-17l & -17l");
		assertEqual("-17", String.valueOf(longData[3] | longData[3]),
				"-17l | -17l");
		assertEqual("0", String.valueOf(longData[3] ^ longData[3]),
				"-17l ^ -17l");
		assertEqual("false", String.valueOf(longData[3] > longData[3]),
				"-17l > -17l");
		assertEqual("false", String.valueOf(longData[3] < longData[3]),
				"-17l < -17l");
		assertEqual("true", String.valueOf(longData[3] == longData[3]),
				"-17l == -17l");
		assertEqual("true", String.valueOf(longData[3] >= longData[3]),
				"-17l >= -17l");
		assertEqual("true", String.valueOf(longData[3] <= longData[3]),
				"-17l <= -17l");
		assertEqual("4026593263", String.valueOf(longData[3] + longData[4]),
				"-17l + 4026593280l");
		assertEqual("-4026593297", String.valueOf(longData[3] - longData[4]),
				"-17l - 4026593280l");
		assertEqual("-68452085760", String.valueOf(longData[3] * longData[4]),
				"-17l * 4026593280l");
		assertEqual("0", String.valueOf(longData[3] / longData[4]),
				"-17l / 4026593280l");
		assertEqual("-17", String.valueOf(longData[3] % longData[4]),
				"-17l % 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[3] & longData[4]),
				"-17l & 4026593280l");
		assertEqual("-17", String.valueOf(longData[3] | longData[4]),
				"-17l | 4026593280l");
		assertEqual("-4026593297", String.valueOf(longData[3] ^ longData[4]),
				"-17l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[3] > longData[4]),
				"-17l > 4026593280l");
		assertEqual("true", String.valueOf(longData[3] < longData[4]),
				"-17l < 4026593280l");
		assertEqual("false", String.valueOf(longData[3] == longData[4]),
				"-17l == 4026593280l");
		assertEqual("false", String.valueOf(longData[3] >= longData[4]),
				"-17l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[3] <= longData[4]),
				"-17l <= 4026593280l");
		assertEqual("251662063", String.valueOf(longData[3] + longData[5]),
				"-17l + 251662080l");
		assertEqual("-251662097", String.valueOf(longData[3] - longData[5]),
				"-17l - 251662080l");
		assertEqual("-4278255360", String.valueOf(longData[3] * longData[5]),
				"-17l * 251662080l");
		assertEqual("0", String.valueOf(longData[3] / longData[5]),
				"-17l / 251662080l");
		assertEqual("-17", String.valueOf(longData[3] % longData[5]),
				"-17l % 251662080l");
		assertEqual("251662080", String.valueOf(longData[3] & longData[5]),
				"-17l & 251662080l");
		assertEqual("-17", String.valueOf(longData[3] | longData[5]),
				"-17l | 251662080l");
		assertEqual("-251662097", String.valueOf(longData[3] ^ longData[5]),
				"-17l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[3] > longData[5]),
				"-17l > 251662080l");
		assertEqual("true", String.valueOf(longData[3] < longData[5]),
				"-17l < 251662080l");
		assertEqual("false", String.valueOf(longData[3] == longData[5]),
				"-17l == 251662080l");
		assertEqual("false", String.valueOf(longData[3] >= longData[5]),
				"-17l >= 251662080l");
		assertEqual("true", String.valueOf(longData[3] <= longData[5]),
				"-17l <= 251662080l");
		assertEqual("-1152657617789587473",
				String.valueOf(longData[3] + longData[6]),
				"-17l + -1152657617789587456l");
		assertEqual("1152657617789587439",
				String.valueOf(longData[3] - longData[6]),
				"-17l - -1152657617789587456l");
		assertEqual("1148435428713435136",
				String.valueOf(longData[3] * longData[6]),
				"-17l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[3] / longData[6]),
				"-17l / -1152657617789587456l");
		assertEqual("-17", String.valueOf(longData[3] % longData[6]),
				"-17l % -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[3] & longData[6]),
				"-17l & -1152657617789587456l");
		assertEqual("-17", String.valueOf(longData[3] | longData[6]),
				"-17l | -1152657617789587456l");
		assertEqual("1152657617789587439",
				String.valueOf(longData[3] ^ longData[6]),
				"-17l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[3] > longData[6]),
				"-17l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[3] < longData[6]),
				"-17l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[3] == longData[6]),
				"-17l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[3] >= longData[6]),
				"-17l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[3] <= longData[6]),
				"-17l <= -1152657617789587456l");
		assertEqual("1080880403494997743",
				String.valueOf(longData[3] + longData[7]),
				"-17l + 1080880403494997760l");
		assertEqual("-1080880403494997777",
				String.valueOf(longData[3] - longData[7]),
				"-17l - 1080880403494997760l");
		assertEqual("71777214294589696",
				String.valueOf(longData[3] * longData[7]),
				"-17l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[3] / longData[7]),
				"-17l / 1080880403494997760l");
		assertEqual("-17", String.valueOf(longData[3] % longData[7]),
				"-17l % 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[3] & longData[7]),
				"-17l & 1080880403494997760l");
		assertEqual("-17", String.valueOf(longData[3] | longData[7]),
				"-17l | 1080880403494997760l");
		assertEqual("-1080880403494997777",
				String.valueOf(longData[3] ^ longData[7]),
				"-17l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[3] > longData[7]),
				"-17l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[3] < longData[7]),
				"-17l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[3] == longData[7]),
				"-17l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[3] >= longData[7]),
				"-17l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[3] <= longData[7]),
				"-17l <= 1080880403494997760l");
		assertEqual("-268374016", String.valueOf((int) longData[4]),
				"l2i 4026593280l");
		assertEqual("4.02659328E9", String.valueOf((float) longData[4]),
				"l2f 4026593280l");
		assertEqual("4.02659328E9", String.valueOf((double) longData[4]),
				"l2d 4026593280l");
		assertEqual("-4026593280", String.valueOf(-longData[4]),
				"lneg 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[4] >> 0),
				"4026593280l >> 0");
		assertEqual("4026593280", String.valueOf(longData[4] >>> 0),
				"4026593280l >>> 0");
		assertEqual("4026593280", String.valueOf(longData[4] << 0),
				"4026593280l << 0");
		assertEqual("2013296640", String.valueOf(longData[4] >> 1),
				"4026593280l >> 1");
		assertEqual("2013296640", String.valueOf(longData[4] >>> 1),
				"4026593280l >>> 1");
		assertEqual("8053186560", String.valueOf(longData[4] << 1),
				"4026593280l << 1");
		assertEqual("1006648320", String.valueOf(longData[4] >> 2),
				"4026593280l >> 2");
		assertEqual("1006648320", String.valueOf(longData[4] >>> 2),
				"4026593280l >>> 2");
		assertEqual("16106373120", String.valueOf(longData[4] << 2),
				"4026593280l << 2");
		assertEqual("503324160", String.valueOf(longData[4] >> 3),
				"4026593280l >> 3");
		assertEqual("503324160", String.valueOf(longData[4] >>> 3),
				"4026593280l >>> 3");
		assertEqual("32212746240", String.valueOf(longData[4] << 3),
				"4026593280l << 3");
		assertEqual("251662080", String.valueOf(longData[4] >> 4),
				"4026593280l >> 4");
		assertEqual("251662080", String.valueOf(longData[4] >>> 4),
				"4026593280l >>> 4");
		assertEqual("64425492480", String.valueOf(longData[4] << 4),
				"4026593280l << 4");
		assertEqual("125831040", String.valueOf(longData[4] >> 5),
				"4026593280l >> 5");
		assertEqual("125831040", String.valueOf(longData[4] >>> 5),
				"4026593280l >>> 5");
		assertEqual("128850984960", String.valueOf(longData[4] << 5),
				"4026593280l << 5");
		assertEqual("62915520", String.valueOf(longData[4] >> 6),
				"4026593280l >> 6");
		assertEqual("62915520", String.valueOf(longData[4] >>> 6),
				"4026593280l >>> 6");
		assertEqual("257701969920", String.valueOf(longData[4] << 6),
				"4026593280l << 6");
		assertEqual("31457760", String.valueOf(longData[4] >> 7),
				"4026593280l >> 7");
		assertEqual("31457760", String.valueOf(longData[4] >>> 7),
				"4026593280l >>> 7");
		assertEqual("515403939840", String.valueOf(longData[4] << 7),
				"4026593280l << 7");
		assertEqual("15728880", String.valueOf(longData[4] >> 8),
				"4026593280l >> 8");
		assertEqual("15728880", String.valueOf(longData[4] >>> 8),
				"4026593280l >>> 8");
		assertEqual("1030807879680", String.valueOf(longData[4] << 8),
				"4026593280l << 8");
		assertEqual("7864440", String.valueOf(longData[4] >> 9),
				"4026593280l >> 9");
		assertEqual("7864440", String.valueOf(longData[4] >>> 9),
				"4026593280l >>> 9");
		assertEqual("2061615759360", String.valueOf(longData[4] << 9),
				"4026593280l << 9");
		assertEqual("3932220", String.valueOf(longData[4] >> 10),
				"4026593280l >> 10");
		assertEqual("3932220", String.valueOf(longData[4] >>> 10),
				"4026593280l >>> 10");
		assertEqual("4123231518720", String.valueOf(longData[4] << 10),
				"4026593280l << 10");
		assertEqual("1966110", String.valueOf(longData[4] >> 11),
				"4026593280l >> 11");
		assertEqual("1966110", String.valueOf(longData[4] >>> 11),
				"4026593280l >>> 11");
		assertEqual("8246463037440", String.valueOf(longData[4] << 11),
				"4026593280l << 11");
		assertEqual("983055", String.valueOf(longData[4] >> 12),
				"4026593280l >> 12");
		assertEqual("983055", String.valueOf(longData[4] >>> 12),
				"4026593280l >>> 12");
		assertEqual("16492926074880", String.valueOf(longData[4] << 12),
				"4026593280l << 12");
		assertEqual("491527", String.valueOf(longData[4] >> 13),
				"4026593280l >> 13");
		assertEqual("491527", String.valueOf(longData[4] >>> 13),
				"4026593280l >>> 13");
		assertEqual("32985852149760", String.valueOf(longData[4] << 13),
				"4026593280l << 13");
		assertEqual("245763", String.valueOf(longData[4] >> 14),
				"4026593280l >> 14");
		assertEqual("245763", String.valueOf(longData[4] >>> 14),
				"4026593280l >>> 14");
		assertEqual("65971704299520", String.valueOf(longData[4] << 14),
				"4026593280l << 14");
		assertEqual("122881", String.valueOf(longData[4] >> 15),
				"4026593280l >> 15");
		assertEqual("122881", String.valueOf(longData[4] >>> 15),
				"4026593280l >>> 15");
		assertEqual("131943408599040", String.valueOf(longData[4] << 15),
				"4026593280l << 15");
		assertEqual("61440", String.valueOf(longData[4] >> 16),
				"4026593280l >> 16");
		assertEqual("61440", String.valueOf(longData[4] >>> 16),
				"4026593280l >>> 16");
		assertEqual("263886817198080", String.valueOf(longData[4] << 16),
				"4026593280l << 16");
		assertEqual("30720", String.valueOf(longData[4] >> 17),
				"4026593280l >> 17");
		assertEqual("30720", String.valueOf(longData[4] >>> 17),
				"4026593280l >>> 17");
		assertEqual("527773634396160", String.valueOf(longData[4] << 17),
				"4026593280l << 17");
		assertEqual("15360", String.valueOf(longData[4] >> 18),
				"4026593280l >> 18");
		assertEqual("15360", String.valueOf(longData[4] >>> 18),
				"4026593280l >>> 18");
		assertEqual("1055547268792320", String.valueOf(longData[4] << 18),
				"4026593280l << 18");
		assertEqual("7680", String.valueOf(longData[4] >> 19),
				"4026593280l >> 19");
		assertEqual("7680", String.valueOf(longData[4] >>> 19),
				"4026593280l >>> 19");
		assertEqual("2111094537584640", String.valueOf(longData[4] << 19),
				"4026593280l << 19");
		assertEqual("3840", String.valueOf(longData[4] >> 20),
				"4026593280l >> 20");
		assertEqual("3840", String.valueOf(longData[4] >>> 20),
				"4026593280l >>> 20");
		assertEqual("4222189075169280", String.valueOf(longData[4] << 20),
				"4026593280l << 20");
		assertEqual("1920", String.valueOf(longData[4] >> 21),
				"4026593280l >> 21");
		assertEqual("1920", String.valueOf(longData[4] >>> 21),
				"4026593280l >>> 21");
		assertEqual("8444378150338560", String.valueOf(longData[4] << 21),
				"4026593280l << 21");
		assertEqual("960", String.valueOf(longData[4] >> 22),
				"4026593280l >> 22");
		assertEqual("960", String.valueOf(longData[4] >>> 22),
				"4026593280l >>> 22");
		assertEqual("16888756300677120", String.valueOf(longData[4] << 22),
				"4026593280l << 22");
		assertEqual("480", String.valueOf(longData[4] >> 23),
				"4026593280l >> 23");
		assertEqual("480", String.valueOf(longData[4] >>> 23),
				"4026593280l >>> 23");
		assertEqual("33777512601354240", String.valueOf(longData[4] << 23),
				"4026593280l << 23");
		assertEqual("240", String.valueOf(longData[4] >> 24),
				"4026593280l >> 24");
		assertEqual("240", String.valueOf(longData[4] >>> 24),
				"4026593280l >>> 24");
		assertEqual("67555025202708480", String.valueOf(longData[4] << 24),
				"4026593280l << 24");
		assertEqual("120", String.valueOf(longData[4] >> 25),
				"4026593280l >> 25");
		assertEqual("120", String.valueOf(longData[4] >>> 25),
				"4026593280l >>> 25");
		assertEqual("135110050405416960", String.valueOf(longData[4] << 25),
				"4026593280l << 25");
		assertEqual("60", String.valueOf(longData[4] >> 26),
				"4026593280l >> 26");
		assertEqual("60", String.valueOf(longData[4] >>> 26),
				"4026593280l >>> 26");
		assertEqual("270220100810833920", String.valueOf(longData[4] << 26),
				"4026593280l << 26");
		assertEqual("30", String.valueOf(longData[4] >> 27),
				"4026593280l >> 27");
		assertEqual("30", String.valueOf(longData[4] >>> 27),
				"4026593280l >>> 27");
		assertEqual("540440201621667840", String.valueOf(longData[4] << 27),
				"4026593280l << 27");
		assertEqual("15", String.valueOf(longData[4] >> 28),
				"4026593280l >> 28");
		assertEqual("15", String.valueOf(longData[4] >>> 28),
				"4026593280l >>> 28");
		assertEqual("1080880403243335680", String.valueOf(longData[4] << 28),
				"4026593280l << 28");
		assertEqual("7", String.valueOf(longData[4] >> 29), "4026593280l >> 29");
		assertEqual("7", String.valueOf(longData[4] >>> 29),
				"4026593280l >>> 29");
		assertEqual("2161760806486671360", String.valueOf(longData[4] << 29),
				"4026593280l << 29");
		assertEqual("3", String.valueOf(longData[4] >> 30), "4026593280l >> 30");
		assertEqual("3", String.valueOf(longData[4] >>> 30),
				"4026593280l >>> 30");
		assertEqual("4323521612973342720", String.valueOf(longData[4] << 30),
				"4026593280l << 30");
		assertEqual("1", String.valueOf(longData[4] >> 31), "4026593280l >> 31");
		assertEqual("1", String.valueOf(longData[4] >>> 31),
				"4026593280l >>> 31");
		assertEqual("8647043225946685440", String.valueOf(longData[4] << 31),
				"4026593280l << 31");
		assertEqual("0", String.valueOf(longData[4] >> 32), "4026593280l >> 32");
		assertEqual("0", String.valueOf(longData[4] >>> 32),
				"4026593280l >>> 32");
		assertEqual("-1152657621816180736", String.valueOf(longData[4] << 32),
				"4026593280l << 32");
		assertEqual("0", String.valueOf(longData[4] >> 33), "4026593280l >> 33");
		assertEqual("0", String.valueOf(longData[4] >>> 33),
				"4026593280l >>> 33");
		assertEqual("-2305315243632361472", String.valueOf(longData[4] << 33),
				"4026593280l << 33");
		assertEqual("0", String.valueOf(longData[4] >> 34), "4026593280l >> 34");
		assertEqual("0", String.valueOf(longData[4] >>> 34),
				"4026593280l >>> 34");
		assertEqual("-4610630487264722944", String.valueOf(longData[4] << 34),
				"4026593280l << 34");
		assertEqual("0", String.valueOf(longData[4] >> 35), "4026593280l >> 35");
		assertEqual("0", String.valueOf(longData[4] >>> 35),
				"4026593280l >>> 35");
		assertEqual("-9221260974529445888", String.valueOf(longData[4] << 35),
				"4026593280l << 35");
		assertEqual("0", String.valueOf(longData[4] >> 36), "4026593280l >> 36");
		assertEqual("0", String.valueOf(longData[4] >>> 36),
				"4026593280l >>> 36");
		assertEqual("4222124650659840", String.valueOf(longData[4] << 36),
				"4026593280l << 36");
		assertEqual("0", String.valueOf(longData[4] >> 37), "4026593280l >> 37");
		assertEqual("0", String.valueOf(longData[4] >>> 37),
				"4026593280l >>> 37");
		assertEqual("8444249301319680", String.valueOf(longData[4] << 37),
				"4026593280l << 37");
		assertEqual("0", String.valueOf(longData[4] >> 38), "4026593280l >> 38");
		assertEqual("0", String.valueOf(longData[4] >>> 38),
				"4026593280l >>> 38");
		assertEqual("16888498602639360", String.valueOf(longData[4] << 38),
				"4026593280l << 38");
		assertEqual("0", String.valueOf(longData[4] >> 39), "4026593280l >> 39");
		assertEqual("0", String.valueOf(longData[4] >>> 39),
				"4026593280l >>> 39");
		assertEqual("33776997205278720", String.valueOf(longData[4] << 39),
				"4026593280l << 39");
		assertEqual("0", String.valueOf(longData[4] >> 40), "4026593280l >> 40");
		assertEqual("0", String.valueOf(longData[4] >>> 40),
				"4026593280l >>> 40");
		assertEqual("67553994410557440", String.valueOf(longData[4] << 40),
				"4026593280l << 40");
		assertEqual("0", String.valueOf(longData[4] >> 41), "4026593280l >> 41");
		assertEqual("0", String.valueOf(longData[4] >>> 41),
				"4026593280l >>> 41");
		assertEqual("135107988821114880", String.valueOf(longData[4] << 41),
				"4026593280l << 41");
		assertEqual("0", String.valueOf(longData[4] >> 42), "4026593280l >> 42");
		assertEqual("0", String.valueOf(longData[4] >>> 42),
				"4026593280l >>> 42");
		assertEqual("270215977642229760", String.valueOf(longData[4] << 42),
				"4026593280l << 42");
		assertEqual("0", String.valueOf(longData[4] >> 43), "4026593280l >> 43");
		assertEqual("0", String.valueOf(longData[4] >>> 43),
				"4026593280l >>> 43");
		assertEqual("540431955284459520", String.valueOf(longData[4] << 43),
				"4026593280l << 43");
		assertEqual("0", String.valueOf(longData[4] >> 44), "4026593280l >> 44");
		assertEqual("0", String.valueOf(longData[4] >>> 44),
				"4026593280l >>> 44");
		assertEqual("1080863910568919040", String.valueOf(longData[4] << 44),
				"4026593280l << 44");
		assertEqual("0", String.valueOf(longData[4] >> 45), "4026593280l >> 45");
		assertEqual("0", String.valueOf(longData[4] >>> 45),
				"4026593280l >>> 45");
		assertEqual("2161727821137838080", String.valueOf(longData[4] << 45),
				"4026593280l << 45");
		assertEqual("0", String.valueOf(longData[4] >> 46), "4026593280l >> 46");
		assertEqual("0", String.valueOf(longData[4] >>> 46),
				"4026593280l >>> 46");
		assertEqual("4323455642275676160", String.valueOf(longData[4] << 46),
				"4026593280l << 46");
		assertEqual("0", String.valueOf(longData[4] >> 47), "4026593280l >> 47");
		assertEqual("0", String.valueOf(longData[4] >>> 47),
				"4026593280l >>> 47");
		assertEqual("8646911284551352320", String.valueOf(longData[4] << 47),
				"4026593280l << 47");
		assertEqual("0", String.valueOf(longData[4] >> 48), "4026593280l >> 48");
		assertEqual("0", String.valueOf(longData[4] >>> 48),
				"4026593280l >>> 48");
		assertEqual("-1152921504606846976", String.valueOf(longData[4] << 48),
				"4026593280l << 48");
		assertEqual("0", String.valueOf(longData[4] >> 49), "4026593280l >> 49");
		assertEqual("0", String.valueOf(longData[4] >>> 49),
				"4026593280l >>> 49");
		assertEqual("-2305843009213693952", String.valueOf(longData[4] << 49),
				"4026593280l << 49");
		assertEqual("0", String.valueOf(longData[4] >> 50), "4026593280l >> 50");
		assertEqual("0", String.valueOf(longData[4] >>> 50),
				"4026593280l >>> 50");
		assertEqual("-4611686018427387904", String.valueOf(longData[4] << 50),
				"4026593280l << 50");
		assertEqual("0", String.valueOf(longData[4] >> 51), "4026593280l >> 51");
		assertEqual("0", String.valueOf(longData[4] >>> 51),
				"4026593280l >>> 51");
		assertEqual("-9223372036854775808", String.valueOf(longData[4] << 51),
				"4026593280l << 51");
		assertEqual("0", String.valueOf(longData[4] >> 52), "4026593280l >> 52");
		assertEqual("0", String.valueOf(longData[4] >>> 52),
				"4026593280l >>> 52");
		assertEqual("0", String.valueOf(longData[4] << 52), "4026593280l << 52");
		assertEqual("0", String.valueOf(longData[4] >> 53), "4026593280l >> 53");
		assertEqual("0", String.valueOf(longData[4] >>> 53),
				"4026593280l >>> 53");
		assertEqual("0", String.valueOf(longData[4] << 53), "4026593280l << 53");
		assertEqual("0", String.valueOf(longData[4] >> 54), "4026593280l >> 54");
		assertEqual("0", String.valueOf(longData[4] >>> 54),
				"4026593280l >>> 54");
		assertEqual("0", String.valueOf(longData[4] << 54), "4026593280l << 54");
		assertEqual("0", String.valueOf(longData[4] >> 55), "4026593280l >> 55");
		assertEqual("0", String.valueOf(longData[4] >>> 55),
				"4026593280l >>> 55");
		assertEqual("0", String.valueOf(longData[4] << 55), "4026593280l << 55");
		assertEqual("0", String.valueOf(longData[4] >> 56), "4026593280l >> 56");
		assertEqual("0", String.valueOf(longData[4] >>> 56),
				"4026593280l >>> 56");
		assertEqual("0", String.valueOf(longData[4] << 56), "4026593280l << 56");
		assertEqual("0", String.valueOf(longData[4] >> 57), "4026593280l >> 57");
		assertEqual("0", String.valueOf(longData[4] >>> 57),
				"4026593280l >>> 57");
		assertEqual("0", String.valueOf(longData[4] << 57), "4026593280l << 57");
		assertEqual("0", String.valueOf(longData[4] >> 58), "4026593280l >> 58");
		assertEqual("0", String.valueOf(longData[4] >>> 58),
				"4026593280l >>> 58");
		assertEqual("0", String.valueOf(longData[4] << 58), "4026593280l << 58");
		assertEqual("0", String.valueOf(longData[4] >> 59), "4026593280l >> 59");
		assertEqual("0", String.valueOf(longData[4] >>> 59),
				"4026593280l >>> 59");
		assertEqual("0", String.valueOf(longData[4] << 59), "4026593280l << 59");
		assertEqual("0", String.valueOf(longData[4] >> 60), "4026593280l >> 60");
		assertEqual("0", String.valueOf(longData[4] >>> 60),
				"4026593280l >>> 60");
		assertEqual("0", String.valueOf(longData[4] << 60), "4026593280l << 60");
		assertEqual("0", String.valueOf(longData[4] >> 61), "4026593280l >> 61");
		assertEqual("0", String.valueOf(longData[4] >>> 61),
				"4026593280l >>> 61");
		assertEqual("0", String.valueOf(longData[4] << 61), "4026593280l << 61");
		assertEqual("0", String.valueOf(longData[4] >> 62), "4026593280l >> 62");
		assertEqual("0", String.valueOf(longData[4] >>> 62),
				"4026593280l >>> 62");
		assertEqual("0", String.valueOf(longData[4] << 62), "4026593280l << 62");
		assertEqual("0", String.valueOf(longData[4] >> 63), "4026593280l >> 63");
		assertEqual("0", String.valueOf(longData[4] >>> 63),
				"4026593280l >>> 63");
		assertEqual("0", String.valueOf(longData[4] << 63), "4026593280l << 63");
		assertEqual("4026593281", String.valueOf(longData[4] + longData[0]),
				"4026593280l + 1l");
		assertEqual("4026593279", String.valueOf(longData[4] - longData[0]),
				"4026593280l - 1l");
		assertEqual("4026593280", String.valueOf(longData[4] * longData[0]),
				"4026593280l * 1l");
		assertEqual("4026593280", String.valueOf(longData[4] / longData[0]),
				"4026593280l / 1l");
		assertEqual("0", String.valueOf(longData[4] % longData[0]),
				"4026593280l % 1l");
		assertEqual("0", String.valueOf(longData[4] & longData[0]),
				"4026593280l & 1l");
		assertEqual("4026593281", String.valueOf(longData[4] | longData[0]),
				"4026593280l | 1l");
		assertEqual("4026593281", String.valueOf(longData[4] ^ longData[0]),
				"4026593280l ^ 1l");
		assertEqual("true", String.valueOf(longData[4] > longData[0]),
				"4026593280l > 1l");
		assertEqual("false", String.valueOf(longData[4] < longData[0]),
				"4026593280l < 1l");
		assertEqual("false", String.valueOf(longData[4] == longData[0]),
				"4026593280l == 1l");
		assertEqual("true", String.valueOf(longData[4] >= longData[0]),
				"4026593280l >= 1l");
		assertEqual("false", String.valueOf(longData[4] <= longData[0]),
				"4026593280l <= 1l");
		assertEqual("4026593279", String.valueOf(longData[4] + longData[1]),
				"4026593280l + -1l");
		assertEqual("4026593281", String.valueOf(longData[4] - longData[1]),
				"4026593280l - -1l");
		assertEqual("-4026593280", String.valueOf(longData[4] * longData[1]),
				"4026593280l * -1l");
		assertEqual("-4026593280", String.valueOf(longData[4] / longData[1]),
				"4026593280l / -1l");
		assertEqual("0", String.valueOf(longData[4] % longData[1]),
				"4026593280l % -1l");
		assertEqual("4026593280", String.valueOf(longData[4] & longData[1]),
				"4026593280l & -1l");
		assertEqual("-1", String.valueOf(longData[4] | longData[1]),
				"4026593280l | -1l");
		assertEqual("-4026593281", String.valueOf(longData[4] ^ longData[1]),
				"4026593280l ^ -1l");
		assertEqual("true", String.valueOf(longData[4] > longData[1]),
				"4026593280l > -1l");
		assertEqual("false", String.valueOf(longData[4] < longData[1]),
				"4026593280l < -1l");
		assertEqual("false", String.valueOf(longData[4] == longData[1]),
				"4026593280l == -1l");
		assertEqual("true", String.valueOf(longData[4] >= longData[1]),
				"4026593280l >= -1l");
		assertEqual("false", String.valueOf(longData[4] <= longData[1]),
				"4026593280l <= -1l");
		assertEqual("4026593297", String.valueOf(longData[4] + longData[2]),
				"4026593280l + 17l");
		assertEqual("4026593263", String.valueOf(longData[4] - longData[2]),
				"4026593280l - 17l");
		assertEqual("68452085760", String.valueOf(longData[4] * longData[2]),
				"4026593280l * 17l");
		assertEqual("236858428", String.valueOf(longData[4] / longData[2]),
				"4026593280l / 17l");
		assertEqual("4", String.valueOf(longData[4] % longData[2]),
				"4026593280l % 17l");
		assertEqual("0", String.valueOf(longData[4] & longData[2]),
				"4026593280l & 17l");
		assertEqual("4026593297", String.valueOf(longData[4] | longData[2]),
				"4026593280l | 17l");
		assertEqual("4026593297", String.valueOf(longData[4] ^ longData[2]),
				"4026593280l ^ 17l");
		assertEqual("true", String.valueOf(longData[4] > longData[2]),
				"4026593280l > 17l");
		assertEqual("false", String.valueOf(longData[4] < longData[2]),
				"4026593280l < 17l");
		assertEqual("false", String.valueOf(longData[4] == longData[2]),
				"4026593280l == 17l");
		assertEqual("true", String.valueOf(longData[4] >= longData[2]),
				"4026593280l >= 17l");
		assertEqual("false", String.valueOf(longData[4] <= longData[2]),
				"4026593280l <= 17l");
		assertEqual("4026593263", String.valueOf(longData[4] + longData[3]),
				"4026593280l + -17l");
		assertEqual("4026593297", String.valueOf(longData[4] - longData[3]),
				"4026593280l - -17l");
		assertEqual("-68452085760", String.valueOf(longData[4] * longData[3]),
				"4026593280l * -17l");
		assertEqual("-236858428", String.valueOf(longData[4] / longData[3]),
				"4026593280l / -17l");
		assertEqual("4", String.valueOf(longData[4] % longData[3]),
				"4026593280l % -17l");
		assertEqual("4026593280", String.valueOf(longData[4] & longData[3]),
				"4026593280l & -17l");
		assertEqual("-17", String.valueOf(longData[4] | longData[3]),
				"4026593280l | -17l");
		assertEqual("-4026593297", String.valueOf(longData[4] ^ longData[3]),
				"4026593280l ^ -17l");
		assertEqual("true", String.valueOf(longData[4] > longData[3]),
				"4026593280l > -17l");
		assertEqual("false", String.valueOf(longData[4] < longData[3]),
				"4026593280l < -17l");
		assertEqual("false", String.valueOf(longData[4] == longData[3]),
				"4026593280l == -17l");
		assertEqual("true", String.valueOf(longData[4] >= longData[3]),
				"4026593280l >= -17l");
		assertEqual("false", String.valueOf(longData[4] <= longData[3]),
				"4026593280l <= -17l");
		assertEqual("8053186560", String.valueOf(longData[4] + longData[4]),
				"4026593280l + 4026593280l");
		assertEqual("0", String.valueOf(longData[4] - longData[4]),
				"4026593280l - 4026593280l");
		assertEqual("-2233290631168393216",
				String.valueOf(longData[4] * longData[4]),
				"4026593280l * 4026593280l");
		assertEqual("1", String.valueOf(longData[4] / longData[4]),
				"4026593280l / 4026593280l");
		assertEqual("0", String.valueOf(longData[4] % longData[4]),
				"4026593280l % 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[4] & longData[4]),
				"4026593280l & 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[4] | longData[4]),
				"4026593280l | 4026593280l");
		assertEqual("0", String.valueOf(longData[4] ^ longData[4]),
				"4026593280l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[4] > longData[4]),
				"4026593280l > 4026593280l");
		assertEqual("false", String.valueOf(longData[4] < longData[4]),
				"4026593280l < 4026593280l");
		assertEqual("true", String.valueOf(longData[4] == longData[4]),
				"4026593280l == 4026593280l");
		assertEqual("true", String.valueOf(longData[4] >= longData[4]),
				"4026593280l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[4] <= longData[4]),
				"4026593280l <= 4026593280l");
		assertEqual("4278255360", String.valueOf(longData[4] + longData[5]),
				"4026593280l + 251662080l");
		assertEqual("3774931200", String.valueOf(longData[4] - longData[5]),
				"4026593280l - 251662080l");
		assertEqual("1013340840158822400",
				String.valueOf(longData[4] * longData[5]),
				"4026593280l * 251662080l");
		assertEqual("16", String.valueOf(longData[4] / longData[5]),
				"4026593280l / 251662080l");
		assertEqual("0", String.valueOf(longData[4] % longData[5]),
				"4026593280l % 251662080l");
		assertEqual("0", String.valueOf(longData[4] & longData[5]),
				"4026593280l & 251662080l");
		assertEqual("4278255360", String.valueOf(longData[4] | longData[5]),
				"4026593280l | 251662080l");
		assertEqual("4278255360", String.valueOf(longData[4] ^ longData[5]),
				"4026593280l ^ 251662080l");
		assertEqual("true", String.valueOf(longData[4] > longData[5]),
				"4026593280l > 251662080l");
		assertEqual("false", String.valueOf(longData[4] < longData[5]),
				"4026593280l < 251662080l");
		assertEqual("false", String.valueOf(longData[4] == longData[5]),
				"4026593280l == 251662080l");
		assertEqual("true", String.valueOf(longData[4] >= longData[5]),
				"4026593280l >= 251662080l");
		assertEqual("false", String.valueOf(longData[4] <= longData[5]),
				"4026593280l <= 251662080l");
		assertEqual("-1152657613762994176",
				String.valueOf(longData[4] + longData[6]),
				"4026593280l + -1152657617789587456l");
		assertEqual("1152657621816180736",
				String.valueOf(longData[4] - longData[6]),
				"4026593280l - -1152657617789587456l");
		assertEqual("-4467076046344159232",
				String.valueOf(longData[4] * longData[6]),
				"4026593280l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[4] / longData[6]),
				"4026593280l / -1152657617789587456l");
		assertEqual("4026593280", String.valueOf(longData[4] % longData[6]),
				"4026593280l % -1152657617789587456l");
		assertEqual("4026593280", String.valueOf(longData[4] & longData[6]),
				"4026593280l & -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[4] | longData[6]),
				"4026593280l | -1152657617789587456l");
		assertEqual("-1152657621816180736",
				String.valueOf(longData[4] ^ longData[6]),
				"4026593280l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[4] > longData[6]),
				"4026593280l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[4] < longData[6]),
				"4026593280l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[4] == longData[6]),
				"4026593280l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[4] >= longData[6]),
				"4026593280l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[4] <= longData[6]),
				"4026593280l <= -1152657617789587456l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[4] + longData[7]),
				"4026593280l + 1080880403494997760l");
		assertEqual("-1080880399468404480",
				String.valueOf(longData[4] - longData[7]),
				"4026593280l - 1080880403494997760l");
		assertEqual("2026650756317184000",
				String.valueOf(longData[4] * longData[7]),
				"4026593280l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[4] / longData[7]),
				"4026593280l / 1080880403494997760l");
		assertEqual("4026593280", String.valueOf(longData[4] % longData[7]),
				"4026593280l % 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[4] & longData[7]),
				"4026593280l & 1080880403494997760l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[4] | longData[7]),
				"4026593280l | 1080880403494997760l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[4] ^ longData[7]),
				"4026593280l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[4] > longData[7]),
				"4026593280l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[4] < longData[7]),
				"4026593280l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[4] == longData[7]),
				"4026593280l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[4] >= longData[7]),
				"4026593280l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[4] <= longData[7]),
				"4026593280l <= 1080880403494997760l");
		assertEqual("251662080", String.valueOf((int) longData[5]),
				"l2i 251662080l");
		assertEqual("2.5166208E8", String.valueOf((float) longData[5]),
				"l2f 251662080l");
		assertEqual("2.5166208E8", String.valueOf((double) longData[5]),
				"l2d 251662080l");
		assertEqual("-251662080", String.valueOf(-longData[5]),
				"lneg 251662080l");
		assertEqual("251662080", String.valueOf(longData[5] >> 0),
				"251662080l >> 0");
		assertEqual("251662080", String.valueOf(longData[5] >>> 0),
				"251662080l >>> 0");
		assertEqual("251662080", String.valueOf(longData[5] << 0),
				"251662080l << 0");
		assertEqual("125831040", String.valueOf(longData[5] >> 1),
				"251662080l >> 1");
		assertEqual("125831040", String.valueOf(longData[5] >>> 1),
				"251662080l >>> 1");
		assertEqual("503324160", String.valueOf(longData[5] << 1),
				"251662080l << 1");
		assertEqual("62915520", String.valueOf(longData[5] >> 2),
				"251662080l >> 2");
		assertEqual("62915520", String.valueOf(longData[5] >>> 2),
				"251662080l >>> 2");
		assertEqual("1006648320", String.valueOf(longData[5] << 2),
				"251662080l << 2");
		assertEqual("31457760", String.valueOf(longData[5] >> 3),
				"251662080l >> 3");
		assertEqual("31457760", String.valueOf(longData[5] >>> 3),
				"251662080l >>> 3");
		assertEqual("2013296640", String.valueOf(longData[5] << 3),
				"251662080l << 3");
		assertEqual("15728880", String.valueOf(longData[5] >> 4),
				"251662080l >> 4");
		assertEqual("15728880", String.valueOf(longData[5] >>> 4),
				"251662080l >>> 4");
		assertEqual("4026593280", String.valueOf(longData[5] << 4),
				"251662080l << 4");
		assertEqual("7864440", String.valueOf(longData[5] >> 5),
				"251662080l >> 5");
		assertEqual("7864440", String.valueOf(longData[5] >>> 5),
				"251662080l >>> 5");
		assertEqual("8053186560", String.valueOf(longData[5] << 5),
				"251662080l << 5");
		assertEqual("3932220", String.valueOf(longData[5] >> 6),
				"251662080l >> 6");
		assertEqual("3932220", String.valueOf(longData[5] >>> 6),
				"251662080l >>> 6");
		assertEqual("16106373120", String.valueOf(longData[5] << 6),
				"251662080l << 6");
		assertEqual("1966110", String.valueOf(longData[5] >> 7),
				"251662080l >> 7");
		assertEqual("1966110", String.valueOf(longData[5] >>> 7),
				"251662080l >>> 7");
		assertEqual("32212746240", String.valueOf(longData[5] << 7),
				"251662080l << 7");
		assertEqual("983055", String.valueOf(longData[5] >> 8),
				"251662080l >> 8");
		assertEqual("983055", String.valueOf(longData[5] >>> 8),
				"251662080l >>> 8");
		assertEqual("64425492480", String.valueOf(longData[5] << 8),
				"251662080l << 8");
		assertEqual("491527", String.valueOf(longData[5] >> 9),
				"251662080l >> 9");
		assertEqual("491527", String.valueOf(longData[5] >>> 9),
				"251662080l >>> 9");
		assertEqual("128850984960", String.valueOf(longData[5] << 9),
				"251662080l << 9");
		assertEqual("245763", String.valueOf(longData[5] >> 10),
				"251662080l >> 10");
		assertEqual("245763", String.valueOf(longData[5] >>> 10),
				"251662080l >>> 10");
		assertEqual("257701969920", String.valueOf(longData[5] << 10),
				"251662080l << 10");
		assertEqual("122881", String.valueOf(longData[5] >> 11),
				"251662080l >> 11");
		assertEqual("122881", String.valueOf(longData[5] >>> 11),
				"251662080l >>> 11");
		assertEqual("515403939840", String.valueOf(longData[5] << 11),
				"251662080l << 11");
		assertEqual("61440", String.valueOf(longData[5] >> 12),
				"251662080l >> 12");
		assertEqual("61440", String.valueOf(longData[5] >>> 12),
				"251662080l >>> 12");
		assertEqual("1030807879680", String.valueOf(longData[5] << 12),
				"251662080l << 12");
		assertEqual("30720", String.valueOf(longData[5] >> 13),
				"251662080l >> 13");
		assertEqual("30720", String.valueOf(longData[5] >>> 13),
				"251662080l >>> 13");
		assertEqual("2061615759360", String.valueOf(longData[5] << 13),
				"251662080l << 13");
		assertEqual("15360", String.valueOf(longData[5] >> 14),
				"251662080l >> 14");
		assertEqual("15360", String.valueOf(longData[5] >>> 14),
				"251662080l >>> 14");
		assertEqual("4123231518720", String.valueOf(longData[5] << 14),
				"251662080l << 14");
		assertEqual("7680", String.valueOf(longData[5] >> 15),
				"251662080l >> 15");
		assertEqual("7680", String.valueOf(longData[5] >>> 15),
				"251662080l >>> 15");
		assertEqual("8246463037440", String.valueOf(longData[5] << 15),
				"251662080l << 15");
		assertEqual("3840", String.valueOf(longData[5] >> 16),
				"251662080l >> 16");
		assertEqual("3840", String.valueOf(longData[5] >>> 16),
				"251662080l >>> 16");
		assertEqual("16492926074880", String.valueOf(longData[5] << 16),
				"251662080l << 16");
		assertEqual("1920", String.valueOf(longData[5] >> 17),
				"251662080l >> 17");
		assertEqual("1920", String.valueOf(longData[5] >>> 17),
				"251662080l >>> 17");
		assertEqual("32985852149760", String.valueOf(longData[5] << 17),
				"251662080l << 17");
		assertEqual("960", String.valueOf(longData[5] >> 18),
				"251662080l >> 18");
		assertEqual("960", String.valueOf(longData[5] >>> 18),
				"251662080l >>> 18");
		assertEqual("65971704299520", String.valueOf(longData[5] << 18),
				"251662080l << 18");
		assertEqual("480", String.valueOf(longData[5] >> 19),
				"251662080l >> 19");
		assertEqual("480", String.valueOf(longData[5] >>> 19),
				"251662080l >>> 19");
		assertEqual("131943408599040", String.valueOf(longData[5] << 19),
				"251662080l << 19");
		assertEqual("240", String.valueOf(longData[5] >> 20),
				"251662080l >> 20");
		assertEqual("240", String.valueOf(longData[5] >>> 20),
				"251662080l >>> 20");
		assertEqual("263886817198080", String.valueOf(longData[5] << 20),
				"251662080l << 20");
		assertEqual("120", String.valueOf(longData[5] >> 21),
				"251662080l >> 21");
		assertEqual("120", String.valueOf(longData[5] >>> 21),
				"251662080l >>> 21");
		assertEqual("527773634396160", String.valueOf(longData[5] << 21),
				"251662080l << 21");
		assertEqual("60", String.valueOf(longData[5] >> 22), "251662080l >> 22");
		assertEqual("60", String.valueOf(longData[5] >>> 22),
				"251662080l >>> 22");
		assertEqual("1055547268792320", String.valueOf(longData[5] << 22),
				"251662080l << 22");
		assertEqual("30", String.valueOf(longData[5] >> 23), "251662080l >> 23");
		assertEqual("30", String.valueOf(longData[5] >>> 23),
				"251662080l >>> 23");
		assertEqual("2111094537584640", String.valueOf(longData[5] << 23),
				"251662080l << 23");
		assertEqual("15", String.valueOf(longData[5] >> 24), "251662080l >> 24");
		assertEqual("15", String.valueOf(longData[5] >>> 24),
				"251662080l >>> 24");
		assertEqual("4222189075169280", String.valueOf(longData[5] << 24),
				"251662080l << 24");
		assertEqual("7", String.valueOf(longData[5] >> 25), "251662080l >> 25");
		assertEqual("7", String.valueOf(longData[5] >>> 25),
				"251662080l >>> 25");
		assertEqual("8444378150338560", String.valueOf(longData[5] << 25),
				"251662080l << 25");
		assertEqual("3", String.valueOf(longData[5] >> 26), "251662080l >> 26");
		assertEqual("3", String.valueOf(longData[5] >>> 26),
				"251662080l >>> 26");
		assertEqual("16888756300677120", String.valueOf(longData[5] << 26),
				"251662080l << 26");
		assertEqual("1", String.valueOf(longData[5] >> 27), "251662080l >> 27");
		assertEqual("1", String.valueOf(longData[5] >>> 27),
				"251662080l >>> 27");
		assertEqual("33777512601354240", String.valueOf(longData[5] << 27),
				"251662080l << 27");
		assertEqual("0", String.valueOf(longData[5] >> 28), "251662080l >> 28");
		assertEqual("0", String.valueOf(longData[5] >>> 28),
				"251662080l >>> 28");
		assertEqual("67555025202708480", String.valueOf(longData[5] << 28),
				"251662080l << 28");
		assertEqual("0", String.valueOf(longData[5] >> 29), "251662080l >> 29");
		assertEqual("0", String.valueOf(longData[5] >>> 29),
				"251662080l >>> 29");
		assertEqual("135110050405416960", String.valueOf(longData[5] << 29),
				"251662080l << 29");
		assertEqual("0", String.valueOf(longData[5] >> 30), "251662080l >> 30");
		assertEqual("0", String.valueOf(longData[5] >>> 30),
				"251662080l >>> 30");
		assertEqual("270220100810833920", String.valueOf(longData[5] << 30),
				"251662080l << 30");
		assertEqual("0", String.valueOf(longData[5] >> 31), "251662080l >> 31");
		assertEqual("0", String.valueOf(longData[5] >>> 31),
				"251662080l >>> 31");
		assertEqual("540440201621667840", String.valueOf(longData[5] << 31),
				"251662080l << 31");
		assertEqual("0", String.valueOf(longData[5] >> 32), "251662080l >> 32");
		assertEqual("0", String.valueOf(longData[5] >>> 32),
				"251662080l >>> 32");
		assertEqual("1080880403243335680", String.valueOf(longData[5] << 32),
				"251662080l << 32");
		assertEqual("0", String.valueOf(longData[5] >> 33), "251662080l >> 33");
		assertEqual("0", String.valueOf(longData[5] >>> 33),
				"251662080l >>> 33");
		assertEqual("2161760806486671360", String.valueOf(longData[5] << 33),
				"251662080l << 33");
		assertEqual("0", String.valueOf(longData[5] >> 34), "251662080l >> 34");
		assertEqual("0", String.valueOf(longData[5] >>> 34),
				"251662080l >>> 34");
		assertEqual("4323521612973342720", String.valueOf(longData[5] << 34),
				"251662080l << 34");
		assertEqual("0", String.valueOf(longData[5] >> 35), "251662080l >> 35");
		assertEqual("0", String.valueOf(longData[5] >>> 35),
				"251662080l >>> 35");
		assertEqual("8647043225946685440", String.valueOf(longData[5] << 35),
				"251662080l << 35");
		assertEqual("0", String.valueOf(longData[5] >> 36), "251662080l >> 36");
		assertEqual("0", String.valueOf(longData[5] >>> 36),
				"251662080l >>> 36");
		assertEqual("-1152657621816180736", String.valueOf(longData[5] << 36),
				"251662080l << 36");
		assertEqual("0", String.valueOf(longData[5] >> 37), "251662080l >> 37");
		assertEqual("0", String.valueOf(longData[5] >>> 37),
				"251662080l >>> 37");
		assertEqual("-2305315243632361472", String.valueOf(longData[5] << 37),
				"251662080l << 37");
		assertEqual("0", String.valueOf(longData[5] >> 38), "251662080l >> 38");
		assertEqual("0", String.valueOf(longData[5] >>> 38),
				"251662080l >>> 38");
		assertEqual("-4610630487264722944", String.valueOf(longData[5] << 38),
				"251662080l << 38");
		assertEqual("0", String.valueOf(longData[5] >> 39), "251662080l >> 39");
		assertEqual("0", String.valueOf(longData[5] >>> 39),
				"251662080l >>> 39");
		assertEqual("-9221260974529445888", String.valueOf(longData[5] << 39),
				"251662080l << 39");
		assertEqual("0", String.valueOf(longData[5] >> 40), "251662080l >> 40");
		assertEqual("0", String.valueOf(longData[5] >>> 40),
				"251662080l >>> 40");
		assertEqual("4222124650659840", String.valueOf(longData[5] << 40),
				"251662080l << 40");
		assertEqual("0", String.valueOf(longData[5] >> 41), "251662080l >> 41");
		assertEqual("0", String.valueOf(longData[5] >>> 41),
				"251662080l >>> 41");
		assertEqual("8444249301319680", String.valueOf(longData[5] << 41),
				"251662080l << 41");
		assertEqual("0", String.valueOf(longData[5] >> 42), "251662080l >> 42");
		assertEqual("0", String.valueOf(longData[5] >>> 42),
				"251662080l >>> 42");
		assertEqual("16888498602639360", String.valueOf(longData[5] << 42),
				"251662080l << 42");
		assertEqual("0", String.valueOf(longData[5] >> 43), "251662080l >> 43");
		assertEqual("0", String.valueOf(longData[5] >>> 43),
				"251662080l >>> 43");
		assertEqual("33776997205278720", String.valueOf(longData[5] << 43),
				"251662080l << 43");
		assertEqual("0", String.valueOf(longData[5] >> 44), "251662080l >> 44");
		assertEqual("0", String.valueOf(longData[5] >>> 44),
				"251662080l >>> 44");
		assertEqual("67553994410557440", String.valueOf(longData[5] << 44),
				"251662080l << 44");
		assertEqual("0", String.valueOf(longData[5] >> 45), "251662080l >> 45");
		assertEqual("0", String.valueOf(longData[5] >>> 45),
				"251662080l >>> 45");
		assertEqual("135107988821114880", String.valueOf(longData[5] << 45),
				"251662080l << 45");
		assertEqual("0", String.valueOf(longData[5] >> 46), "251662080l >> 46");
		assertEqual("0", String.valueOf(longData[5] >>> 46),
				"251662080l >>> 46");
		assertEqual("270215977642229760", String.valueOf(longData[5] << 46),
				"251662080l << 46");
		assertEqual("0", String.valueOf(longData[5] >> 47), "251662080l >> 47");
		assertEqual("0", String.valueOf(longData[5] >>> 47),
				"251662080l >>> 47");
		assertEqual("540431955284459520", String.valueOf(longData[5] << 47),
				"251662080l << 47");
		assertEqual("0", String.valueOf(longData[5] >> 48), "251662080l >> 48");
		assertEqual("0", String.valueOf(longData[5] >>> 48),
				"251662080l >>> 48");
		assertEqual("1080863910568919040", String.valueOf(longData[5] << 48),
				"251662080l << 48");
		assertEqual("0", String.valueOf(longData[5] >> 49), "251662080l >> 49");
		assertEqual("0", String.valueOf(longData[5] >>> 49),
				"251662080l >>> 49");
		assertEqual("2161727821137838080", String.valueOf(longData[5] << 49),
				"251662080l << 49");
		assertEqual("0", String.valueOf(longData[5] >> 50), "251662080l >> 50");
		assertEqual("0", String.valueOf(longData[5] >>> 50),
				"251662080l >>> 50");
		assertEqual("4323455642275676160", String.valueOf(longData[5] << 50),
				"251662080l << 50");
		assertEqual("0", String.valueOf(longData[5] >> 51), "251662080l >> 51");
		assertEqual("0", String.valueOf(longData[5] >>> 51),
				"251662080l >>> 51");
		assertEqual("8646911284551352320", String.valueOf(longData[5] << 51),
				"251662080l << 51");
		assertEqual("0", String.valueOf(longData[5] >> 52), "251662080l >> 52");
		assertEqual("0", String.valueOf(longData[5] >>> 52),
				"251662080l >>> 52");
		assertEqual("-1152921504606846976", String.valueOf(longData[5] << 52),
				"251662080l << 52");
		assertEqual("0", String.valueOf(longData[5] >> 53), "251662080l >> 53");
		assertEqual("0", String.valueOf(longData[5] >>> 53),
				"251662080l >>> 53");
		assertEqual("-2305843009213693952", String.valueOf(longData[5] << 53),
				"251662080l << 53");
		assertEqual("0", String.valueOf(longData[5] >> 54), "251662080l >> 54");
		assertEqual("0", String.valueOf(longData[5] >>> 54),
				"251662080l >>> 54");
		assertEqual("-4611686018427387904", String.valueOf(longData[5] << 54),
				"251662080l << 54");
		assertEqual("0", String.valueOf(longData[5] >> 55), "251662080l >> 55");
		assertEqual("0", String.valueOf(longData[5] >>> 55),
				"251662080l >>> 55");
		assertEqual("-9223372036854775808", String.valueOf(longData[5] << 55),
				"251662080l << 55");
		assertEqual("0", String.valueOf(longData[5] >> 56), "251662080l >> 56");
		assertEqual("0", String.valueOf(longData[5] >>> 56),
				"251662080l >>> 56");
		assertEqual("0", String.valueOf(longData[5] << 56), "251662080l << 56");
		assertEqual("0", String.valueOf(longData[5] >> 57), "251662080l >> 57");
		assertEqual("0", String.valueOf(longData[5] >>> 57),
				"251662080l >>> 57");
		assertEqual("0", String.valueOf(longData[5] << 57), "251662080l << 57");
		assertEqual("0", String.valueOf(longData[5] >> 58), "251662080l >> 58");
		assertEqual("0", String.valueOf(longData[5] >>> 58),
				"251662080l >>> 58");
		assertEqual("0", String.valueOf(longData[5] << 58), "251662080l << 58");
		assertEqual("0", String.valueOf(longData[5] >> 59), "251662080l >> 59");
		assertEqual("0", String.valueOf(longData[5] >>> 59),
				"251662080l >>> 59");
		assertEqual("0", String.valueOf(longData[5] << 59), "251662080l << 59");
		assertEqual("0", String.valueOf(longData[5] >> 60), "251662080l >> 60");
		assertEqual("0", String.valueOf(longData[5] >>> 60),
				"251662080l >>> 60");
		assertEqual("0", String.valueOf(longData[5] << 60), "251662080l << 60");
		assertEqual("0", String.valueOf(longData[5] >> 61), "251662080l >> 61");
		assertEqual("0", String.valueOf(longData[5] >>> 61),
				"251662080l >>> 61");
		assertEqual("0", String.valueOf(longData[5] << 61), "251662080l << 61");
		assertEqual("0", String.valueOf(longData[5] >> 62), "251662080l >> 62");
		assertEqual("0", String.valueOf(longData[5] >>> 62),
				"251662080l >>> 62");
		assertEqual("0", String.valueOf(longData[5] << 62), "251662080l << 62");
		assertEqual("0", String.valueOf(longData[5] >> 63), "251662080l >> 63");
		assertEqual("0", String.valueOf(longData[5] >>> 63),
				"251662080l >>> 63");
		assertEqual("0", String.valueOf(longData[5] << 63), "251662080l << 63");
		assertEqual("251662081", String.valueOf(longData[5] + longData[0]),
				"251662080l + 1l");
		assertEqual("251662079", String.valueOf(longData[5] - longData[0]),
				"251662080l - 1l");
		assertEqual("251662080", String.valueOf(longData[5] * longData[0]),
				"251662080l * 1l");
		assertEqual("251662080", String.valueOf(longData[5] / longData[0]),
				"251662080l / 1l");
		assertEqual("0", String.valueOf(longData[5] % longData[0]),
				"251662080l % 1l");
		assertEqual("0", String.valueOf(longData[5] & longData[0]),
				"251662080l & 1l");
		assertEqual("251662081", String.valueOf(longData[5] | longData[0]),
				"251662080l | 1l");
		assertEqual("251662081", String.valueOf(longData[5] ^ longData[0]),
				"251662080l ^ 1l");
		assertEqual("true", String.valueOf(longData[5] > longData[0]),
				"251662080l > 1l");
		assertEqual("false", String.valueOf(longData[5] < longData[0]),
				"251662080l < 1l");
		assertEqual("false", String.valueOf(longData[5] == longData[0]),
				"251662080l == 1l");
		assertEqual("true", String.valueOf(longData[5] >= longData[0]),
				"251662080l >= 1l");
		assertEqual("false", String.valueOf(longData[5] <= longData[0]),
				"251662080l <= 1l");
		assertEqual("251662079", String.valueOf(longData[5] + longData[1]),
				"251662080l + -1l");
		assertEqual("251662081", String.valueOf(longData[5] - longData[1]),
				"251662080l - -1l");
		assertEqual("-251662080", String.valueOf(longData[5] * longData[1]),
				"251662080l * -1l");
		assertEqual("-251662080", String.valueOf(longData[5] / longData[1]),
				"251662080l / -1l");
		assertEqual("0", String.valueOf(longData[5] % longData[1]),
				"251662080l % -1l");
		assertEqual("251662080", String.valueOf(longData[5] & longData[1]),
				"251662080l & -1l");
		assertEqual("-1", String.valueOf(longData[5] | longData[1]),
				"251662080l | -1l");
		assertEqual("-251662081", String.valueOf(longData[5] ^ longData[1]),
				"251662080l ^ -1l");
		assertEqual("true", String.valueOf(longData[5] > longData[1]),
				"251662080l > -1l");
		assertEqual("false", String.valueOf(longData[5] < longData[1]),
				"251662080l < -1l");
		assertEqual("false", String.valueOf(longData[5] == longData[1]),
				"251662080l == -1l");
		assertEqual("true", String.valueOf(longData[5] >= longData[1]),
				"251662080l >= -1l");
		assertEqual("false", String.valueOf(longData[5] <= longData[1]),
				"251662080l <= -1l");
		assertEqual("251662097", String.valueOf(longData[5] + longData[2]),
				"251662080l + 17l");
		assertEqual("251662063", String.valueOf(longData[5] - longData[2]),
				"251662080l - 17l");
		assertEqual("4278255360", String.valueOf(longData[5] * longData[2]),
				"251662080l * 17l");
		assertEqual("14803651", String.valueOf(longData[5] / longData[2]),
				"251662080l / 17l");
		assertEqual("13", String.valueOf(longData[5] % longData[2]),
				"251662080l % 17l");
		assertEqual("0", String.valueOf(longData[5] & longData[2]),
				"251662080l & 17l");
		assertEqual("251662097", String.valueOf(longData[5] | longData[2]),
				"251662080l | 17l");
		assertEqual("251662097", String.valueOf(longData[5] ^ longData[2]),
				"251662080l ^ 17l");
		assertEqual("true", String.valueOf(longData[5] > longData[2]),
				"251662080l > 17l");
		assertEqual("false", String.valueOf(longData[5] < longData[2]),
				"251662080l < 17l");
		assertEqual("false", String.valueOf(longData[5] == longData[2]),
				"251662080l == 17l");
		assertEqual("true", String.valueOf(longData[5] >= longData[2]),
				"251662080l >= 17l");
		assertEqual("false", String.valueOf(longData[5] <= longData[2]),
				"251662080l <= 17l");
		assertEqual("251662063", String.valueOf(longData[5] + longData[3]),
				"251662080l + -17l");
		assertEqual("251662097", String.valueOf(longData[5] - longData[3]),
				"251662080l - -17l");
		assertEqual("-4278255360", String.valueOf(longData[5] * longData[3]),
				"251662080l * -17l");
		assertEqual("-14803651", String.valueOf(longData[5] / longData[3]),
				"251662080l / -17l");
		assertEqual("13", String.valueOf(longData[5] % longData[3]),
				"251662080l % -17l");
		assertEqual("251662080", String.valueOf(longData[5] & longData[3]),
				"251662080l & -17l");
		assertEqual("-17", String.valueOf(longData[5] | longData[3]),
				"251662080l | -17l");
		assertEqual("-251662097", String.valueOf(longData[5] ^ longData[3]),
				"251662080l ^ -17l");
		assertEqual("true", String.valueOf(longData[5] > longData[3]),
				"251662080l > -17l");
		assertEqual("false", String.valueOf(longData[5] < longData[3]),
				"251662080l < -17l");
		assertEqual("false", String.valueOf(longData[5] == longData[3]),
				"251662080l == -17l");
		assertEqual("true", String.valueOf(longData[5] >= longData[3]),
				"251662080l >= -17l");
		assertEqual("false", String.valueOf(longData[5] <= longData[3]),
				"251662080l <= -17l");
		assertEqual("4278255360", String.valueOf(longData[5] + longData[4]),
				"251662080l + 4026593280l");
		assertEqual("-3774931200", String.valueOf(longData[5] - longData[4]),
				"251662080l - 4026593280l");
		assertEqual("1013340840158822400",
				String.valueOf(longData[5] * longData[4]),
				"251662080l * 4026593280l");
		assertEqual("0", String.valueOf(longData[5] / longData[4]),
				"251662080l / 4026593280l");
		assertEqual("251662080", String.valueOf(longData[5] % longData[4]),
				"251662080l % 4026593280l");
		assertEqual("0", String.valueOf(longData[5] & longData[4]),
				"251662080l & 4026593280l");
		assertEqual("4278255360", String.valueOf(longData[5] | longData[4]),
				"251662080l | 4026593280l");
		assertEqual("4278255360", String.valueOf(longData[5] ^ longData[4]),
				"251662080l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[5] > longData[4]),
				"251662080l > 4026593280l");
		assertEqual("true", String.valueOf(longData[5] < longData[4]),
				"251662080l < 4026593280l");
		assertEqual("false", String.valueOf(longData[5] == longData[4]),
				"251662080l == 4026593280l");
		assertEqual("false", String.valueOf(longData[5] >= longData[4]),
				"251662080l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[5] <= longData[4]),
				"251662080l <= 4026593280l");
		assertEqual("503324160", String.valueOf(longData[5] + longData[5]),
				"251662080l + 251662080l");
		assertEqual("0", String.valueOf(longData[5] - longData[5]),
				"251662080l - 251662080l");
		assertEqual("63333802509926400",
				String.valueOf(longData[5] * longData[5]),
				"251662080l * 251662080l");
		assertEqual("1", String.valueOf(longData[5] / longData[5]),
				"251662080l / 251662080l");
		assertEqual("0", String.valueOf(longData[5] % longData[5]),
				"251662080l % 251662080l");
		assertEqual("251662080", String.valueOf(longData[5] & longData[5]),
				"251662080l & 251662080l");
		assertEqual("251662080", String.valueOf(longData[5] | longData[5]),
				"251662080l | 251662080l");
		assertEqual("0", String.valueOf(longData[5] ^ longData[5]),
				"251662080l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[5] > longData[5]),
				"251662080l > 251662080l");
		assertEqual("false", String.valueOf(longData[5] < longData[5]),
				"251662080l < 251662080l");
		assertEqual("true", String.valueOf(longData[5] == longData[5]),
				"251662080l == 251662080l");
		assertEqual("true", String.valueOf(longData[5] >= longData[5]),
				"251662080l >= 251662080l");
		assertEqual("true", String.valueOf(longData[5] <= longData[5]),
				"251662080l <= 251662080l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[5] + longData[6]),
				"251662080l + -1152657617789587456l");
		assertEqual("1152657618041249536",
				String.valueOf(longData[5] - longData[6]),
				"251662080l - -1152657617789587456l");
		assertEqual("2026650756317184000",
				String.valueOf(longData[5] * longData[6]),
				"251662080l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[5] / longData[6]),
				"251662080l / -1152657617789587456l");
		assertEqual("251662080", String.valueOf(longData[5] % longData[6]),
				"251662080l % -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[5] & longData[6]),
				"251662080l & -1152657617789587456l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[5] | longData[6]),
				"251662080l | -1152657617789587456l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[5] ^ longData[6]),
				"251662080l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[5] > longData[6]),
				"251662080l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[5] < longData[6]),
				"251662080l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[5] == longData[6]),
				"251662080l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[5] >= longData[6]),
				"251662080l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[5] <= longData[6]),
				"251662080l <= -1152657617789587456l");
		assertEqual("1080880403746659840",
				String.valueOf(longData[5] + longData[7]),
				"251662080l + 1080880403494997760l");
		assertEqual("-1080880403243335680",
				String.valueOf(longData[5] - longData[7]),
				"251662080l - 1080880403494997760l");
		assertEqual("126665672269824000",
				String.valueOf(longData[5] * longData[7]),
				"251662080l * 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[5] / longData[7]),
				"251662080l / 1080880403494997760l");
		assertEqual("251662080", String.valueOf(longData[5] % longData[7]),
				"251662080l % 1080880403494997760l");
		assertEqual("251662080", String.valueOf(longData[5] & longData[7]),
				"251662080l & 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[5] | longData[7]),
				"251662080l | 1080880403494997760l");
		assertEqual("1080880403243335680",
				String.valueOf(longData[5] ^ longData[7]),
				"251662080l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[5] > longData[7]),
				"251662080l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[5] < longData[7]),
				"251662080l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[5] == longData[7]),
				"251662080l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[5] >= longData[7]),
				"251662080l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[5] <= longData[7]),
				"251662080l <= 1080880403494997760l");
		assertEqual("-268374016", String.valueOf((int) longData[6]),
				"l2i -1152657617789587456l");
		assertEqual("-1.15265762E18", String.valueOf((float) longData[6]),
				"l2f -1152657617789587456l");
		assertEqual("-1.15265761778958746E18",
				String.valueOf((double) longData[6]),
				"l2d -1152657617789587456l");
		assertEqual("1152657617789587456", String.valueOf(-longData[6]),
				"lneg -1152657617789587456l");
		assertEqual("-1152657617789587456", String.valueOf(longData[6] >> 0),
				"-1152657617789587456l >> 0");
		assertEqual("-1152657617789587456", String.valueOf(longData[6] >>> 0),
				"-1152657617789587456l >>> 0");
		assertEqual("-1152657617789587456", String.valueOf(longData[6] << 0),
				"-1152657617789587456l << 0");
		assertEqual("-576328808894793728", String.valueOf(longData[6] >> 1),
				"-1152657617789587456l >> 1");
		assertEqual("8647043227959982080", String.valueOf(longData[6] >>> 1),
				"-1152657617789587456l >>> 1");
		assertEqual("-2305315235579174912", String.valueOf(longData[6] << 1),
				"-1152657617789587456l << 1");
		assertEqual("-288164404447396864", String.valueOf(longData[6] >> 2),
				"-1152657617789587456l >> 2");
		assertEqual("4323521613979991040", String.valueOf(longData[6] >>> 2),
				"-1152657617789587456l >>> 2");
		assertEqual("-4610630471158349824", String.valueOf(longData[6] << 2),
				"-1152657617789587456l << 2");
		assertEqual("-144082202223698432", String.valueOf(longData[6] >> 3),
				"-1152657617789587456l >> 3");
		assertEqual("2161760806989995520", String.valueOf(longData[6] >>> 3),
				"-1152657617789587456l >>> 3");
		assertEqual("-9221260942316699648", String.valueOf(longData[6] << 3),
				"-1152657617789587456l << 3");
		assertEqual("-72041101111849216", String.valueOf(longData[6] >> 4),
				"-1152657617789587456l >> 4");
		assertEqual("1080880403494997760", String.valueOf(longData[6] >>> 4),
				"-1152657617789587456l >>> 4");
		assertEqual("4222189076152320", String.valueOf(longData[6] << 4),
				"-1152657617789587456l << 4");
		assertEqual("-36020550555924608", String.valueOf(longData[6] >> 5),
				"-1152657617789587456l >> 5");
		assertEqual("540440201747498880", String.valueOf(longData[6] >>> 5),
				"-1152657617789587456l >>> 5");
		assertEqual("8444378152304640", String.valueOf(longData[6] << 5),
				"-1152657617789587456l << 5");
		assertEqual("-18010275277962304", String.valueOf(longData[6] >> 6),
				"-1152657617789587456l >> 6");
		assertEqual("270220100873749440", String.valueOf(longData[6] >>> 6),
				"-1152657617789587456l >>> 6");
		assertEqual("16888756304609280", String.valueOf(longData[6] << 6),
				"-1152657617789587456l << 6");
		assertEqual("-9005137638981152", String.valueOf(longData[6] >> 7),
				"-1152657617789587456l >> 7");
		assertEqual("135110050436874720", String.valueOf(longData[6] >>> 7),
				"-1152657617789587456l >>> 7");
		assertEqual("33777512609218560", String.valueOf(longData[6] << 7),
				"-1152657617789587456l << 7");
		assertEqual("-4502568819490576", String.valueOf(longData[6] >> 8),
				"-1152657617789587456l >> 8");
		assertEqual("67555025218437360", String.valueOf(longData[6] >>> 8),
				"-1152657617789587456l >>> 8");
		assertEqual("67555025218437120", String.valueOf(longData[6] << 8),
				"-1152657617789587456l << 8");
		assertEqual("-2251284409745288", String.valueOf(longData[6] >> 9),
				"-1152657617789587456l >> 9");
		assertEqual("33777512609218680", String.valueOf(longData[6] >>> 9),
				"-1152657617789587456l >>> 9");
		assertEqual("135110050436874240", String.valueOf(longData[6] << 9),
				"-1152657617789587456l << 9");
		assertEqual("-1125642204872644", String.valueOf(longData[6] >> 10),
				"-1152657617789587456l >> 10");
		assertEqual("16888756304609340", String.valueOf(longData[6] >>> 10),
				"-1152657617789587456l >>> 10");
		assertEqual("270220100873748480", String.valueOf(longData[6] << 10),
				"-1152657617789587456l << 10");
		assertEqual("-562821102436322", String.valueOf(longData[6] >> 11),
				"-1152657617789587456l >> 11");
		assertEqual("8444378152304670", String.valueOf(longData[6] >>> 11),
				"-1152657617789587456l >>> 11");
		assertEqual("540440201747496960", String.valueOf(longData[6] << 11),
				"-1152657617789587456l << 11");
		assertEqual("-281410551218161", String.valueOf(longData[6] >> 12),
				"-1152657617789587456l >> 12");
		assertEqual("4222189076152335", String.valueOf(longData[6] >>> 12),
				"-1152657617789587456l >>> 12");
		assertEqual("1080880403494993920", String.valueOf(longData[6] << 12),
				"-1152657617789587456l << 12");
		assertEqual("-140705275609081", String.valueOf(longData[6] >> 13),
				"-1152657617789587456l >> 13");
		assertEqual("2111094538076167", String.valueOf(longData[6] >>> 13),
				"-1152657617789587456l >>> 13");
		assertEqual("2161760806989987840", String.valueOf(longData[6] << 13),
				"-1152657617789587456l << 13");
		assertEqual("-70352637804541", String.valueOf(longData[6] >> 14),
				"-1152657617789587456l >> 14");
		assertEqual("1055547269038083", String.valueOf(longData[6] >>> 14),
				"-1152657617789587456l >>> 14");
		assertEqual("4323521613979975680", String.valueOf(longData[6] << 14),
				"-1152657617789587456l << 14");
		assertEqual("-35176318902271", String.valueOf(longData[6] >> 15),
				"-1152657617789587456l >> 15");
		assertEqual("527773634519041", String.valueOf(longData[6] >>> 15),
				"-1152657617789587456l >>> 15");
		assertEqual("8647043227959951360", String.valueOf(longData[6] << 15),
				"-1152657617789587456l << 15");
		assertEqual("-17588159451136", String.valueOf(longData[6] >> 16),
				"-1152657617789587456l >> 16");
		assertEqual("263886817259520", String.valueOf(longData[6] >>> 16),
				"-1152657617789587456l >>> 16");
		assertEqual("-1152657617789648896", String.valueOf(longData[6] << 16),
				"-1152657617789587456l << 16");
		assertEqual("-8794079725568", String.valueOf(longData[6] >> 17),
				"-1152657617789587456l >> 17");
		assertEqual("131943408629760", String.valueOf(longData[6] >>> 17),
				"-1152657617789587456l >>> 17");
		assertEqual("-2305315235579297792", String.valueOf(longData[6] << 17),
				"-1152657617789587456l << 17");
		assertEqual("-4397039862784", String.valueOf(longData[6] >> 18),
				"-1152657617789587456l >> 18");
		assertEqual("65971704314880", String.valueOf(longData[6] >>> 18),
				"-1152657617789587456l >>> 18");
		assertEqual("-4610630471158595584", String.valueOf(longData[6] << 18),
				"-1152657617789587456l << 18");
		assertEqual("-2198519931392", String.valueOf(longData[6] >> 19),
				"-1152657617789587456l >> 19");
		assertEqual("32985852157440", String.valueOf(longData[6] >>> 19),
				"-1152657617789587456l >>> 19");
		assertEqual("-9221260942317191168", String.valueOf(longData[6] << 19),
				"-1152657617789587456l << 19");
		assertEqual("-1099259965696", String.valueOf(longData[6] >> 20),
				"-1152657617789587456l >> 20");
		assertEqual("16492926078720", String.valueOf(longData[6] >>> 20),
				"-1152657617789587456l >>> 20");
		assertEqual("4222189075169280", String.valueOf(longData[6] << 20),
				"-1152657617789587456l << 20");
		assertEqual("-549629982848", String.valueOf(longData[6] >> 21),
				"-1152657617789587456l >> 21");
		assertEqual("8246463039360", String.valueOf(longData[6] >>> 21),
				"-1152657617789587456l >>> 21");
		assertEqual("8444378150338560", String.valueOf(longData[6] << 21),
				"-1152657617789587456l << 21");
		assertEqual("-274814991424", String.valueOf(longData[6] >> 22),
				"-1152657617789587456l >> 22");
		assertEqual("4123231519680", String.valueOf(longData[6] >>> 22),
				"-1152657617789587456l >>> 22");
		assertEqual("16888756300677120", String.valueOf(longData[6] << 22),
				"-1152657617789587456l << 22");
		assertEqual("-137407495712", String.valueOf(longData[6] >> 23),
				"-1152657617789587456l >> 23");
		assertEqual("2061615759840", String.valueOf(longData[6] >>> 23),
				"-1152657617789587456l >>> 23");
		assertEqual("33777512601354240", String.valueOf(longData[6] << 23),
				"-1152657617789587456l << 23");
		assertEqual("-68703747856", String.valueOf(longData[6] >> 24),
				"-1152657617789587456l >> 24");
		assertEqual("1030807879920", String.valueOf(longData[6] >>> 24),
				"-1152657617789587456l >>> 24");
		assertEqual("67555025202708480", String.valueOf(longData[6] << 24),
				"-1152657617789587456l << 24");
		assertEqual("-34351873928", String.valueOf(longData[6] >> 25),
				"-1152657617789587456l >> 25");
		assertEqual("515403939960", String.valueOf(longData[6] >>> 25),
				"-1152657617789587456l >>> 25");
		assertEqual("135110050405416960", String.valueOf(longData[6] << 25),
				"-1152657617789587456l << 25");
		assertEqual("-17175936964", String.valueOf(longData[6] >> 26),
				"-1152657617789587456l >> 26");
		assertEqual("257701969980", String.valueOf(longData[6] >>> 26),
				"-1152657617789587456l >>> 26");
		assertEqual("270220100810833920", String.valueOf(longData[6] << 26),
				"-1152657617789587456l << 26");
		assertEqual("-8587968482", String.valueOf(longData[6] >> 27),
				"-1152657617789587456l >> 27");
		assertEqual("128850984990", String.valueOf(longData[6] >>> 27),
				"-1152657617789587456l >>> 27");
		assertEqual("540440201621667840", String.valueOf(longData[6] << 27),
				"-1152657617789587456l << 27");
		assertEqual("-4293984241", String.valueOf(longData[6] >> 28),
				"-1152657617789587456l >> 28");
		assertEqual("64425492495", String.valueOf(longData[6] >>> 28),
				"-1152657617789587456l >>> 28");
		assertEqual("1080880403243335680", String.valueOf(longData[6] << 28),
				"-1152657617789587456l << 28");
		assertEqual("-2146992121", String.valueOf(longData[6] >> 29),
				"-1152657617789587456l >> 29");
		assertEqual("32212746247", String.valueOf(longData[6] >>> 29),
				"-1152657617789587456l >>> 29");
		assertEqual("2161760806486671360", String.valueOf(longData[6] << 29),
				"-1152657617789587456l << 29");
		assertEqual("-1073496061", String.valueOf(longData[6] >> 30),
				"-1152657617789587456l >> 30");
		assertEqual("16106373123", String.valueOf(longData[6] >>> 30),
				"-1152657617789587456l >>> 30");
		assertEqual("4323521612973342720", String.valueOf(longData[6] << 30),
				"-1152657617789587456l << 30");
		assertEqual("-536748031", String.valueOf(longData[6] >> 31),
				"-1152657617789587456l >> 31");
		assertEqual("8053186561", String.valueOf(longData[6] >>> 31),
				"-1152657617789587456l >>> 31");
		assertEqual("8647043225946685440", String.valueOf(longData[6] << 31),
				"-1152657617789587456l << 31");
		assertEqual("-268374016", String.valueOf(longData[6] >> 32),
				"-1152657617789587456l >> 32");
		assertEqual("4026593280", String.valueOf(longData[6] >>> 32),
				"-1152657617789587456l >>> 32");
		assertEqual("-1152657621816180736", String.valueOf(longData[6] << 32),
				"-1152657617789587456l << 32");
		assertEqual("-134187008", String.valueOf(longData[6] >> 33),
				"-1152657617789587456l >> 33");
		assertEqual("2013296640", String.valueOf(longData[6] >>> 33),
				"-1152657617789587456l >>> 33");
		assertEqual("-2305315243632361472", String.valueOf(longData[6] << 33),
				"-1152657617789587456l << 33");
		assertEqual("-67093504", String.valueOf(longData[6] >> 34),
				"-1152657617789587456l >> 34");
		assertEqual("1006648320", String.valueOf(longData[6] >>> 34),
				"-1152657617789587456l >>> 34");
		assertEqual("-4610630487264722944", String.valueOf(longData[6] << 34),
				"-1152657617789587456l << 34");
		assertEqual("-33546752", String.valueOf(longData[6] >> 35),
				"-1152657617789587456l >> 35");
		assertEqual("503324160", String.valueOf(longData[6] >>> 35),
				"-1152657617789587456l >>> 35");
		assertEqual("-9221260974529445888", String.valueOf(longData[6] << 35),
				"-1152657617789587456l << 35");
		assertEqual("-16773376", String.valueOf(longData[6] >> 36),
				"-1152657617789587456l >> 36");
		assertEqual("251662080", String.valueOf(longData[6] >>> 36),
				"-1152657617789587456l >>> 36");
		assertEqual("4222124650659840", String.valueOf(longData[6] << 36),
				"-1152657617789587456l << 36");
		assertEqual("-8386688", String.valueOf(longData[6] >> 37),
				"-1152657617789587456l >> 37");
		assertEqual("125831040", String.valueOf(longData[6] >>> 37),
				"-1152657617789587456l >>> 37");
		assertEqual("8444249301319680", String.valueOf(longData[6] << 37),
				"-1152657617789587456l << 37");
		assertEqual("-4193344", String.valueOf(longData[6] >> 38),
				"-1152657617789587456l >> 38");
		assertEqual("62915520", String.valueOf(longData[6] >>> 38),
				"-1152657617789587456l >>> 38");
		assertEqual("16888498602639360", String.valueOf(longData[6] << 38),
				"-1152657617789587456l << 38");
		assertEqual("-2096672", String.valueOf(longData[6] >> 39),
				"-1152657617789587456l >> 39");
		assertEqual("31457760", String.valueOf(longData[6] >>> 39),
				"-1152657617789587456l >>> 39");
		assertEqual("33776997205278720", String.valueOf(longData[6] << 39),
				"-1152657617789587456l << 39");
		assertEqual("-1048336", String.valueOf(longData[6] >> 40),
				"-1152657617789587456l >> 40");
		assertEqual("15728880", String.valueOf(longData[6] >>> 40),
				"-1152657617789587456l >>> 40");
		assertEqual("67553994410557440", String.valueOf(longData[6] << 40),
				"-1152657617789587456l << 40");
		assertEqual("-524168", String.valueOf(longData[6] >> 41),
				"-1152657617789587456l >> 41");
		assertEqual("7864440", String.valueOf(longData[6] >>> 41),
				"-1152657617789587456l >>> 41");
		assertEqual("135107988821114880", String.valueOf(longData[6] << 41),
				"-1152657617789587456l << 41");
		assertEqual("-262084", String.valueOf(longData[6] >> 42),
				"-1152657617789587456l >> 42");
		assertEqual("3932220", String.valueOf(longData[6] >>> 42),
				"-1152657617789587456l >>> 42");
		assertEqual("270215977642229760", String.valueOf(longData[6] << 42),
				"-1152657617789587456l << 42");
		assertEqual("-131042", String.valueOf(longData[6] >> 43),
				"-1152657617789587456l >> 43");
		assertEqual("1966110", String.valueOf(longData[6] >>> 43),
				"-1152657617789587456l >>> 43");
		assertEqual("540431955284459520", String.valueOf(longData[6] << 43),
				"-1152657617789587456l << 43");
		assertEqual("-65521", String.valueOf(longData[6] >> 44),
				"-1152657617789587456l >> 44");
		assertEqual("983055", String.valueOf(longData[6] >>> 44),
				"-1152657617789587456l >>> 44");
		assertEqual("1080863910568919040", String.valueOf(longData[6] << 44),
				"-1152657617789587456l << 44");
		assertEqual("-32761", String.valueOf(longData[6] >> 45),
				"-1152657617789587456l >> 45");
		assertEqual("491527", String.valueOf(longData[6] >>> 45),
				"-1152657617789587456l >>> 45");
		assertEqual("2161727821137838080", String.valueOf(longData[6] << 45),
				"-1152657617789587456l << 45");
		assertEqual("-16381", String.valueOf(longData[6] >> 46),
				"-1152657617789587456l >> 46");
		assertEqual("245763", String.valueOf(longData[6] >>> 46),
				"-1152657617789587456l >>> 46");
		assertEqual("4323455642275676160", String.valueOf(longData[6] << 46),
				"-1152657617789587456l << 46");
		assertEqual("-8191", String.valueOf(longData[6] >> 47),
				"-1152657617789587456l >> 47");
		assertEqual("122881", String.valueOf(longData[6] >>> 47),
				"-1152657617789587456l >>> 47");
		assertEqual("8646911284551352320", String.valueOf(longData[6] << 47),
				"-1152657617789587456l << 47");
		assertEqual("-4096", String.valueOf(longData[6] >> 48),
				"-1152657617789587456l >> 48");
		assertEqual("61440", String.valueOf(longData[6] >>> 48),
				"-1152657617789587456l >>> 48");
		assertEqual("-1152921504606846976", String.valueOf(longData[6] << 48),
				"-1152657617789587456l << 48");
		assertEqual("-2048", String.valueOf(longData[6] >> 49),
				"-1152657617789587456l >> 49");
		assertEqual("30720", String.valueOf(longData[6] >>> 49),
				"-1152657617789587456l >>> 49");
		assertEqual("-2305843009213693952", String.valueOf(longData[6] << 49),
				"-1152657617789587456l << 49");
		assertEqual("-1024", String.valueOf(longData[6] >> 50),
				"-1152657617789587456l >> 50");
		assertEqual("15360", String.valueOf(longData[6] >>> 50),
				"-1152657617789587456l >>> 50");
		assertEqual("-4611686018427387904", String.valueOf(longData[6] << 50),
				"-1152657617789587456l << 50");
		assertEqual("-512", String.valueOf(longData[6] >> 51),
				"-1152657617789587456l >> 51");
		assertEqual("7680", String.valueOf(longData[6] >>> 51),
				"-1152657617789587456l >>> 51");
		assertEqual("-9223372036854775808", String.valueOf(longData[6] << 51),
				"-1152657617789587456l << 51");
		assertEqual("-256", String.valueOf(longData[6] >> 52),
				"-1152657617789587456l >> 52");
		assertEqual("3840", String.valueOf(longData[6] >>> 52),
				"-1152657617789587456l >>> 52");
		assertEqual("0", String.valueOf(longData[6] << 52),
				"-1152657617789587456l << 52");
		assertEqual("-128", String.valueOf(longData[6] >> 53),
				"-1152657617789587456l >> 53");
		assertEqual("1920", String.valueOf(longData[6] >>> 53),
				"-1152657617789587456l >>> 53");
		assertEqual("0", String.valueOf(longData[6] << 53),
				"-1152657617789587456l << 53");
		assertEqual("-64", String.valueOf(longData[6] >> 54),
				"-1152657617789587456l >> 54");
		assertEqual("960", String.valueOf(longData[6] >>> 54),
				"-1152657617789587456l >>> 54");
		assertEqual("0", String.valueOf(longData[6] << 54),
				"-1152657617789587456l << 54");
		assertEqual("-32", String.valueOf(longData[6] >> 55),
				"-1152657617789587456l >> 55");
		assertEqual("480", String.valueOf(longData[6] >>> 55),
				"-1152657617789587456l >>> 55");
		assertEqual("0", String.valueOf(longData[6] << 55),
				"-1152657617789587456l << 55");
		assertEqual("-16", String.valueOf(longData[6] >> 56),
				"-1152657617789587456l >> 56");
		assertEqual("240", String.valueOf(longData[6] >>> 56),
				"-1152657617789587456l >>> 56");
		assertEqual("0", String.valueOf(longData[6] << 56),
				"-1152657617789587456l << 56");
		assertEqual("-8", String.valueOf(longData[6] >> 57),
				"-1152657617789587456l >> 57");
		assertEqual("120", String.valueOf(longData[6] >>> 57),
				"-1152657617789587456l >>> 57");
		assertEqual("0", String.valueOf(longData[6] << 57),
				"-1152657617789587456l << 57");
		assertEqual("-4", String.valueOf(longData[6] >> 58),
				"-1152657617789587456l >> 58");
		assertEqual("60", String.valueOf(longData[6] >>> 58),
				"-1152657617789587456l >>> 58");
		assertEqual("0", String.valueOf(longData[6] << 58),
				"-1152657617789587456l << 58");
		assertEqual("-2", String.valueOf(longData[6] >> 59),
				"-1152657617789587456l >> 59");
		assertEqual("30", String.valueOf(longData[6] >>> 59),
				"-1152657617789587456l >>> 59");
		assertEqual("0", String.valueOf(longData[6] << 59),
				"-1152657617789587456l << 59");
		assertEqual("-1", String.valueOf(longData[6] >> 60),
				"-1152657617789587456l >> 60");
		assertEqual("15", String.valueOf(longData[6] >>> 60),
				"-1152657617789587456l >>> 60");
		assertEqual("0", String.valueOf(longData[6] << 60),
				"-1152657617789587456l << 60");
		assertEqual("-1", String.valueOf(longData[6] >> 61),
				"-1152657617789587456l >> 61");
		assertEqual("7", String.valueOf(longData[6] >>> 61),
				"-1152657617789587456l >>> 61");
		assertEqual("0", String.valueOf(longData[6] << 61),
				"-1152657617789587456l << 61");
		assertEqual("-1", String.valueOf(longData[6] >> 62),
				"-1152657617789587456l >> 62");
		assertEqual("3", String.valueOf(longData[6] >>> 62),
				"-1152657617789587456l >>> 62");
		assertEqual("0", String.valueOf(longData[6] << 62),
				"-1152657617789587456l << 62");
		assertEqual("-1", String.valueOf(longData[6] >> 63),
				"-1152657617789587456l >> 63");
		assertEqual("1", String.valueOf(longData[6] >>> 63),
				"-1152657617789587456l >>> 63");
		assertEqual("0", String.valueOf(longData[6] << 63),
				"-1152657617789587456l << 63");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[6] + longData[0]),
				"-1152657617789587456l + 1l");
		assertEqual("-1152657617789587457",
				String.valueOf(longData[6] - longData[0]),
				"-1152657617789587456l - 1l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] * longData[0]),
				"-1152657617789587456l * 1l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] / longData[0]),
				"-1152657617789587456l / 1l");
		assertEqual("0", String.valueOf(longData[6] % longData[0]),
				"-1152657617789587456l % 1l");
		assertEqual("0", String.valueOf(longData[6] & longData[0]),
				"-1152657617789587456l & 1l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[6] | longData[0]),
				"-1152657617789587456l | 1l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[6] ^ longData[0]),
				"-1152657617789587456l ^ 1l");
		assertEqual("false", String.valueOf(longData[6] > longData[0]),
				"-1152657617789587456l > 1l");
		assertEqual("true", String.valueOf(longData[6] < longData[0]),
				"-1152657617789587456l < 1l");
		assertEqual("false", String.valueOf(longData[6] == longData[0]),
				"-1152657617789587456l == 1l");
		assertEqual("false", String.valueOf(longData[6] >= longData[0]),
				"-1152657617789587456l >= 1l");
		assertEqual("true", String.valueOf(longData[6] <= longData[0]),
				"-1152657617789587456l <= 1l");
		assertEqual("-1152657617789587457",
				String.valueOf(longData[6] + longData[1]),
				"-1152657617789587456l + -1l");
		assertEqual("-1152657617789587455",
				String.valueOf(longData[6] - longData[1]),
				"-1152657617789587456l - -1l");
		assertEqual("1152657617789587456",
				String.valueOf(longData[6] * longData[1]),
				"-1152657617789587456l * -1l");
		assertEqual("1152657617789587456",
				String.valueOf(longData[6] / longData[1]),
				"-1152657617789587456l / -1l");
		assertEqual("0", String.valueOf(longData[6] % longData[1]),
				"-1152657617789587456l % -1l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] & longData[1]),
				"-1152657617789587456l & -1l");
		assertEqual("-1", String.valueOf(longData[6] | longData[1]),
				"-1152657617789587456l | -1l");
		assertEqual("1152657617789587455",
				String.valueOf(longData[6] ^ longData[1]),
				"-1152657617789587456l ^ -1l");
		assertEqual("false", String.valueOf(longData[6] > longData[1]),
				"-1152657617789587456l > -1l");
		assertEqual("true", String.valueOf(longData[6] < longData[1]),
				"-1152657617789587456l < -1l");
		assertEqual("false", String.valueOf(longData[6] == longData[1]),
				"-1152657617789587456l == -1l");
		assertEqual("false", String.valueOf(longData[6] >= longData[1]),
				"-1152657617789587456l >= -1l");
		assertEqual("true", String.valueOf(longData[6] <= longData[1]),
				"-1152657617789587456l <= -1l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[6] + longData[2]),
				"-1152657617789587456l + 17l");
		assertEqual("-1152657617789587473",
				String.valueOf(longData[6] - longData[2]),
				"-1152657617789587456l - 17l");
		assertEqual("-1148435428713435136",
				String.valueOf(longData[6] * longData[2]),
				"-1152657617789587456l * 17l");
		assertEqual("-67803389281740438",
				String.valueOf(longData[6] / longData[2]),
				"-1152657617789587456l / 17l");
		assertEqual("-10", String.valueOf(longData[6] % longData[2]),
				"-1152657617789587456l % 17l");
		assertEqual("0", String.valueOf(longData[6] & longData[2]),
				"-1152657617789587456l & 17l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[6] | longData[2]),
				"-1152657617789587456l | 17l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[6] ^ longData[2]),
				"-1152657617789587456l ^ 17l");
		assertEqual("false", String.valueOf(longData[6] > longData[2]),
				"-1152657617789587456l > 17l");
		assertEqual("true", String.valueOf(longData[6] < longData[2]),
				"-1152657617789587456l < 17l");
		assertEqual("false", String.valueOf(longData[6] == longData[2]),
				"-1152657617789587456l == 17l");
		assertEqual("false", String.valueOf(longData[6] >= longData[2]),
				"-1152657617789587456l >= 17l");
		assertEqual("true", String.valueOf(longData[6] <= longData[2]),
				"-1152657617789587456l <= 17l");
		assertEqual("-1152657617789587473",
				String.valueOf(longData[6] + longData[3]),
				"-1152657617789587456l + -17l");
		assertEqual("-1152657617789587439",
				String.valueOf(longData[6] - longData[3]),
				"-1152657617789587456l - -17l");
		assertEqual("1148435428713435136",
				String.valueOf(longData[6] * longData[3]),
				"-1152657617789587456l * -17l");
		assertEqual("67803389281740438",
				String.valueOf(longData[6] / longData[3]),
				"-1152657617789587456l / -17l");
		assertEqual("-10", String.valueOf(longData[6] % longData[3]),
				"-1152657617789587456l % -17l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] & longData[3]),
				"-1152657617789587456l & -17l");
		assertEqual("-17", String.valueOf(longData[6] | longData[3]),
				"-1152657617789587456l | -17l");
		assertEqual("1152657617789587439",
				String.valueOf(longData[6] ^ longData[3]),
				"-1152657617789587456l ^ -17l");
		assertEqual("false", String.valueOf(longData[6] > longData[3]),
				"-1152657617789587456l > -17l");
		assertEqual("true", String.valueOf(longData[6] < longData[3]),
				"-1152657617789587456l < -17l");
		assertEqual("false", String.valueOf(longData[6] == longData[3]),
				"-1152657617789587456l == -17l");
		assertEqual("false", String.valueOf(longData[6] >= longData[3]),
				"-1152657617789587456l >= -17l");
		assertEqual("true", String.valueOf(longData[6] <= longData[3]),
				"-1152657617789587456l <= -17l");
		assertEqual("-1152657613762994176",
				String.valueOf(longData[6] + longData[4]),
				"-1152657617789587456l + 4026593280l");
		assertEqual("-1152657621816180736",
				String.valueOf(longData[6] - longData[4]),
				"-1152657617789587456l - 4026593280l");
		assertEqual("-4467076046344159232",
				String.valueOf(longData[6] * longData[4]),
				"-1152657617789587456l * 4026593280l");
		assertEqual("-286261248", String.valueOf(longData[6] / longData[4]),
				"-1152657617789587456l / 4026593280l");
		assertEqual("-268374016", String.valueOf(longData[6] % longData[4]),
				"-1152657617789587456l % 4026593280l");
		assertEqual("4026593280", String.valueOf(longData[6] & longData[4]),
				"-1152657617789587456l & 4026593280l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] | longData[4]),
				"-1152657617789587456l | 4026593280l");
		assertEqual("-1152657621816180736",
				String.valueOf(longData[6] ^ longData[4]),
				"-1152657617789587456l ^ 4026593280l");
		assertEqual("false", String.valueOf(longData[6] > longData[4]),
				"-1152657617789587456l > 4026593280l");
		assertEqual("true", String.valueOf(longData[6] < longData[4]),
				"-1152657617789587456l < 4026593280l");
		assertEqual("false", String.valueOf(longData[6] == longData[4]),
				"-1152657617789587456l == 4026593280l");
		assertEqual("false", String.valueOf(longData[6] >= longData[4]),
				"-1152657617789587456l >= 4026593280l");
		assertEqual("true", String.valueOf(longData[6] <= longData[4]),
				"-1152657617789587456l <= 4026593280l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[6] + longData[5]),
				"-1152657617789587456l + 251662080l");
		assertEqual("-1152657618041249536",
				String.valueOf(longData[6] - longData[5]),
				"-1152657617789587456l - 251662080l");
		assertEqual("2026650756317184000",
				String.valueOf(longData[6] * longData[5]),
				"-1152657617789587456l * 251662080l");
		assertEqual("-4580179969", String.valueOf(longData[6] / longData[5]),
				"-1152657617789587456l / 251662080l");
		assertEqual("-16711936", String.valueOf(longData[6] % longData[5]),
				"-1152657617789587456l % 251662080l");
		assertEqual("0", String.valueOf(longData[6] & longData[5]),
				"-1152657617789587456l & 251662080l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[6] | longData[5]),
				"-1152657617789587456l | 251662080l");
		assertEqual("-1152657617537925376",
				String.valueOf(longData[6] ^ longData[5]),
				"-1152657617789587456l ^ 251662080l");
		assertEqual("false", String.valueOf(longData[6] > longData[5]),
				"-1152657617789587456l > 251662080l");
		assertEqual("true", String.valueOf(longData[6] < longData[5]),
				"-1152657617789587456l < 251662080l");
		assertEqual("false", String.valueOf(longData[6] == longData[5]),
				"-1152657617789587456l == 251662080l");
		assertEqual("false", String.valueOf(longData[6] >= longData[5]),
				"-1152657617789587456l >= 251662080l");
		assertEqual("true", String.valueOf(longData[6] <= longData[5]),
				"-1152657617789587456l <= 251662080l");
		assertEqual("-2305315235579174912",
				String.valueOf(longData[6] + longData[6]),
				"-1152657617789587456l + -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[6] - longData[6]),
				"-1152657617789587456l - -1152657617789587456l");
		assertEqual("-6700861461519925248",
				String.valueOf(longData[6] * longData[6]),
				"-1152657617789587456l * -1152657617789587456l");
		assertEqual("1", String.valueOf(longData[6] / longData[6]),
				"-1152657617789587456l / -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[6] % longData[6]),
				"-1152657617789587456l % -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] & longData[6]),
				"-1152657617789587456l & -1152657617789587456l");
		assertEqual("-1152657617789587456",
				String.valueOf(longData[6] | longData[6]),
				"-1152657617789587456l | -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[6] ^ longData[6]),
				"-1152657617789587456l ^ -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[6] > longData[6]),
				"-1152657617789587456l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[6] < longData[6]),
				"-1152657617789587456l < -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[6] == longData[6]),
				"-1152657617789587456l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[6] >= longData[6]),
				"-1152657617789587456l >= -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[6] <= longData[6]),
				"-1152657617789587456l <= -1152657617789587456l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[6] + longData[7]),
				"-1152657617789587456l + 1080880403494997760l");
		assertEqual("-2233538021284585216",
				String.valueOf(longData[6] - longData[7]),
				"-1152657617789587456l - 1080880403494997760l");
		assertEqual("3039960672475545600",
				String.valueOf(longData[6] * longData[7]),
				"-1152657617789587456l * 1080880403494997760l");
		assertEqual("-1", String.valueOf(longData[6] / longData[7]),
				"-1152657617789587456l / 1080880403494997760l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[6] % longData[7]),
				"-1152657617789587456l % 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[6] & longData[7]),
				"-1152657617789587456l & 1080880403494997760l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[6] | longData[7]),
				"-1152657617789587456l | 1080880403494997760l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[6] ^ longData[7]),
				"-1152657617789587456l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[6] > longData[7]),
				"-1152657617789587456l > 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[6] < longData[7]),
				"-1152657617789587456l < 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[6] == longData[7]),
				"-1152657617789587456l == 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[6] >= longData[7]),
				"-1152657617789587456l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[6] <= longData[7]),
				"-1152657617789587456l <= 1080880403494997760l");
		assertEqual("251662080", String.valueOf((int) longData[7]),
				"l2i 1080880403494997760l");
		assertEqual("1.0808804E18", String.valueOf((float) longData[7]),
				"l2f 1080880403494997760l");
		assertEqual("1.08088040349499776E18",
				String.valueOf((double) longData[7]),
				"l2d 1080880403494997760l");
		assertEqual("-1080880403494997760", String.valueOf(-longData[7]),
				"lneg 1080880403494997760l");
		assertEqual("1080880403494997760", String.valueOf(longData[7] >> 0),
				"1080880403494997760l >> 0");
		assertEqual("1080880403494997760", String.valueOf(longData[7] >>> 0),
				"1080880403494997760l >>> 0");
		assertEqual("1080880403494997760", String.valueOf(longData[7] << 0),
				"1080880403494997760l << 0");
		assertEqual("540440201747498880", String.valueOf(longData[7] >> 1),
				"1080880403494997760l >> 1");
		assertEqual("540440201747498880", String.valueOf(longData[7] >>> 1),
				"1080880403494997760l >>> 1");
		assertEqual("2161760806989995520", String.valueOf(longData[7] << 1),
				"1080880403494997760l << 1");
		assertEqual("270220100873749440", String.valueOf(longData[7] >> 2),
				"1080880403494997760l >> 2");
		assertEqual("270220100873749440", String.valueOf(longData[7] >>> 2),
				"1080880403494997760l >>> 2");
		assertEqual("4323521613979991040", String.valueOf(longData[7] << 2),
				"1080880403494997760l << 2");
		assertEqual("135110050436874720", String.valueOf(longData[7] >> 3),
				"1080880403494997760l >> 3");
		assertEqual("135110050436874720", String.valueOf(longData[7] >>> 3),
				"1080880403494997760l >>> 3");
		assertEqual("8647043227959982080", String.valueOf(longData[7] << 3),
				"1080880403494997760l << 3");
		assertEqual("67555025218437360", String.valueOf(longData[7] >> 4),
				"1080880403494997760l >> 4");
		assertEqual("67555025218437360", String.valueOf(longData[7] >>> 4),
				"1080880403494997760l >>> 4");
		assertEqual("-1152657617789587456", String.valueOf(longData[7] << 4),
				"1080880403494997760l << 4");
		assertEqual("33777512609218680", String.valueOf(longData[7] >> 5),
				"1080880403494997760l >> 5");
		assertEqual("33777512609218680", String.valueOf(longData[7] >>> 5),
				"1080880403494997760l >>> 5");
		assertEqual("-2305315235579174912", String.valueOf(longData[7] << 5),
				"1080880403494997760l << 5");
		assertEqual("16888756304609340", String.valueOf(longData[7] >> 6),
				"1080880403494997760l >> 6");
		assertEqual("16888756304609340", String.valueOf(longData[7] >>> 6),
				"1080880403494997760l >>> 6");
		assertEqual("-4610630471158349824", String.valueOf(longData[7] << 6),
				"1080880403494997760l << 6");
		assertEqual("8444378152304670", String.valueOf(longData[7] >> 7),
				"1080880403494997760l >> 7");
		assertEqual("8444378152304670", String.valueOf(longData[7] >>> 7),
				"1080880403494997760l >>> 7");
		assertEqual("-9221260942316699648", String.valueOf(longData[7] << 7),
				"1080880403494997760l << 7");
		assertEqual("4222189076152335", String.valueOf(longData[7] >> 8),
				"1080880403494997760l >> 8");
		assertEqual("4222189076152335", String.valueOf(longData[7] >>> 8),
				"1080880403494997760l >>> 8");
		assertEqual("4222189076152320", String.valueOf(longData[7] << 8),
				"1080880403494997760l << 8");
		assertEqual("2111094538076167", String.valueOf(longData[7] >> 9),
				"1080880403494997760l >> 9");
		assertEqual("2111094538076167", String.valueOf(longData[7] >>> 9),
				"1080880403494997760l >>> 9");
		assertEqual("8444378152304640", String.valueOf(longData[7] << 9),
				"1080880403494997760l << 9");
		assertEqual("1055547269038083", String.valueOf(longData[7] >> 10),
				"1080880403494997760l >> 10");
		assertEqual("1055547269038083", String.valueOf(longData[7] >>> 10),
				"1080880403494997760l >>> 10");
		assertEqual("16888756304609280", String.valueOf(longData[7] << 10),
				"1080880403494997760l << 10");
		assertEqual("527773634519041", String.valueOf(longData[7] >> 11),
				"1080880403494997760l >> 11");
		assertEqual("527773634519041", String.valueOf(longData[7] >>> 11),
				"1080880403494997760l >>> 11");
		assertEqual("33777512609218560", String.valueOf(longData[7] << 11),
				"1080880403494997760l << 11");
		assertEqual("263886817259520", String.valueOf(longData[7] >> 12),
				"1080880403494997760l >> 12");
		assertEqual("263886817259520", String.valueOf(longData[7] >>> 12),
				"1080880403494997760l >>> 12");
		assertEqual("67555025218437120", String.valueOf(longData[7] << 12),
				"1080880403494997760l << 12");
		assertEqual("131943408629760", String.valueOf(longData[7] >> 13),
				"1080880403494997760l >> 13");
		assertEqual("131943408629760", String.valueOf(longData[7] >>> 13),
				"1080880403494997760l >>> 13");
		assertEqual("135110050436874240", String.valueOf(longData[7] << 13),
				"1080880403494997760l << 13");
		assertEqual("65971704314880", String.valueOf(longData[7] >> 14),
				"1080880403494997760l >> 14");
		assertEqual("65971704314880", String.valueOf(longData[7] >>> 14),
				"1080880403494997760l >>> 14");
		assertEqual("270220100873748480", String.valueOf(longData[7] << 14),
				"1080880403494997760l << 14");
		assertEqual("32985852157440", String.valueOf(longData[7] >> 15),
				"1080880403494997760l >> 15");
		assertEqual("32985852157440", String.valueOf(longData[7] >>> 15),
				"1080880403494997760l >>> 15");
		assertEqual("540440201747496960", String.valueOf(longData[7] << 15),
				"1080880403494997760l << 15");
		assertEqual("16492926078720", String.valueOf(longData[7] >> 16),
				"1080880403494997760l >> 16");
		assertEqual("16492926078720", String.valueOf(longData[7] >>> 16),
				"1080880403494997760l >>> 16");
		assertEqual("1080880403494993920", String.valueOf(longData[7] << 16),
				"1080880403494997760l << 16");
		assertEqual("8246463039360", String.valueOf(longData[7] >> 17),
				"1080880403494997760l >> 17");
		assertEqual("8246463039360", String.valueOf(longData[7] >>> 17),
				"1080880403494997760l >>> 17");
		assertEqual("2161760806989987840", String.valueOf(longData[7] << 17),
				"1080880403494997760l << 17");
		assertEqual("4123231519680", String.valueOf(longData[7] >> 18),
				"1080880403494997760l >> 18");
		assertEqual("4123231519680", String.valueOf(longData[7] >>> 18),
				"1080880403494997760l >>> 18");
		assertEqual("4323521613979975680", String.valueOf(longData[7] << 18),
				"1080880403494997760l << 18");
		assertEqual("2061615759840", String.valueOf(longData[7] >> 19),
				"1080880403494997760l >> 19");
		assertEqual("2061615759840", String.valueOf(longData[7] >>> 19),
				"1080880403494997760l >>> 19");
		assertEqual("8647043227959951360", String.valueOf(longData[7] << 19),
				"1080880403494997760l << 19");
		assertEqual("1030807879920", String.valueOf(longData[7] >> 20),
				"1080880403494997760l >> 20");
		assertEqual("1030807879920", String.valueOf(longData[7] >>> 20),
				"1080880403494997760l >>> 20");
		assertEqual("-1152657617789648896", String.valueOf(longData[7] << 20),
				"1080880403494997760l << 20");
		assertEqual("515403939960", String.valueOf(longData[7] >> 21),
				"1080880403494997760l >> 21");
		assertEqual("515403939960", String.valueOf(longData[7] >>> 21),
				"1080880403494997760l >>> 21");
		assertEqual("-2305315235579297792", String.valueOf(longData[7] << 21),
				"1080880403494997760l << 21");
		assertEqual("257701969980", String.valueOf(longData[7] >> 22),
				"1080880403494997760l >> 22");
		assertEqual("257701969980", String.valueOf(longData[7] >>> 22),
				"1080880403494997760l >>> 22");
		assertEqual("-4610630471158595584", String.valueOf(longData[7] << 22),
				"1080880403494997760l << 22");
		assertEqual("128850984990", String.valueOf(longData[7] >> 23),
				"1080880403494997760l >> 23");
		assertEqual("128850984990", String.valueOf(longData[7] >>> 23),
				"1080880403494997760l >>> 23");
		assertEqual("-9221260942317191168", String.valueOf(longData[7] << 23),
				"1080880403494997760l << 23");
		assertEqual("64425492495", String.valueOf(longData[7] >> 24),
				"1080880403494997760l >> 24");
		assertEqual("64425492495", String.valueOf(longData[7] >>> 24),
				"1080880403494997760l >>> 24");
		assertEqual("4222189075169280", String.valueOf(longData[7] << 24),
				"1080880403494997760l << 24");
		assertEqual("32212746247", String.valueOf(longData[7] >> 25),
				"1080880403494997760l >> 25");
		assertEqual("32212746247", String.valueOf(longData[7] >>> 25),
				"1080880403494997760l >>> 25");
		assertEqual("8444378150338560", String.valueOf(longData[7] << 25),
				"1080880403494997760l << 25");
		assertEqual("16106373123", String.valueOf(longData[7] >> 26),
				"1080880403494997760l >> 26");
		assertEqual("16106373123", String.valueOf(longData[7] >>> 26),
				"1080880403494997760l >>> 26");
		assertEqual("16888756300677120", String.valueOf(longData[7] << 26),
				"1080880403494997760l << 26");
		assertEqual("8053186561", String.valueOf(longData[7] >> 27),
				"1080880403494997760l >> 27");
		assertEqual("8053186561", String.valueOf(longData[7] >>> 27),
				"1080880403494997760l >>> 27");
		assertEqual("33777512601354240", String.valueOf(longData[7] << 27),
				"1080880403494997760l << 27");
		assertEqual("4026593280", String.valueOf(longData[7] >> 28),
				"1080880403494997760l >> 28");
		assertEqual("4026593280", String.valueOf(longData[7] >>> 28),
				"1080880403494997760l >>> 28");
		assertEqual("67555025202708480", String.valueOf(longData[7] << 28),
				"1080880403494997760l << 28");
		assertEqual("2013296640", String.valueOf(longData[7] >> 29),
				"1080880403494997760l >> 29");
		assertEqual("2013296640", String.valueOf(longData[7] >>> 29),
				"1080880403494997760l >>> 29");
		assertEqual("135110050405416960", String.valueOf(longData[7] << 29),
				"1080880403494997760l << 29");
		assertEqual("1006648320", String.valueOf(longData[7] >> 30),
				"1080880403494997760l >> 30");
		assertEqual("1006648320", String.valueOf(longData[7] >>> 30),
				"1080880403494997760l >>> 30");
		assertEqual("270220100810833920", String.valueOf(longData[7] << 30),
				"1080880403494997760l << 30");
		assertEqual("503324160", String.valueOf(longData[7] >> 31),
				"1080880403494997760l >> 31");
		assertEqual("503324160", String.valueOf(longData[7] >>> 31),
				"1080880403494997760l >>> 31");
		assertEqual("540440201621667840", String.valueOf(longData[7] << 31),
				"1080880403494997760l << 31");
		assertEqual("251662080", String.valueOf(longData[7] >> 32),
				"1080880403494997760l >> 32");
		assertEqual("251662080", String.valueOf(longData[7] >>> 32),
				"1080880403494997760l >>> 32");
		assertEqual("1080880403243335680", String.valueOf(longData[7] << 32),
				"1080880403494997760l << 32");
		assertEqual("125831040", String.valueOf(longData[7] >> 33),
				"1080880403494997760l >> 33");
		assertEqual("125831040", String.valueOf(longData[7] >>> 33),
				"1080880403494997760l >>> 33");
		assertEqual("2161760806486671360", String.valueOf(longData[7] << 33),
				"1080880403494997760l << 33");
		assertEqual("62915520", String.valueOf(longData[7] >> 34),
				"1080880403494997760l >> 34");
		assertEqual("62915520", String.valueOf(longData[7] >>> 34),
				"1080880403494997760l >>> 34");
		assertEqual("4323521612973342720", String.valueOf(longData[7] << 34),
				"1080880403494997760l << 34");
		assertEqual("31457760", String.valueOf(longData[7] >> 35),
				"1080880403494997760l >> 35");
		assertEqual("31457760", String.valueOf(longData[7] >>> 35),
				"1080880403494997760l >>> 35");
		assertEqual("8647043225946685440", String.valueOf(longData[7] << 35),
				"1080880403494997760l << 35");
		assertEqual("15728880", String.valueOf(longData[7] >> 36),
				"1080880403494997760l >> 36");
		assertEqual("15728880", String.valueOf(longData[7] >>> 36),
				"1080880403494997760l >>> 36");
		assertEqual("-1152657621816180736", String.valueOf(longData[7] << 36),
				"1080880403494997760l << 36");
		assertEqual("7864440", String.valueOf(longData[7] >> 37),
				"1080880403494997760l >> 37");
		assertEqual("7864440", String.valueOf(longData[7] >>> 37),
				"1080880403494997760l >>> 37");
		assertEqual("-2305315243632361472", String.valueOf(longData[7] << 37),
				"1080880403494997760l << 37");
		assertEqual("3932220", String.valueOf(longData[7] >> 38),
				"1080880403494997760l >> 38");
		assertEqual("3932220", String.valueOf(longData[7] >>> 38),
				"1080880403494997760l >>> 38");
		assertEqual("-4610630487264722944", String.valueOf(longData[7] << 38),
				"1080880403494997760l << 38");
		assertEqual("1966110", String.valueOf(longData[7] >> 39),
				"1080880403494997760l >> 39");
		assertEqual("1966110", String.valueOf(longData[7] >>> 39),
				"1080880403494997760l >>> 39");
		assertEqual("-9221260974529445888", String.valueOf(longData[7] << 39),
				"1080880403494997760l << 39");
		assertEqual("983055", String.valueOf(longData[7] >> 40),
				"1080880403494997760l >> 40");
		assertEqual("983055", String.valueOf(longData[7] >>> 40),
				"1080880403494997760l >>> 40");
		assertEqual("4222124650659840", String.valueOf(longData[7] << 40),
				"1080880403494997760l << 40");
		assertEqual("491527", String.valueOf(longData[7] >> 41),
				"1080880403494997760l >> 41");
		assertEqual("491527", String.valueOf(longData[7] >>> 41),
				"1080880403494997760l >>> 41");
		assertEqual("8444249301319680", String.valueOf(longData[7] << 41),
				"1080880403494997760l << 41");
		assertEqual("245763", String.valueOf(longData[7] >> 42),
				"1080880403494997760l >> 42");
		assertEqual("245763", String.valueOf(longData[7] >>> 42),
				"1080880403494997760l >>> 42");
		assertEqual("16888498602639360", String.valueOf(longData[7] << 42),
				"1080880403494997760l << 42");
		assertEqual("122881", String.valueOf(longData[7] >> 43),
				"1080880403494997760l >> 43");
		assertEqual("122881", String.valueOf(longData[7] >>> 43),
				"1080880403494997760l >>> 43");
		assertEqual("33776997205278720", String.valueOf(longData[7] << 43),
				"1080880403494997760l << 43");
		assertEqual("61440", String.valueOf(longData[7] >> 44),
				"1080880403494997760l >> 44");
		assertEqual("61440", String.valueOf(longData[7] >>> 44),
				"1080880403494997760l >>> 44");
		assertEqual("67553994410557440", String.valueOf(longData[7] << 44),
				"1080880403494997760l << 44");
		assertEqual("30720", String.valueOf(longData[7] >> 45),
				"1080880403494997760l >> 45");
		assertEqual("30720", String.valueOf(longData[7] >>> 45),
				"1080880403494997760l >>> 45");
		assertEqual("135107988821114880", String.valueOf(longData[7] << 45),
				"1080880403494997760l << 45");
		assertEqual("15360", String.valueOf(longData[7] >> 46),
				"1080880403494997760l >> 46");
		assertEqual("15360", String.valueOf(longData[7] >>> 46),
				"1080880403494997760l >>> 46");
		assertEqual("270215977642229760", String.valueOf(longData[7] << 46),
				"1080880403494997760l << 46");
		assertEqual("7680", String.valueOf(longData[7] >> 47),
				"1080880403494997760l >> 47");
		assertEqual("7680", String.valueOf(longData[7] >>> 47),
				"1080880403494997760l >>> 47");
		assertEqual("540431955284459520", String.valueOf(longData[7] << 47),
				"1080880403494997760l << 47");
		assertEqual("3840", String.valueOf(longData[7] >> 48),
				"1080880403494997760l >> 48");
		assertEqual("3840", String.valueOf(longData[7] >>> 48),
				"1080880403494997760l >>> 48");
		assertEqual("1080863910568919040", String.valueOf(longData[7] << 48),
				"1080880403494997760l << 48");
		assertEqual("1920", String.valueOf(longData[7] >> 49),
				"1080880403494997760l >> 49");
		assertEqual("1920", String.valueOf(longData[7] >>> 49),
				"1080880403494997760l >>> 49");
		assertEqual("2161727821137838080", String.valueOf(longData[7] << 49),
				"1080880403494997760l << 49");
		assertEqual("960", String.valueOf(longData[7] >> 50),
				"1080880403494997760l >> 50");
		assertEqual("960", String.valueOf(longData[7] >>> 50),
				"1080880403494997760l >>> 50");
		assertEqual("4323455642275676160", String.valueOf(longData[7] << 50),
				"1080880403494997760l << 50");
		assertEqual("480", String.valueOf(longData[7] >> 51),
				"1080880403494997760l >> 51");
		assertEqual("480", String.valueOf(longData[7] >>> 51),
				"1080880403494997760l >>> 51");
		assertEqual("8646911284551352320", String.valueOf(longData[7] << 51),
				"1080880403494997760l << 51");
		assertEqual("240", String.valueOf(longData[7] >> 52),
				"1080880403494997760l >> 52");
		assertEqual("240", String.valueOf(longData[7] >>> 52),
				"1080880403494997760l >>> 52");
		assertEqual("-1152921504606846976", String.valueOf(longData[7] << 52),
				"1080880403494997760l << 52");
		assertEqual("120", String.valueOf(longData[7] >> 53),
				"1080880403494997760l >> 53");
		assertEqual("120", String.valueOf(longData[7] >>> 53),
				"1080880403494997760l >>> 53");
		assertEqual("-2305843009213693952", String.valueOf(longData[7] << 53),
				"1080880403494997760l << 53");
		assertEqual("60", String.valueOf(longData[7] >> 54),
				"1080880403494997760l >> 54");
		assertEqual("60", String.valueOf(longData[7] >>> 54),
				"1080880403494997760l >>> 54");
		assertEqual("-4611686018427387904", String.valueOf(longData[7] << 54),
				"1080880403494997760l << 54");
		assertEqual("30", String.valueOf(longData[7] >> 55),
				"1080880403494997760l >> 55");
		assertEqual("30", String.valueOf(longData[7] >>> 55),
				"1080880403494997760l >>> 55");
		assertEqual("-9223372036854775808", String.valueOf(longData[7] << 55),
				"1080880403494997760l << 55");
		assertEqual("15", String.valueOf(longData[7] >> 56),
				"1080880403494997760l >> 56");
		assertEqual("15", String.valueOf(longData[7] >>> 56),
				"1080880403494997760l >>> 56");
		assertEqual("0", String.valueOf(longData[7] << 56),
				"1080880403494997760l << 56");
		assertEqual("7", String.valueOf(longData[7] >> 57),
				"1080880403494997760l >> 57");
		assertEqual("7", String.valueOf(longData[7] >>> 57),
				"1080880403494997760l >>> 57");
		assertEqual("0", String.valueOf(longData[7] << 57),
				"1080880403494997760l << 57");
		assertEqual("3", String.valueOf(longData[7] >> 58),
				"1080880403494997760l >> 58");
		assertEqual("3", String.valueOf(longData[7] >>> 58),
				"1080880403494997760l >>> 58");
		assertEqual("0", String.valueOf(longData[7] << 58),
				"1080880403494997760l << 58");
		assertEqual("1", String.valueOf(longData[7] >> 59),
				"1080880403494997760l >> 59");
		assertEqual("1", String.valueOf(longData[7] >>> 59),
				"1080880403494997760l >>> 59");
		assertEqual("0", String.valueOf(longData[7] << 59),
				"1080880403494997760l << 59");
		assertEqual("0", String.valueOf(longData[7] >> 60),
				"1080880403494997760l >> 60");
		assertEqual("0", String.valueOf(longData[7] >>> 60),
				"1080880403494997760l >>> 60");
		assertEqual("0", String.valueOf(longData[7] << 60),
				"1080880403494997760l << 60");
		assertEqual("0", String.valueOf(longData[7] >> 61),
				"1080880403494997760l >> 61");
		assertEqual("0", String.valueOf(longData[7] >>> 61),
				"1080880403494997760l >>> 61");
		assertEqual("0", String.valueOf(longData[7] << 61),
				"1080880403494997760l << 61");
		assertEqual("0", String.valueOf(longData[7] >> 62),
				"1080880403494997760l >> 62");
		assertEqual("0", String.valueOf(longData[7] >>> 62),
				"1080880403494997760l >>> 62");
		assertEqual("0", String.valueOf(longData[7] << 62),
				"1080880403494997760l << 62");
		assertEqual("0", String.valueOf(longData[7] >> 63),
				"1080880403494997760l >> 63");
		assertEqual("0", String.valueOf(longData[7] >>> 63),
				"1080880403494997760l >>> 63");
		assertEqual("0", String.valueOf(longData[7] << 63),
				"1080880403494997760l << 63");
		assertEqual("1080880403494997761",
				String.valueOf(longData[7] + longData[0]),
				"1080880403494997760l + 1l");
		assertEqual("1080880403494997759",
				String.valueOf(longData[7] - longData[0]),
				"1080880403494997760l - 1l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] * longData[0]),
				"1080880403494997760l * 1l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] / longData[0]),
				"1080880403494997760l / 1l");
		assertEqual("0", String.valueOf(longData[7] % longData[0]),
				"1080880403494997760l % 1l");
		assertEqual("0", String.valueOf(longData[7] & longData[0]),
				"1080880403494997760l & 1l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[7] | longData[0]),
				"1080880403494997760l | 1l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[7] ^ longData[0]),
				"1080880403494997760l ^ 1l");
		assertEqual("true", String.valueOf(longData[7] > longData[0]),
				"1080880403494997760l > 1l");
		assertEqual("false", String.valueOf(longData[7] < longData[0]),
				"1080880403494997760l < 1l");
		assertEqual("false", String.valueOf(longData[7] == longData[0]),
				"1080880403494997760l == 1l");
		assertEqual("true", String.valueOf(longData[7] >= longData[0]),
				"1080880403494997760l >= 1l");
		assertEqual("false", String.valueOf(longData[7] <= longData[0]),
				"1080880403494997760l <= 1l");
		assertEqual("1080880403494997759",
				String.valueOf(longData[7] + longData[1]),
				"1080880403494997760l + -1l");
		assertEqual("1080880403494997761",
				String.valueOf(longData[7] - longData[1]),
				"1080880403494997760l - -1l");
		assertEqual("-1080880403494997760",
				String.valueOf(longData[7] * longData[1]),
				"1080880403494997760l * -1l");
		assertEqual("-1080880403494997760",
				String.valueOf(longData[7] / longData[1]),
				"1080880403494997760l / -1l");
		assertEqual("0", String.valueOf(longData[7] % longData[1]),
				"1080880403494997760l % -1l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] & longData[1]),
				"1080880403494997760l & -1l");
		assertEqual("-1", String.valueOf(longData[7] | longData[1]),
				"1080880403494997760l | -1l");
		assertEqual("-1080880403494997761",
				String.valueOf(longData[7] ^ longData[1]),
				"1080880403494997760l ^ -1l");
		assertEqual("true", String.valueOf(longData[7] > longData[1]),
				"1080880403494997760l > -1l");
		assertEqual("false", String.valueOf(longData[7] < longData[1]),
				"1080880403494997760l < -1l");
		assertEqual("false", String.valueOf(longData[7] == longData[1]),
				"1080880403494997760l == -1l");
		assertEqual("true", String.valueOf(longData[7] >= longData[1]),
				"1080880403494997760l >= -1l");
		assertEqual("false", String.valueOf(longData[7] <= longData[1]),
				"1080880403494997760l <= -1l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[7] + longData[2]),
				"1080880403494997760l + 17l");
		assertEqual("1080880403494997743",
				String.valueOf(longData[7] - longData[2]),
				"1080880403494997760l - 17l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[7] * longData[2]),
				"1080880403494997760l * 17l");
		assertEqual("63581200205588103",
				String.valueOf(longData[7] / longData[2]),
				"1080880403494997760l / 17l");
		assertEqual("9", String.valueOf(longData[7] % longData[2]),
				"1080880403494997760l % 17l");
		assertEqual("0", String.valueOf(longData[7] & longData[2]),
				"1080880403494997760l & 17l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[7] | longData[2]),
				"1080880403494997760l | 17l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[7] ^ longData[2]),
				"1080880403494997760l ^ 17l");
		assertEqual("true", String.valueOf(longData[7] > longData[2]),
				"1080880403494997760l > 17l");
		assertEqual("false", String.valueOf(longData[7] < longData[2]),
				"1080880403494997760l < 17l");
		assertEqual("false", String.valueOf(longData[7] == longData[2]),
				"1080880403494997760l == 17l");
		assertEqual("true", String.valueOf(longData[7] >= longData[2]),
				"1080880403494997760l >= 17l");
		assertEqual("false", String.valueOf(longData[7] <= longData[2]),
				"1080880403494997760l <= 17l");
		assertEqual("1080880403494997743",
				String.valueOf(longData[7] + longData[3]),
				"1080880403494997760l + -17l");
		assertEqual("1080880403494997777",
				String.valueOf(longData[7] - longData[3]),
				"1080880403494997760l - -17l");
		assertEqual("71777214294589696",
				String.valueOf(longData[7] * longData[3]),
				"1080880403494997760l * -17l");
		assertEqual("-63581200205588103",
				String.valueOf(longData[7] / longData[3]),
				"1080880403494997760l / -17l");
		assertEqual("9", String.valueOf(longData[7] % longData[3]),
				"1080880403494997760l % -17l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] & longData[3]),
				"1080880403494997760l & -17l");
		assertEqual("-17", String.valueOf(longData[7] | longData[3]),
				"1080880403494997760l | -17l");
		assertEqual("-1080880403494997777",
				String.valueOf(longData[7] ^ longData[3]),
				"1080880403494997760l ^ -17l");
		assertEqual("true", String.valueOf(longData[7] > longData[3]),
				"1080880403494997760l > -17l");
		assertEqual("false", String.valueOf(longData[7] < longData[3]),
				"1080880403494997760l < -17l");
		assertEqual("false", String.valueOf(longData[7] == longData[3]),
				"1080880403494997760l == -17l");
		assertEqual("true", String.valueOf(longData[7] >= longData[3]),
				"1080880403494997760l >= -17l");
		assertEqual("false", String.valueOf(longData[7] <= longData[3]),
				"1080880403494997760l <= -17l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[7] + longData[4]),
				"1080880403494997760l + 4026593280l");
		assertEqual("1080880399468404480",
				String.valueOf(longData[7] - longData[4]),
				"1080880403494997760l - 4026593280l");
		assertEqual("2026650756317184000",
				String.valueOf(longData[7] * longData[4]),
				"1080880403494997760l * 4026593280l");
		assertEqual("268435456", String.valueOf(longData[7] / longData[4]),
				"1080880403494997760l / 4026593280l");
		assertEqual("251662080", String.valueOf(longData[7] % longData[4]),
				"1080880403494997760l % 4026593280l");
		assertEqual("0", String.valueOf(longData[7] & longData[4]),
				"1080880403494997760l & 4026593280l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[7] | longData[4]),
				"1080880403494997760l | 4026593280l");
		assertEqual("1080880407521591040",
				String.valueOf(longData[7] ^ longData[4]),
				"1080880403494997760l ^ 4026593280l");
		assertEqual("true", String.valueOf(longData[7] > longData[4]),
				"1080880403494997760l > 4026593280l");
		assertEqual("false", String.valueOf(longData[7] < longData[4]),
				"1080880403494997760l < 4026593280l");
		assertEqual("false", String.valueOf(longData[7] == longData[4]),
				"1080880403494997760l == 4026593280l");
		assertEqual("true", String.valueOf(longData[7] >= longData[4]),
				"1080880403494997760l >= 4026593280l");
		assertEqual("false", String.valueOf(longData[7] <= longData[4]),
				"1080880403494997760l <= 4026593280l");
		assertEqual("1080880403746659840",
				String.valueOf(longData[7] + longData[5]),
				"1080880403494997760l + 251662080l");
		assertEqual("1080880403243335680",
				String.valueOf(longData[7] - longData[5]),
				"1080880403494997760l - 251662080l");
		assertEqual("126665672269824000",
				String.valueOf(longData[7] * longData[5]),
				"1080880403494997760l * 251662080l");
		assertEqual("4294967297", String.valueOf(longData[7] / longData[5]),
				"1080880403494997760l / 251662080l");
		assertEqual("0", String.valueOf(longData[7] % longData[5]),
				"1080880403494997760l % 251662080l");
		assertEqual("251662080", String.valueOf(longData[7] & longData[5]),
				"1080880403494997760l & 251662080l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] | longData[5]),
				"1080880403494997760l | 251662080l");
		assertEqual("1080880403243335680",
				String.valueOf(longData[7] ^ longData[5]),
				"1080880403494997760l ^ 251662080l");
		assertEqual("true", String.valueOf(longData[7] > longData[5]),
				"1080880403494997760l > 251662080l");
		assertEqual("false", String.valueOf(longData[7] < longData[5]),
				"1080880403494997760l < 251662080l");
		assertEqual("false", String.valueOf(longData[7] == longData[5]),
				"1080880403494997760l == 251662080l");
		assertEqual("true", String.valueOf(longData[7] >= longData[5]),
				"1080880403494997760l >= 251662080l");
		assertEqual("false", String.valueOf(longData[7] <= longData[5]),
				"1080880403494997760l <= 251662080l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[7] + longData[6]),
				"1080880403494997760l + -1152657617789587456l");
		assertEqual("2233538021284585216",
				String.valueOf(longData[7] - longData[6]),
				"1080880403494997760l - -1152657617789587456l");
		assertEqual("3039960672475545600",
				String.valueOf(longData[7] * longData[6]),
				"1080880403494997760l * -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[7] / longData[6]),
				"1080880403494997760l / -1152657617789587456l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] % longData[6]),
				"1080880403494997760l % -1152657617789587456l");
		assertEqual("0", String.valueOf(longData[7] & longData[6]),
				"1080880403494997760l & -1152657617789587456l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[7] | longData[6]),
				"1080880403494997760l | -1152657617789587456l");
		assertEqual("-71777214294589696",
				String.valueOf(longData[7] ^ longData[6]),
				"1080880403494997760l ^ -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[7] > longData[6]),
				"1080880403494997760l > -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[7] < longData[6]),
				"1080880403494997760l < -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[7] == longData[6]),
				"1080880403494997760l == -1152657617789587456l");
		assertEqual("true", String.valueOf(longData[7] >= longData[6]),
				"1080880403494997760l >= -1152657617789587456l");
		assertEqual("false", String.valueOf(longData[7] <= longData[6]),
				"1080880403494997760l <= -1152657617789587456l");
		assertEqual("2161760806989995520",
				String.valueOf(longData[7] + longData[7]),
				"1080880403494997760l + 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[7] - longData[7]),
				"1080880403494997760l - 1080880403494997760l");
		assertEqual("189997542029721600",
				String.valueOf(longData[7] * longData[7]),
				"1080880403494997760l * 1080880403494997760l");
		assertEqual("1", String.valueOf(longData[7] / longData[7]),
				"1080880403494997760l / 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[7] % longData[7]),
				"1080880403494997760l % 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] & longData[7]),
				"1080880403494997760l & 1080880403494997760l");
		assertEqual("1080880403494997760",
				String.valueOf(longData[7] | longData[7]),
				"1080880403494997760l | 1080880403494997760l");
		assertEqual("0", String.valueOf(longData[7] ^ longData[7]),
				"1080880403494997760l ^ 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[7] > longData[7]),
				"1080880403494997760l > 1080880403494997760l");
		assertEqual("false", String.valueOf(longData[7] < longData[7]),
				"1080880403494997760l < 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[7] == longData[7]),
				"1080880403494997760l == 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[7] >= longData[7]),
				"1080880403494997760l >= 1080880403494997760l");
		assertEqual("true", String.valueOf(longData[7] <= longData[7]),
				"1080880403494997760l <= 1080880403494997760l");

	}
	
	public static void strictTestInt(){
		assertEqual("1", String.valueOf((long)intData[0]), "i2l 1");
		assertEqual("1.0", String.valueOf((float)intData[0]), "i2f 1");
		assertEqual("1.0", String.valueOf((double)intData[0]), "i2d 1");
		assertEqual("1", String.valueOf((byte)intData[0]), "i2b 1");
		assertEqual("1", String.valueOf((short)intData[0]), "i2s 1");
		assertEqual("-1", String.valueOf(-intData[0]), "ineg 1");
		assertEqual("1", String.valueOf(intData[0] >> 0), "1 >> 0"); 
		assertEqual("1", String.valueOf(intData[0] >>> 0), "1 >>> 0"); 
		assertEqual("1", String.valueOf(intData[0] << 0), "1 << 0"); 
		assertEqual("0", String.valueOf(intData[0] >> 1), "1 >> 1"); 
		assertEqual("0", String.valueOf(intData[0] >>> 1), "1 >>> 1"); 
		assertEqual("2", String.valueOf(intData[0] << 1), "1 << 1"); 
		assertEqual("0", String.valueOf(intData[0] >> 2), "1 >> 2"); 
		assertEqual("0", String.valueOf(intData[0] >>> 2), "1 >>> 2"); 
		assertEqual("4", String.valueOf(intData[0] << 2), "1 << 2"); 
		assertEqual("0", String.valueOf(intData[0] >> 3), "1 >> 3"); 
		assertEqual("0", String.valueOf(intData[0] >>> 3), "1 >>> 3"); 
		assertEqual("8", String.valueOf(intData[0] << 3), "1 << 3"); 
		assertEqual("0", String.valueOf(intData[0] >> 4), "1 >> 4"); 
		assertEqual("0", String.valueOf(intData[0] >>> 4), "1 >>> 4"); 
		assertEqual("16", String.valueOf(intData[0] << 4), "1 << 4"); 
		assertEqual("0", String.valueOf(intData[0] >> 5), "1 >> 5"); 
		assertEqual("0", String.valueOf(intData[0] >>> 5), "1 >>> 5"); 
		assertEqual("32", String.valueOf(intData[0] << 5), "1 << 5"); 
		assertEqual("0", String.valueOf(intData[0] >> 6), "1 >> 6"); 
		assertEqual("0", String.valueOf(intData[0] >>> 6), "1 >>> 6"); 
		assertEqual("64", String.valueOf(intData[0] << 6), "1 << 6"); 
		assertEqual("0", String.valueOf(intData[0] >> 7), "1 >> 7"); 
		assertEqual("0", String.valueOf(intData[0] >>> 7), "1 >>> 7"); 
		assertEqual("128", String.valueOf(intData[0] << 7), "1 << 7"); 
		assertEqual("0", String.valueOf(intData[0] >> 8), "1 >> 8"); 
		assertEqual("0", String.valueOf(intData[0] >>> 8), "1 >>> 8"); 
		assertEqual("256", String.valueOf(intData[0] << 8), "1 << 8"); 
		assertEqual("0", String.valueOf(intData[0] >> 9), "1 >> 9"); 
		assertEqual("0", String.valueOf(intData[0] >>> 9), "1 >>> 9"); 
		assertEqual("512", String.valueOf(intData[0] << 9), "1 << 9"); 
		assertEqual("0", String.valueOf(intData[0] >> 10), "1 >> 10"); 
		assertEqual("0", String.valueOf(intData[0] >>> 10), "1 >>> 10"); 
		assertEqual("1024", String.valueOf(intData[0] << 10), "1 << 10"); 
		assertEqual("0", String.valueOf(intData[0] >> 11), "1 >> 11"); 
		assertEqual("0", String.valueOf(intData[0] >>> 11), "1 >>> 11"); 
		assertEqual("2048", String.valueOf(intData[0] << 11), "1 << 11"); 
		assertEqual("0", String.valueOf(intData[0] >> 12), "1 >> 12"); 
		assertEqual("0", String.valueOf(intData[0] >>> 12), "1 >>> 12"); 
		assertEqual("4096", String.valueOf(intData[0] << 12), "1 << 12"); 
		assertEqual("0", String.valueOf(intData[0] >> 13), "1 >> 13"); 
		assertEqual("0", String.valueOf(intData[0] >>> 13), "1 >>> 13"); 
		assertEqual("8192", String.valueOf(intData[0] << 13), "1 << 13"); 
		assertEqual("0", String.valueOf(intData[0] >> 14), "1 >> 14"); 
		assertEqual("0", String.valueOf(intData[0] >>> 14), "1 >>> 14"); 
		assertEqual("16384", String.valueOf(intData[0] << 14), "1 << 14"); 
		assertEqual("0", String.valueOf(intData[0] >> 15), "1 >> 15"); 
		assertEqual("0", String.valueOf(intData[0] >>> 15), "1 >>> 15"); 
		assertEqual("32768", String.valueOf(intData[0] << 15), "1 << 15"); 
		assertEqual("0", String.valueOf(intData[0] >> 16), "1 >> 16"); 
		assertEqual("0", String.valueOf(intData[0] >>> 16), "1 >>> 16"); 
		assertEqual("65536", String.valueOf(intData[0] << 16), "1 << 16"); 
		assertEqual("0", String.valueOf(intData[0] >> 17), "1 >> 17"); 
		assertEqual("0", String.valueOf(intData[0] >>> 17), "1 >>> 17"); 
		assertEqual("131072", String.valueOf(intData[0] << 17), "1 << 17"); 
		assertEqual("0", String.valueOf(intData[0] >> 18), "1 >> 18"); 
		assertEqual("0", String.valueOf(intData[0] >>> 18), "1 >>> 18"); 
		assertEqual("262144", String.valueOf(intData[0] << 18), "1 << 18"); 
		assertEqual("0", String.valueOf(intData[0] >> 19), "1 >> 19"); 
		assertEqual("0", String.valueOf(intData[0] >>> 19), "1 >>> 19"); 
		assertEqual("524288", String.valueOf(intData[0] << 19), "1 << 19"); 
		assertEqual("0", String.valueOf(intData[0] >> 20), "1 >> 20"); 
		assertEqual("0", String.valueOf(intData[0] >>> 20), "1 >>> 20"); 
		assertEqual("1048576", String.valueOf(intData[0] << 20), "1 << 20"); 
		assertEqual("0", String.valueOf(intData[0] >> 21), "1 >> 21"); 
		assertEqual("0", String.valueOf(intData[0] >>> 21), "1 >>> 21"); 
		assertEqual("2097152", String.valueOf(intData[0] << 21), "1 << 21"); 
		assertEqual("0", String.valueOf(intData[0] >> 22), "1 >> 22"); 
		assertEqual("0", String.valueOf(intData[0] >>> 22), "1 >>> 22"); 
		assertEqual("4194304", String.valueOf(intData[0] << 22), "1 << 22"); 
		assertEqual("0", String.valueOf(intData[0] >> 23), "1 >> 23"); 
		assertEqual("0", String.valueOf(intData[0] >>> 23), "1 >>> 23"); 
		assertEqual("8388608", String.valueOf(intData[0] << 23), "1 << 23"); 
		assertEqual("0", String.valueOf(intData[0] >> 24), "1 >> 24"); 
		assertEqual("0", String.valueOf(intData[0] >>> 24), "1 >>> 24"); 
		assertEqual("16777216", String.valueOf(intData[0] << 24), "1 << 24"); 
		assertEqual("0", String.valueOf(intData[0] >> 25), "1 >> 25"); 
		assertEqual("0", String.valueOf(intData[0] >>> 25), "1 >>> 25"); 
		assertEqual("33554432", String.valueOf(intData[0] << 25), "1 << 25"); 
		assertEqual("0", String.valueOf(intData[0] >> 26), "1 >> 26"); 
		assertEqual("0", String.valueOf(intData[0] >>> 26), "1 >>> 26"); 
		assertEqual("67108864", String.valueOf(intData[0] << 26), "1 << 26"); 
		assertEqual("0", String.valueOf(intData[0] >> 27), "1 >> 27"); 
		assertEqual("0", String.valueOf(intData[0] >>> 27), "1 >>> 27"); 
		assertEqual("134217728", String.valueOf(intData[0] << 27), "1 << 27"); 
		assertEqual("0", String.valueOf(intData[0] >> 28), "1 >> 28"); 
		assertEqual("0", String.valueOf(intData[0] >>> 28), "1 >>> 28"); 
		assertEqual("268435456", String.valueOf(intData[0] << 28), "1 << 28"); 
		assertEqual("0", String.valueOf(intData[0] >> 29), "1 >> 29"); 
		assertEqual("0", String.valueOf(intData[0] >>> 29), "1 >>> 29"); 
		assertEqual("536870912", String.valueOf(intData[0] << 29), "1 << 29"); 
		assertEqual("0", String.valueOf(intData[0] >> 30), "1 >> 30"); 
		assertEqual("0", String.valueOf(intData[0] >>> 30), "1 >>> 30"); 
		assertEqual("1073741824", String.valueOf(intData[0] << 30), "1 << 30"); 
		assertEqual("0", String.valueOf(intData[0] >> 31), "1 >> 31"); 
		assertEqual("0", String.valueOf(intData[0] >>> 31), "1 >>> 31"); 
		assertEqual("-2147483648", String.valueOf(intData[0] << 31), "1 << 31"); 
		assertEqual("2", String.valueOf(intData[0] + intData[0]), "1 + 1"); 
		assertEqual("0", String.valueOf(intData[0] - intData[0]), "1 - 1"); 
		assertEqual("1", String.valueOf(intData[0] * intData[0]), "1 * 1"); 
		assertEqual("1", String.valueOf(intData[0] / intData[0]), "1 / 1"); 
		assertEqual("0", String.valueOf(intData[0] % intData[0]), "1 % 1"); 
		assertEqual("1", String.valueOf(intData[0] & intData[0]), "1 & 1"); 
		assertEqual("1", String.valueOf(intData[0] | intData[0]), "1 | 1"); 
		assertEqual("0", String.valueOf(intData[0] ^ intData[0]), "1 ^ 1"); 
		assertEqual("false", String.valueOf(intData[0] > intData[0]), "1 > 1"); 
		assertEqual("false", String.valueOf(intData[0] < intData[0]), "1 < 1"); 
		assertEqual("true", String.valueOf(intData[0] == intData[0]), "1 == 1"); 
		assertEqual("true", String.valueOf(intData[0] >= intData[0]), "1 >= 1"); 
		assertEqual("true", String.valueOf(intData[0] <= intData[0]), "1 <= 1"); 
		assertEqual("0", String.valueOf(intData[0] + intData[1]), "1 + -1"); 
		assertEqual("2", String.valueOf(intData[0] - intData[1]), "1 - -1"); 
		assertEqual("-1", String.valueOf(intData[0] * intData[1]), "1 * -1"); 
		assertEqual("-1", String.valueOf(intData[0] / intData[1]), "1 / -1"); 
		assertEqual("0", String.valueOf(intData[0] % intData[1]), "1 % -1"); 
		assertEqual("1", String.valueOf(intData[0] & intData[1]), "1 & -1"); 
		assertEqual("-1", String.valueOf(intData[0] | intData[1]), "1 | -1"); 
		assertEqual("-2", String.valueOf(intData[0] ^ intData[1]), "1 ^ -1"); 
		assertEqual("true", String.valueOf(intData[0] > intData[1]), "1 > -1"); 
		assertEqual("false", String.valueOf(intData[0] < intData[1]), "1 < -1"); 
		assertEqual("false", String.valueOf(intData[0] == intData[1]), "1 == -1"); 
		assertEqual("true", String.valueOf(intData[0] >= intData[1]), "1 >= -1"); 
		assertEqual("false", String.valueOf(intData[0] <= intData[1]), "1 <= -1"); 
		assertEqual("18", String.valueOf(intData[0] + intData[2]), "1 + 17"); 
		assertEqual("-16", String.valueOf(intData[0] - intData[2]), "1 - 17"); 
		assertEqual("17", String.valueOf(intData[0] * intData[2]), "1 * 17"); 
		assertEqual("0", String.valueOf(intData[0] / intData[2]), "1 / 17"); 
		assertEqual("1", String.valueOf(intData[0] % intData[2]), "1 % 17"); 
		assertEqual("1", String.valueOf(intData[0] & intData[2]), "1 & 17"); 
		assertEqual("17", String.valueOf(intData[0] | intData[2]), "1 | 17"); 
		assertEqual("16", String.valueOf(intData[0] ^ intData[2]), "1 ^ 17"); 
		assertEqual("false", String.valueOf(intData[0] > intData[2]), "1 > 17"); 
		assertEqual("true", String.valueOf(intData[0] < intData[2]), "1 < 17"); 
		assertEqual("false", String.valueOf(intData[0] == intData[2]), "1 == 17"); 
		assertEqual("false", String.valueOf(intData[0] >= intData[2]), "1 >= 17"); 
		assertEqual("true", String.valueOf(intData[0] <= intData[2]), "1 <= 17"); 
		assertEqual("-16", String.valueOf(intData[0] + intData[3]), "1 + -17"); 
		assertEqual("18", String.valueOf(intData[0] - intData[3]), "1 - -17"); 
		assertEqual("-17", String.valueOf(intData[0] * intData[3]), "1 * -17"); 
		assertEqual("0", String.valueOf(intData[0] / intData[3]), "1 / -17"); 
		assertEqual("1", String.valueOf(intData[0] % intData[3]), "1 % -17"); 
		assertEqual("1", String.valueOf(intData[0] & intData[3]), "1 & -17"); 
		assertEqual("-17", String.valueOf(intData[0] | intData[3]), "1 | -17"); 
		assertEqual("-18", String.valueOf(intData[0] ^ intData[3]), "1 ^ -17"); 
		assertEqual("true", String.valueOf(intData[0] > intData[3]), "1 > -17"); 
		assertEqual("false", String.valueOf(intData[0] < intData[3]), "1 < -17"); 
		assertEqual("false", String.valueOf(intData[0] == intData[3]), "1 == -17"); 
		assertEqual("true", String.valueOf(intData[0] >= intData[3]), "1 >= -17"); 
		assertEqual("false", String.valueOf(intData[0] <= intData[3]), "1 <= -17"); 
		assertEqual("-268374015", String.valueOf(intData[0] + intData[4]), "1 + -268374016"); 
		assertEqual("268374017", String.valueOf(intData[0] - intData[4]), "1 - -268374016"); 
		assertEqual("-268374016", String.valueOf(intData[0] * intData[4]), "1 * -268374016"); 
		assertEqual("0", String.valueOf(intData[0] / intData[4]), "1 / -268374016"); 
		assertEqual("1", String.valueOf(intData[0] % intData[4]), "1 % -268374016"); 
		assertEqual("0", String.valueOf(intData[0] & intData[4]), "1 & -268374016"); 
		assertEqual("-268374015", String.valueOf(intData[0] | intData[4]), "1 | -268374016"); 
		assertEqual("-268374015", String.valueOf(intData[0] ^ intData[4]), "1 ^ -268374016"); 
		assertEqual("true", String.valueOf(intData[0] > intData[4]), "1 > -268374016"); 
		assertEqual("false", String.valueOf(intData[0] < intData[4]), "1 < -268374016"); 
		assertEqual("false", String.valueOf(intData[0] == intData[4]), "1 == -268374016"); 
		assertEqual("true", String.valueOf(intData[0] >= intData[4]), "1 >= -268374016"); 
		assertEqual("false", String.valueOf(intData[0] <= intData[4]), "1 <= -268374016"); 
		assertEqual("251662081", String.valueOf(intData[0] + intData[5]), "1 + 251662080"); 
		assertEqual("-251662079", String.valueOf(intData[0] - intData[5]), "1 - 251662080"); 
		assertEqual("251662080", String.valueOf(intData[0] * intData[5]), "1 * 251662080"); 
		assertEqual("0", String.valueOf(intData[0] / intData[5]), "1 / 251662080"); 
		assertEqual("1", String.valueOf(intData[0] % intData[5]), "1 % 251662080"); 
		assertEqual("0", String.valueOf(intData[0] & intData[5]), "1 & 251662080"); 
		assertEqual("251662081", String.valueOf(intData[0] | intData[5]), "1 | 251662080"); 
		assertEqual("251662081", String.valueOf(intData[0] ^ intData[5]), "1 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[0] > intData[5]), "1 > 251662080"); 
		assertEqual("true", String.valueOf(intData[0] < intData[5]), "1 < 251662080"); 
		assertEqual("false", String.valueOf(intData[0] == intData[5]), "1 == 251662080"); 
		assertEqual("false", String.valueOf(intData[0] >= intData[5]), "1 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[0] <= intData[5]), "1 <= 251662080"); 
		assertEqual("-1", String.valueOf((long)intData[1]), "i2l -1");
		assertEqual("-1.0", String.valueOf((float)intData[1]), "i2f -1");
		assertEqual("-1.0", String.valueOf((double)intData[1]), "i2d -1");
		assertEqual("-1", String.valueOf((byte)intData[1]), "i2b -1");
		assertEqual("-1", String.valueOf((short)intData[1]), "i2s -1");
		assertEqual("1", String.valueOf(-intData[1]), "ineg -1");
		assertEqual("-1", String.valueOf(intData[1] >> 0), "-1 >> 0"); 
		assertEqual("-1", String.valueOf(intData[1] >>> 0), "-1 >>> 0"); 
		assertEqual("-1", String.valueOf(intData[1] << 0), "-1 << 0"); 
		assertEqual("-1", String.valueOf(intData[1] >> 1), "-1 >> 1"); 
		assertEqual("2147483647", String.valueOf(intData[1] >>> 1), "-1 >>> 1"); 
		assertEqual("-2", String.valueOf(intData[1] << 1), "-1 << 1"); 
		assertEqual("-1", String.valueOf(intData[1] >> 2), "-1 >> 2"); 
		assertEqual("1073741823", String.valueOf(intData[1] >>> 2), "-1 >>> 2"); 
		assertEqual("-4", String.valueOf(intData[1] << 2), "-1 << 2"); 
		assertEqual("-1", String.valueOf(intData[1] >> 3), "-1 >> 3"); 
		assertEqual("536870911", String.valueOf(intData[1] >>> 3), "-1 >>> 3"); 
		assertEqual("-8", String.valueOf(intData[1] << 3), "-1 << 3"); 
		assertEqual("-1", String.valueOf(intData[1] >> 4), "-1 >> 4"); 
		assertEqual("268435455", String.valueOf(intData[1] >>> 4), "-1 >>> 4"); 
		assertEqual("-16", String.valueOf(intData[1] << 4), "-1 << 4"); 
		assertEqual("-1", String.valueOf(intData[1] >> 5), "-1 >> 5"); 
		assertEqual("134217727", String.valueOf(intData[1] >>> 5), "-1 >>> 5"); 
		assertEqual("-32", String.valueOf(intData[1] << 5), "-1 << 5"); 
		assertEqual("-1", String.valueOf(intData[1] >> 6), "-1 >> 6"); 
		assertEqual("67108863", String.valueOf(intData[1] >>> 6), "-1 >>> 6"); 
		assertEqual("-64", String.valueOf(intData[1] << 6), "-1 << 6"); 
		assertEqual("-1", String.valueOf(intData[1] >> 7), "-1 >> 7"); 
		assertEqual("33554431", String.valueOf(intData[1] >>> 7), "-1 >>> 7"); 
		assertEqual("-128", String.valueOf(intData[1] << 7), "-1 << 7"); 
		assertEqual("-1", String.valueOf(intData[1] >> 8), "-1 >> 8"); 
		assertEqual("16777215", String.valueOf(intData[1] >>> 8), "-1 >>> 8"); 
		assertEqual("-256", String.valueOf(intData[1] << 8), "-1 << 8"); 
		assertEqual("-1", String.valueOf(intData[1] >> 9), "-1 >> 9"); 
		assertEqual("8388607", String.valueOf(intData[1] >>> 9), "-1 >>> 9"); 
		assertEqual("-512", String.valueOf(intData[1] << 9), "-1 << 9"); 
		assertEqual("-1", String.valueOf(intData[1] >> 10), "-1 >> 10"); 
		assertEqual("4194303", String.valueOf(intData[1] >>> 10), "-1 >>> 10"); 
		assertEqual("-1024", String.valueOf(intData[1] << 10), "-1 << 10"); 
		assertEqual("-1", String.valueOf(intData[1] >> 11), "-1 >> 11"); 
		assertEqual("2097151", String.valueOf(intData[1] >>> 11), "-1 >>> 11"); 
		assertEqual("-2048", String.valueOf(intData[1] << 11), "-1 << 11"); 
		assertEqual("-1", String.valueOf(intData[1] >> 12), "-1 >> 12"); 
		assertEqual("1048575", String.valueOf(intData[1] >>> 12), "-1 >>> 12"); 
		assertEqual("-4096", String.valueOf(intData[1] << 12), "-1 << 12"); 
		assertEqual("-1", String.valueOf(intData[1] >> 13), "-1 >> 13"); 
		assertEqual("524287", String.valueOf(intData[1] >>> 13), "-1 >>> 13"); 
		assertEqual("-8192", String.valueOf(intData[1] << 13), "-1 << 13"); 
		assertEqual("-1", String.valueOf(intData[1] >> 14), "-1 >> 14"); 
		assertEqual("262143", String.valueOf(intData[1] >>> 14), "-1 >>> 14"); 
		assertEqual("-16384", String.valueOf(intData[1] << 14), "-1 << 14"); 
		assertEqual("-1", String.valueOf(intData[1] >> 15), "-1 >> 15"); 
		assertEqual("131071", String.valueOf(intData[1] >>> 15), "-1 >>> 15"); 
		assertEqual("-32768", String.valueOf(intData[1] << 15), "-1 << 15"); 
		assertEqual("-1", String.valueOf(intData[1] >> 16), "-1 >> 16"); 
		assertEqual("65535", String.valueOf(intData[1] >>> 16), "-1 >>> 16"); 
		assertEqual("-65536", String.valueOf(intData[1] << 16), "-1 << 16"); 
		assertEqual("-1", String.valueOf(intData[1] >> 17), "-1 >> 17"); 
		assertEqual("32767", String.valueOf(intData[1] >>> 17), "-1 >>> 17"); 
		assertEqual("-131072", String.valueOf(intData[1] << 17), "-1 << 17"); 
		assertEqual("-1", String.valueOf(intData[1] >> 18), "-1 >> 18"); 
		assertEqual("16383", String.valueOf(intData[1] >>> 18), "-1 >>> 18"); 
		assertEqual("-262144", String.valueOf(intData[1] << 18), "-1 << 18"); 
		assertEqual("-1", String.valueOf(intData[1] >> 19), "-1 >> 19"); 
		assertEqual("8191", String.valueOf(intData[1] >>> 19), "-1 >>> 19"); 
		assertEqual("-524288", String.valueOf(intData[1] << 19), "-1 << 19"); 
		assertEqual("-1", String.valueOf(intData[1] >> 20), "-1 >> 20"); 
		assertEqual("4095", String.valueOf(intData[1] >>> 20), "-1 >>> 20"); 
		assertEqual("-1048576", String.valueOf(intData[1] << 20), "-1 << 20"); 
		assertEqual("-1", String.valueOf(intData[1] >> 21), "-1 >> 21"); 
		assertEqual("2047", String.valueOf(intData[1] >>> 21), "-1 >>> 21"); 
		assertEqual("-2097152", String.valueOf(intData[1] << 21), "-1 << 21"); 
		assertEqual("-1", String.valueOf(intData[1] >> 22), "-1 >> 22"); 
		assertEqual("1023", String.valueOf(intData[1] >>> 22), "-1 >>> 22"); 
		assertEqual("-4194304", String.valueOf(intData[1] << 22), "-1 << 22"); 
		assertEqual("-1", String.valueOf(intData[1] >> 23), "-1 >> 23"); 
		assertEqual("511", String.valueOf(intData[1] >>> 23), "-1 >>> 23"); 
		assertEqual("-8388608", String.valueOf(intData[1] << 23), "-1 << 23"); 
		assertEqual("-1", String.valueOf(intData[1] >> 24), "-1 >> 24"); 
		assertEqual("255", String.valueOf(intData[1] >>> 24), "-1 >>> 24"); 
		assertEqual("-16777216", String.valueOf(intData[1] << 24), "-1 << 24"); 
		assertEqual("-1", String.valueOf(intData[1] >> 25), "-1 >> 25"); 
		assertEqual("127", String.valueOf(intData[1] >>> 25), "-1 >>> 25"); 
		assertEqual("-33554432", String.valueOf(intData[1] << 25), "-1 << 25"); 
		assertEqual("-1", String.valueOf(intData[1] >> 26), "-1 >> 26"); 
		assertEqual("63", String.valueOf(intData[1] >>> 26), "-1 >>> 26"); 
		assertEqual("-67108864", String.valueOf(intData[1] << 26), "-1 << 26"); 
		assertEqual("-1", String.valueOf(intData[1] >> 27), "-1 >> 27"); 
		assertEqual("31", String.valueOf(intData[1] >>> 27), "-1 >>> 27"); 
		assertEqual("-134217728", String.valueOf(intData[1] << 27), "-1 << 27"); 
		assertEqual("-1", String.valueOf(intData[1] >> 28), "-1 >> 28"); 
		assertEqual("15", String.valueOf(intData[1] >>> 28), "-1 >>> 28"); 
		assertEqual("-268435456", String.valueOf(intData[1] << 28), "-1 << 28"); 
		assertEqual("-1", String.valueOf(intData[1] >> 29), "-1 >> 29"); 
		assertEqual("7", String.valueOf(intData[1] >>> 29), "-1 >>> 29"); 
		assertEqual("-536870912", String.valueOf(intData[1] << 29), "-1 << 29"); 
		assertEqual("-1", String.valueOf(intData[1] >> 30), "-1 >> 30"); 
		assertEqual("3", String.valueOf(intData[1] >>> 30), "-1 >>> 30"); 
		assertEqual("-1073741824", String.valueOf(intData[1] << 30), "-1 << 30"); 
		assertEqual("-1", String.valueOf(intData[1] >> 31), "-1 >> 31"); 
		assertEqual("1", String.valueOf(intData[1] >>> 31), "-1 >>> 31"); 
		assertEqual("-2147483648", String.valueOf(intData[1] << 31), "-1 << 31"); 
		assertEqual("0", String.valueOf(intData[1] + intData[0]), "-1 + 1"); 
		assertEqual("-2", String.valueOf(intData[1] - intData[0]), "-1 - 1"); 
		assertEqual("-1", String.valueOf(intData[1] * intData[0]), "-1 * 1"); 
		assertEqual("-1", String.valueOf(intData[1] / intData[0]), "-1 / 1"); 
		assertEqual("0", String.valueOf(intData[1] % intData[0]), "-1 % 1"); 
		assertEqual("1", String.valueOf(intData[1] & intData[0]), "-1 & 1"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[0]), "-1 | 1"); 
		assertEqual("-2", String.valueOf(intData[1] ^ intData[0]), "-1 ^ 1"); 
		assertEqual("false", String.valueOf(intData[1] > intData[0]), "-1 > 1"); 
		assertEqual("true", String.valueOf(intData[1] < intData[0]), "-1 < 1"); 
		assertEqual("false", String.valueOf(intData[1] == intData[0]), "-1 == 1"); 
		assertEqual("false", String.valueOf(intData[1] >= intData[0]), "-1 >= 1"); 
		assertEqual("true", String.valueOf(intData[1] <= intData[0]), "-1 <= 1"); 
		assertEqual("-2", String.valueOf(intData[1] + intData[1]), "-1 + -1"); 
		assertEqual("0", String.valueOf(intData[1] - intData[1]), "-1 - -1"); 
		assertEqual("1", String.valueOf(intData[1] * intData[1]), "-1 * -1"); 
		assertEqual("1", String.valueOf(intData[1] / intData[1]), "-1 / -1"); 
		assertEqual("0", String.valueOf(intData[1] % intData[1]), "-1 % -1"); 
		assertEqual("-1", String.valueOf(intData[1] & intData[1]), "-1 & -1"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[1]), "-1 | -1"); 
		assertEqual("0", String.valueOf(intData[1] ^ intData[1]), "-1 ^ -1"); 
		assertEqual("false", String.valueOf(intData[1] > intData[1]), "-1 > -1"); 
		assertEqual("false", String.valueOf(intData[1] < intData[1]), "-1 < -1"); 
		assertEqual("true", String.valueOf(intData[1] == intData[1]), "-1 == -1"); 
		assertEqual("true", String.valueOf(intData[1] >= intData[1]), "-1 >= -1"); 
		assertEqual("true", String.valueOf(intData[1] <= intData[1]), "-1 <= -1"); 
		assertEqual("16", String.valueOf(intData[1] + intData[2]), "-1 + 17"); 
		assertEqual("-18", String.valueOf(intData[1] - intData[2]), "-1 - 17"); 
		assertEqual("-17", String.valueOf(intData[1] * intData[2]), "-1 * 17"); 
		assertEqual("0", String.valueOf(intData[1] / intData[2]), "-1 / 17"); 
		assertEqual("-1", String.valueOf(intData[1] % intData[2]), "-1 % 17"); 
		assertEqual("17", String.valueOf(intData[1] & intData[2]), "-1 & 17"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[2]), "-1 | 17"); 
		assertEqual("-18", String.valueOf(intData[1] ^ intData[2]), "-1 ^ 17"); 
		assertEqual("false", String.valueOf(intData[1] > intData[2]), "-1 > 17"); 
		assertEqual("true", String.valueOf(intData[1] < intData[2]), "-1 < 17"); 
		assertEqual("false", String.valueOf(intData[1] == intData[2]), "-1 == 17"); 
		assertEqual("false", String.valueOf(intData[1] >= intData[2]), "-1 >= 17"); 
		assertEqual("true", String.valueOf(intData[1] <= intData[2]), "-1 <= 17"); 
		assertEqual("-18", String.valueOf(intData[1] + intData[3]), "-1 + -17"); 
		assertEqual("16", String.valueOf(intData[1] - intData[3]), "-1 - -17"); 
		assertEqual("17", String.valueOf(intData[1] * intData[3]), "-1 * -17"); 
		assertEqual("0", String.valueOf(intData[1] / intData[3]), "-1 / -17"); 
		assertEqual("-1", String.valueOf(intData[1] % intData[3]), "-1 % -17"); 
		assertEqual("-17", String.valueOf(intData[1] & intData[3]), "-1 & -17"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[3]), "-1 | -17"); 
		assertEqual("16", String.valueOf(intData[1] ^ intData[3]), "-1 ^ -17"); 
		assertEqual("true", String.valueOf(intData[1] > intData[3]), "-1 > -17"); 
		assertEqual("false", String.valueOf(intData[1] < intData[3]), "-1 < -17"); 
		assertEqual("false", String.valueOf(intData[1] == intData[3]), "-1 == -17"); 
		assertEqual("true", String.valueOf(intData[1] >= intData[3]), "-1 >= -17"); 
		assertEqual("false", String.valueOf(intData[1] <= intData[3]), "-1 <= -17"); 
		assertEqual("-268374017", String.valueOf(intData[1] + intData[4]), "-1 + -268374016"); 
		assertEqual("268374015", String.valueOf(intData[1] - intData[4]), "-1 - -268374016"); 
		assertEqual("268374016", String.valueOf(intData[1] * intData[4]), "-1 * -268374016"); 
		assertEqual("0", String.valueOf(intData[1] / intData[4]), "-1 / -268374016"); 
		assertEqual("-1", String.valueOf(intData[1] % intData[4]), "-1 % -268374016"); 
		assertEqual("-268374016", String.valueOf(intData[1] & intData[4]), "-1 & -268374016"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[4]), "-1 | -268374016"); 
		assertEqual("268374015", String.valueOf(intData[1] ^ intData[4]), "-1 ^ -268374016"); 
		assertEqual("true", String.valueOf(intData[1] > intData[4]), "-1 > -268374016"); 
		assertEqual("false", String.valueOf(intData[1] < intData[4]), "-1 < -268374016"); 
		assertEqual("false", String.valueOf(intData[1] == intData[4]), "-1 == -268374016"); 
		assertEqual("true", String.valueOf(intData[1] >= intData[4]), "-1 >= -268374016"); 
		assertEqual("false", String.valueOf(intData[1] <= intData[4]), "-1 <= -268374016"); 
		assertEqual("251662079", String.valueOf(intData[1] + intData[5]), "-1 + 251662080"); 
		assertEqual("-251662081", String.valueOf(intData[1] - intData[5]), "-1 - 251662080"); 
		assertEqual("-251662080", String.valueOf(intData[1] * intData[5]), "-1 * 251662080"); 
		assertEqual("0", String.valueOf(intData[1] / intData[5]), "-1 / 251662080"); 
		assertEqual("-1", String.valueOf(intData[1] % intData[5]), "-1 % 251662080"); 
		assertEqual("251662080", String.valueOf(intData[1] & intData[5]), "-1 & 251662080"); 
		assertEqual("-1", String.valueOf(intData[1] | intData[5]), "-1 | 251662080"); 
		assertEqual("-251662081", String.valueOf(intData[1] ^ intData[5]), "-1 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[1] > intData[5]), "-1 > 251662080"); 
		assertEqual("true", String.valueOf(intData[1] < intData[5]), "-1 < 251662080"); 
		assertEqual("false", String.valueOf(intData[1] == intData[5]), "-1 == 251662080"); 
		assertEqual("false", String.valueOf(intData[1] >= intData[5]), "-1 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[1] <= intData[5]), "-1 <= 251662080"); 
		assertEqual("17", String.valueOf((long)intData[2]), "i2l 17");
		assertEqual("17.0", String.valueOf((float)intData[2]), "i2f 17");
		assertEqual("17.0", String.valueOf((double)intData[2]), "i2d 17");
		assertEqual("17", String.valueOf((byte)intData[2]), "i2b 17");
		assertEqual("17", String.valueOf((short)intData[2]), "i2s 17");
		assertEqual("-17", String.valueOf(-intData[2]), "ineg 17");
		assertEqual("17", String.valueOf(intData[2] >> 0), "17 >> 0"); 
		assertEqual("17", String.valueOf(intData[2] >>> 0), "17 >>> 0"); 
		assertEqual("17", String.valueOf(intData[2] << 0), "17 << 0"); 
		assertEqual("8", String.valueOf(intData[2] >> 1), "17 >> 1"); 
		assertEqual("8", String.valueOf(intData[2] >>> 1), "17 >>> 1"); 
		assertEqual("34", String.valueOf(intData[2] << 1), "17 << 1"); 
		assertEqual("4", String.valueOf(intData[2] >> 2), "17 >> 2"); 
		assertEqual("4", String.valueOf(intData[2] >>> 2), "17 >>> 2"); 
		assertEqual("68", String.valueOf(intData[2] << 2), "17 << 2"); 
		assertEqual("2", String.valueOf(intData[2] >> 3), "17 >> 3"); 
		assertEqual("2", String.valueOf(intData[2] >>> 3), "17 >>> 3"); 
		assertEqual("136", String.valueOf(intData[2] << 3), "17 << 3"); 
		assertEqual("1", String.valueOf(intData[2] >> 4), "17 >> 4"); 
		assertEqual("1", String.valueOf(intData[2] >>> 4), "17 >>> 4"); 
		assertEqual("272", String.valueOf(intData[2] << 4), "17 << 4"); 
		assertEqual("0", String.valueOf(intData[2] >> 5), "17 >> 5"); 
		assertEqual("0", String.valueOf(intData[2] >>> 5), "17 >>> 5"); 
		assertEqual("544", String.valueOf(intData[2] << 5), "17 << 5"); 
		assertEqual("0", String.valueOf(intData[2] >> 6), "17 >> 6"); 
		assertEqual("0", String.valueOf(intData[2] >>> 6), "17 >>> 6"); 
		assertEqual("1088", String.valueOf(intData[2] << 6), "17 << 6"); 
		assertEqual("0", String.valueOf(intData[2] >> 7), "17 >> 7"); 
		assertEqual("0", String.valueOf(intData[2] >>> 7), "17 >>> 7"); 
		assertEqual("2176", String.valueOf(intData[2] << 7), "17 << 7"); 
		assertEqual("0", String.valueOf(intData[2] >> 8), "17 >> 8"); 
		assertEqual("0", String.valueOf(intData[2] >>> 8), "17 >>> 8"); 
		assertEqual("4352", String.valueOf(intData[2] << 8), "17 << 8"); 
		assertEqual("0", String.valueOf(intData[2] >> 9), "17 >> 9"); 
		assertEqual("0", String.valueOf(intData[2] >>> 9), "17 >>> 9"); 
		assertEqual("8704", String.valueOf(intData[2] << 9), "17 << 9"); 
		assertEqual("0", String.valueOf(intData[2] >> 10), "17 >> 10"); 
		assertEqual("0", String.valueOf(intData[2] >>> 10), "17 >>> 10"); 
		assertEqual("17408", String.valueOf(intData[2] << 10), "17 << 10"); 
		assertEqual("0", String.valueOf(intData[2] >> 11), "17 >> 11"); 
		assertEqual("0", String.valueOf(intData[2] >>> 11), "17 >>> 11"); 
		assertEqual("34816", String.valueOf(intData[2] << 11), "17 << 11"); 
		assertEqual("0", String.valueOf(intData[2] >> 12), "17 >> 12"); 
		assertEqual("0", String.valueOf(intData[2] >>> 12), "17 >>> 12"); 
		assertEqual("69632", String.valueOf(intData[2] << 12), "17 << 12"); 
		assertEqual("0", String.valueOf(intData[2] >> 13), "17 >> 13"); 
		assertEqual("0", String.valueOf(intData[2] >>> 13), "17 >>> 13"); 
		assertEqual("139264", String.valueOf(intData[2] << 13), "17 << 13"); 
		assertEqual("0", String.valueOf(intData[2] >> 14), "17 >> 14"); 
		assertEqual("0", String.valueOf(intData[2] >>> 14), "17 >>> 14"); 
		assertEqual("278528", String.valueOf(intData[2] << 14), "17 << 14"); 
		assertEqual("0", String.valueOf(intData[2] >> 15), "17 >> 15"); 
		assertEqual("0", String.valueOf(intData[2] >>> 15), "17 >>> 15"); 
		assertEqual("557056", String.valueOf(intData[2] << 15), "17 << 15"); 
		assertEqual("0", String.valueOf(intData[2] >> 16), "17 >> 16"); 
		assertEqual("0", String.valueOf(intData[2] >>> 16), "17 >>> 16"); 
		assertEqual("1114112", String.valueOf(intData[2] << 16), "17 << 16"); 
		assertEqual("0", String.valueOf(intData[2] >> 17), "17 >> 17"); 
		assertEqual("0", String.valueOf(intData[2] >>> 17), "17 >>> 17"); 
		assertEqual("2228224", String.valueOf(intData[2] << 17), "17 << 17"); 
		assertEqual("0", String.valueOf(intData[2] >> 18), "17 >> 18"); 
		assertEqual("0", String.valueOf(intData[2] >>> 18), "17 >>> 18"); 
		assertEqual("4456448", String.valueOf(intData[2] << 18), "17 << 18"); 
		assertEqual("0", String.valueOf(intData[2] >> 19), "17 >> 19"); 
		assertEqual("0", String.valueOf(intData[2] >>> 19), "17 >>> 19"); 
		assertEqual("8912896", String.valueOf(intData[2] << 19), "17 << 19"); 
		assertEqual("0", String.valueOf(intData[2] >> 20), "17 >> 20"); 
		assertEqual("0", String.valueOf(intData[2] >>> 20), "17 >>> 20"); 
		assertEqual("17825792", String.valueOf(intData[2] << 20), "17 << 20"); 
		assertEqual("0", String.valueOf(intData[2] >> 21), "17 >> 21"); 
		assertEqual("0", String.valueOf(intData[2] >>> 21), "17 >>> 21"); 
		assertEqual("35651584", String.valueOf(intData[2] << 21), "17 << 21"); 
		assertEqual("0", String.valueOf(intData[2] >> 22), "17 >> 22"); 
		assertEqual("0", String.valueOf(intData[2] >>> 22), "17 >>> 22"); 
		assertEqual("71303168", String.valueOf(intData[2] << 22), "17 << 22"); 
		assertEqual("0", String.valueOf(intData[2] >> 23), "17 >> 23"); 
		assertEqual("0", String.valueOf(intData[2] >>> 23), "17 >>> 23"); 
		assertEqual("142606336", String.valueOf(intData[2] << 23), "17 << 23"); 
		assertEqual("0", String.valueOf(intData[2] >> 24), "17 >> 24"); 
		assertEqual("0", String.valueOf(intData[2] >>> 24), "17 >>> 24"); 
		assertEqual("285212672", String.valueOf(intData[2] << 24), "17 << 24"); 
		assertEqual("0", String.valueOf(intData[2] >> 25), "17 >> 25"); 
		assertEqual("0", String.valueOf(intData[2] >>> 25), "17 >>> 25"); 
		assertEqual("570425344", String.valueOf(intData[2] << 25), "17 << 25"); 
		assertEqual("0", String.valueOf(intData[2] >> 26), "17 >> 26"); 
		assertEqual("0", String.valueOf(intData[2] >>> 26), "17 >>> 26"); 
		assertEqual("1140850688", String.valueOf(intData[2] << 26), "17 << 26"); 
		assertEqual("0", String.valueOf(intData[2] >> 27), "17 >> 27"); 
		assertEqual("0", String.valueOf(intData[2] >>> 27), "17 >>> 27"); 
		assertEqual("-2013265920", String.valueOf(intData[2] << 27), "17 << 27"); 
		assertEqual("0", String.valueOf(intData[2] >> 28), "17 >> 28"); 
		assertEqual("0", String.valueOf(intData[2] >>> 28), "17 >>> 28"); 
		assertEqual("268435456", String.valueOf(intData[2] << 28), "17 << 28"); 
		assertEqual("0", String.valueOf(intData[2] >> 29), "17 >> 29"); 
		assertEqual("0", String.valueOf(intData[2] >>> 29), "17 >>> 29"); 
		assertEqual("536870912", String.valueOf(intData[2] << 29), "17 << 29"); 
		assertEqual("0", String.valueOf(intData[2] >> 30), "17 >> 30"); 
		assertEqual("0", String.valueOf(intData[2] >>> 30), "17 >>> 30"); 
		assertEqual("1073741824", String.valueOf(intData[2] << 30), "17 << 30"); 
		assertEqual("0", String.valueOf(intData[2] >> 31), "17 >> 31"); 
		assertEqual("0", String.valueOf(intData[2] >>> 31), "17 >>> 31"); 
		assertEqual("-2147483648", String.valueOf(intData[2] << 31), "17 << 31"); 
		assertEqual("18", String.valueOf(intData[2] + intData[0]), "17 + 1"); 
		assertEqual("16", String.valueOf(intData[2] - intData[0]), "17 - 1"); 
		assertEqual("17", String.valueOf(intData[2] * intData[0]), "17 * 1"); 
		assertEqual("17", String.valueOf(intData[2] / intData[0]), "17 / 1"); 
		assertEqual("0", String.valueOf(intData[2] % intData[0]), "17 % 1"); 
		assertEqual("1", String.valueOf(intData[2] & intData[0]), "17 & 1"); 
		assertEqual("17", String.valueOf(intData[2] | intData[0]), "17 | 1"); 
		assertEqual("16", String.valueOf(intData[2] ^ intData[0]), "17 ^ 1"); 
		assertEqual("true", String.valueOf(intData[2] > intData[0]), "17 > 1"); 
		assertEqual("false", String.valueOf(intData[2] < intData[0]), "17 < 1"); 
		assertEqual("false", String.valueOf(intData[2] == intData[0]), "17 == 1"); 
		assertEqual("true", String.valueOf(intData[2] >= intData[0]), "17 >= 1"); 
		assertEqual("false", String.valueOf(intData[2] <= intData[0]), "17 <= 1"); 
		assertEqual("16", String.valueOf(intData[2] + intData[1]), "17 + -1"); 
		assertEqual("18", String.valueOf(intData[2] - intData[1]), "17 - -1"); 
		assertEqual("-17", String.valueOf(intData[2] * intData[1]), "17 * -1"); 
		assertEqual("-17", String.valueOf(intData[2] / intData[1]), "17 / -1"); 
		assertEqual("0", String.valueOf(intData[2] % intData[1]), "17 % -1"); 
		assertEqual("17", String.valueOf(intData[2] & intData[1]), "17 & -1"); 
		assertEqual("-1", String.valueOf(intData[2] | intData[1]), "17 | -1"); 
		assertEqual("-18", String.valueOf(intData[2] ^ intData[1]), "17 ^ -1"); 
		assertEqual("true", String.valueOf(intData[2] > intData[1]), "17 > -1"); 
		assertEqual("false", String.valueOf(intData[2] < intData[1]), "17 < -1"); 
		assertEqual("false", String.valueOf(intData[2] == intData[1]), "17 == -1"); 
		assertEqual("true", String.valueOf(intData[2] >= intData[1]), "17 >= -1"); 
		assertEqual("false", String.valueOf(intData[2] <= intData[1]), "17 <= -1"); 
		assertEqual("34", String.valueOf(intData[2] + intData[2]), "17 + 17"); 
		assertEqual("0", String.valueOf(intData[2] - intData[2]), "17 - 17"); 
		assertEqual("289", String.valueOf(intData[2] * intData[2]), "17 * 17"); 
		assertEqual("1", String.valueOf(intData[2] / intData[2]), "17 / 17"); 
		assertEqual("0", String.valueOf(intData[2] % intData[2]), "17 % 17"); 
		assertEqual("17", String.valueOf(intData[2] & intData[2]), "17 & 17"); 
		assertEqual("17", String.valueOf(intData[2] | intData[2]), "17 | 17"); 
		assertEqual("0", String.valueOf(intData[2] ^ intData[2]), "17 ^ 17"); 
		assertEqual("false", String.valueOf(intData[2] > intData[2]), "17 > 17"); 
		assertEqual("false", String.valueOf(intData[2] < intData[2]), "17 < 17"); 
		assertEqual("true", String.valueOf(intData[2] == intData[2]), "17 == 17"); 
		assertEqual("true", String.valueOf(intData[2] >= intData[2]), "17 >= 17"); 
		assertEqual("true", String.valueOf(intData[2] <= intData[2]), "17 <= 17"); 
		assertEqual("0", String.valueOf(intData[2] + intData[3]), "17 + -17"); 
		assertEqual("34", String.valueOf(intData[2] - intData[3]), "17 - -17"); 
		assertEqual("-289", String.valueOf(intData[2] * intData[3]), "17 * -17"); 
		assertEqual("-1", String.valueOf(intData[2] / intData[3]), "17 / -17"); 
		assertEqual("0", String.valueOf(intData[2] % intData[3]), "17 % -17"); 
		assertEqual("1", String.valueOf(intData[2] & intData[3]), "17 & -17"); 
		assertEqual("-1", String.valueOf(intData[2] | intData[3]), "17 | -17"); 
		assertEqual("-2", String.valueOf(intData[2] ^ intData[3]), "17 ^ -17"); 
		assertEqual("true", String.valueOf(intData[2] > intData[3]), "17 > -17"); 
		assertEqual("false", String.valueOf(intData[2] < intData[3]), "17 < -17"); 
		assertEqual("false", String.valueOf(intData[2] == intData[3]), "17 == -17"); 
		assertEqual("true", String.valueOf(intData[2] >= intData[3]), "17 >= -17"); 
		assertEqual("false", String.valueOf(intData[2] <= intData[3]), "17 <= -17"); 
		assertEqual("-268373999", String.valueOf(intData[2] + intData[4]), "17 + -268374016"); 
		assertEqual("268374033", String.valueOf(intData[2] - intData[4]), "17 - -268374016"); 
		assertEqual("-267390976", String.valueOf(intData[2] * intData[4]), "17 * -268374016"); 
		assertEqual("0", String.valueOf(intData[2] / intData[4]), "17 / -268374016"); 
		assertEqual("17", String.valueOf(intData[2] % intData[4]), "17 % -268374016"); 
		assertEqual("0", String.valueOf(intData[2] & intData[4]), "17 & -268374016"); 
		assertEqual("-268373999", String.valueOf(intData[2] | intData[4]), "17 | -268374016"); 
		assertEqual("-268373999", String.valueOf(intData[2] ^ intData[4]), "17 ^ -268374016"); 
		assertEqual("true", String.valueOf(intData[2] > intData[4]), "17 > -268374016"); 
		assertEqual("false", String.valueOf(intData[2] < intData[4]), "17 < -268374016"); 
		assertEqual("false", String.valueOf(intData[2] == intData[4]), "17 == -268374016"); 
		assertEqual("true", String.valueOf(intData[2] >= intData[4]), "17 >= -268374016"); 
		assertEqual("false", String.valueOf(intData[2] <= intData[4]), "17 <= -268374016"); 
		assertEqual("251662097", String.valueOf(intData[2] + intData[5]), "17 + 251662080"); 
		assertEqual("-251662063", String.valueOf(intData[2] - intData[5]), "17 - 251662080"); 
		assertEqual("-16711936", String.valueOf(intData[2] * intData[5]), "17 * 251662080"); 
		assertEqual("0", String.valueOf(intData[2] / intData[5]), "17 / 251662080"); 
		assertEqual("17", String.valueOf(intData[2] % intData[5]), "17 % 251662080"); 
		assertEqual("0", String.valueOf(intData[2] & intData[5]), "17 & 251662080"); 
		assertEqual("251662097", String.valueOf(intData[2] | intData[5]), "17 | 251662080"); 
		assertEqual("251662097", String.valueOf(intData[2] ^ intData[5]), "17 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[2] > intData[5]), "17 > 251662080"); 
		assertEqual("true", String.valueOf(intData[2] < intData[5]), "17 < 251662080"); 
		assertEqual("false", String.valueOf(intData[2] == intData[5]), "17 == 251662080"); 
		assertEqual("false", String.valueOf(intData[2] >= intData[5]), "17 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[2] <= intData[5]), "17 <= 251662080"); 
		assertEqual("-17", String.valueOf((long)intData[3]), "i2l -17");
		assertEqual("-17.0", String.valueOf((float)intData[3]), "i2f -17");
		assertEqual("-17.0", String.valueOf((double)intData[3]), "i2d -17");
		assertEqual("-17", String.valueOf((byte)intData[3]), "i2b -17");
		assertEqual("-17", String.valueOf((short)intData[3]), "i2s -17");
		assertEqual("17", String.valueOf(-intData[3]), "ineg -17");
		assertEqual("-17", String.valueOf(intData[3] >> 0), "-17 >> 0"); 
		assertEqual("-17", String.valueOf(intData[3] >>> 0), "-17 >>> 0"); 
		assertEqual("-17", String.valueOf(intData[3] << 0), "-17 << 0"); 
		assertEqual("-9", String.valueOf(intData[3] >> 1), "-17 >> 1"); 
		assertEqual("2147483639", String.valueOf(intData[3] >>> 1), "-17 >>> 1"); 
		assertEqual("-34", String.valueOf(intData[3] << 1), "-17 << 1"); 
		assertEqual("-5", String.valueOf(intData[3] >> 2), "-17 >> 2"); 
		assertEqual("1073741819", String.valueOf(intData[3] >>> 2), "-17 >>> 2"); 
		assertEqual("-68", String.valueOf(intData[3] << 2), "-17 << 2"); 
		assertEqual("-3", String.valueOf(intData[3] >> 3), "-17 >> 3"); 
		assertEqual("536870909", String.valueOf(intData[3] >>> 3), "-17 >>> 3"); 
		assertEqual("-136", String.valueOf(intData[3] << 3), "-17 << 3"); 
		assertEqual("-2", String.valueOf(intData[3] >> 4), "-17 >> 4"); 
		assertEqual("268435454", String.valueOf(intData[3] >>> 4), "-17 >>> 4"); 
		assertEqual("-272", String.valueOf(intData[3] << 4), "-17 << 4"); 
		assertEqual("-1", String.valueOf(intData[3] >> 5), "-17 >> 5"); 
		assertEqual("134217727", String.valueOf(intData[3] >>> 5), "-17 >>> 5"); 
		assertEqual("-544", String.valueOf(intData[3] << 5), "-17 << 5"); 
		assertEqual("-1", String.valueOf(intData[3] >> 6), "-17 >> 6"); 
		assertEqual("67108863", String.valueOf(intData[3] >>> 6), "-17 >>> 6"); 
		assertEqual("-1088", String.valueOf(intData[3] << 6), "-17 << 6"); 
		assertEqual("-1", String.valueOf(intData[3] >> 7), "-17 >> 7"); 
		assertEqual("33554431", String.valueOf(intData[3] >>> 7), "-17 >>> 7"); 
		assertEqual("-2176", String.valueOf(intData[3] << 7), "-17 << 7"); 
		assertEqual("-1", String.valueOf(intData[3] >> 8), "-17 >> 8"); 
		assertEqual("16777215", String.valueOf(intData[3] >>> 8), "-17 >>> 8"); 
		assertEqual("-4352", String.valueOf(intData[3] << 8), "-17 << 8"); 
		assertEqual("-1", String.valueOf(intData[3] >> 9), "-17 >> 9"); 
		assertEqual("8388607", String.valueOf(intData[3] >>> 9), "-17 >>> 9"); 
		assertEqual("-8704", String.valueOf(intData[3] << 9), "-17 << 9"); 
		assertEqual("-1", String.valueOf(intData[3] >> 10), "-17 >> 10"); 
		assertEqual("4194303", String.valueOf(intData[3] >>> 10), "-17 >>> 10"); 
		assertEqual("-17408", String.valueOf(intData[3] << 10), "-17 << 10"); 
		assertEqual("-1", String.valueOf(intData[3] >> 11), "-17 >> 11"); 
		assertEqual("2097151", String.valueOf(intData[3] >>> 11), "-17 >>> 11"); 
		assertEqual("-34816", String.valueOf(intData[3] << 11), "-17 << 11"); 
		assertEqual("-1", String.valueOf(intData[3] >> 12), "-17 >> 12"); 
		assertEqual("1048575", String.valueOf(intData[3] >>> 12), "-17 >>> 12"); 
		assertEqual("-69632", String.valueOf(intData[3] << 12), "-17 << 12"); 
		assertEqual("-1", String.valueOf(intData[3] >> 13), "-17 >> 13"); 
		assertEqual("524287", String.valueOf(intData[3] >>> 13), "-17 >>> 13"); 
		assertEqual("-139264", String.valueOf(intData[3] << 13), "-17 << 13"); 
		assertEqual("-1", String.valueOf(intData[3] >> 14), "-17 >> 14"); 
		assertEqual("262143", String.valueOf(intData[3] >>> 14), "-17 >>> 14"); 
		assertEqual("-278528", String.valueOf(intData[3] << 14), "-17 << 14"); 
		assertEqual("-1", String.valueOf(intData[3] >> 15), "-17 >> 15"); 
		assertEqual("131071", String.valueOf(intData[3] >>> 15), "-17 >>> 15"); 
		assertEqual("-557056", String.valueOf(intData[3] << 15), "-17 << 15"); 
		assertEqual("-1", String.valueOf(intData[3] >> 16), "-17 >> 16"); 
		assertEqual("65535", String.valueOf(intData[3] >>> 16), "-17 >>> 16"); 
		assertEqual("-1114112", String.valueOf(intData[3] << 16), "-17 << 16"); 
		assertEqual("-1", String.valueOf(intData[3] >> 17), "-17 >> 17"); 
		assertEqual("32767", String.valueOf(intData[3] >>> 17), "-17 >>> 17"); 
		assertEqual("-2228224", String.valueOf(intData[3] << 17), "-17 << 17"); 
		assertEqual("-1", String.valueOf(intData[3] >> 18), "-17 >> 18"); 
		assertEqual("16383", String.valueOf(intData[3] >>> 18), "-17 >>> 18"); 
		assertEqual("-4456448", String.valueOf(intData[3] << 18), "-17 << 18"); 
		assertEqual("-1", String.valueOf(intData[3] >> 19), "-17 >> 19"); 
		assertEqual("8191", String.valueOf(intData[3] >>> 19), "-17 >>> 19"); 
		assertEqual("-8912896", String.valueOf(intData[3] << 19), "-17 << 19"); 
		assertEqual("-1", String.valueOf(intData[3] >> 20), "-17 >> 20"); 
		assertEqual("4095", String.valueOf(intData[3] >>> 20), "-17 >>> 20"); 
		assertEqual("-17825792", String.valueOf(intData[3] << 20), "-17 << 20"); 
		assertEqual("-1", String.valueOf(intData[3] >> 21), "-17 >> 21"); 
		assertEqual("2047", String.valueOf(intData[3] >>> 21), "-17 >>> 21"); 
		assertEqual("-35651584", String.valueOf(intData[3] << 21), "-17 << 21"); 
		assertEqual("-1", String.valueOf(intData[3] >> 22), "-17 >> 22"); 
		assertEqual("1023", String.valueOf(intData[3] >>> 22), "-17 >>> 22"); 
		assertEqual("-71303168", String.valueOf(intData[3] << 22), "-17 << 22"); 
		assertEqual("-1", String.valueOf(intData[3] >> 23), "-17 >> 23"); 
		assertEqual("511", String.valueOf(intData[3] >>> 23), "-17 >>> 23"); 
		assertEqual("-142606336", String.valueOf(intData[3] << 23), "-17 << 23"); 
		assertEqual("-1", String.valueOf(intData[3] >> 24), "-17 >> 24"); 
		assertEqual("255", String.valueOf(intData[3] >>> 24), "-17 >>> 24"); 
		assertEqual("-285212672", String.valueOf(intData[3] << 24), "-17 << 24"); 
		assertEqual("-1", String.valueOf(intData[3] >> 25), "-17 >> 25"); 
		assertEqual("127", String.valueOf(intData[3] >>> 25), "-17 >>> 25"); 
		assertEqual("-570425344", String.valueOf(intData[3] << 25), "-17 << 25"); 
		assertEqual("-1", String.valueOf(intData[3] >> 26), "-17 >> 26"); 
		assertEqual("63", String.valueOf(intData[3] >>> 26), "-17 >>> 26"); 
		assertEqual("-1140850688", String.valueOf(intData[3] << 26), "-17 << 26"); 
		assertEqual("-1", String.valueOf(intData[3] >> 27), "-17 >> 27"); 
		assertEqual("31", String.valueOf(intData[3] >>> 27), "-17 >>> 27"); 
		assertEqual("2013265920", String.valueOf(intData[3] << 27), "-17 << 27"); 
		assertEqual("-1", String.valueOf(intData[3] >> 28), "-17 >> 28"); 
		assertEqual("15", String.valueOf(intData[3] >>> 28), "-17 >>> 28"); 
		assertEqual("-268435456", String.valueOf(intData[3] << 28), "-17 << 28"); 
		assertEqual("-1", String.valueOf(intData[3] >> 29), "-17 >> 29"); 
		assertEqual("7", String.valueOf(intData[3] >>> 29), "-17 >>> 29"); 
		assertEqual("-536870912", String.valueOf(intData[3] << 29), "-17 << 29"); 
		assertEqual("-1", String.valueOf(intData[3] >> 30), "-17 >> 30"); 
		assertEqual("3", String.valueOf(intData[3] >>> 30), "-17 >>> 30"); 
		assertEqual("-1073741824", String.valueOf(intData[3] << 30), "-17 << 30"); 
		assertEqual("-1", String.valueOf(intData[3] >> 31), "-17 >> 31"); 
		assertEqual("1", String.valueOf(intData[3] >>> 31), "-17 >>> 31"); 
		assertEqual("-2147483648", String.valueOf(intData[3] << 31), "-17 << 31"); 
		assertEqual("-16", String.valueOf(intData[3] + intData[0]), "-17 + 1"); 
		assertEqual("-18", String.valueOf(intData[3] - intData[0]), "-17 - 1"); 
		assertEqual("-17", String.valueOf(intData[3] * intData[0]), "-17 * 1"); 
		assertEqual("-17", String.valueOf(intData[3] / intData[0]), "-17 / 1"); 
		assertEqual("0", String.valueOf(intData[3] % intData[0]), "-17 % 1"); 
		assertEqual("1", String.valueOf(intData[3] & intData[0]), "-17 & 1"); 
		assertEqual("-17", String.valueOf(intData[3] | intData[0]), "-17 | 1"); 
		assertEqual("-18", String.valueOf(intData[3] ^ intData[0]), "-17 ^ 1"); 
		assertEqual("false", String.valueOf(intData[3] > intData[0]), "-17 > 1"); 
		assertEqual("true", String.valueOf(intData[3] < intData[0]), "-17 < 1"); 
		assertEqual("false", String.valueOf(intData[3] == intData[0]), "-17 == 1"); 
		assertEqual("false", String.valueOf(intData[3] >= intData[0]), "-17 >= 1"); 
		assertEqual("true", String.valueOf(intData[3] <= intData[0]), "-17 <= 1"); 
		assertEqual("-18", String.valueOf(intData[3] + intData[1]), "-17 + -1"); 
		assertEqual("-16", String.valueOf(intData[3] - intData[1]), "-17 - -1"); 
		assertEqual("17", String.valueOf(intData[3] * intData[1]), "-17 * -1"); 
		assertEqual("17", String.valueOf(intData[3] / intData[1]), "-17 / -1"); 
		assertEqual("0", String.valueOf(intData[3] % intData[1]), "-17 % -1"); 
		assertEqual("-17", String.valueOf(intData[3] & intData[1]), "-17 & -1"); 
		assertEqual("-1", String.valueOf(intData[3] | intData[1]), "-17 | -1"); 
		assertEqual("16", String.valueOf(intData[3] ^ intData[1]), "-17 ^ -1"); 
		assertEqual("false", String.valueOf(intData[3] > intData[1]), "-17 > -1"); 
		assertEqual("true", String.valueOf(intData[3] < intData[1]), "-17 < -1"); 
		assertEqual("false", String.valueOf(intData[3] == intData[1]), "-17 == -1"); 
		assertEqual("false", String.valueOf(intData[3] >= intData[1]), "-17 >= -1"); 
		assertEqual("true", String.valueOf(intData[3] <= intData[1]), "-17 <= -1"); 
		assertEqual("0", String.valueOf(intData[3] + intData[2]), "-17 + 17"); 
		assertEqual("-34", String.valueOf(intData[3] - intData[2]), "-17 - 17"); 
		assertEqual("-289", String.valueOf(intData[3] * intData[2]), "-17 * 17"); 
		assertEqual("-1", String.valueOf(intData[3] / intData[2]), "-17 / 17"); 
		assertEqual("0", String.valueOf(intData[3] % intData[2]), "-17 % 17"); 
		assertEqual("1", String.valueOf(intData[3] & intData[2]), "-17 & 17"); 
		assertEqual("-1", String.valueOf(intData[3] | intData[2]), "-17 | 17"); 
		assertEqual("-2", String.valueOf(intData[3] ^ intData[2]), "-17 ^ 17"); 
		assertEqual("false", String.valueOf(intData[3] > intData[2]), "-17 > 17"); 
		assertEqual("true", String.valueOf(intData[3] < intData[2]), "-17 < 17"); 
		assertEqual("false", String.valueOf(intData[3] == intData[2]), "-17 == 17"); 
		assertEqual("false", String.valueOf(intData[3] >= intData[2]), "-17 >= 17"); 
		assertEqual("true", String.valueOf(intData[3] <= intData[2]), "-17 <= 17"); 
		assertEqual("-34", String.valueOf(intData[3] + intData[3]), "-17 + -17"); 
		assertEqual("0", String.valueOf(intData[3] - intData[3]), "-17 - -17"); 
		assertEqual("289", String.valueOf(intData[3] * intData[3]), "-17 * -17"); 
		assertEqual("1", String.valueOf(intData[3] / intData[3]), "-17 / -17"); 
		assertEqual("0", String.valueOf(intData[3] % intData[3]), "-17 % -17"); 
		assertEqual("-17", String.valueOf(intData[3] & intData[3]), "-17 & -17"); 
		assertEqual("-17", String.valueOf(intData[3] | intData[3]), "-17 | -17"); 
		assertEqual("0", String.valueOf(intData[3] ^ intData[3]), "-17 ^ -17"); 
		assertEqual("false", String.valueOf(intData[3] > intData[3]), "-17 > -17"); 
		assertEqual("false", String.valueOf(intData[3] < intData[3]), "-17 < -17"); 
		assertEqual("true", String.valueOf(intData[3] == intData[3]), "-17 == -17"); 
		assertEqual("true", String.valueOf(intData[3] >= intData[3]), "-17 >= -17"); 
		assertEqual("true", String.valueOf(intData[3] <= intData[3]), "-17 <= -17"); 
		assertEqual("-268374033", String.valueOf(intData[3] + intData[4]), "-17 + -268374016"); 
		assertEqual("268373999", String.valueOf(intData[3] - intData[4]), "-17 - -268374016"); 
		assertEqual("267390976", String.valueOf(intData[3] * intData[4]), "-17 * -268374016"); 
		assertEqual("0", String.valueOf(intData[3] / intData[4]), "-17 / -268374016"); 
		assertEqual("-17", String.valueOf(intData[3] % intData[4]), "-17 % -268374016"); 
		assertEqual("-268374016", String.valueOf(intData[3] & intData[4]), "-17 & -268374016"); 
		assertEqual("-17", String.valueOf(intData[3] | intData[4]), "-17 | -268374016"); 
		assertEqual("268373999", String.valueOf(intData[3] ^ intData[4]), "-17 ^ -268374016"); 
		assertEqual("true", String.valueOf(intData[3] > intData[4]), "-17 > -268374016"); 
		assertEqual("false", String.valueOf(intData[3] < intData[4]), "-17 < -268374016"); 
		assertEqual("false", String.valueOf(intData[3] == intData[4]), "-17 == -268374016"); 
		assertEqual("true", String.valueOf(intData[3] >= intData[4]), "-17 >= -268374016"); 
		assertEqual("false", String.valueOf(intData[3] <= intData[4]), "-17 <= -268374016"); 
		assertEqual("251662063", String.valueOf(intData[3] + intData[5]), "-17 + 251662080"); 
		assertEqual("-251662097", String.valueOf(intData[3] - intData[5]), "-17 - 251662080"); 
		assertEqual("16711936", String.valueOf(intData[3] * intData[5]), "-17 * 251662080"); 
		assertEqual("0", String.valueOf(intData[3] / intData[5]), "-17 / 251662080"); 
		assertEqual("-17", String.valueOf(intData[3] % intData[5]), "-17 % 251662080"); 
		assertEqual("251662080", String.valueOf(intData[3] & intData[5]), "-17 & 251662080"); 
		assertEqual("-17", String.valueOf(intData[3] | intData[5]), "-17 | 251662080"); 
		assertEqual("-251662097", String.valueOf(intData[3] ^ intData[5]), "-17 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[3] > intData[5]), "-17 > 251662080"); 
		assertEqual("true", String.valueOf(intData[3] < intData[5]), "-17 < 251662080"); 
		assertEqual("false", String.valueOf(intData[3] == intData[5]), "-17 == 251662080"); 
		assertEqual("false", String.valueOf(intData[3] >= intData[5]), "-17 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[3] <= intData[5]), "-17 <= 251662080"); 
		assertEqual("-268374016", String.valueOf((long)intData[4]), "i2l -268374016");
		assertEqual("-2.6837402E8", String.valueOf((float)intData[4]), "i2f -268374016");
		assertEqual("-2.68374016E8", String.valueOf((double)intData[4]), "i2d -268374016");
		assertEqual("0", String.valueOf((byte)intData[4]), "i2b -268374016");
		assertEqual("-4096", String.valueOf((short)intData[4]), "i2s -268374016");
		assertEqual("268374016", String.valueOf(-intData[4]), "ineg -268374016");
		assertEqual("-268374016", String.valueOf(intData[4] >> 0), "-268374016 >> 0"); 
		assertEqual("-268374016", String.valueOf(intData[4] >>> 0), "-268374016 >>> 0"); 
		assertEqual("-268374016", String.valueOf(intData[4] << 0), "-268374016 << 0"); 
		assertEqual("-134187008", String.valueOf(intData[4] >> 1), "-268374016 >> 1"); 
		assertEqual("2013296640", String.valueOf(intData[4] >>> 1), "-268374016 >>> 1"); 
		assertEqual("-536748032", String.valueOf(intData[4] << 1), "-268374016 << 1"); 
		assertEqual("-67093504", String.valueOf(intData[4] >> 2), "-268374016 >> 2"); 
		assertEqual("1006648320", String.valueOf(intData[4] >>> 2), "-268374016 >>> 2"); 
		assertEqual("-1073496064", String.valueOf(intData[4] << 2), "-268374016 << 2"); 
		assertEqual("-33546752", String.valueOf(intData[4] >> 3), "-268374016 >> 3"); 
		assertEqual("503324160", String.valueOf(intData[4] >>> 3), "-268374016 >>> 3"); 
		assertEqual("-2146992128", String.valueOf(intData[4] << 3), "-268374016 << 3"); 
		assertEqual("-16773376", String.valueOf(intData[4] >> 4), "-268374016 >> 4"); 
		assertEqual("251662080", String.valueOf(intData[4] >>> 4), "-268374016 >>> 4"); 
		assertEqual("983040", String.valueOf(intData[4] << 4), "-268374016 << 4"); 
		assertEqual("-8386688", String.valueOf(intData[4] >> 5), "-268374016 >> 5"); 
		assertEqual("125831040", String.valueOf(intData[4] >>> 5), "-268374016 >>> 5"); 
		assertEqual("1966080", String.valueOf(intData[4] << 5), "-268374016 << 5"); 
		assertEqual("-4193344", String.valueOf(intData[4] >> 6), "-268374016 >> 6"); 
		assertEqual("62915520", String.valueOf(intData[4] >>> 6), "-268374016 >>> 6"); 
		assertEqual("3932160", String.valueOf(intData[4] << 6), "-268374016 << 6"); 
		assertEqual("-2096672", String.valueOf(intData[4] >> 7), "-268374016 >> 7"); 
		assertEqual("31457760", String.valueOf(intData[4] >>> 7), "-268374016 >>> 7"); 
		assertEqual("7864320", String.valueOf(intData[4] << 7), "-268374016 << 7"); 
		assertEqual("-1048336", String.valueOf(intData[4] >> 8), "-268374016 >> 8"); 
		assertEqual("15728880", String.valueOf(intData[4] >>> 8), "-268374016 >>> 8"); 
		assertEqual("15728640", String.valueOf(intData[4] << 8), "-268374016 << 8"); 
		assertEqual("-524168", String.valueOf(intData[4] >> 9), "-268374016 >> 9"); 
		assertEqual("7864440", String.valueOf(intData[4] >>> 9), "-268374016 >>> 9"); 
		assertEqual("31457280", String.valueOf(intData[4] << 9), "-268374016 << 9"); 
		assertEqual("-262084", String.valueOf(intData[4] >> 10), "-268374016 >> 10"); 
		assertEqual("3932220", String.valueOf(intData[4] >>> 10), "-268374016 >>> 10"); 
		assertEqual("62914560", String.valueOf(intData[4] << 10), "-268374016 << 10"); 
		assertEqual("-131042", String.valueOf(intData[4] >> 11), "-268374016 >> 11"); 
		assertEqual("1966110", String.valueOf(intData[4] >>> 11), "-268374016 >>> 11"); 
		assertEqual("125829120", String.valueOf(intData[4] << 11), "-268374016 << 11"); 
		assertEqual("-65521", String.valueOf(intData[4] >> 12), "-268374016 >> 12"); 
		assertEqual("983055", String.valueOf(intData[4] >>> 12), "-268374016 >>> 12"); 
		assertEqual("251658240", String.valueOf(intData[4] << 12), "-268374016 << 12"); 
		assertEqual("-32761", String.valueOf(intData[4] >> 13), "-268374016 >> 13"); 
		assertEqual("491527", String.valueOf(intData[4] >>> 13), "-268374016 >>> 13"); 
		assertEqual("503316480", String.valueOf(intData[4] << 13), "-268374016 << 13"); 
		assertEqual("-16381", String.valueOf(intData[4] >> 14), "-268374016 >> 14"); 
		assertEqual("245763", String.valueOf(intData[4] >>> 14), "-268374016 >>> 14"); 
		assertEqual("1006632960", String.valueOf(intData[4] << 14), "-268374016 << 14"); 
		assertEqual("-8191", String.valueOf(intData[4] >> 15), "-268374016 >> 15"); 
		assertEqual("122881", String.valueOf(intData[4] >>> 15), "-268374016 >>> 15"); 
		assertEqual("2013265920", String.valueOf(intData[4] << 15), "-268374016 << 15"); 
		assertEqual("-4096", String.valueOf(intData[4] >> 16), "-268374016 >> 16"); 
		assertEqual("61440", String.valueOf(intData[4] >>> 16), "-268374016 >>> 16"); 
		assertEqual("-268435456", String.valueOf(intData[4] << 16), "-268374016 << 16"); 
		assertEqual("-2048", String.valueOf(intData[4] >> 17), "-268374016 >> 17"); 
		assertEqual("30720", String.valueOf(intData[4] >>> 17), "-268374016 >>> 17"); 
		assertEqual("-536870912", String.valueOf(intData[4] << 17), "-268374016 << 17"); 
		assertEqual("-1024", String.valueOf(intData[4] >> 18), "-268374016 >> 18"); 
		assertEqual("15360", String.valueOf(intData[4] >>> 18), "-268374016 >>> 18"); 
		assertEqual("-1073741824", String.valueOf(intData[4] << 18), "-268374016 << 18"); 
		assertEqual("-512", String.valueOf(intData[4] >> 19), "-268374016 >> 19"); 
		assertEqual("7680", String.valueOf(intData[4] >>> 19), "-268374016 >>> 19"); 
		assertEqual("-2147483648", String.valueOf(intData[4] << 19), "-268374016 << 19"); 
		assertEqual("-256", String.valueOf(intData[4] >> 20), "-268374016 >> 20"); 
		assertEqual("3840", String.valueOf(intData[4] >>> 20), "-268374016 >>> 20"); 
		assertEqual("0", String.valueOf(intData[4] << 20), "-268374016 << 20"); 
		assertEqual("-128", String.valueOf(intData[4] >> 21), "-268374016 >> 21"); 
		assertEqual("1920", String.valueOf(intData[4] >>> 21), "-268374016 >>> 21"); 
		assertEqual("0", String.valueOf(intData[4] << 21), "-268374016 << 21"); 
		assertEqual("-64", String.valueOf(intData[4] >> 22), "-268374016 >> 22"); 
		assertEqual("960", String.valueOf(intData[4] >>> 22), "-268374016 >>> 22"); 
		assertEqual("0", String.valueOf(intData[4] << 22), "-268374016 << 22"); 
		assertEqual("-32", String.valueOf(intData[4] >> 23), "-268374016 >> 23"); 
		assertEqual("480", String.valueOf(intData[4] >>> 23), "-268374016 >>> 23"); 
		assertEqual("0", String.valueOf(intData[4] << 23), "-268374016 << 23"); 
		assertEqual("-16", String.valueOf(intData[4] >> 24), "-268374016 >> 24"); 
		assertEqual("240", String.valueOf(intData[4] >>> 24), "-268374016 >>> 24"); 
		assertEqual("0", String.valueOf(intData[4] << 24), "-268374016 << 24"); 
		assertEqual("-8", String.valueOf(intData[4] >> 25), "-268374016 >> 25"); 
		assertEqual("120", String.valueOf(intData[4] >>> 25), "-268374016 >>> 25"); 
		assertEqual("0", String.valueOf(intData[4] << 25), "-268374016 << 25"); 
		assertEqual("-4", String.valueOf(intData[4] >> 26), "-268374016 >> 26"); 
		assertEqual("60", String.valueOf(intData[4] >>> 26), "-268374016 >>> 26"); 
		assertEqual("0", String.valueOf(intData[4] << 26), "-268374016 << 26"); 
		assertEqual("-2", String.valueOf(intData[4] >> 27), "-268374016 >> 27"); 
		assertEqual("30", String.valueOf(intData[4] >>> 27), "-268374016 >>> 27"); 
		assertEqual("0", String.valueOf(intData[4] << 27), "-268374016 << 27"); 
		assertEqual("-1", String.valueOf(intData[4] >> 28), "-268374016 >> 28"); 
		assertEqual("15", String.valueOf(intData[4] >>> 28), "-268374016 >>> 28"); 
		assertEqual("0", String.valueOf(intData[4] << 28), "-268374016 << 28"); 
		assertEqual("-1", String.valueOf(intData[4] >> 29), "-268374016 >> 29"); 
		assertEqual("7", String.valueOf(intData[4] >>> 29), "-268374016 >>> 29"); 
		assertEqual("0", String.valueOf(intData[4] << 29), "-268374016 << 29"); 
		assertEqual("-1", String.valueOf(intData[4] >> 30), "-268374016 >> 30"); 
		assertEqual("3", String.valueOf(intData[4] >>> 30), "-268374016 >>> 30"); 
		assertEqual("0", String.valueOf(intData[4] << 30), "-268374016 << 30"); 
		assertEqual("-1", String.valueOf(intData[4] >> 31), "-268374016 >> 31"); 
		assertEqual("1", String.valueOf(intData[4] >>> 31), "-268374016 >>> 31"); 
		assertEqual("0", String.valueOf(intData[4] << 31), "-268374016 << 31"); 
		assertEqual("-268374015", String.valueOf(intData[4] + intData[0]), "-268374016 + 1"); 
		assertEqual("-268374017", String.valueOf(intData[4] - intData[0]), "-268374016 - 1"); 
		assertEqual("-268374016", String.valueOf(intData[4] * intData[0]), "-268374016 * 1"); 
		assertEqual("-268374016", String.valueOf(intData[4] / intData[0]), "-268374016 / 1"); 
		assertEqual("0", String.valueOf(intData[4] % intData[0]), "-268374016 % 1"); 
		assertEqual("0", String.valueOf(intData[4] & intData[0]), "-268374016 & 1"); 
		assertEqual("-268374015", String.valueOf(intData[4] | intData[0]), "-268374016 | 1"); 
		assertEqual("-268374015", String.valueOf(intData[4] ^ intData[0]), "-268374016 ^ 1"); 
		assertEqual("false", String.valueOf(intData[4] > intData[0]), "-268374016 > 1"); 
		assertEqual("true", String.valueOf(intData[4] < intData[0]), "-268374016 < 1"); 
		assertEqual("false", String.valueOf(intData[4] == intData[0]), "-268374016 == 1"); 
		assertEqual("false", String.valueOf(intData[4] >= intData[0]), "-268374016 >= 1"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[0]), "-268374016 <= 1"); 
		assertEqual("-268374017", String.valueOf(intData[4] + intData[1]), "-268374016 + -1"); 
		assertEqual("-268374015", String.valueOf(intData[4] - intData[1]), "-268374016 - -1"); 
		assertEqual("268374016", String.valueOf(intData[4] * intData[1]), "-268374016 * -1"); 
		assertEqual("268374016", String.valueOf(intData[4] / intData[1]), "-268374016 / -1"); 
		assertEqual("0", String.valueOf(intData[4] % intData[1]), "-268374016 % -1"); 
		assertEqual("-268374016", String.valueOf(intData[4] & intData[1]), "-268374016 & -1"); 
		assertEqual("-1", String.valueOf(intData[4] | intData[1]), "-268374016 | -1"); 
		assertEqual("268374015", String.valueOf(intData[4] ^ intData[1]), "-268374016 ^ -1"); 
		assertEqual("false", String.valueOf(intData[4] > intData[1]), "-268374016 > -1"); 
		assertEqual("true", String.valueOf(intData[4] < intData[1]), "-268374016 < -1"); 
		assertEqual("false", String.valueOf(intData[4] == intData[1]), "-268374016 == -1"); 
		assertEqual("false", String.valueOf(intData[4] >= intData[1]), "-268374016 >= -1"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[1]), "-268374016 <= -1"); 
		assertEqual("-268373999", String.valueOf(intData[4] + intData[2]), "-268374016 + 17"); 
		assertEqual("-268374033", String.valueOf(intData[4] - intData[2]), "-268374016 - 17"); 
		assertEqual("-267390976", String.valueOf(intData[4] * intData[2]), "-268374016 * 17"); 
		assertEqual("-15786706", String.valueOf(intData[4] / intData[2]), "-268374016 / 17"); 
		assertEqual("-14", String.valueOf(intData[4] % intData[2]), "-268374016 % 17"); 
		assertEqual("0", String.valueOf(intData[4] & intData[2]), "-268374016 & 17"); 
		assertEqual("-268373999", String.valueOf(intData[4] | intData[2]), "-268374016 | 17"); 
		assertEqual("-268373999", String.valueOf(intData[4] ^ intData[2]), "-268374016 ^ 17"); 
		assertEqual("false", String.valueOf(intData[4] > intData[2]), "-268374016 > 17"); 
		assertEqual("true", String.valueOf(intData[4] < intData[2]), "-268374016 < 17"); 
		assertEqual("false", String.valueOf(intData[4] == intData[2]), "-268374016 == 17"); 
		assertEqual("false", String.valueOf(intData[4] >= intData[2]), "-268374016 >= 17"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[2]), "-268374016 <= 17"); 
		assertEqual("-268374033", String.valueOf(intData[4] + intData[3]), "-268374016 + -17"); 
		assertEqual("-268373999", String.valueOf(intData[4] - intData[3]), "-268374016 - -17"); 
		assertEqual("267390976", String.valueOf(intData[4] * intData[3]), "-268374016 * -17"); 
		assertEqual("15786706", String.valueOf(intData[4] / intData[3]), "-268374016 / -17"); 
		assertEqual("-14", String.valueOf(intData[4] % intData[3]), "-268374016 % -17"); 
		assertEqual("-268374016", String.valueOf(intData[4] & intData[3]), "-268374016 & -17"); 
		assertEqual("-17", String.valueOf(intData[4] | intData[3]), "-268374016 | -17"); 
		assertEqual("268373999", String.valueOf(intData[4] ^ intData[3]), "-268374016 ^ -17"); 
		assertEqual("false", String.valueOf(intData[4] > intData[3]), "-268374016 > -17"); 
		assertEqual("true", String.valueOf(intData[4] < intData[3]), "-268374016 < -17"); 
		assertEqual("false", String.valueOf(intData[4] == intData[3]), "-268374016 == -17"); 
		assertEqual("false", String.valueOf(intData[4] >= intData[3]), "-268374016 >= -17"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[3]), "-268374016 <= -17"); 
		assertEqual("-536748032", String.valueOf(intData[4] + intData[4]), "-268374016 + -268374016"); 
		assertEqual("0", String.valueOf(intData[4] - intData[4]), "-268374016 - -268374016"); 
		assertEqual("-520093696", String.valueOf(intData[4] * intData[4]), "-268374016 * -268374016"); 
		assertEqual("1", String.valueOf(intData[4] / intData[4]), "-268374016 / -268374016"); 
		assertEqual("0", String.valueOf(intData[4] % intData[4]), "-268374016 % -268374016"); 
		assertEqual("-268374016", String.valueOf(intData[4] & intData[4]), "-268374016 & -268374016"); 
		assertEqual("-268374016", String.valueOf(intData[4] | intData[4]), "-268374016 | -268374016"); 
		assertEqual("0", String.valueOf(intData[4] ^ intData[4]), "-268374016 ^ -268374016"); 
		assertEqual("false", String.valueOf(intData[4] > intData[4]), "-268374016 > -268374016"); 
		assertEqual("false", String.valueOf(intData[4] < intData[4]), "-268374016 < -268374016"); 
		assertEqual("true", String.valueOf(intData[4] == intData[4]), "-268374016 == -268374016"); 
		assertEqual("true", String.valueOf(intData[4] >= intData[4]), "-268374016 >= -268374016"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[4]), "-268374016 <= -268374016"); 
		assertEqual("-16711936", String.valueOf(intData[4] + intData[5]), "-268374016 + 251662080"); 
		assertEqual("-520036096", String.valueOf(intData[4] - intData[5]), "-268374016 - 251662080"); 
		assertEqual("235929600", String.valueOf(intData[4] * intData[5]), "-268374016 * 251662080"); 
		assertEqual("-1", String.valueOf(intData[4] / intData[5]), "-268374016 / 251662080"); 
		assertEqual("-16711936", String.valueOf(intData[4] % intData[5]), "-268374016 % 251662080"); 
		assertEqual("0", String.valueOf(intData[4] & intData[5]), "-268374016 & 251662080"); 
		assertEqual("-16711936", String.valueOf(intData[4] | intData[5]), "-268374016 | 251662080"); 
		assertEqual("-16711936", String.valueOf(intData[4] ^ intData[5]), "-268374016 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[4] > intData[5]), "-268374016 > 251662080"); 
		assertEqual("true", String.valueOf(intData[4] < intData[5]), "-268374016 < 251662080"); 
		assertEqual("false", String.valueOf(intData[4] == intData[5]), "-268374016 == 251662080"); 
		assertEqual("false", String.valueOf(intData[4] >= intData[5]), "-268374016 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[4] <= intData[5]), "-268374016 <= 251662080"); 
		assertEqual("251662080", String.valueOf((long)intData[5]), "i2l 251662080");
		assertEqual("2.5166208E8", String.valueOf((float)intData[5]), "i2f 251662080");
		assertEqual("2.5166208E8", String.valueOf((double)intData[5]), "i2d 251662080");
		assertEqual("0", String.valueOf((byte)intData[5]), "i2b 251662080");
		assertEqual("3840", String.valueOf((short)intData[5]), "i2s 251662080");
		assertEqual("-251662080", String.valueOf(-intData[5]), "ineg 251662080");
		assertEqual("251662080", String.valueOf(intData[5] >> 0), "251662080 >> 0"); 
		assertEqual("251662080", String.valueOf(intData[5] >>> 0), "251662080 >>> 0"); 
		assertEqual("251662080", String.valueOf(intData[5] << 0), "251662080 << 0"); 
		assertEqual("125831040", String.valueOf(intData[5] >> 1), "251662080 >> 1"); 
		assertEqual("125831040", String.valueOf(intData[5] >>> 1), "251662080 >>> 1"); 
		assertEqual("503324160", String.valueOf(intData[5] << 1), "251662080 << 1"); 
		assertEqual("62915520", String.valueOf(intData[5] >> 2), "251662080 >> 2"); 
		assertEqual("62915520", String.valueOf(intData[5] >>> 2), "251662080 >>> 2"); 
		assertEqual("1006648320", String.valueOf(intData[5] << 2), "251662080 << 2"); 
		assertEqual("31457760", String.valueOf(intData[5] >> 3), "251662080 >> 3"); 
		assertEqual("31457760", String.valueOf(intData[5] >>> 3), "251662080 >>> 3"); 
		assertEqual("2013296640", String.valueOf(intData[5] << 3), "251662080 << 3"); 
		assertEqual("15728880", String.valueOf(intData[5] >> 4), "251662080 >> 4"); 
		assertEqual("15728880", String.valueOf(intData[5] >>> 4), "251662080 >>> 4"); 
		assertEqual("-268374016", String.valueOf(intData[5] << 4), "251662080 << 4"); 
		assertEqual("7864440", String.valueOf(intData[5] >> 5), "251662080 >> 5"); 
		assertEqual("7864440", String.valueOf(intData[5] >>> 5), "251662080 >>> 5"); 
		assertEqual("-536748032", String.valueOf(intData[5] << 5), "251662080 << 5"); 
		assertEqual("3932220", String.valueOf(intData[5] >> 6), "251662080 >> 6"); 
		assertEqual("3932220", String.valueOf(intData[5] >>> 6), "251662080 >>> 6"); 
		assertEqual("-1073496064", String.valueOf(intData[5] << 6), "251662080 << 6"); 
		assertEqual("1966110", String.valueOf(intData[5] >> 7), "251662080 >> 7"); 
		assertEqual("1966110", String.valueOf(intData[5] >>> 7), "251662080 >>> 7"); 
		assertEqual("-2146992128", String.valueOf(intData[5] << 7), "251662080 << 7"); 
		assertEqual("983055", String.valueOf(intData[5] >> 8), "251662080 >> 8"); 
		assertEqual("983055", String.valueOf(intData[5] >>> 8), "251662080 >>> 8"); 
		assertEqual("983040", String.valueOf(intData[5] << 8), "251662080 << 8"); 
		assertEqual("491527", String.valueOf(intData[5] >> 9), "251662080 >> 9"); 
		assertEqual("491527", String.valueOf(intData[5] >>> 9), "251662080 >>> 9"); 
		assertEqual("1966080", String.valueOf(intData[5] << 9), "251662080 << 9"); 
		assertEqual("245763", String.valueOf(intData[5] >> 10), "251662080 >> 10"); 
		assertEqual("245763", String.valueOf(intData[5] >>> 10), "251662080 >>> 10"); 
		assertEqual("3932160", String.valueOf(intData[5] << 10), "251662080 << 10"); 
		assertEqual("122881", String.valueOf(intData[5] >> 11), "251662080 >> 11"); 
		assertEqual("122881", String.valueOf(intData[5] >>> 11), "251662080 >>> 11"); 
		assertEqual("7864320", String.valueOf(intData[5] << 11), "251662080 << 11"); 
		assertEqual("61440", String.valueOf(intData[5] >> 12), "251662080 >> 12"); 
		assertEqual("61440", String.valueOf(intData[5] >>> 12), "251662080 >>> 12"); 
		assertEqual("15728640", String.valueOf(intData[5] << 12), "251662080 << 12"); 
		assertEqual("30720", String.valueOf(intData[5] >> 13), "251662080 >> 13"); 
		assertEqual("30720", String.valueOf(intData[5] >>> 13), "251662080 >>> 13"); 
		assertEqual("31457280", String.valueOf(intData[5] << 13), "251662080 << 13"); 
		assertEqual("15360", String.valueOf(intData[5] >> 14), "251662080 >> 14"); 
		assertEqual("15360", String.valueOf(intData[5] >>> 14), "251662080 >>> 14"); 
		assertEqual("62914560", String.valueOf(intData[5] << 14), "251662080 << 14"); 
		assertEqual("7680", String.valueOf(intData[5] >> 15), "251662080 >> 15"); 
		assertEqual("7680", String.valueOf(intData[5] >>> 15), "251662080 >>> 15"); 
		assertEqual("125829120", String.valueOf(intData[5] << 15), "251662080 << 15"); 
		assertEqual("3840", String.valueOf(intData[5] >> 16), "251662080 >> 16"); 
		assertEqual("3840", String.valueOf(intData[5] >>> 16), "251662080 >>> 16"); 
		assertEqual("251658240", String.valueOf(intData[5] << 16), "251662080 << 16"); 
		assertEqual("1920", String.valueOf(intData[5] >> 17), "251662080 >> 17"); 
		assertEqual("1920", String.valueOf(intData[5] >>> 17), "251662080 >>> 17"); 
		assertEqual("503316480", String.valueOf(intData[5] << 17), "251662080 << 17"); 
		assertEqual("960", String.valueOf(intData[5] >> 18), "251662080 >> 18"); 
		assertEqual("960", String.valueOf(intData[5] >>> 18), "251662080 >>> 18"); 
		assertEqual("1006632960", String.valueOf(intData[5] << 18), "251662080 << 18"); 
		assertEqual("480", String.valueOf(intData[5] >> 19), "251662080 >> 19"); 
		assertEqual("480", String.valueOf(intData[5] >>> 19), "251662080 >>> 19"); 
		assertEqual("2013265920", String.valueOf(intData[5] << 19), "251662080 << 19"); 
		assertEqual("240", String.valueOf(intData[5] >> 20), "251662080 >> 20"); 
		assertEqual("240", String.valueOf(intData[5] >>> 20), "251662080 >>> 20"); 
		assertEqual("-268435456", String.valueOf(intData[5] << 20), "251662080 << 20"); 
		assertEqual("120", String.valueOf(intData[5] >> 21), "251662080 >> 21"); 
		assertEqual("120", String.valueOf(intData[5] >>> 21), "251662080 >>> 21"); 
		assertEqual("-536870912", String.valueOf(intData[5] << 21), "251662080 << 21"); 
		assertEqual("60", String.valueOf(intData[5] >> 22), "251662080 >> 22"); 
		assertEqual("60", String.valueOf(intData[5] >>> 22), "251662080 >>> 22"); 
		assertEqual("-1073741824", String.valueOf(intData[5] << 22), "251662080 << 22"); 
		assertEqual("30", String.valueOf(intData[5] >> 23), "251662080 >> 23"); 
		assertEqual("30", String.valueOf(intData[5] >>> 23), "251662080 >>> 23"); 
		assertEqual("-2147483648", String.valueOf(intData[5] << 23), "251662080 << 23"); 
		assertEqual("15", String.valueOf(intData[5] >> 24), "251662080 >> 24"); 
		assertEqual("15", String.valueOf(intData[5] >>> 24), "251662080 >>> 24"); 
		assertEqual("0", String.valueOf(intData[5] << 24), "251662080 << 24"); 
		assertEqual("7", String.valueOf(intData[5] >> 25), "251662080 >> 25"); 
		assertEqual("7", String.valueOf(intData[5] >>> 25), "251662080 >>> 25"); 
		assertEqual("0", String.valueOf(intData[5] << 25), "251662080 << 25"); 
		assertEqual("3", String.valueOf(intData[5] >> 26), "251662080 >> 26"); 
		assertEqual("3", String.valueOf(intData[5] >>> 26), "251662080 >>> 26"); 
		assertEqual("0", String.valueOf(intData[5] << 26), "251662080 << 26"); 
		assertEqual("1", String.valueOf(intData[5] >> 27), "251662080 >> 27"); 
		assertEqual("1", String.valueOf(intData[5] >>> 27), "251662080 >>> 27"); 
		assertEqual("0", String.valueOf(intData[5] << 27), "251662080 << 27"); 
		assertEqual("0", String.valueOf(intData[5] >> 28), "251662080 >> 28"); 
		assertEqual("0", String.valueOf(intData[5] >>> 28), "251662080 >>> 28"); 
		assertEqual("0", String.valueOf(intData[5] << 28), "251662080 << 28"); 
		assertEqual("0", String.valueOf(intData[5] >> 29), "251662080 >> 29"); 
		assertEqual("0", String.valueOf(intData[5] >>> 29), "251662080 >>> 29"); 
		assertEqual("0", String.valueOf(intData[5] << 29), "251662080 << 29"); 
		assertEqual("0", String.valueOf(intData[5] >> 30), "251662080 >> 30"); 
		assertEqual("0", String.valueOf(intData[5] >>> 30), "251662080 >>> 30"); 
		assertEqual("0", String.valueOf(intData[5] << 30), "251662080 << 30"); 
		assertEqual("0", String.valueOf(intData[5] >> 31), "251662080 >> 31"); 
		assertEqual("0", String.valueOf(intData[5] >>> 31), "251662080 >>> 31"); 
		assertEqual("0", String.valueOf(intData[5] << 31), "251662080 << 31"); 
		assertEqual("251662081", String.valueOf(intData[5] + intData[0]), "251662080 + 1"); 
		assertEqual("251662079", String.valueOf(intData[5] - intData[0]), "251662080 - 1"); 
		assertEqual("251662080", String.valueOf(intData[5] * intData[0]), "251662080 * 1"); 
		assertEqual("251662080", String.valueOf(intData[5] / intData[0]), "251662080 / 1"); 
		assertEqual("0", String.valueOf(intData[5] % intData[0]), "251662080 % 1"); 
		assertEqual("0", String.valueOf(intData[5] & intData[0]), "251662080 & 1"); 
		assertEqual("251662081", String.valueOf(intData[5] | intData[0]), "251662080 | 1"); 
		assertEqual("251662081", String.valueOf(intData[5] ^ intData[0]), "251662080 ^ 1"); 
		assertEqual("true", String.valueOf(intData[5] > intData[0]), "251662080 > 1"); 
		assertEqual("false", String.valueOf(intData[5] < intData[0]), "251662080 < 1"); 
		assertEqual("false", String.valueOf(intData[5] == intData[0]), "251662080 == 1"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[0]), "251662080 >= 1"); 
		assertEqual("false", String.valueOf(intData[5] <= intData[0]), "251662080 <= 1"); 
		assertEqual("251662079", String.valueOf(intData[5] + intData[1]), "251662080 + -1"); 
		assertEqual("251662081", String.valueOf(intData[5] - intData[1]), "251662080 - -1"); 
		assertEqual("-251662080", String.valueOf(intData[5] * intData[1]), "251662080 * -1"); 
		assertEqual("-251662080", String.valueOf(intData[5] / intData[1]), "251662080 / -1"); 
		assertEqual("0", String.valueOf(intData[5] % intData[1]), "251662080 % -1"); 
		assertEqual("251662080", String.valueOf(intData[5] & intData[1]), "251662080 & -1"); 
		assertEqual("-1", String.valueOf(intData[5] | intData[1]), "251662080 | -1"); 
		assertEqual("-251662081", String.valueOf(intData[5] ^ intData[1]), "251662080 ^ -1"); 
		assertEqual("true", String.valueOf(intData[5] > intData[1]), "251662080 > -1"); 
		assertEqual("false", String.valueOf(intData[5] < intData[1]), "251662080 < -1"); 
		assertEqual("false", String.valueOf(intData[5] == intData[1]), "251662080 == -1"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[1]), "251662080 >= -1"); 
		assertEqual("false", String.valueOf(intData[5] <= intData[1]), "251662080 <= -1"); 
		assertEqual("251662097", String.valueOf(intData[5] + intData[2]), "251662080 + 17"); 
		assertEqual("251662063", String.valueOf(intData[5] - intData[2]), "251662080 - 17"); 
		assertEqual("-16711936", String.valueOf(intData[5] * intData[2]), "251662080 * 17"); 
		assertEqual("14803651", String.valueOf(intData[5] / intData[2]), "251662080 / 17"); 
		assertEqual("13", String.valueOf(intData[5] % intData[2]), "251662080 % 17"); 
		assertEqual("0", String.valueOf(intData[5] & intData[2]), "251662080 & 17"); 
		assertEqual("251662097", String.valueOf(intData[5] | intData[2]), "251662080 | 17"); 
		assertEqual("251662097", String.valueOf(intData[5] ^ intData[2]), "251662080 ^ 17"); 
		assertEqual("true", String.valueOf(intData[5] > intData[2]), "251662080 > 17"); 
		assertEqual("false", String.valueOf(intData[5] < intData[2]), "251662080 < 17"); 
		assertEqual("false", String.valueOf(intData[5] == intData[2]), "251662080 == 17"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[2]), "251662080 >= 17"); 
		assertEqual("false", String.valueOf(intData[5] <= intData[2]), "251662080 <= 17"); 
		assertEqual("251662063", String.valueOf(intData[5] + intData[3]), "251662080 + -17"); 
		assertEqual("251662097", String.valueOf(intData[5] - intData[3]), "251662080 - -17"); 
		assertEqual("16711936", String.valueOf(intData[5] * intData[3]), "251662080 * -17"); 
		assertEqual("-14803651", String.valueOf(intData[5] / intData[3]), "251662080 / -17"); 
		assertEqual("13", String.valueOf(intData[5] % intData[3]), "251662080 % -17"); 
		assertEqual("251662080", String.valueOf(intData[5] & intData[3]), "251662080 & -17"); 
		assertEqual("-17", String.valueOf(intData[5] | intData[3]), "251662080 | -17"); 
		assertEqual("-251662097", String.valueOf(intData[5] ^ intData[3]), "251662080 ^ -17"); 
		assertEqual("true", String.valueOf(intData[5] > intData[3]), "251662080 > -17"); 
		assertEqual("false", String.valueOf(intData[5] < intData[3]), "251662080 < -17"); 
		assertEqual("false", String.valueOf(intData[5] == intData[3]), "251662080 == -17"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[3]), "251662080 >= -17"); 
		assertEqual("false", String.valueOf(intData[5] <= intData[3]), "251662080 <= -17"); 
		assertEqual("-16711936", String.valueOf(intData[5] + intData[4]), "251662080 + -268374016"); 
		assertEqual("520036096", String.valueOf(intData[5] - intData[4]), "251662080 - -268374016"); 
		assertEqual("235929600", String.valueOf(intData[5] * intData[4]), "251662080 * -268374016"); 
		assertEqual("0", String.valueOf(intData[5] / intData[4]), "251662080 / -268374016"); 
		assertEqual("251662080", String.valueOf(intData[5] % intData[4]), "251662080 % -268374016"); 
		assertEqual("0", String.valueOf(intData[5] & intData[4]), "251662080 & -268374016"); 
		assertEqual("-16711936", String.valueOf(intData[5] | intData[4]), "251662080 | -268374016"); 
		assertEqual("-16711936", String.valueOf(intData[5] ^ intData[4]), "251662080 ^ -268374016"); 
		assertEqual("true", String.valueOf(intData[5] > intData[4]), "251662080 > -268374016"); 
		assertEqual("false", String.valueOf(intData[5] < intData[4]), "251662080 < -268374016"); 
		assertEqual("false", String.valueOf(intData[5] == intData[4]), "251662080 == -268374016"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[4]), "251662080 >= -268374016"); 
		assertEqual("false", String.valueOf(intData[5] <= intData[4]), "251662080 <= -268374016"); 
		assertEqual("503324160", String.valueOf(intData[5] + intData[5]), "251662080 + 251662080"); 
		assertEqual("0", String.valueOf(intData[5] - intData[5]), "251662080 - 251662080"); 
		assertEqual("14745600", String.valueOf(intData[5] * intData[5]), "251662080 * 251662080"); 
		assertEqual("1", String.valueOf(intData[5] / intData[5]), "251662080 / 251662080"); 
		assertEqual("0", String.valueOf(intData[5] % intData[5]), "251662080 % 251662080"); 
		assertEqual("251662080", String.valueOf(intData[5] & intData[5]), "251662080 & 251662080"); 
		assertEqual("251662080", String.valueOf(intData[5] | intData[5]), "251662080 | 251662080"); 
		assertEqual("0", String.valueOf(intData[5] ^ intData[5]), "251662080 ^ 251662080"); 
		assertEqual("false", String.valueOf(intData[5] > intData[5]), "251662080 > 251662080"); 
		assertEqual("false", String.valueOf(intData[5] < intData[5]), "251662080 < 251662080"); 
		assertEqual("true", String.valueOf(intData[5] == intData[5]), "251662080 == 251662080"); 
		assertEqual("true", String.valueOf(intData[5] >= intData[5]), "251662080 >= 251662080"); 
		assertEqual("true", String.valueOf(intData[5] <= intData[5]), "251662080 <= 251662080"); 
	}

	public static void main(String[] args) {
		FiScEVM.infoOut("Begin!");
		assertEqual("33554432", String.valueOf(1 << 25));
		{
			String a = "" + 1 + 2 + 3;
			String b = String.valueOf(6);
			if (a.equals(b)) {
				TestService.fail("String compare error");
				throw new RuntimeException();
			}
		}
		new ArchitectureTest().run();
		FiScEVM.infoOut(ArchitectureTest.class.getName());
		try {
			Object o = new HashMap();
			ArrayList a = (ArrayList) o;
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		strictTestInt();
		strictTestLong();

		int a = 5000;
		int b = 6000;
		int c = 0;
		int na = -5000;
		long al = 500000000000L;
		long bl = 600000000000L;
		long cl = 5;
		long nal = -500000000000L;
		byte[] ab = { 1, 2, 3, 4, 5, 6 };
		char[] ac = { 1, 2, 3, 4, 5, 6, 1000, 2000 };
		float f1 = 1.1f;
		float f2 = 2.25f;
		float f3 = 20000000000f;
		float nf2 = -2.25f;
		float nf3 = -20000000000f;
		float f4 = 3f;
		double d1 = 1.1f;
		double d2 = 2.2f;
		double d3 = 20000000000.0;
		double d4 = 3f;
		double nd2 = -2.2f;
		double nd3 = -20000000000.0;

		int i1 = 130;
		byte b1;
		b1 = (byte) i1;

		int i1a;
		i1a = b1;

		if (b1 > 0 || i1a > 0) {
			TestService.fail("I2B Error sign");
		}

		int i2 = 50000;
		short s1 = (short) i2;
		char c1 = (char) i2;

		int i2a = s1;
		int i2b = c1;

		if (s1 > 0 || i2a > 0) {
			TestService.fail("I2S Error sign");
		}

		if (c1 < 0 || i2b < 0) {
			TestService.fail("I2C Error sign");
		}

		int i3 = -10000;
		short s2 = (short) i3;
		char c2 = (char) i3;

		int i3a = s2;
		int i3b = c2;

		if (s2 > 0 || i3a > 0) {
			TestService.fail("I2S Error sign2");
		}

		if (c2 < 0 || i3b < 0) {
			TestService.fail("I2C Error sign2");
		}

		int i4 = -40000;
		short s3 = (short) i4;
		char c3 = (char) i4;

		int i4a = s3;
		int i4b = c3;

		if (s3 < 0 || i4a < 0) {
			TestService.fail("I2S Error sign3");
		}

		if (c3 < 0 || i4b < 0) {
			TestService.fail("I2C Error sign3");
		}

		if (((float) a) != 5000.0f) {
			TestService.fail("I2F Error: 5000 => " + ((float) a));
		}

		if (((long) a) != 5000l) {
			TestService.fail("I2L Error: 5000 => " + ((long) a));
		}

		if (((double) a) != 5000.0) {
			TestService.fail("I2D Error: 5000 => " + ((double) a));
		}

		if (((float) al) != 500000000000.0f) {
			TestService.fail("L2F Error: 500000000000 => " + ((float) al));
		}

		if (((int) al) != 1783793664) {
			TestService.fail("L2I Error: 1783793664 => " + ((int) al));
		}

		if (((double) al) != 500000000000.0) {
			TestService.fail("L2D Error: 500000000000 => " + ((double) al));
		}

		if (((long) f2) != 2l) {
			TestService.fail("F2L Error: 2 => " + ((long) f2));
		}

		if (((long) f3) != 20000000000l) {
			TestService.fail("F2L Error: 20000000000 => " + ((long) f3));
		}

		if (((int) f2) != 2) {
			TestService.fail("F2I Error: 2 => " + ((int) f2));
		}

		if (((double) f2) != 2.25) {
			TestService.fail("F2D Error: 2.2 => " + ((double) f2));
		}

		if (((long) d2) != 2l) {
			TestService.fail("D2L Error: 2 => " + ((long) d2));
		}

		if (((long) d3) != 20000000000l) {
			TestService.fail("D2L Error: 20000000000 => " + ((long) d3));
		}

		if (((int) d2) != 2) {
			TestService.fail("D2I Error: 2 => " + ((int) d2));
		}

		if (((float) d2) != 2.2f) {
			TestService.fail("D2F Error: 2.25 => " + ((float) d2));
		}

		if (((float) na) != -5000.0f) {
			TestService.fail("I2F Error: -5000 => " + ((float) na));
		}

		if (((long) na) != -5000l) {
			TestService.fail("I2L Error: -5000 => " + ((long) na));
		}

		if (((double) na) != -5000.0) {
			TestService.fail("I2D Error: -5000 => " + ((double) na));
		}

		if (((float) nal) != -500000000000.0f) {
			TestService.fail("L2F Error: -500000000000 => " + ((float) nal));
		}

		if (((int) nal) != -1783793664) {
			TestService.fail("L2I Error: -1783793664 => " + ((int) nal));
		}

		if (((double) nal) != -500000000000.0) {
			TestService.fail("L2D Error: -500000000000 => " + ((double) nal));
		}

		if (((long) nf2) != -2l) {
			TestService.fail("F2L Error: -2 => " + ((long) nf2));
		}

		if (((long) nf3) != -20000000000l) {
			TestService.fail("F2L Error: -20000000000 => " + ((long) nf3));
		}

		if (((int) nf2) != -2) {
			TestService.fail("F2I Error: -2 => " + ((int) nf2));
		}

		if (((double) nf2) != -2.25) {
			TestService.fail("F2D Error: -2.25 => " + ((double) nf2));
		}

		if (((long) nd2) != -2l) {
			TestService.fail("D2L Error: -2 => " + ((long) nd2));
		}

		if (((long) nd3) != -20000000000l) {
			TestService.fail("D2L Error: -20000000000 => " + ((long) nd3));
		}

		if (((int) nd2) != -2) {
			TestService.fail("D2I Error: -2 => " + ((int) nd2));
		}

		if (((float) nd2) != -2.2f) {
			TestService.fail("D2F Error: -2.2 => " + ((float) nd2));
		}

		if (a + b != 11000) {
			TestService.fail("mistake in adding two integers");
		}
		if (a - b != -1000) {
			TestService.fail("mistake in subing two integers");
		}
		if (a * b != 30000000) {
			TestService.fail("mistake in muling two integers");
		}
		if (b / a != 1) {
			TestService.fail("mistake in diving two integers 1");
		}
		if (a / b != 0) {
			TestService.fail("mistake in diving two integers 2");
		}
		if (!String.valueOf(al + bl).equals("1100000000000")) {
			TestService.fail("mistake in adding two longs "
					+ String.valueOf(al + bl));
		}
		if (!String.valueOf(al - bl).equals("-100000000000")) {
			TestService.fail("mistake in subing two longs "
					+ String.valueOf(al - bl));
		}
		if (!String.valueOf(al * bl).equals("601129261562068992")) {
			TestService.fail("mistake in muling two longs"
					+ String.valueOf(al * bl));
		}
		if (!String.valueOf(bl / al).equals("1")) {
			TestService.fail("mistake in diving two longs"
					+ String.valueOf(al * bl));
		}
		if (!String.valueOf(bl / 1000).equals("600000000")) {
			TestService.fail("mistake in diving long with int"
					+ String.valueOf(al * bl));
		}
		try {
			c = a / c;
			TestService.fail("mistake in invoking divided by zero");
		} catch (ArithmeticException ae) {
			if (ae == null) {
				TestService.fail("mistake in exception throw");
			}
			ae.printStackTrace();
		}

	}
}
