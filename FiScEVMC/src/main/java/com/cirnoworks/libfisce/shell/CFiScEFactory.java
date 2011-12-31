package com.cirnoworks.libfisce.shell;

import java.io.InputStream;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.IToolkit;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMFactory;

public class CFiScEFactory implements VMFactory {

	@Override
	public FiScEVM getVM() {
		FYContext context = new FYContext();
		context.addToolkit(new IToolkit() {

			@Override
			public void setupContext() {
			}

			@Override
			public void setContext(FiScEVM context) {
			}

			@Override
			public void saveData(Element data) throws VMCriticalException {
			}

			@Override
			public void loadData(Element data) throws VMCriticalException {
			}

			@Override
			public InputStream getResourceByClassName(String className) {
				return this.getClass().getResourceAsStream(
						"/fisce_scripts/" + className + ".class");
			}
		});
		context.addToolkit(new IToolkit() {

			@Override
			public void setupContext() {
			}

			@Override
			public void setContext(FiScEVM context) {
			}

			@Override
			public void saveData(Element data) throws VMCriticalException {
			}

			@Override
			public void loadData(Element data) throws VMCriticalException {
			}

			@Override
			public InputStream getResourceByClassName(String className) {
				return this.getClass().getResourceAsStream(
						"/classes/" + className + ".class");
			}
		});
		context.addToolkit(new IToolkit() {

			@Override
			public void setupContext() {
			}

			@Override
			public void setContext(FiScEVM context) {
			}

			@Override
			public void saveData(Element data) throws VMCriticalException {
			}

			@Override
			public void loadData(Element data) throws VMCriticalException {
			}

			@Override
			public InputStream getResourceByClassName(String className) {
				return this.getClass().getResourceAsStream(
						"/bin/" + className + ".class");
			}
		});
		return context;
	}

}
