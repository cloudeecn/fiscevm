/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

package java.lang;

import java.security.AccessController;
import java.security.Permission;

/**
 * Provides security verification facilities for applications.
 * {@code SecurityManager} contains a set of {@code checkXXX} methods which
 * determine if it is safe to perform a specific operation such as establishing
 * network connections, modifying files, and many more. In general, these
 * methods simply return if they allow the application to perform the operation;
 * if an operation is not allowed, then they throw a {@link SecurityException}.
 * The only exception is {@link #checkTopLevelWindow(Object)}, which returns a
 * boolean to indicate permission.
 */
public class SecurityManager {

	/**
	 * Constructs a new {@code SecurityManager} instance.
	 * <p>
	 * The {@code RuntimePermission("createSecurityManager")} is checked if a
	 * security manager is installed.
	 */
	public SecurityManager() {
	}

	/**
	 * Checks whether the calling thread is allowed to accept socket
	 * connections.
	 * 
	 * @param host
	 *            the address of the host that attempts to connect.
	 * @param port
	 *            the port number to check.
	 * @throws NullPointerException
	 *             if {@code host} is {@code null}.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to accept socket
	 *             connections from {@code host} through {@code port}.
	 */
	public void checkAccept(String host, int port) {
	}

	/**
	 * Checks whether the calling thread is allowed to modify the specified
	 * thread.
	 * 
	 * @param thread
	 *            the thread to access.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access {@code thread}
	 *             .
	 */
	public void checkAccess(Thread thread) {
	}

	/**
	 * Checks whether the calling thread is allowed to modify the specified
	 * thread group.
	 * 
	 * @param group
	 *            the thread group to access.
	 * @throws NullPointerException
	 *             if {@code group} is {@code null}.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access {@code group}.
	 */
	public void checkAccess(Object group) {
	}

	/**
	 * Checks whether the calling thread is allowed to establish socket
	 * connections. A -1 port indicates the caller is trying to resolve the
	 * hostname.
	 * 
	 * @param host
	 *            the address of the host to connect to.
	 * @param port
	 *            the port number to check, or -1 for resolve.
	 * @throws NullPointerException
	 *             if {@code host} is {@code null}.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to connect to
	 *             {@code host} through {@code port}.
	 */
	public void checkConnect(String host, int port) {
	}

	/**
	 * Checks whether the specified security context is allowed to establish
	 * socket connections. A -1 port indicates the caller is trying to resolve
	 * the hostname.
	 * 
	 * @param host
	 *            the address of the host to connect to.
	 * @param port
	 *            the port number to check, or -1 for resolve.
	 * @param context
	 *            the security context to use for the check.
	 * @throws NullPointerException
	 *             if {@code host} is {@code null}.
	 * @throws SecurityException
	 *             if {@code context} is not allowed to connect to {@code host}
	 *             through {@code port}.
	 */
	public void checkConnect(String host, int port, Object context) {
	}

	/**
	 * Checks whether the calling thread is allowed to create a class loader.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to create a class
	 *             loader.
	 */
	public void checkCreateClassLoader() {
	}

	/**
	 * Checks whether the calling thread is allowed to delete the file with the
	 * specified name, which should be passed in canonical form.
	 * 
	 * @param file
	 *            the name of the file to delete.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to delete {@code file}.
	 */
	public void checkDelete(String file) {
	}

	/**
	 * Checks whether the calling thread is allowed to execute the specified
	 * platform specific command.
	 * 
	 * @param cmd
	 *            the command line to execute.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to execute {@code cmd}.
	 */
	public void checkExec(String cmd) {
	}

	/**
	 * Checks whether the calling thread is allowed to terminate the virtual
	 * machine.
	 * 
	 * @param status
	 *            the status that the virtual machine returns when it is
	 *            terminated.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to terminate the virtual
	 *             machine with {@code status}.
	 */
	public void checkExit(int status) {
	}

	/**
	 * Checks whether the calling thread is allowed to load the specified native
	 * library.
	 * 
	 * @param libName
	 *            the name of the library to load.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to load {@code libName}.
	 */
	public void checkLink(String libName) {
	}

	/**
	 * Checks whether the calling thread is allowed to listen on the specified
	 * port.
	 * 
	 * @param port
	 *            the port number to check.
	 * @throws SecurityException
	 *             if the calling thread is not allowed listen on {@code port}.
	 */
	public void checkListen(int port) {
	}

	/**
	 * Checks whether the calling thread is allowed to access members. The
	 * default is to allow access to public members (that is,
	 * {@code java.lang.reflect.Member.PUBLIC}) and to classes loaded by the
	 * same loader as the original caller (that is, the method that called the
	 * reflect API). Due to the nature of the check, overriding implementations
	 * cannot call {@code super.checkMemberAccess()} since the stack would no
	 * longer be of the expected shape.
	 * 
	 * @param cls
	 *            the class of which members are accessed.
	 * @param type
	 *            the access type, either
	 *            {@code java.lang.reflect.Member.PUBLIC} or
	 *            {@code java.lang.reflect.Member.DECLARED}.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access members of
	 *             {@code cls}.
	 */
	public void checkMemberAccess(Class<?> cls, int type) {
	}

	/**
	 * Checks whether the calling thread is allowed to use the specified IP
	 * multicast group address.
	 * 
	 * @param maddr
	 *            the internet group address to use.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to use {@code maddr}.
	 */
	public void checkMulticast(Object maddr) {
	}

	/**
	 * Checks whether the calling thread is allowed to use the specified IP
	 * multicast group address.
	 * 
	 * @param maddr
	 *            the internet group address to use.
	 * @param ttl
	 *            the value in use for multicast send. This parameter is
	 *            ignored.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to use {@code maddr}.
	 * @deprecated use {@link #checkMulticast(java.net.InetAddress)}
	 */
	@Deprecated
	public void checkMulticast(Object maddr, byte ttl) {
	}

