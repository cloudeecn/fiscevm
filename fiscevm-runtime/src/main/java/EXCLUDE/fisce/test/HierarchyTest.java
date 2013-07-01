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