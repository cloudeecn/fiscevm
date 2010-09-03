package com.cirnoworks.fisce;
import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.vm.default_impl.DefaultHeap;
import com.cirnoworks.fisce.vm.default_impl.DefaultThreadManager;

public class FMS {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		VMContext vm = new VMContext();
		vm.setClassLoader(new DefaultClassLoader());
		vm.setHeap(new DefaultHeap());
		vm.setThreadManager(new DefaultThreadManager());
		vm.setConsole(new SystemOutConsole());
		vm.addToolkit(new BaseToolkit());
		try {
			vm.bootFromClass(args[0]);
			vm.start();
			vm.waitTillStopped(0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
