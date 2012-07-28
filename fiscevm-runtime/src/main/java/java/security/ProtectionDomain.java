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

package java.security;

/**
 * {@code ProtectionDomain} represents all permissions that are granted to a
 * specific code source. The {@link ClassLoader} associates each class with the
 * corresponding {@code ProtectionDomain}, depending on the location and the
 * certificates (encapsulates in {@link CodeSource}) it loads the code from.
 * <p>
 * A class belongs to exactly one protection domain and the protection domain
 * can not be changed during the lifetime of the class.
 */
public class ProtectionDomain {

    // CodeSource for this ProtectionDomain

    // Static permissions for this ProtectionDomain
    private PermissionCollection permissions;

    // ClassLoader
    private ClassLoader classLoader;

    // Set of principals associated with this ProtectionDomain
    private Principal[] principals;

    // false if this ProtectionDomain was constructed with static 
    // permissions, true otherwise. 
    private boolean dynamicPerms;


    /**
     * Returns the {@code ClassLoader} associated with this {@code
     * ProtectionDomain}.
     *
     * @return the {@code ClassLoader} associated with this {@code
     *         ProtectionDomain}, maybe {@code null}.
     */
    public final ClassLoader getClassLoader() {
        return classLoader;
    }

    /**
     * Returns the {@code CodeSource} of this {@code ProtectionDomain}.
     *
     * @return the {@code CodeSource} of this {@code ProtectionDomain}, maybe
     *         {@code null}.
     */
    public final Object getCodeSource() {
        return null;
    }

    /**
     * Returns the static permissions that are granted to this {@code
     * ProtectionDomain}.
     *
     * @return the static permissions that are granted to this {@code
     *         ProtectionDomain}, maybe {@code null}.
     */
    public final PermissionCollection getPermissions() {
        return permissions;
    }

    /**
     * Returns the principals associated with this {@code ProtectionDomain}.
     * Modifications of the returned {@code Principal} array has no impact on
     * this {@code ProtectionDomain}.
     *
     * @return the principals associated with this {@code ProtectionDomain}.
     */
    public final Principal[] getPrincipals() {
        if( principals == null ) {
            return new Principal[0];
        }
        Principal[] tmp = new Principal[principals.length];
        System.arraycopy(principals, 0, tmp, 0, tmp.length);
        return tmp;
    }

    /**
     * Indicates whether the specified permission is implied by this {@code
     * ProtectionDomain}.
     * <p>
     * If this {@code ProtectionDomain} was constructed with
     * {@link #ProtectionDomain(CodeSource, PermissionCollection)}, the
     * specified permission is only checked against the permission collection
     * provided in the constructor. If {@code null} was provided, {@code false}
     * is returned.
     * <p>
     * If this {@code ProtectionDomain} was constructed with
     * {@link #ProtectionDomain(CodeSource, PermissionCollection, ClassLoader, Principal[])}
     * , the specified permission is checked against the policy and the
     * permission collection provided in the constructor.
     *
     * @param permission
     *            the permission to check against the domain.
     * @return {@code true} if the specified {@code permission} is implied by
     *         this {@code ProtectionDomain}, {@code false} otherwise.
     */
    public boolean implies(Permission permission) {
        return true;
    }

    
    /*
     * Returns true if the caller has permission to retrieve the current
     * security policy settings, or false if not.
     */
    private boolean canGetPolicy() {
        return true;
    }
}
