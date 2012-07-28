/* 
 * Licensed to the Apache Software Foundation (ASF) under one or more
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

package java.security;

/**
 * The vm vendor may choose to implement this class. The reference
 * implementation must be used if the reference implementation of
 * AccessController is used.
 * 
 * An AccessControlContext encapsulates the information which is needed by class
 * AccessController to detect if a Permission would be granted at a particular
 * point in a programs execution.
 * 
 */

public final class AccessControlContext {
	private static final SecurityPermission createAccessControlContext = new SecurityPermission(
			"createAccessControlContext");

	private static final SecurityPermission getDomainCombiner = new SecurityPermission(
			"getDomainCombiner");

	/**
	 * Constructs a new instance of this class given an array of protection
	 * domains.
	 * 
	 */
	public AccessControlContext(ProtectionDomain[] context) {
	}

	AccessControlContext(ProtectionDomain[] context, boolean ignored) {
	}

	/**
	 * Constructs a new instance of this class given a context and a
	 * DomainCombiner
	 */
	public AccessControlContext(AccessControlContext acc,
			Object combiner) {
	}

	/**
	 * Checks if the permission <code>perm</code> is allowed in this context.
	 * All ProtectionDomains must grant the permission for it to be granted.
	 * 
	 * @param perm
	 *            java.security.Permission the permission to check
	 * @exception java.security.AccessControlException
	 *                thrown when perm is not granted.
	 */
	public void checkPermission(Permission perm) throws AccessControlException {
	}


}
