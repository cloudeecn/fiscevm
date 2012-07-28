package java.security;

import java.util.Enumeration;

public class DummyPermissionCollection extends PermissionCollection {

	@Override
	public void add(Permission permission) {
		// TODO Auto-generated method stub

	}

	@Override
	public Enumeration<Permission> elements() {
		return null;
	}

	@Override
	public boolean implies(Permission permission) {
		return true;
	}

}
