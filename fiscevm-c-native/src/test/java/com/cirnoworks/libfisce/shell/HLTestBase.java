package com.cirnoworks.libfisce.shell;

import java.io.File;
import java.io.IOException;

import junit.framework.TestCase;

import com.cirnoworks.fisce.intf.IStateListener;

public abstract class HLTestBase extends TestCase {
	static {
		try {
			File tempLibFile = File.createTempFile("fiscej-test", null);
			tempLibFile.delete();
			tempLibFile.mkdirs();
			tempLibFile.deleteOnExit();
			NativeLoader.extract(tempLibFile, NativeLoader.getOS()
					+ NativeLoader.getOSPostfix());
			tempLibFile = null;
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	private static ThreadLocal<FYContext> contextHolder = new ThreadLocal<FYContext>();

	protected FYContext context() {
		return contextHolder.get();
	}

	@Override
	protected void setUp() throws Exception {
		if (contextHolder.get() != null) {
			throw new IllegalStateException("Test context holder is not clean!");
		}
//		InputStream is = getClass().getResourceAsStream(
//				"/fisce_scripts/java/lang/Object.class");
//		assertNotNull("/fisce_scripts/java/lang/Object.class", is);
//		is.close();
		FYContext context = new FYContext();
		context.addToolkit(new TestSupportToolkit());
		context.addStateListener(new IStateListener() {

			@Override
			public void onVMDead() {
				System.out.println("vmdead");
			}

			@Override
			public void onException(Throwable e) {
				if (e instanceof RuntimeException) {
					throw (RuntimeException) e;
				} else if (e instanceof Error) {
					throw (Error) e;
				} else {
					throw new RuntimeException(e);
				}
			}
		});
		contextHolder.set(context);
	}

	protected void run(String className) {
		System.out.println("Booting: " + className);
		context().bootFromClass(className);
		System.out.println("Running: " + className);
		context().runSync();
	}

	@Override
	protected void tearDown() throws Exception {
		contextHolder.remove();
		
	}

}
