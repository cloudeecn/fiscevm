package com.cirnoworks.libfisce;

import junit.framework.Assert;
import junit.framework.TestCase;

import com.cirnoworks.libfisce.linux.LinuxLocator;
import com.cirnoworks.libfisce.win32.Win32Locator;

public class LocatorTest extends TestCase {

	public void testLinuxLocator() {
		Assert.assertNotNull(LinuxLocator.class
				.getResourceAsStream("libfyjni.so"));
	}

	public void testWin32Locator() {
		Assert.assertNotNull(Win32Locator.class
				.getResourceAsStream("libfyjni.dll"));
	}
}
