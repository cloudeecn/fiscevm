package com.cirnoworks.fisce.intf;

import java.io.InputStream;

public abstract class AbstractVM implements FiScEVM {
	public InputStream getInputStream(String name) {
		InputStream is = null;
		for (IToolkit toolkit : this.getToolkits()) {
			is = toolkit.getResourceByName(name);
			if (is != null) {
				break;
			}
		}
		return is;
	}
}
