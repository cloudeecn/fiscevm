package com.cirnoworks.fisce.jvm13.data;

import java.io.FileInputStream;
import java.io.FileOutputStream;

import junit.framework.TestCase;

import com.cirnoworks.fisce.jvm13.FiScEVM;
import com.cirnoworks.fisce.jvm13.Log4JConsole;
import com.cirnoworks.fisce.jvm13.TestStatics;
import com.cirnoworks.fisce.jvm13.VMContext;

public class GCTest extends TestCase {
	public void testGC() throws Exception {
		FiScEVM context = TestStatics.prepareContext(getClass().getName());
		context.bootFromClass("ox/cirnoworks/test/TestGC");
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
		context = TestStatics.prepareContext(getClass().getName());
		context.setConsole(com.cirnoworks.fisce.jvm13.Log4JConsole.getConsole());
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
