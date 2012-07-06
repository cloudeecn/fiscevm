package com.cirnoworks.fisce.intf;

public final class FiScEVMFactory {
	private static VMFactory factory;

	public static void setFactory(VMFactory factory) {
		FiScEVMFactory.factory = factory;
	}

	public static FiScEVM getVM() {
		try {
			return factory.getVM();
		} catch (NullPointerException e) {
			throw new NullPointerException(
					"Please use setFactory to initialize first.");
		}
	}
}
