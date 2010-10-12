/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.env.minimal;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.cirnoworks.fisce.vm.IDebugConsole;

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
