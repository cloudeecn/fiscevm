package com.cirnoworks.fisce.env.minimal;

import java.io.FileInputStream;
import java.io.FileOutputStream;

import junit.framework.TestCase;

import com.cirnoworks.fisce.vm.JHeap;
import com.cirnoworks.fisce.vm.VMContext;

public class GCTest extends TestCase {
	public void testGC() throws Exception {
		VMContext context = TestInitializer.getContext();
		context.bootFromClass("EXCLUDE/fisce/test/TestGC");
		// context.requestStop();
		// context.setConsole(Log4JConsole.getConsole());
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("data.xml");
		context.saveData(fos);
		fos.close();

		((JHeap) context.getHeap()).gc();
		fos = new FileOutputStream("data0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getContext();
		// context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("data0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("data1.xml");
		context.saveData(fos);
		fos.close();
		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}

	public void testGCFast() throws Exception {
		VMContext context = TestInitializer.getFastContext();
		context.bootFromClass("EXCLUDE/fisce/test/TestGC");
		// context.requestStop();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("data.xml");
		context.saveData(fos);
		fos.close();

		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getFastContext();
		// context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("data0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("data1.xml");
		context.saveData(fos);
		fos.close();
		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}

	public void testGCArray() throws Exception {
		VMContext context = TestInitializer.getArrayContext();
		// context.setConsole(Log4JConsole.getConsole());
		context.bootFromClass("EXCLUDE/fisce/test/TestGC");
		// context.requestStop();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("dataa.xml");
		context.saveData(fos);
		fos.close();

		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("dataa0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getArrayContext();
		// context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("dataa0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("dataa1.xml");
		context.saveData(fos);
		fos.close();
		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("dataa2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}

	public void testGCComp1() throws Exception {
		VMContext context = TestInitializer.getArrayContext();
		// context.setConsole(Log4JConsole.getConsole());
		context.bootFromClass("EXCLUDE/fisce/test/TestGC");
		// context.requestStop();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("data.xml");
		context.saveData(fos);
		fos.close();

		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getContext();
		// context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("data0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("data1.xml");
		context.saveData(fos);
		fos.close();
		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}

	public void testGCComp2() throws Exception {
		VMContext context = TestInitializer.getContext();
		// context.setConsole(Log4JConsole.getConsole());
		context.bootFromClass("EXCLUDE/fisce/test/TestGC");
		// context.requestStop();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
		System.out.println("STOP&RESUME!*************");
		FileOutputStream fos;
		fos = new FileOutputStream("data.xml");
		context.saveData(fos);
		fos.close();

		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data0.xml");
		context.saveData(fos);
		fos.close();
		context = TestInitializer.getArrayContext();
		// context.setConsole(Log4JConsole.getConsole());
		FileInputStream fis = new FileInputStream("data0.xml");
		context.bootFromData(fis);
		fis.close();
		fos = new FileOutputStream("data1.xml");
		context.saveData(fos);
		fos.close();
		((JHeap) ((VMContext) context).getHeap()).gc();
		fos = new FileOutputStream("data2.xml");
		context.saveData(fos);
		fos.close();
		context.start();
		try {
			context.waitTillStopped(300);
		} catch (InterruptedException e) {
		}
		context.requestStop();
		context.waitTillStopped(0);
	}
}
