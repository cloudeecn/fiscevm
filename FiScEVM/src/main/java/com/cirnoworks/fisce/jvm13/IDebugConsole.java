/**
 *  Copyright 2010 Yuxuan Huang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.jvm13;

public interface IDebugConsole {

	boolean debug(String msg);

	boolean debug(String msg, Throwable t);

	boolean info(String msg);

	boolean info(String msg, Throwable t);

	boolean warn(String msg);

	boolean warn(String msg, Throwable t);

	boolean error(String msg);

	boolean error(String msg, Throwable t);

}
