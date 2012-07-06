package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class UnicodeTest {

	public static void main(String[] args) {
		FiScEVM.debugOut("" + (int) '中');
		FiScEVM.debugOut("" + (int) '文');
		FiScEVM.debugOut("" + (int) "中文".charAt(0));
		FiScEVM.debugOut("中文");
	}

}
