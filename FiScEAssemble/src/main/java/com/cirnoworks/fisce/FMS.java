package com.cirnoworks.fisce;
import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.*;

public class FMS {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		VMContext vm = new VMContext();
		vm.setClassLoader(new DefaultClassLoader());
		vm.setHeap(new DefaultHeap());
		vm.setThreadManager(new FastThreadManager());
		vm.setConsole(new SystemOutConsole());
		vm.addToolkit(new BaseToolkit());
		String boot;
		if(args.length>0){
			boot=args[0];
		}else{
			boot="com/cirnoworks/fisce/privat/Profile";
		}
		try {
			vm.bootFromClass(boot);
			vm.start();
			vm.waitTillStopped(0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
