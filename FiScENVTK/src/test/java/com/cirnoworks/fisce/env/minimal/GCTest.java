package com.cirnoworks.fisce.env.minimal;

import java.io.FileInputStream;
import java.io.FileOutputStream;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.VMContext;

public class GCTest extends TestCase {
	public void testGC() throws Exception {
		
		FiScEVM context = TestInitializer.getContext();
		context.bootFromClass("com/cirnoworks/fisce/test/TestGC");
		// context.requestStop();
		context.start();
		try {
			context.waitTillStopped(1000);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("data.xml");
		context.saveData(fos);
		fos.close();

		((VMContext) context).getHeap().gc();
		fos = new FileOutputStream("data0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getContext();
		context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("data0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("data1.xml");
		context.saveData(fos);
		fos.close();
		((VMContext) context).getHeap().gc();
		fos = new FileOutputStream("data2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(1000);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}
}
