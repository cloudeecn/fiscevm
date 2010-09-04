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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package java.lang;

public final class StackTraceElement {
	// Initialized by VM
	private String declaringClass;
	private String methodName;
	private String fileName;
	private int lineNumber;

	private StackTraceElement() {
	}

	public String getFileName() {
		return fileName;
	}

	public int getLineNumber() {
		return lineNumber;
	}

	public String getClassName() {
		return declaringClass;
	}

	public String getMethodName() {
		return methodName;
	}

	public boolean isNativeMethod() {
		return lineNumber == -2;
	}

	public String toString() {
		return getClassName()
				+ "."
				+ methodName
				+ (isNativeMethod() ? "(Native Method)" : (fileName != null
						&& lineNumber >= 0 ? "(" + fileName + ":" + lineNumber
						+ ")" : (fileName != null ? "(" + fileName + ")"
						: "(Unknown Source)")));
	}

	public boolean equals(Object obj) {
		if (obj == this)
			return true;
		if (!(obj instanceof StackTraceElement))
			return false;
		StackTraceElement e = (StackTraceElement) obj;
		return e.declaringClass.equals(declaringClass)
				&& e.lineNumber == lineNumber && eq(methodName, e.methodName)
				&& eq(fileName, e.fileName);
	}

	private static boolean eq(Object a, Object b) {
		return a == b || (a != null && a.equals(b));
	}

	public int hashCode() {
		int result = 31 * declaringClass.hashCode() + methodName.hashCode();
		result = 31 * result + (fileName == null ? 0 : fileName.hashCode());
		result = 31 * result + lineNumber;
		return result;
	}

}
