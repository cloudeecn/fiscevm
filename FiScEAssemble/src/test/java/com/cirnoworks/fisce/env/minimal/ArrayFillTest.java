package com.cirnoworks.fisce.env.minimal;

import java.io.InputStream;
import java.util.Random;

import org.w3c.dom.Element;

import com.cirnoworks.fisce.intf.FiScEVM;
import com.cirnoworks.fisce.intf.IHeap;
import com.cirnoworks.fisce.intf.IThread;
import com.cirnoworks.fisce.intf.IToolkit;
import com.cirnoworks.fisce.intf.NativeHandlerTemplate;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.ClassArray;

public class ArrayFillTest {

	public void testArrayFill() throws Exception {
		VMContext context = TestInitializer.getContext();
		context.addToolkit(new ArrayFillToolkit());
		context.bootFromClass("EXCLUDE/fisce/test/ArrayFillTest");
		context.start();
		context.waitTillStopped(0);
	}

	public void testArrayFillFast() throws Exception {
		VMContext context = TestInitializer.getFastContext();
		context.addToolkit(new ArrayFillToolkit());
		context.bootFromClass("EXCLUDE/fisce/test/ArrayFillTest");
		context.start();
		context.waitTillStopped(0);
	}

	public void testArrayFillArray() throws Exception {
		VMContext context = TestInitializer.getArrayContext();
		context.addToolkit(new ArrayFillToolkit());
		context.bootFromClass("EXCLUDE/fisce/test/ArrayFillTest");
		context.start();
		context.waitTillStopped(0);
	}

}

class ArrayFillToolkit implements IToolkit {

	private VMContext context;

	public void setContext(FiScEVM context) {
		this.context = (VMContext) context;
	}

	public void setupContext() {
		context.registerNativeHandler(new ArrayFillHandler());
		context.registerNativeHandler(new ArrayGetHandler());
	}

	public InputStream getResourceByClassName(String className) {
		return null;
	}

	public void saveData(Element data) throws VMCriticalException {

	}

	public void loadData(Element data) throws VMCriticalException {

	}

}

class ArrayFillHandler extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		// args[0] -> arrayOpt
		// args[1] -> arrayNormal
		IHeap heap = context.getHeap();
		ClassArray arrayOptClass = (ClassArray) ((VMContext) context)
				.getClazzById(heap.getClass(args[0]));
		ClassArray arrayNormalClass = (ClassArray) ((VMContext) context)
				.getClazzById(heap.getClass(args[1]));
		assert arrayOptClass == arrayNormalClass : "type mismatch!";
		int len = heap.getArrayLength(args[0]);
		assert len == heap.getArrayLength(args[1]) : "length mismatch!";
		char type = arrayOptClass.getName().charAt(1);
		Random random = new Random();
		// Z B S C I J F D
		switch (type) {
		case 'Z': {
			boolean[] tmp = new boolean[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = random.nextBoolean();
				heap.putArrayBoolean(args[1], i, tmp[i]);
			}
			heap.fillArrayBoolean(args[0], 0, tmp, 0, len);
			break;
		}
		case 'B': {
			byte[] tmp = new byte[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = (byte) random.nextInt();
				heap.putArrayByte(args[1], i, tmp[i]);
			}
			heap.fillArrayByte(args[0], 0, tmp, 0, len);
			break;
		}
		case 'S': {
			short[] tmp = new short[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = (short) random.nextInt();
				heap.putArrayShort(args[1], i, tmp[i]);
			}
			heap.fillArrayShort(args[0], 0, tmp, 0, len);
			break;
		}
		case 'C': {
			char[] tmp = new char[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = (char) random.nextInt();
				heap.putArrayChar(args[1], i, tmp[i]);
			}
			heap.fillArrayChar(args[0], 0, tmp, 0, len);
			break;
		}
		case 'I': {
			int[] tmp = new int[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = random.nextInt();
				heap.putArrayInt(args[1], i, tmp[i]);
			}
			heap.fillArrayInt(args[0], 0, tmp, 0, len);
			break;
		}
		case 'J': {
			long[] tmp = new long[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = random.nextLong();
				heap.putArrayLong(args[1], i, tmp[i]);
			}
			heap.fillArrayLong(args[0], 0, tmp, 0, len);
			break;
		}
		case 'F': {
			float[] tmp = new float[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = random.nextFloat();
				heap.putArrayFloat(args[1], i, tmp[i]);
			}
			heap.fillArrayFloat(args[0], 0, tmp, 0, len);
			break;
		}
		case 'D': {
			double[] tmp = new double[len];
			for (int i = 0; i < len; i++) {
				tmp[i] = random.nextDouble();
				heap.putArrayDouble(args[1], i, tmp[i]);
			}
			heap.fillArrayDouble(args[0], 0, tmp, 0, len);
			break;
		}
		default: {
			assert false : "Illegal type " + type;
		}
		}
	}

	public String getUniqueName() {
		return "EXCLUDE/fisce/test/ArrayFillTest.fillArrayTest.(Ljava/lang/Object;Ljava/lang/Object;)V";
	}

}

class ArrayGetHandler extends NativeHandlerTemplate {

	public void dealNative(int[] args, IThread thread) throws VMException,
			VMCriticalException {
		// args[0] -> arrayOpt
		// args[1] -> arrayNormal
		IHeap heap = context.getHeap();
		ClassArray arrayClass = (ClassArray) ((VMContext) context)
				.getClazzById(heap.getClass(args[0]));
		int len = heap.getArrayLength(args[0]);
		assert arrayClass instanceof ClassArray : "Type mismatch!";
		char type = arrayClass.getName().charAt(1);
		// Z B S C I J F D
		switch (type) {
		case 'Z': {
			boolean[] tmp = new boolean[len];
			heap.getArrayBoolean(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayBoolean(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'B': {
			byte[] tmp = new byte[len];
			heap.getArrayByte(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayByte(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'S': {
			short[] tmp = new short[len];
			heap.getArrayShort(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayShort(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'C': {
			char[] tmp = new char[len];
			heap.getArrayChar(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayChar(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'I': {
			int[] tmp = new int[len];
			heap.getArrayInt(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayInt(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'J': {
			long[] tmp = new long[len];
			heap.getArrayLong(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayLong(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'F': {
			float[] tmp = new float[len];
			heap.getArrayFloat(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayFloat(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		case 'D': {
			double[] tmp = new double[len];
			heap.getArrayDouble(tmp, 0, args[0], 0, len);
			for (int i = 0; i < len; i++) {
				assert heap.getArrayDouble(args[0], i) == tmp[i] : "Mismatch at "
						+ i;
			}
			break;
		}
		default: {
			assert false : "Illegal type " + type;
		}
		}
	}

	public String getUniqueName() {
		return "EXCLUDE/fisce/test/ArrayFillTest.getArrayTest.(Ljava/lang/Object;)V";
	}

}