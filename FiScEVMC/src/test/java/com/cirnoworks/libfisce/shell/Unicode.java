package com.cirnoworks.libfisce.shell;

import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class Unicode {

	public void unicode() throws InterruptedException {
		CFiScEFactory cf = new CFiScEFactory();
		final FYContext vm = cf.getVM();
		vm.registerNativeHandler(new NativeHandlerTemplate() {

			public String getUniqueName() {
				return "com/cirnoworks/fisce/privat/FiScEVM.debugOut.(Ljava/lang/String;)V";
			}

			public void dealNative(int[] args, IThread thread)
					throws VMException, VMCriticalException {
				IHeap heap = vm.getHeap();
				String str = heap.getString(args[0]);
				System.out.println("debugout: " + str);
			}
		});
		FisceService
				.unregisterNativeHandler(vm.getContext(),
						"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(Ljava/lang/String;)V");
		vm.bootFromClass("EXCLUDE/fisce/test/UnicodeTest");
		vm.start();
		vm.waitTillStopped(1000);
	}

	public static void main(String[] args) {
		try {
			new Unicode().unicode();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
