/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

	@Override
	public String toString() {
		return getClassName()
				+ "."
				+ methodName
				+ (isNativeMethod() ? "(Native Method)" : (fileName != null
						&& lineNumber >= 0 ? "(" + fileName + ":" + lineNumber
						+ ")" : (fileName != null ? "(" + fileName + ")"
						: "(Unknown Source)")));
	}

	@Override
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

	@Override
	public int hashCode() {
		int result = 31 * declaringClass.hashCode() + methodName.hashCode();
		result = 31 * result + (fileName == null ? 0 : fileName.hashCode());
		result = 31 * result + lineNumber;
		return result;
	}

}
