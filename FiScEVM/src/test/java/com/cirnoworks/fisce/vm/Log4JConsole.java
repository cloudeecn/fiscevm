package com.cirnoworks.fisce.vm;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public final class Log4JConsole implements IDebugConsole {
	private static final Logger LOG = Logger.getLogger(Log4JConsole.class);
	private static Log4JConsole console = new Log4JConsole();

	private Log4JConsole() {
		PropertyConfigurator.configure("log4j-test.properties");
	}

	public static Log4JConsole getConsole() {
		return console;
	}

	public boolean debug(final String line) {
		LOG.debug(line);
		return true;
	}

	public boolean debug(String msg, Throwable throwable) {
		LOG.debug(msg, throwable);
		return true;

	}

	public boolean error(String msg) {
		LOG.error(msg);
		return true;

	}

	public boolean error(String msg, Throwable t) {
		LOG.error(msg, t);
		return true;
	}

	public boolean info(String msg) {
		LOG.info(msg);
		return true;
	}

	public boolean info(String msg, Throwable t) {
		LOG.info(msg, t);
		return true;
	}

	public boolean warn(String msg) {
		LOG.warn(msg);
		return true;
	}

	public boolean warn(String msg, Throwable t) {
		LOG.warn(msg, t);
		return true;
	}
}
