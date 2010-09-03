package com.cirnoworks.fisce.vm.data;

import java.io.FileInputStream;
import java.io.FileOutputStream;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.FiScEVM;
import com.cirnoworks.fisce.vm.Log4JConsole;
import com.cirnoworks.fisce.vm.TestStatics;
import com.cirnoworks.fisce.vm.VMContext;

public class GCTest extends TestCase {
	public void testGC() throws Exception {
		if (true) {
			return;
		}
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
		context.setConsole(com.cirnoworks.fisce.vm.Log4JConsole.getConsole());
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
