package com.cirnoworks.fisce.intf;

import java.io.InputStream;

public abstract class AbstractVM implements FiScEVM {
	public InputStream getInputStream(String name) {
		if (name.startsWith("/")) {
			name = name.substring(1);
		}
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
