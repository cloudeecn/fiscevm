package com.cirnoworks.libfisce.shell;

import java.io.InputStream;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.AbstractToolkit;
import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMFactory;

public class CFiScEFactory implements VMFactory {

	public FYContext getVM() {
		FYContext context = new FYContext();
		context.addToolkit(new AbstractToolkit() {

			public void setupContext() {
			}

			public void setContext(FiScEVM context) {
			}

			public void saveData(Element data) throws VMCriticalException {
			}

			public void loadData(Element data) throws VMCriticalException {
			}

			public InputStream getResourceByName(String className) {
				return this.getClass().getResourceAsStream(
						"/fisce_scripts/" + className);
			}
		});
		context.addToolkit(new AbstractToolkit() {

			public void setupContext() {
			}

			public void setContext(FiScEVM context) {
			}

			public void saveData(Element data) throws VMCriticalException {
			}

			public void loadData(Element data) throws VMCriticalException {
			}

			public InputStream getResourceByName(String className) {
				return this.getClass().getResourceAsStream(
						"/classes/" + className);
			}
		});
		context.addToolkit(new AbstractToolkit() {

			public void setupContext() {
			}

			public void setContext(FiScEVM context) {
			}

			public void saveData(Element data) throws VMCriticalException {
			}

			public void loadData(Element data) throws VMCriticalException {
			}

			public InputStream getResourceByName(String className) {
				return this.getClass().getResourceAsStream("/bin/" + className);
			}
		});
		return context;
	}

}
