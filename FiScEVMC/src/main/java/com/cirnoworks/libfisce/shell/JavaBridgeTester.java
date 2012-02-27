package com.cirnoworks.libfisce.shell;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;

public class JavaBridgeTester {
	public static void main(String[] args) {
		FiScEVM vm = new CFiScEFactory().getVM();
		vm.registerNativeHandler(new NativeHandlerTemplate() {

			public String getUniqueName() {
				return "EXCLUDE/fisce/test/ArrayToByteBufferTest.test.([I)Z";
			}

			public void dealNative(int[] args, IThread thread)
					throws VMException, VMCriticalException {
				IHeap heap = context.getHeap();
				ByteBuffer bb = ByteBuffer.allocateDirect(20).order(
						ByteOrder.nativeOrder());
				IntBuffer ib = bb.asIntBuffer();
				heap.getArrayByteBuffer(bb, args[0], 0, 5);
				int succeed = 1;
				for (int i = 0; i < 5; i++) {
					System.out.println(i + " - " + ib.get(i));
					if (ib.get(i) != i + 1) {
						succeed = 0;
					}
				}
				thread.nativeReturnInt(succeed);
			}
		});
		vm.bootFromClass("EXCLUDE/fisce/test/ArrayToByteBufferTest");
		vm.start();
	}

}
