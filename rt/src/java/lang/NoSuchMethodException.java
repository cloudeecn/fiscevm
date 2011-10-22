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
package java.lang;

/**
 * @author Cloudee
 * 
 */
public class NoSuchMethodException extends Exception {

	/**
	 * 
	 */
	public NoSuchMethodException() {
		super();
	}

	/**
	 * @param message
	 * @param cause
	 */
	public NoSuchMethodException(java.lang.String message,
			java.lang.Throwable cause) {
		super(message, cause);
	}

	/**
	 * @param message
	 */
	public NoSuchMethodException(java.lang.String message) {
		super(message);
	}

	/**
	 * @param cause
	 */
	public NoSuchMethodException(java.lang.Throwable cause) {
		super(cause);
	}

}
