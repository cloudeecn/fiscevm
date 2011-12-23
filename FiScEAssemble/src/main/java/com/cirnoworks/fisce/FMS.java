package com.cirnoworks.fisce;

import java.util.HashSet;

import com.cirnoworks.fisce.env.minimal.BaseToolkit;
import com.cirnoworks.fisce.vm.JHeap;
import com.cirnoworks.fisce.vm.JThreadManager;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.default_impl.ArrayHeap;
import com.cirnoworks.fisce.vm.default_impl.ArrayThreadManager;
import com.cirnoworks.fisce.vm.default_impl.DefaultClassLoader;
import com.cirnoworks.fisce.vm.default_impl.DefaultHeap;
import com.cirnoworks.fisce.vm.default_impl.DefaultThreadManager;
import com.cirnoworks.fisce.vm.default_impl.FastThreadManager;

public class FMS {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int i;
		int max = args.length;
		boolean readme = false;
		HashSet<String> params = new HashSet<String>();
		for (i = 0; i < max; i++) {
			if (args[i].startsWith("-")) {
				if (args[i].length() == 1) {
					readme = true;
					break;
				}
				params.add(args[i].substring(1));
			} else {
				break;
			}
		}
		if (readme) {
			System.out.println("Params wrong!");

			return;
		}
		JHeap heap;
		JThreadManager threadManager;
		if (params.contains("Harray")) {
			heap = new ArrayHeap();
		} else {
			heap = new DefaultHeap();
		}
		if (params.contains("Tarray")) {
			threadManager = new ArrayThreadManager();
		} else if (params.contains("Tfast")) {
			threadManager = new FastThreadManager();
		} else {
			threadManager = new DefaultThreadManager();
		}

		VMContext vm = new VMContext();
		vm.setClassLoader(new DefaultClassLoader());
		vm.setHeap(heap);
		vm.setThreadManager(threadManager);
		vm.setConsole(new SystemOutConsole());
		vm.addToolkit(new BaseToolkit());
		vm.addToolkit(new EclipseRunSupportToolkit());
		String boot;
		if (i < max) {
			boot = args[i];
		} else {
			boot = "com/cirnoworks/fisce/privat/Profile";
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
