package com.cirnoworks.asm;

import org.junit.Test;
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.commons.Method;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.analysis.Analyzer;
import org.objectweb.asm.tree.analysis.BasicValue;
import org.objectweb.asm.tree.analysis.BasicVerifier;
import org.objectweb.asm.tree.analysis.Frame;

import com.cirnoworks.fisce.data.ClassData;
import com.cirnoworks.fisce.data.MethodData;

public class InstsTest {

	@Test
	public void testInsts() throws Exception {
		ClassReader cr = new ClassReader(Method.class.getName());
		ClassData cd = new ClassData(cr);
		cr.accept(cd, ClassReader.EXPAND_FRAMES);
		Analyzer<BasicValue> a = new Analyzer<BasicValue>(new BasicVerifier());
		for (MethodData md : cd.getMethods()) {
			int i = 0;
			System.out.println(md.name + "." + md.desc + ":");
			Frame<BasicValue>[] frames = a.analyze(cd.name, md);
			for (Frame<BasicValue> frame : frames) {
				AbstractInsnNode ain = md.instructions.get(i);
				int op = ain.getOpcode();
				System.out.println(i + "\t" + op + "\t" + "\t" + frame + "\t"
						+ ain);
				i++;
			}
			i = 0;
		}
	}
}
