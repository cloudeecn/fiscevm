package EXCLUDE.fisce.test;

import com.cirnoworks.fisce.privat.FiScEVM;

public class HierarchyTest extends TestService {
	public static void main(String[] args) {
		try {
			JspTag jt = new TestTag();
			FiScEVM.breakpoint();
			Tag tag = (Tag) jt;
			IterationTag iTag = (IterationTag) jt;
			assertEqual(1, jt.method1());
			assertEqual(2, tag.method2());
			assertEqual(3, iTag.method3());
		} catch (Exception e) {
			e.printStackTrace();
			fail("Exception occored: " + e);
		}
	}
}

interface JspTag {

	int method1();
}

interface Tag extends JspTag {
	int method2();
}

interface IterationTag extends Tag {
	int method3();
}

class TestTag implements IterationTag {

	@Override
	public int method2() {
		return 2;
	}

	@Override
	public int method1() {
		return 1;
	}

	@Override
	public int method3() {
		return 3;
	}

}