package com.cirnoworks.libfisce.shell;

import java.io.InputStream;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.AbstractToolkit;
import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;

public class TestSupportToolkit extends AbstractToolkit {

	private FYContext context;

	@Override
	public void setContext(FiScEVM context) {
		this.context = (FYContext) context;
	}

	@Override
	public void setupContext() {
		NativeHandlerTemplate sosw = new SOSWriterHandler();
		NativeHandlerTemplate vme = new VMEncodeHandler();
		NativeHandlerTemplate vmd = new VMDecodeHandler();
		context.unregisterNativeHandler(sosw.getUniqueName());
		context.registerNativeHandler(sosw);
		context.unregisterNativeHandler(vme.getUniqueName());
		context.registerNativeHandler(vme);
		context.unregisterNativeHandler(vmd.getUniqueName());
		context.registerNativeHandler(vmd);
		context.registerNativeHandler(new TestFailHandler());
	}

	@Override
	public InputStream getResourceByName(String name) {
		InputStream is = getClass().getResourceAsStream(
				"/fisce_scripts/" + name);
		if (is == null) {
			is = getClass().getResourceAsStream("/classes/" + name);
		}
		return is;
	}

	@Override
	public void saveData(Element data) throws VMCriticalException {

	}

	@Override
	public void loadData(Element data) throws VMCriticalException {

	}

}
