package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class UnicodeTest {

	public static void main(String[] args) {
		FiScEVM.infoOut("" + (int) '中');
		FiScEVM.infoOut("" + (int) '文');
		FiScEVM.infoOut("" + (int) "中文".charAt(0));
		FiScEVM.infoOut("中文");
	}

}
