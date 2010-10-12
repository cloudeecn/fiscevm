package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class EnumTester {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			En en = En.value2;
			if (en.eval(5, 5) > 0.1 || en.eval(5, 5) < -0.1) {
				FiScEVM.throwOut(new RuntimeException("eval"));
			}
			switch (en) {
			case value2:
				break;
			case value1:
			case value3:
			default:
				FiScEVM.throwOut(new RuntimeException("switch"));
			}
			en = En.valueOf("value2");
			if (en.eval(5, 5) > 0.1 || en.eval(5, 5) < -0.1) {
				FiScEVM.throwOut(new RuntimeException("eval2"));
			}
			switch (en) {
			case value2:
				break;
			case value1:
			case value3:
			default:
				FiScEVM.throwOut(new RuntimeException("switch2"));
			}
		} catch (Throwable t) {
			FiScEVM.throwOut(t);
		}
	}
}

enum En {
	value1 {
		double eval(double x, double y) {
			return x + y;
		}
	},
	value2 {
		double eval(double x, double y) {
			return x - y;
		}
	},
	value3 {
		double eval(double x, double y) {
			return x * y;
		}
	};
	abstract double eval(double x, double y);

}