	/**
	 * Checks whether the calling thread is allowed to access the specified
	 * package.
	 * 
	 * @param packageName
	 *            the name of the package to access.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access
	 *             {@code packageName}.
	 */
	public void checkPackageAccess(String packageName) {
	}

	/**
	 * Checks whether the calling thread is allowed to define new classes in the
	 * specified package.
	 * 
	 * @param packageName
	 *            the name of the package to add a class to.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to add classes to
	 *             {@code packageName}.
	 */
	public void checkPackageDefinition(String packageName) {
	}

	/**
	 * Checks whether the calling thread is allowed to access the system
	 * properties.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access system
	 *             properties.
	 */
	public void checkPropertiesAccess() {
	}

	/**
	 * Checks whether the calling thread is allowed to access a particular
	 * system property.
	 * 
	 * @param key
	 *            the name of the property to access.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access the
	 *             {@code key} system property.
	 */
	public void checkPropertyAccess(String key) {
	}

	/**
	 * Checks whether the calling thread is allowed to read from the file with
	 * the specified file descriptor.
	 * 
	 * @param fd
	 *            the file descriptor of the file to read from.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to read from {@code fd}.
	 */
	public void checkRead(Object fd) {
	}

	/**
	 * Checks whether the calling thread is allowed to read from the file with
	 * the specified name, which should be passed in canonical form.
	 * 
	 * @param file
	 *            the name of the file or directory to read from.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to read from
	 *             {@code file}.
	 */
	public void checkRead(String file) {
	}

	/**
	 * Checks whether the given security context is allowed to read from the
	 * file named by the argument, which should be passed in canonical form.
	 * 
	 * @param file
	 *            the name of the file or directory to check.
	 * @param context
	 *            the security context to use for the check.
	 * @throws SecurityException
	 *             if {@code context} is not allowed to read from {@code file}.
	 */
	public void checkRead(String file, Object context) {
	}

	/**
	 * Checks whether the calling thread is allowed to perform the security
	 * operation named by the target.
	 * 
	 * @param target
	 *            the name of the operation to perform.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to perform
	 *             {@code target}.
	 */
	public void checkSecurityAccess(String target) {
	}

	/**
	 * Checks whether the calling thread is allowed to set the net object
	 * factories.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to set the net object
	 *             factories.
	 */
	public void checkSetFactory() {
	}

	/**
	 * Checks whether the calling thread is trusted to show the specified top
	 * level window.
	 * 
	 * @param window
	 *            the window to show.
	 * @return {@code true} if the calling thread is allowed to show
	 *         {@code window}; {@code false} otherwise.
	 * @throws NullPointerException
	 *             if {@code window} is {@code null}.
	 */
	public boolean checkTopLevelWindow(Object window) {
		return true;
	}

	/**
	 * Checks whether the calling thread is allowed to access the system
	 * clipboard.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access the system
	 *             clipboard.
	 */
	public void checkSystemClipboardAccess() {
	}

	/**
	 * Checks whether the calling thread is allowed to access the AWT event
	 * queue.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to access the AWT event
	 *             queue.
	 */
	public void checkAwtEventQueueAccess() {
	}

	/**
	 * Checks whether the calling thread is allowed to start a new print job.
	 * 
	 * @throws SecurityException
	 *             if the calling thread is not allowed to start a new print
	 *             job.
	 */
	public void checkPrintJobAccess() {
	}

	/**
	 * Checks whether the calling thread is allowed to write to the file with
	 * the specified file descriptor.
	 * 
	 * @param fd
	 *            the file descriptor of the file to write to.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to write to {@code fd}.
	 */
	public void checkWrite(Object fd) {
	}

	/**
	 * Checks whether the calling thread is allowed to write to the file with
	 * the specified name, which should be passed in canonical form.
	 * 
	 * @param file
	 *            the name of the file or directory to write to.
	 * @throws SecurityException
	 *             if the calling thread is not allowed to write to {@code file}
	 *             .
	 */
	public void checkWrite(String file) {
	}

	/**
	 * Indicates if this security manager is currently checking something.
	 * 
	 * @return {@code true} if this security manager is executing a security
	 *         check method; {@code false} otherwise.
	 * @deprecated Use {@link #checkPermission}.
	 */
	@Deprecated
	public boolean getInCheck() {
		return false;
	}

	/**
	 * Returns an object which encapsulates the security state of the current
	 * point in the execution. In our case, this is an
	 * {@link java.security.AccessControlContext}.
	 * 
	 * @return an object that encapsulates information about the current
	 *         execution environment.
	 */
	public Object getSecurityContext() {
		return AccessController.getContext();
	}

	/**
	 * Checks whether the calling thread is allowed to access the resource being
	 * guarded by the specified permission object.
	 * 
	 * @param permission
	 *            the permission to check.
	 * @throws SecurityException
	 *             if the requested {@code permission} is denied according to
	 *             the current security policy.
	 */
	public void checkPermission(Permission permission) {
	}

	/**
	 * Checks whether the specified security context is allowed to access the
	 * resource being guarded by the specified permission object.
	 * 
	 * @param permission
	 *            the permission to check.
	 * @param context
	 *            the security context for which to check permission.
	 * @throws SecurityException
	 *             if {@code context} is not an instance of
	 *             {@code AccessControlContext} or if the requested
	 *             {@code permission} is denied for {@code context} according to
	 *             the current security policy.
	 */
	public void checkPermission(Permission permission, Object context) {
	}
}
