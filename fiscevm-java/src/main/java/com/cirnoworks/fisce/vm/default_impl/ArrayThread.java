/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.vm.default_impl;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.cirnoworks.fisce.classloader.utils.DescriptorAnalyzer;
import com.cirnoworks.fisce.intf.VMCriticalException;
import com.cirnoworks.fisce.intf.VMException;
import com.cirnoworks.fisce.intf.idata.IMethod;
import com.cirnoworks.fisce.util.BufferUtil;
import com.cirnoworks.fisce.util.TypeUtil;
import com.cirnoworks.fisce.vm.JHeap;
import com.cirnoworks.fisce.vm.JThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.data.AbstractClass;
import com.cirnoworks.fisce.vm.data.ClassArray;
import com.cirnoworks.fisce.vm.data.ClassBase;
import com.cirnoworks.fisce.vm.data.ClassField;
import com.cirnoworks.fisce.vm.data.ClassMethod;
import com.cirnoworks.fisce.vm.data.StackTraceElement;
import com.cirnoworks.fisce.vm.data.attributes.Attribute;
import com.cirnoworks.fisce.vm.data.attributes.AttributeSourceFile;
import com.cirnoworks.fisce.vm.data.attributes.ExceptionHandler;
import com.cirnoworks.fisce.vm.data.attributes.LineNumber;
import com.cirnoworks.fisce.vm.data.constants.Constant;
import com.cirnoworks.fisce.vm.data.constants.ConstantClass;
import com.cirnoworks.fisce.vm.data.constants.ConstantDouble;
import com.cirnoworks.fisce.vm.data.constants.ConstantFieldRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantFloat;
import com.cirnoworks.fisce.vm.data.constants.ConstantInteger;
import com.cirnoworks.fisce.vm.data.constants.ConstantInterfaceMethodRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantLong;
import com.cirnoworks.fisce.vm.data.constants.ConstantMethodRef;
import com.cirnoworks.fisce.vm.data.constants.ConstantString;

/**
 * 
 * Default thread contains a space for frames. <br />
 * the stack have following content:<br />
 * u4 handle handle to the Thread object.<br />
 * u4 current throwable handle u4 status u4 fp pointer to the last-size pos of
 * the frame<br />
 * 
 * Each frame have following content:<br />
 * 
 * u1[] oprand-stack type <br />
 * u1[] local vars type <br />
 * u4[] oprand-stack<br />
 * u4[] local vars. size is from <br />
 * u4 pc PC (the pointer points )<br />
 * u4 lpc last pc<br />
 * u4 sr rel. pos of the oprand-stack<br />
 * u4 sb Base Address of the local vars.<br />
 * u4 sc count of the oprand-stack<br />
 * u4 lb Base Address of the local vars.<br />
 * u4 lc count of local vars.<br />
 * u4 mid method id of this frame<br />
 * u4 SIZE frame size in bytes(including this size itself)<br />
 * <------------fp point to here
 * 
 * 
 * @author cloudee
 * 
 */
public final class ArrayThread implements JThread {

	public static final int CMD_BREAK = 1;
	public static final int CMD_GOON = 2;
	public static final int CMD_BACK = 3;

	private static final int pThreadHandle = 0;
	private static final int pCurrentThrowable = 4;
	private static final int pStatus = 8;
	private static final int pPriority = 12;
	private static final int pThreadId = 16;
	// private final int pWaitForLock = 20;
	private static final int pFP = 24;
	private static final int dSIZE = 0;
	private static final int dMID = -4;
	private static final int dLC = -8;
	private static final int dLB = -12;
	private static final int dSC = -16;
	private static final int dSB = -20;
	private static final int dSR = -24;
	private static final int dLTB = -28;
	private static final int dSTB = -32;
	private static final int dLPC = -36;
	private static final int dPC = -40;
	private final VMContext context;
	private final ArrayThreadManager manager;
	private final JHeap heap;
	private byte[] code;
	private ClassMethod method;
	// private Frame currentFrame;
	private boolean yield;
	/*
	 * *****************
	 * Cache for current frame
	 */

	int pc;
	int lpc;
	int[] localVars;
	byte[] localVarTypes;
	int[] opStacks;
	byte[] opStackTypes;
	int sp;
	int methodId = -1;

	class TypeContainer {
		byte type;
	}

	TypeContainer tc = new TypeContainer();

	/*
	 * ******************
	 * Persist data
	 */
	private int threadHandle;
	private int currentThrowable;
	private int status;
	private int priority;
	private int threadId;

	private static class Frame {
		int pc;
		int lpc;
		int[] localVars;
		byte[] localVarTypes;
		int[] opStacks;
		byte[] opStackTypes;
		int sp;
		int methodId;

		int getSize() {
			return 44 + localVars.length * 4 + localVarTypes.length
					+ opStacks.length * 4 + opStackTypes.length;
		}
	}

	private final ArrayList<Frame> frames = new ArrayList<Frame>();

	public byte[] getFullStack() {
		saveFromCache();
		int size = 28;
		for (Frame frame : frames) {
			size += frame.getSize();
		}
		ByteBuffer stack = BufferUtil.createBuffer(size);
		stack.limit(size);

		stack.putInt(pThreadHandle, threadHandle);
		stack.putInt(pCurrentThrowable, currentThrowable);
		stack.putInt(pStatus, status);
		stack.putInt(pPriority, priority);
		stack.putInt(pThreadId, threadId);
		stack.putInt(pFP, size - 4);

		int fp = pFP;

		for (Frame frame : frames) {
			int ltsize = frame.localVarTypes.length;
			int stsize = frame.opStackTypes.length;
			int lsize = frame.localVars.length * 4;
			int ssize = frame.opStacks.length * 4;
			int fsize = -dPC + 4 + ssize + lsize + stsize + ltsize;

			fp += fsize;
			int lb = fp + dPC - lsize;
			int sb = lb - ssize;
			int ltb = sb - ltsize;
			int stb = ltb - stsize;

			stack.putInt(fp + dSIZE, fsize);
			stack.putInt(fp + dMID, frame.methodId);
			stack.putInt(fp + dLC, ltsize);
			stack.putInt(fp + dLB, lb);
			stack.putInt(fp + dSC, stsize);
			stack.putInt(fp + dSB, sb);
			stack.putInt(fp + dSTB, stb);
			stack.putInt(fp + dLTB, ltb);
			stack.putInt(fp + dPC, frame.pc);
			stack.putInt(fp + dLPC, frame.lpc);
			stack.putInt(fp + dSR, frame.sp);

			stack.position(stb);
			stack.put(frame.opStackTypes);

			stack.position(ltb);
			stack.put(frame.localVarTypes);

			stack.position(0);

			for (int i = 0, max = frame.opStacks.length; i < max; i++) {
				stack.putInt(sb + i * 4, frame.opStacks[i]);
			}

			for (int i = 0, max = frame.localVars.length; i < max; i++) {
				stack.putInt(lb + i * 4, frame.localVars[i]);
			}
		}

		byte[] ret = new byte[size];
		stack.get(ret);
		return ret;
	}

	public void createFromData(byte[] data) {
		ArrayList<Frame> frames = new ArrayList<Frame>();
		ByteBuffer stack = BufferUtil.createBuffer(data.length);
		stack.put(data);
		stack.position(0);
		threadHandle = stack.getInt(pThreadHandle);
		currentThrowable = stack.getInt(pCurrentThrowable);
		status = stack.getInt(pStatus);
		priority = stack.getInt(pPriority);
		threadId = stack.getInt(pThreadId);
		int fp = stack.getInt(pFP);
		while (fp > pFP) {
			Frame frame = new Frame();
			int size = stack.getInt(fp + dSIZE);
			int localVarCount = stack.getInt(fp + dLC);
			int opStackCount = stack.getInt(fp + dSC);
			frame.pc = stack.getInt(fp + dPC);
			frame.lpc = stack.getInt(fp + dLPC);
			frame.sp = stack.getInt(fp + dSR);
			frame.methodId = stack.getInt(fp + dMID);
			int ltb = stack.getInt(fp + dLTB);
			int stb = stack.getInt(fp + dSTB);
			int lb = stack.getInt(fp + dLB);
			int sb = stack.getInt(fp + dSB);
			byte[] localVarTypes = new byte[localVarCount];
			byte[] opStackTypes = new byte[opStackCount];
			int[] localVars = new int[localVarCount];
			int[] opStacks = new int[opStackCount];
			for (int i = 0; i < localVarCount; i++) {
				localVarTypes[i] = stack.get(ltb + i);
				localVars[i] = stack.getInt(lb + i * 4);
			}
			for (int i = 0, max = frame.sp; i < max; i++) {
				opStackTypes[i] = stack.get(stb + i);
				opStacks[i] = stack.getInt(sb + i * 4);
			}
			frame.localVarTypes = localVarTypes;
			frame.localVars = localVars;
			frame.opStacks = opStacks;
			frame.opStackTypes = opStackTypes;
			frames.add(frame);

			fp -= size;
		}

		for (int i = 0, max = frames.size(); i < max; i++) {
			this.frames.add(frames.get(max - i - 1));
		}
		loadToCache();
		updateLocalBuf();
	}

	public final int getThreadHandle() {
		return threadHandle;
	}

	private final void setThreadHandle(int handle) throws VMException {
		assert context.getClass(handle) != null;
		threadHandle = handle;
	}

	public final int getCurrentThrowable() {
		return currentThrowable;
	}

	public final void setCurrentThrowable(int handle)
			throws VMCriticalException {
		if (handle != 0) {

			try {
				AbstractClass clazz = (AbstractClass) context.getClass(handle);
				if (!context.getClass("java/lang/Throwable").isSuperClassOf(
						clazz)) {
					throw new VMException("java/lang/VirtualMachineError",
							"wrong object throws " + clazz.getName());
				}
			} catch (VMException e) {
				context.getConsole().error("", e);
				throw new VMCriticalException(e);
			}
		}
		currentThrowable = handle;
	}

	public final int getStatus() {
		return status;
	}

	public final int getPriority() {
		return priority;
	}

	public final void setPriority(int value) {
		priority = value;
	}

	public final int getThreadId() {
		return threadId;
	}

	public final void setThreadId(int value) {
		threadId = value;
	}

	public final Frame getCurrentFrame() {
		int size = frames.size();
		return size == 0 ? null : frames.get(size - 1);
	}

	public boolean isYield() {
		return yield;
	}

	public void setYield(boolean yield) {
		this.yield = yield;
	}

	public ArrayThread(VMContext context, ArrayThreadManager manager) {
		this.context = context;
		this.manager = manager;
		heap = (JHeap) context.getHeap();
	}

	public void create(int threadHandle, ClassMethod method)
			throws VMException, VMCriticalException {
		// The first method must have no return value.
		if (!method.getUniqueName().endsWith("main.([Ljava/lang/String;)V")) {
			throw new VMException("java/lang/IllegalArgumentException",
					"The create(int,ClassMethod) is used to start a static method!");
		}
		if (!AbstractClass.hasFlag(method.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/VirtualMachineError",
					"The first method of a thread must have no return value.");
		}
		setThreadHandle(threadHandle);
		pushFrame(method);
		putLocalHandle(0, heap.allocate(
				(ClassArray) context.getClass("[Ljava/lang/String;"), 0));
		clinit(method.getOwner());
	}

	public void create(int handle) throws VMException, VMCriticalException {
		AbstractClass clazz = (AbstractClass) context.getClass(handle);
		if (!clazz.canCastTo(context.getClass("java/lang/Thread"))) {
			throw new VMCriticalException(
					"The create(int) is used to start a java/lang/Thread!\n"
							+ clazz);
		}
		ClassBase runnerClass = (ClassBase) clazz;
		ClassMethod runner = context
				.lookupMethodVirtual(runnerClass, "run.()V");
		if (runner == null) {
			throw new VMException("java/lang/NoSuchMethodError",
					runnerClass.getName() + "." + ".run.()V");
		}
		setThreadHandle(handle);
		pushFrame(runner);
		// "this"
		putLocalHandle(0, handle);
	}

	// private void movePC(int ofs) {
	// this.pc += ofs;
	// }
	//
	// private void go(int ofs) {
	// this.pc = this.lpc + ofs;
	// }

	private void updateLocalBuf() {
		int size = frames.size();
		if (size == 0) {
			method = null;
			code = null;
			methodId = -1;
		} else {

			method = getCurrentMethod();
			code = method.getCode();
		}
	}

	private void loadToCache() {
		int size = frames.size();
		if (size == 0) {
			methodId = -1;
		} else {
			Frame currentFrame = frames.get(size - 1);
			pc = currentFrame.pc;
			lpc = currentFrame.lpc;
			localVars = currentFrame.localVars;
			localVarTypes = currentFrame.localVarTypes;
			opStacks = currentFrame.opStacks;
			opStackTypes = currentFrame.opStackTypes;
			sp = currentFrame.sp;
			methodId = currentFrame.methodId;
		}
	}

	private void saveFromCache() {
		int size = frames.size();
		if (size == 0) {
			methodId = -1;
		} else {
			Frame currentFrame = frames.get(size - 1);
			currentFrame.pc = pc;
			currentFrame.lpc = lpc;
			currentFrame.localVars = localVars;
			currentFrame.localVarTypes = localVarTypes;
			currentFrame.opStacks = opStacks;
			currentFrame.opStackTypes = opStackTypes;
			currentFrame.sp = sp;
			currentFrame.methodId = methodId;
		}
	}

	private String getFrameInfo() {
		return "STACK["
				+ this.sp
				+ "/"
				+ this.opStacks.length
				+ "] VARS=["
				+ this.localVars.length
				+ "] "
				+ (!AbstractClass.hasFlag(method.getAccessFlags(),
						AbstractClass.ACC_STATIC));
	}

	public void pushFrame(IMethod imt) {
		saveFromCache();
		ClassMethod mt = (ClassMethod) imt;
		Frame frame = new Frame();
		frame.localVars = new int[mt.getMaxLocals()];
		frame.localVarTypes = new byte[mt.getMaxLocals()];
		frame.opStacks = new int[mt.getMaxStack()];
		frame.opStackTypes = new byte[mt.getMaxStack()];
		frame.methodId = context.getMethodId(mt);
		frames.add(frame);
		loadToCache();
		updateLocalBuf();
		assert context.getConsole().debug(">>>Push frame to " + getFrameInfo());
		// switchIn();
	}

	private void popFrame() {
		// switchOut();
		assert context.getConsole()
				.debug(">>>Pop frame from " + getFrameInfo());
		frames.remove(frames.size() - 1);
		loadToCache();
		updateLocalBuf();
		if (methodId != -1) {
			assert context.getConsole().debug(
					">>>Pop frame to " + getFrameInfo());
		} else {
			assert context.getConsole().debug(">>>Pop frame prepare to exit!");
		}
	}

	public void pushMethod(IMethod imt, boolean isStatic, int argsCount,
			int[] args, byte[] types) {
		ClassMethod invoke = (ClassMethod) imt;
		pushFrame(invoke);
		for (int i = 0; i < argsCount; i++) {
			putLocalType(i, args[i], types[i]);
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_SYNCHRONIZED)) {
			monitorEnter(isStatic ? context.getClassObjectHandleForClass(invoke
					.getOwner()) : args[0]);
		}
	}

	// no throw VMException, all exceptions must be handled into inner
	// exception!
	public boolean run() throws VMCriticalException {
		return run(Integer.MAX_VALUE);
		// switchOut();
	}

	public boolean run(int ops) throws VMCriticalException {

		for (int i = 0; i < ops; i++) {
			if (methodId == -1) {
				// Time to quit!
				int th = getCurrentThrowable();
				if (th > 0) {
					// Still exception in this thread!
					assert context.getConsole().debug(
							"XXXXXXXXXXUnhandled Exception!!!XXXXXXXXXXXX");
					ClassMethod method = context
							.getMethod("java/lang/Throwable.printStackTrace.()V");
					assert method != null;
					pushFrame(method);
					putLocalHandle(0, th);
					setCurrentThrowable(0);
					continue;
				} else {
					// Thread dead
					return true;
				}
			}

			int th = getCurrentThrowable();
			if (th > 0) {
				processThrowable(th);
				// process next cmd.
				continue;
			}
			runOneInst();
			if (yield) {
				yield = false;
				return false;
			}
		}
		return false;
	}

	private void processThrowable(int th) throws VMCriticalException {
		assert context.getConsole().debug(
				"*EXCEPTION HANDLE LOOKUP: LPC=" + this.lpc);
		AbstractClass throwableClass;
		try {
			throwableClass = (ClassBase) context.getClass(th);
		} catch (VMException e) {
			context.getConsole().error(
					"Error in processing exception. Thread dead.", e);
			throw new VMCriticalException(e);
		}

		ExceptionHandler[] handlers = method.getExceptionTable();
		int lpc = this.lpc;
		int target = -1;
		for (int i = 0, max = handlers.length; i < max; i++) {
			ExceptionHandler eh = handlers[i];
			assert context.getConsole().debug("*" + eh);
			if (lpc >= eh.startPc && lpc < eh.endPc) {
				try {
					if (eh.catchClass == null) {
						target = eh.handlerPc;
						break;
					} else {
						AbstractClass handlerClass = eh.catchClass.getClazz();
						if (throwableClass.canCastTo(handlerClass)) {
							target = eh.handlerPc;
							break;
						}
					}
				} catch (VMException ex) {
					context.getConsole().error(
							"Critical error in processer thread!", ex);
					throw new VMCriticalException(ex);
				}
			}
		}
		if (target >= 0) {
			// can handle, clear stack and push exception and jump
			assert context.getConsole().debug("*FOUND JUMP=" + target);
			this.sp = 0;
			pushHandle(th);
			setCurrentThrowable(0);
			this.pc = target;
		} else {
			// throw to caller.
			assert context.getConsole().debug("NOT FOUND!");
			try {
				if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
					if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
						monitorExit(context.getClassObjectHandleForClass(method
								.getOwner()));
					} else {
						monitorExit(getLocalHandle(0));
					}
				}
			} catch (VMException ex) {
				context.getConsole().error(
						"Critical error in processer thread!", ex);
				throw new VMCriticalException(ex);
			}
			popFrame();
		}
	}

	int[] count = new int[256];
	long[] tcount = new long[256];
	long nextShowTime;
	private final boolean profile;
	{
		profile = false;
	}

	private void showCount() {
		TreeMap<Integer, ArrayList<Integer>> tm = new TreeMap<Integer, ArrayList<Integer>>();
		for (int i = 0; i < 256; i++) {
			int c = count[i];
			if (tm.containsKey(c)) {
				tm.get(c).add(i);
			} else {
				ArrayList<Integer> al = new ArrayList<Integer>();
				al.add(i);
				tm.put(c, al);
			}
		}
		System.out.println("\n\n===========================\n");
		int displayed = 0;
		for (Entry<Integer, ArrayList<Integer>> entry : tm.entrySet()) {
			int c = entry.getKey();
			for (int i : entry.getValue()) {
				if (displayed > 245 && c > 0) {
					System.out.println(OP_NAME[i] + ": " + c + " " + tcount[i]
							/ c / 1000000f + "ms");
				}
				displayed++;
			}
		}
	}

	/**
	 * 
	 * @return yield
	 * @throws VMCriticalException
	 */
	private void runOneInst() throws VMCriticalException {

		this.lpc = this.pc;
		int op = code[this.pc++] & 0xff;
		if (lpc == 0 && method.isClinit()) {

			ClassBase targetClass = method.getOwner().getSuperClass();
			boolean cl = clinit(targetClass);
			assert context.getConsole().debug(
					method.getUniqueName()
							+ " BEGIN_CLINIT LOOKUP SUPERCLASS="
							+ (targetClass == null ? null : (targetClass
									.getName())) + " " + cl);
			if (cl) {
				return;
			}
		}
		long time = 0;
		if (profile) {
			count[op]++;
			time = System.nanoTime();
			if (time > nextShowTime) {
				nextShowTime = time + 10000000000l;
				showCount();
			}
		}

		assert context.getConsole().debug(
				method.getUniqueName() + " " + this.lpc + " " + OP_NAME[op]
						+ " SR=" + this.pc);
		try {
			switch (op) {
			case AALOAD: {
				int index = popInt();
				int aref = popHandle();
				pushHandle(heap.getArrayHandle(aref, index));
				break;
			}
			case IALOAD: {
				int index = popInt();
				int aref = popHandle();
				pushInt(heap.getArrayInt(aref, index));
				break;
			}
			case AASTORE: {
				int value = popHandle();
				int index = popInt();
				int aref = popHandle();

				assert context.getConsole().debug(
						"AASTORE " + aref + "[" + index + "]=" + value);

				ClassArray ca = (ClassArray) context.getClass(aref);
				AbstractClass content = ca.getContentClass();
				if (value != 0
						&& !((AbstractClass) context.getClass(value))
								.canCastTo(content)) {
					throw new VMException("java/lang/ArrayStoreException",
							"Data type not compatable!");
				}

				heap.putArrayHandle(aref, index, value);
				break;
			}
			case IASTORE: {
				int value = popInt();
				int index = popInt();
				int aref = popHandle();
				heap.putArrayInt(aref, index, value);
				break;
			}
			case ACONST_NULL: {
				pushHandle(0);
				break;
			}

			case ILOAD:
			case FLOAD: {
				int index = code[this.pc++] & 0xff;
				int value = getLocalInt(index);
				pushInt(value);
				break;
			}
			case ALOAD: {
				int index = code[this.pc++] & 0xff;
				int value = getLocalHandle(index);
				pushHandle(value);
				break;
			}
			case ILOAD_0:
			case FLOAD_0: {
				pushInt(getLocalInt(0));
				break;
			}
			case ALOAD_0: {
				pushHandle(getLocalHandle(0));
				break;
			}
			case ILOAD_1:
			case FLOAD_1: {
				pushInt(getLocalInt(1));
				break;
			}
			case ALOAD_1: {
				pushHandle(getLocalHandle(1));
				break;
			}
			case ILOAD_2:
			case FLOAD_2: {
				pushInt(getLocalInt(2));
				break;
			}
			case ALOAD_2: {
				pushHandle(getLocalHandle(2));
				break;
			}
			case ILOAD_3:
			case FLOAD_3: {
				pushInt(getLocalInt(3));
				break;
			}
			case ALOAD_3: {
				pushHandle(getLocalHandle(3));
				break;
			}
			case ANEWARRAY: {
				opANewArray();
				break;
			}
			case IRETURN:
			case FRETURN: {
				opIFReturn();
				break;
			}
			case ARETURN: {
				opAReturn();
				break;
			}
			case ARRAYLENGTH: {
				int aref = popHandle();
				pushInt(heap.getArrayLength(aref));
				break;
			}
			case ISTORE:
			case FSTORE: {
				int index = code[this.pc++] & 0xff;
				int value = popInt();

				putLocalInt(index, value);
				break;
			}
			case ASTORE: {
				int index = code[this.pc++] & 0xff;
				int aref = popType(tc);
				switch (tc.type) {
				case DescriptorAnalyzer.TYPE_HANDLE:
					putLocalHandle(index, aref);
					break;
				case DescriptorAnalyzer.TYPE_RETURN:
					putLocalReturn(index, aref);
					break;
				default:
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!" + (char) tc.type);
				}
				break;
			}
			case ISTORE_0:
			case FSTORE_0: {
				int value = popInt();
				putLocalInt(0, value);
				break;
			}
			case ASTORE_0: {
				int aref = popType(tc);
				switch (tc.type) {
				case DescriptorAnalyzer.TYPE_HANDLE:
					putLocalHandle(0, aref);
					break;
				case DescriptorAnalyzer.TYPE_RETURN:
					putLocalReturn(0, aref);
					break;
				default:
					assert false;
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!");
				}
				break;
			}
			case ISTORE_1:
			case FSTORE_1: {
				int value = popInt();
				putLocalInt(1, value);
				break;
			}
			case ASTORE_1: {
				int aref = popType(tc);
				switch (tc.type) {
				case DescriptorAnalyzer.TYPE_HANDLE:
					putLocalHandle(1, aref);
					break;
				case DescriptorAnalyzer.TYPE_RETURN:
					putLocalReturn(1, aref);
					break;
				default:
					assert false;
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!");
				}
				break;
			}
			case ISTORE_2:
			case FSTORE_2: {
				int value = popInt();
				putLocalInt(2, value);
				break;
			}
			case ASTORE_2: {
				int aref = popType(tc);
				switch (tc.type) {
				case DescriptorAnalyzer.TYPE_HANDLE:
					putLocalHandle(2, aref);
					break;
				case DescriptorAnalyzer.TYPE_RETURN:
					putLocalReturn(2, aref);
					break;
				default:
					assert false;
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!");
				}
				break;
			}
			case ISTORE_3:
			case FSTORE_3: {
				int value = popInt();
				putLocalInt(3, value);
				break;
			}
			case ASTORE_3: {
				int aref = popType(tc);
				switch (tc.type) {
				case DescriptorAnalyzer.TYPE_HANDLE:
					putLocalHandle(3, aref);
					break;
				case DescriptorAnalyzer.TYPE_RETURN:
					putLocalReturn(3, aref);
					break;
				default:
					assert false;
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!");

				}
				break;
			}
			case ATHROW: {
				int handle = popHandle();
				setCurrentThrowable(handle);
				break;
			}
			case BALOAD: {
				int index = popInt();
				int aref = popHandle();
				pushInt(heap.getArrayByte(aref, index));
				break;
			}
			case BASTORE: {
				int value = popInt();
				int index = popInt();
				int aref = popHandle();
				heap.putArrayByte(aref, index, (byte) value);
				break;
			}
			case BIPUSH: {
				int value = code[this.pc++];
				pushInt(value);
				break;
			}
			case FALOAD:
			case CALOAD: {
				int index = popInt();
				int aref = popHandle();
				pushInt(heap.getArrayInt(aref, index));
				break;
			}
			case FASTORE:
			case CASTORE: {
				int value = popInt();
				int index = popInt();
				int aref = popHandle();
				heap.putArrayInt(aref, index, value);
				break;
			}
			case CHECKCAST: {
				byte ib1 = code[this.pc++];
				byte ib2 = code[this.pc++];
				int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
				int handle = popHandle();
				if (handle == 0) {
					pushHandle(handle);
					break;
				}
				AbstractClass clazzS = (AbstractClass) context.getClass(handle);
				AbstractClass clazzT = getClassFromConstant(m);
				if (clazzS.canCastTo(clazzT)) {
					pushHandle(handle);
				} else {
					throw new VMException("java/lang/ClassCastException",
							" from " + clazzS.getName() + " to "
									+ clazzT.getName());
				}
				break;
			}
			case D2F: {
				pushFloat((float) popDouble());
				break;
			}
			case D2I: {
				pushInt((int) popDouble());
				break;
			}
			case D2L: {
				pushLong((long) popDouble());
				break;
			}
			case DADD: {
				pushDouble(popDouble() + popDouble());
				break;
			}
			case DALOAD: {
				int index = popInt();
				int handle = popHandle();
				pushDouble(heap.getArrayDouble(handle, index));
				break;
			}
			case DASTORE: {
				double value = popDouble();
				int index = popInt();
				int handle = popHandle();
				heap.putArrayDouble(handle, index, value);
				break;
			}
			case DCMPG: {
				double value2 = popDouble();
				double value1 = popDouble();
				if (value1 == Double.NaN || value2 == Double.NaN) {
					pushInt(1);
				} else {
					pushInt((int) Math.signum(value1 - value2));
				}
				break;
			}
			case DCMPL: {
				double value2 = popDouble();
				double value1 = popDouble();
				if (value1 == Double.NaN || value2 == Double.NaN) {
					pushInt(-1);
				} else {
					pushInt((int) Math.signum(value1 - value2));
				}
				break;
			}
			case DCONST_0: {
				pushDouble(0);
				break;
			}
			case DCONST_1: {
				pushDouble(1);
				break;
			}
			case DDIV: {
				double value2 = popDouble();
				double value1 = popDouble();
				pushDouble(value1 / value2);
				break;
			}
			case DLOAD: {
				int index = code[this.pc++] & 0xff;
				pushDouble(getLocalDouble(index));
				break;
			}
			case DLOAD_0: {
				pushDouble(getLocalDouble(0));
				break;
			}
			case DLOAD_1: {
				pushDouble(getLocalDouble(1));
				break;
			}
			case DLOAD_2: {
				pushDouble(getLocalDouble(2));
				break;
			}
			case DLOAD_3: {
				pushDouble(getLocalDouble(3));
				break;
			}
			case DMUL: {
				double value2 = popDouble();
				double value1 = popDouble();
				pushDouble(value1 * value2);
				break;
			}
			case DNEG: {
				pushDouble(-popDouble());
				break;
			}
			case DREM: {
				double value2 = popDouble();
				double value1 = popDouble();
				pushDouble(value1 % value2);
				break;
			}
			case DRETURN: {
				double value = popDouble();
				if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
					if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
						monitorExit(context.getClassObjectHandleForClass(method
								.getOwner()));
					} else {
						monitorExit(getLocalHandle(0));
					}
				}
				popFrame();
				pushDouble(value);
				break;
			}
			case DSTORE: {
				int index = code[this.pc++] & 0xff;

				double value = popDouble();
				putLocalDouble(index, value);
				break;
			}
			case DSTORE_0: {
				double value = popDouble();
				putLocalDouble(0, value);
				break;
			}
			case DSTORE_1: {
				double value = popDouble();
				putLocalDouble(1, value);
				break;
			}
			case DSTORE_2: {
				double value = popDouble();
				putLocalDouble(2, value);
				break;
			}
			case DSTORE_3: {
				double value = popDouble();
				putLocalDouble(3, value);
				break;
			}
			case DSUB: {
				double value2 = popDouble();
				double value1 = popDouble();
				pushDouble(value1 - value2);
				break;
			}
			case DUP: {
				int value = popType(tc);
				pushType(value, tc.type);
				pushType(value, tc.type);
				break;
			}
			case DUP_X1: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				pushType(value1, type1);
				pushType(value2, type2);
				pushType(value1, type1);
				break;
			}
			case DUP_X2: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				int value3 = popType(tc);
				byte type3 = tc.type;
				pushType(value1, type1);
				pushType(value3, type3);
				pushType(value2, type2);
				pushType(value1, type1);
				break;
			}
			case DUP2: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				pushType(value2, type2);
				pushType(value1, type1);
				pushType(value2, type2);
				pushType(value1, type1);
				break;
			}
			case DUP2_X1: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				int value3 = popType(tc);
				byte type3 = tc.type;
				pushType(value2, type2);
				pushType(value1, type1);
				pushType(value3, type3);
				pushType(value2, type2);
				pushType(value1, type1);
				break;
			}
			case DUP2_X2: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				int value3 = popType(tc);
				byte type3 = tc.type;
				int value4 = popType(tc);
				byte type4 = tc.type;
				pushType(value2, type2);
				pushType(value1, type1);
				pushType(value4, type4);
				pushType(value3, type3);
				pushType(value2, type2);
				pushType(value1, type1);
				break;
			}
			case F2D: {
				pushDouble(popFloat());
				break;
			}
			case F2I: {
				pushInt((int) popFloat());
				break;
			}
			case F2L: {
				pushLong((long) popFloat());
				break;
			}
			case FADD: {
				pushFloat(popFloat() + popFloat());
				break;
			}
			case FCMPG: {
				double value2 = popFloat();
				double value1 = popFloat();
				if (value1 == Float.NaN || value2 == Float.NaN) {
					pushInt(1);
				} else {
					pushInt((int) Math.signum(value1 - value2));
				}
				break;
			}
			case FCMPL: {
				double value2 = popFloat();
				double value1 = popFloat();
				if (value1 == Float.NaN || value2 == Float.NaN) {
					pushInt(-1);
				} else {
					pushInt((int) Math.signum(value1 - value2));
				}
				break;
			}
			case FCONST_0: {
				pushFloat(0f);
				break;
			}
			case FCONST_1: {
				pushFloat(1f);
				break;
			}
			case FCONST_2: {
				pushFloat(2f);
				break;
			}
			case FDIV: {
				float value2 = popFloat();
				pushFloat(popFloat() / value2);
				break;
			}
			case FMUL: {
				pushFloat(popFloat() * popFloat());
				break;
			}
			case FNEG: {
				pushFloat(-popFloat());
				break;
			}
			case FREM: {
				float value2 = popFloat();
				float value1 = popFloat();
				pushFloat(value1 % value2);
				break;
			}
			case FSUB: {
				float value2 = popFloat();
				float value1 = popFloat();
				pushFloat(value1 - value2);
				break;
			}
			case GETFIELD: {
				opGetField();
				break;
			}
			case GETSTATIC: {
				opGetStatic();
				break;
			}
			case GOTO: {
				byte ib1 = code[this.pc++];
				byte ib2 = code[this.pc++];
				this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				break;
			}
			case GOTO_W: {
				byte ib1 = code[this.pc++];
				byte ib2 = code[this.pc++];
				byte ib3 = code[this.pc++];
				byte ib4 = code[this.pc++];
				this.pc = this.lpc
						+ TypeUtil.bytesToSignedInt(ib1, ib2, ib3, ib4);
				break;
			}
			case I2B: {
				pushInt((byte) popInt());
				break;
			}
			case I2C: {
				pushInt((char) popInt());
				break;
			}
			case I2D: {
				pushDouble((double) popInt());
				break;
			}
			case I2F: {
				pushFloat((float) popInt());
				break;
			}
			case I2L: {
				pushLong((long) popInt());
				break;
			}
			case I2S: {
				pushInt((short) popInt());
				break;
			}
			case IADD: {
				opIAdd();
				break;
			}
			case IAND: {
				pushInt(popInt() & popInt());
				break;
			}
			case ICONST_M1: {
				pushInt(-1);
				break;
			}
			case ICONST_0: {
				pushInt(0);
				break;
			}
			case ICONST_1: {
				pushInt(1);
				break;
			}
			case ICONST_2: {
				pushInt(2);
				break;
			}
			case ICONST_3: {
				pushInt(3);
				break;
			}
			case ICONST_4: {
				pushInt(4);
				break;
			}
			case ICONST_5: {
				pushInt(5);
				break;
			}
			case IDIV: {
				int value = popInt();
				if (value == 0) {
					throw new VMException("java/lang/ArithmeticException",
							"Divided by zero!");
				}
				pushInt(popInt() / value);
				break;
			}
			case IF_ICMPEQ: {

				int value2 = popInt();
				int value1 = popInt();
				if (value1 == value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ACMPEQ: {
				int value2 = popHandle();
				int value1 = popHandle();
				if (value1 == value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ICMPNE: {
				int value2 = popInt();
				int value1 = popInt();
				if (value1 != value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ACMPNE: {
				int value2 = popHandle();
				int value1 = popHandle();
				if (value1 != value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ICMPLT: {
				int value2 = popInt();
				int value1 = popInt();
				if (value1 < value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ICMPLE: {
				int value2 = popInt();
				int value1 = popInt();
				if (value1 <= value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ICMPGT: {
				int value2 = popInt();
				int value1 = popInt();
				if (value1 > value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IF_ICMPGE: {
				int value2 = popInt();
				int value1 = popInt();
				if (value1 >= value2) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}

			case IFEQ: {
				int value = popInt();
				if (value == 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IFNULL: {
				int value = popHandle();
				if (value == 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}

			case IFNE: {
				int value = popInt();
				if (value != 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IFNONNULL: {
				int value = popHandle();
				if (value != 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}

			case IFLT: {
				int value = popInt();
				if (value < 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IFLE: {
				int value = popInt();
				if (value <= 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IFGT: {
				int value = popInt();
				if (value > 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IFGE: {
				int value = popInt();
				if (value >= 0) {
					byte ib1 = code[this.pc++];
					byte ib2 = code[this.pc++];
					this.pc = this.lpc + TypeUtil.bytesToSignedInt(ib1, ib2);
				} else {
					this.pc += 2;
				}
				break;
			}
			case IINC: {
				opIINC();
				break;
			}
			case IMUL: {
				pushInt(popInt() * popInt());
				break;
			}
			case INEG: {
				pushInt(-popInt());
				break;
			}
			case INSTANCEOF: {
				byte ib1 = code[this.pc++];
				byte ib2 = code[this.pc++];
				int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
				int handle = popHandle();
				AbstractClass clazz = (AbstractClass) context.getClass(handle);
				AbstractClass castto = getClassFromConstant(m);
				pushInt(clazz.canCastTo(castto) ? 1 : 0);
				break;
			}
			case INVOKEINTERFACE: {
				opInvokeInterface();
				break;
			}
			case INVOKESPECIAL: {
				opInvokeSpecial();
				break;
			}
			case INVOKESTATIC: {
				opInvokeStatic();
				break;
			}
			case INVOKEVIRTUAL: {
				opInvokeVirtual();
				break;
			}
			case IOR: {
				pushInt(popInt() | popInt());
				break;
			}
			case IREM: {
				int value2 = popInt();
				int value1 = popInt();
				if (value2 == 0) {
					throw new VMException("java/lang/ArithmeticException",
							"Divided by zero!");
				}
				pushInt(value1 % value2);
				break;
			}
			case ISHL: {
				int value2 = popInt();
				int value1 = popInt();
				pushInt(value1 << value2);
				break;
			}
			case ISHR: {
				int value2 = popInt();
				int value1 = popInt();
				pushInt(value1 >> value2);
				break;
			}
			case ISUB: {
				int value2 = popInt();
				int value1 = popInt();
				pushInt(value1 - value2);
				break;
			}
			case IUSHR: {
				int value2 = popInt();
				int value1 = popInt();
				pushInt(value1 >>> value2);
				break;
			}
			case IXOR: {
				int value2 = popInt();
				int value1 = popInt();
				pushInt(value1 ^ value2);
				break;
			}
			case JSR: {
				byte bb1 = code[this.pc++];
				byte bb2 = code[this.pc++];
				int target = TypeUtil.bytesToSignedInt(bb1, bb2);
				pushType(this.pc, DescriptorAnalyzer.TYPE_RETURN);
				this.pc = this.lpc + target;
				break;
			}
			case JSR_W: {
				byte bb1 = code[this.pc++];
				byte bb2 = code[this.pc++];
				byte bb3 = code[this.pc++];
				byte bb4 = code[this.pc++];
				int target = TypeUtil.bytesToSignedInt(bb1, bb2, bb3, bb4);
				pushType(this.pc, DescriptorAnalyzer.TYPE_RETURN);
				this.pc = this.lpc + target;
				break;
			}
			case L2D: {
				pushDouble(popLong());
				break;
			}
			case L2F: {
				pushFloat(popLong());
				break;
			}
			case L2I: {
				pushInt((int) popLong());
				break;
			}
			case LADD: {
				pushLong(popLong() + popLong());
				break;
			}
			case LALOAD: {
				int index = popInt();
				int handle = popHandle();
				pushLong(heap.getArrayLong(handle, index));
				break;
			}
			case LAND: {
				pushLong(popLong() & popLong());
				break;
			}
			case LASTORE: {
				long value = popLong();
				int index = popInt();
				int handle = popHandle();
				heap.putArrayLong(handle, index, value);
				break;
			}
			case LCMP: {
				long value2 = popLong();
				long value1 = popLong();
				pushInt(value1 == value2 ? 0 : (value1 > value2 ? 1 : -1));
				break;
			}
			case LCONST_0: {
				pushLong(0);
				break;
			}
			case LCONST_1: {
				pushLong(1);
				break;
			}
			case LDC: {
				opLDC();
				break;
			}
			case LDC_W: {
				opLDCW();
				break;
			}
			case LDC2_W: {
				opLDC2W();
				break;
			}
			case LDIV: {
				long value2 = popLong();
				long value1 = popLong();
				if (value2 == 0) {
					throw new VMException("java/lang/ArithmeticException",
							"Divided by zero!");
				}
				pushLong(value1 / value2);
				break;
			}
			case LLOAD: {
				int index = code[this.pc++] & 0xff;
				pushLong(getLocalLong(index));
				break;
			}
			case LLOAD_0: {
				pushLong(getLocalLong(0));
				break;
			}
			case LLOAD_1: {
				pushLong(getLocalLong(1));
				break;
			}
			case LLOAD_2: {
				pushLong(getLocalLong(2));
				break;
			}
			case LLOAD_3: {
				pushLong(getLocalLong(3));
				break;
			}
			case LMUL: {
				pushLong(popLong() * popLong());
				break;
			}
			case LNEG: {
				pushLong(-popLong());
				break;
			}
			case LOOKUPSWITCH: {
				opLookupSwitch();
				break;
			}
			case LOR: {
				pushLong(popLong() | popLong());
				break;
			}
			case LREM: {
				long value2 = popLong();
				long value1 = popLong();
				if (value2 == 0) {
					throw new VMException("java/lang/ArithmeticException",
							"Divided by zero!");
				}
				pushLong(value1 % value2);
				break;
			}
			case LRETURN: {
				opLReturn();

				break;
			}
			case LSHL: {
				long value2 = popInt() & 0x3f;
				long value1 = popLong();
				pushLong(value1 << value2);
				break;
			}
			case LSHR: {
				long value2 = popInt() & 0x3f;
				long value1 = popLong();
				pushLong(value1 >> value2);
				break;
			}
			case LSTORE: {
				int index = code[this.pc++] & 0xff;
				putLocalLong(index, popLong());
				break;
			}
			case LSTORE_0: {
				putLocalLong(0, popLong());
				break;
			}
			case LSTORE_1: {
				putLocalLong(1, popLong());
				break;
			}
			case LSTORE_2: {
				putLocalLong(2, popLong());
				break;
			}
			case LSTORE_3: {
				putLocalLong(3, popLong());
				break;
			}
			case LSUB: {
				long value2 = popLong();
				long value1 = popLong();
				pushLong(value1 - value2);
				break;
			}
			case LUSHR: {
				long value2 = popInt() & 0x3f;
				long value1 = popLong();
				pushLong(value1 >>> value2);
				break;
			}
			case LXOR: {
				long value2 = popLong();
				long value1 = popLong();
				pushLong(value1 ^ value2);
				break;
			}
			case MONITORENTER: {
				int handle = popHandle();
				monitorEnter(handle);
				break;
			}
			case MONITOREXIT: {
				int handle = popHandle();
				monitorExit(handle);
				break;
			}
			case MULTIANEWARRAY: {
				byte ib1 = code[this.pc++];
				byte ib2 = code[this.pc++];
				int dims = code[this.pc++] & 0xff;
				int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
				ClassArray clazz = (ClassArray) getClassFromConstant(m);
				int[] count = new int[dims];
				for (int i = dims - 1; i >= 0; i--) {
					count[i] = popInt();
				}
				pushHandle(mulitANewArray(clazz.getName(), dims, count));
				break;
			}
			case NEW: {
				opNew();
				break;
			}
			case NEWARRAY: {
				opNewArray();
				break;
			}
			case NOP: {
				break;
			}
			case POP: {
				popType(tc);
				break;
			}
			case POP2: {
				popType(tc);
				popType(tc);
				break;
			}
			case PUTFIELD: {
				opPutField();
				break;
			}
			case PUTSTATIC: {
				opPutStatic();

				break;
			}
			case RET: {
				int index = code[this.pc++] & 0xff;
				int addr = getLocalReturn(index);
				this.pc = addr;
				break;
			}
			case RETURN: {
				opReturn();
				break;
			}
			case SALOAD: {
				int index = popInt();
				int handle = popHandle();
				pushInt(heap.getArrayShort(handle, index));
				break;
			}
			case SASTORE: {
				int value = popInt();
				int index = popInt();
				int handle = popHandle();
				heap.putArrayShort(handle, index, (short) value);
				break;
			}
			case SIPUSH: {
				byte bb1 = code[this.pc++];
				byte bb2 = code[this.pc++];
				int value = TypeUtil.bytesToSignedInt(bb1, bb2);
				pushInt(value);
				break;
			}
			case SWAP: {
				opSwap();
				break;
			}
			case TABLESWITCH: {
				opTableSwitch();
				break;
			}
			case WIDE: {
				opWide();
				break;
			}
			}

		} catch (VMException e) {
			// e.printStackTrace();
			assert context.getConsole().debug("Exception thrown", e);
			try {
				int handle = context.getThrower().prepareThrowable(e, this);
				if (handle == 0) {
					throw new VMException("java/lang/NullPointerException",
							"Can't get throwable handle!");
				}
				setCurrentThrowable(handle);
			} catch (VMException ex) {
				context.getConsole().error(
						"Error when posting error message to VM!", e);
				throw new VMCriticalException(
						"Exception thrown in exception processer!", ex);
			}
		} catch (RuntimeException e2) {
			throw new VMCriticalException("Unhandled exception in VM!", e2);
		} finally {
			if (profile) {
				long time1 = System.nanoTime();
				tcount[op] += time1 - time;
			}
		}
		return;
	}

	public void opIINC() {
		int index = code[this.pc++] & 0xff;
		int value = code[this.pc++];
		this.localVars[index] += value;
	}

	private void opLDC2W() throws VMException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int index = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		Constant con = getConstant(index);
		if (con instanceof ConstantLong) {
			pushLong(((ConstantLong) con).getData());
		} else if (con instanceof ConstantDouble) {
			pushDouble(((ConstantDouble) con).getData());
		} else {
			throw new VMException("java/lang/VirtualMachineError",
					"LDC2_W type wrong!" + con);
		}
	}

	private void opLDCW() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int index = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		Constant con = getConstant(index);
		if (con instanceof ConstantInteger) {
			pushInt(((ConstantInteger) con).getData());
		} else if (con instanceof ConstantFloat) {
			pushFloat(((ConstantFloat) con).getData());
		} else if (con instanceof ConstantString) {
			pushHandle(((ConstantString) con).getHandle());
		} else if (con instanceof ConstantClass) {
			pushHandle(context
					.getClassObjectHandleForClass(((ConstantClass) con)
							.getClazz()));
		} else {
			throw new VMException("java/lang/VirtualMachineError",
					"LDCW type wrong!" + con);
		}
	}

	private void opLDC() throws VMException, VMCriticalException {
		char index = (char) (code[this.pc++] & 0xff);
		Constant con = getConstant(index);
		if (con instanceof ConstantInteger) {
			pushInt(((ConstantInteger) con).getData());
		} else if (con instanceof ConstantFloat) {
			pushFloat(((ConstantFloat) con).getData());
		} else if (con instanceof ConstantString) {
			pushHandle(((ConstantString) con).getHandle());
		} else if (con instanceof ConstantClass) {
			pushHandle(context
					.getClassObjectHandleForClass(((ConstantClass) con)
							.getClazz()));
		} else {
			throw new VMException("java/lang/VirtualMachineError",
					"LDC type wrong!" + con);
		}
	}

	private void opIAdd() {
		sp--;
		opStacks[sp - 1] += opStacks[sp];
		// pushInt(popInt() + popInt());
	}

	private void opAReturn() throws VMException, VMCriticalException {
		int aref = popHandle();
		if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
			if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
				monitorExit(context.getClassObjectHandleForClass(method
						.getOwner()));
			} else {
				monitorExit(getLocalHandle(0));
			}
		}
		popFrame();
		pushHandle(aref);
	}

	private void opIFReturn() throws VMException, VMCriticalException {
		int value = popInt();
		if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
			if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
				monitorExit(context.getClassObjectHandleForClass(method
						.getOwner()));
			} else {
				monitorExit(getLocalHandle(0));
			}
		}
		popFrame();
		pushInt(value);
	}

	private void opLReturn() throws VMException, VMCriticalException {
		long value = popLong();
		if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
			if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
				monitorExit(context.getClassObjectHandleForClass(method
						.getOwner()));
			} else {
				monitorExit(getLocalHandle(0));
			}
		}
		popFrame();
		pushLong(value);
	}

	private void opReturn() throws VMException, VMCriticalException {
		if ((method.getAccessFlags() & AbstractClass.ACC_SYNCHRONIZED) > 0) {
			if ((method.getAccessFlags() & AbstractClass.ACC_STATIC) > 0) {
				monitorExit(context.getClassObjectHandleForClass(method
						.getOwner()));
			} else {
				monitorExit(getLocalHandle(0));
			}
		}
		if (method.isClinit()) {
			context.finishClinited(method.getOwner());
		}
		popFrame();
	}

	private void opSwap() throws VMException, VMCriticalException {
		int value1 = popType(tc);
		byte type1 = tc.type;
		int value2 = popType(tc);
		byte type2 = tc.type;
		pushType(value1, type1);
		pushType(value2, type2);
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opGetField() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		int handle = popHandle();
		ClassField field = ((ConstantFieldRef) method.getOwner()
				.getConstantPool()[m]).getTargetField();
		if (AbstractClass.hasFlag(field.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"field " + field.getUniqueName() + " is static");
		}
		char type = field.getDescriptor().charAt(0);
		switch (type) {
		case 'D':
		case 'J':
			pushLong(heap.getFieldLong(handle, field));
			break;
		case 'L':
		case '[':
			pushHandle(heap.getFieldInt(handle, field));
			break;
		default:
			pushInt(heap.getFieldInt(handle, field));
			break;
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opGetStatic() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ConstantFieldRef cfr = (ConstantFieldRef) method.getOwner()
				.getConstantPool()[m];
		ClassBase targetClass = (ClassBase) cfr.getClazz().getClazz();
		if (clinit(targetClass)) {
			return;
		}

		ClassField field = cfr.getTargetField();
		char type = field.getDescriptor().charAt(0);
		switch (type) {
		case 'D':
		case 'J':
			pushLong(heap.getStaticLong(field));
			break;
		case 'L':
		case '[':
			pushHandle(heap.getStaticInt(field));
			break;
		default:
			pushInt(heap.getStaticInt(field));
			break;
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opInvokeInterface() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int count = code[this.pc++] & 0xff;
		this.pc++;
		int[] args = new int[count];
		byte[] types = new byte[count];
		for (int i = count - 1; i >= 0; i--) {
			args[i] = popType(tc);
			types[i] = tc.type;
		}
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		AbstractClass clazz = (AbstractClass) context.getClass(args[0]);
		ClassMethod lookup = getInterfaceMethodFromConstant(m);
		if (!clazz.canCastTo(lookup.getOwner())) {
			throw new VMException("java/lang/IncompatibleClassChangeError", "");
		}
		ClassMethod invoke = context.lookupMethodVirtual(
				(AbstractClass) context.getClass(args[0]), lookup.getMethodName());
		if (invoke == null) {
			throw new VMException("java/lang/AbstractMethodError", "");
		}
		if (!AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_PUBLIC)) {
			throw new VMException("java/lang/IllegalAccessError", "");
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_ABSTRACT)) {
			throw new VMException("java/lang/AbstractMethodError", "");
		}
		if ((invoke.getAccessFlags() & AbstractClass.ACC_NATIVE) > 0) {
			invoke.getNativeHandler().dealNative(args, this);
		} else {
			pushMethod(invoke, false, count, args, types);
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opInvokeStatic() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ClassMethod invoke = getMethodFromConstant(m);
		if (!AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError", "");
		}
		ClassBase targetClass = invoke.getOwner();
		if (clinit(targetClass)) {
			return;
		}
		int count = invoke.getParamCount();
		int[] args = new int[count];
		byte[] types = new byte[count];
		for (int i = count - 1; i >= 0; i--) {
			args[i] = popType(tc);
			types[i] = tc.type;
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_NATIVE)) {
			invoke.getNativeHandler().dealNative(args, this);
		} else {
			pushMethod(invoke, true, count, args, types);
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opInvokeVirtual() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ClassMethod lookup = getMethodFromConstant(m);

		int count = lookup.getParamCount() + 1;
		int[] args = new int[count];
		byte[] types = new byte[count];
		for (int i = count - 1; i >= 0; i--) {
			args[i] = popType(tc);
			types[i] = tc.type;
		}
		assert ((JHeap) heap).isHandleValid(args[0]) : "invalid 'this' handle";
		ClassMethod invoke = context.lookupMethodVirtual(
				(AbstractClass) context.getClass(args[0]), lookup.getMethodName());
		if (invoke == null) {
			throw new VMException("java/lang/AbstractMethodError", "");
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					invoke.getUniqueName());
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					invoke.getUniqueName());
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_ABSTRACT)) {
			throw new VMException("java/lang/AbstractMethodError",
					invoke.getUniqueName());
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_NATIVE)) {
			invoke.getNativeHandler().dealNative(args, this);
		} else {
			pushMethod(invoke, false, count, args, types);
		}
	}

	/**
	 * 
	 */
	private void opLookupSwitch() {
		int padSize = (65536 - this.pc) % 4;
		this.pc += padSize;
		byte db1 = code[this.pc++];
		byte db2 = code[this.pc++];
		byte db3 = code[this.pc++];
		byte db4 = code[this.pc++];
		int db = TypeUtil.bytesToSignedInt(db1, db2, db3, db4);
		byte np1 = code[this.pc++];
		byte np2 = code[this.pc++];
		byte np3 = code[this.pc++];
		byte np4 = code[this.pc++];
		int np = TypeUtil.bytesToSignedInt(np1, np2, np3, np4);
		int[] match = new int[np];
		int[] offset = new int[np];
		for (int i = 0; i < np; i++) {
			byte ma1 = code[this.pc++];
			byte ma2 = code[this.pc++];
			byte ma3 = code[this.pc++];
			byte ma4 = code[this.pc++];
			byte of1 = code[this.pc++];
			byte of2 = code[this.pc++];
			byte of3 = code[this.pc++];
			byte of4 = code[this.pc++];
			match[i] = TypeUtil.bytesToSignedInt(ma1, ma2, ma3, ma4);
			offset[i] = TypeUtil.bytesToSignedInt(of1, of2, of3, of4);
		}
		int key = popInt();
		boolean matched = false;
		for (int i = 0; i < np; i++) {
			if (key == match[i]) {
				this.pc = this.lpc + offset[i];
				matched = true;
				break;
			}
		}
		if (!matched) {
			this.pc = this.lpc + db;
		}
	}

	/**
	 * @throws VMCriticalException
	 * 
	 */
	private void opNew() throws VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int idx = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		try {
			ClassBase targetClass = (ClassBase) getClassFromConstant(idx);
			if (AbstractClass.hasFlag(targetClass.getAccessFlags(),
					AbstractClass.ACC_INTERFACE | AbstractClass.ACC_ABSTRACT)) {
				throw new VMException("java/lang/InstantiationError",
						targetClass.getName());
			}
			if (clinit(targetClass)) {
				return;
			}
			pushHandle(heap.allocate(targetClass));
		} catch (VMException e) {
			throw new VMCriticalException(e);
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opNewArray() throws VMException, VMCriticalException {
		byte atype = code[this.pc++];
		String name;
		switch (atype) {
		case 4:
			name = "[Z";
			break;
		case 5:
			name = "[C";
			break;
		case 6:
			name = "[F";
			break;
		case 7:
			name = "[D";
			break;
		case 8:
			name = "[B";
			break;
		case 9:
			name = "[S";
			break;
		case 10:
			name = "[I";
			break;
		case 11:
			name = "[J";
			break;
		default:
			throw new VMException("java/lang/VirtualMachineError",
					"Unknown array type in NEWARRAY type=" + atype);
		}
		int count = popInt();
		if (count < 0) {
			throw new VMException("java/lang/NegativeArraySizeException", ""
					+ count);
		}
		int handle = heap.allocate((ClassArray) context.getClass(name), count);
		pushHandle(handle);
	}

	/**
	 * @throws VMException
	 * @throws VMCriticalException
	 * 
	 */
	private void opPutField() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int index = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ClassField field = getFieldFromConstant(index);
		char type = field.getDescriptor().charAt(0);
		long value;
		switch (type) {
		case 'D':
		case 'J':
			value = popLong();
			break;
		case 'L':
		case '[':
			value = popHandle();
			break;
		default:
			value = popInt();
		}
		int handle = popHandle();

		if (AbstractClass.hasFlag(field.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					"field " + field.getUniqueName() + " is static");
		}
		if (AbstractClass.hasFlag(field.getAccessFlags(),
				AbstractClass.ACC_FINAL)
				&& method.getOwner() != field.getOwner()) {
			throw new VMException("java/lang/IllegalAccessError", "field "
					+ field.getUniqueName() + " is final");
		}
		switch (type) {
		case 'D':
		case 'J':
			heap.putFieldLong(handle, field, value);
			break;
		default:
			heap.putFieldInt(handle, field, (int) value);
			break;
		}
	}

	/**
	 * 
	 */
	private void opTableSwitch() {
		int pad = (65536 - this.pc) % 4;
		this.pc += pad;
		byte db1 = code[this.pc++];
		byte db2 = code[this.pc++];
		byte db3 = code[this.pc++];
		byte db4 = code[this.pc++];
		byte lb1 = code[this.pc++];
		byte lb2 = code[this.pc++];
		byte lb3 = code[this.pc++];
		byte lb4 = code[this.pc++];
		byte hb1 = code[this.pc++];
		byte hb2 = code[this.pc++];
		byte hb3 = code[this.pc++];
		byte hb4 = code[this.pc++];
		int db = TypeUtil.bytesToSignedInt(db1, db2, db3, db4);
		int lb = TypeUtil.bytesToSignedInt(lb1, lb2, lb3, lb4);
		int hb = TypeUtil.bytesToSignedInt(hb1, hb2, hb3, hb4);
		int count = hb - lb + 1;
		int[] address = new int[count];
		for (int i = 0; i < count; i++) {
			byte ab1 = code[this.pc++];
			byte ab2 = code[this.pc++];
			byte ab3 = code[this.pc++];
			byte ab4 = code[this.pc++];
			address[i] = TypeUtil.bytesToSignedInt(ab1, ab2, ab3, ab4);
		}
		int target;
		int index = popInt();
		if (index < lb || index > hb) {
			target = db;
		} else {
			target = address[index - lb];
		}
		this.pc = this.lpc + target;
	}

	/**
	 * @throws VMException
	 * 
	 */
	private void opWide() throws VMException {
		int op2 = code[this.pc++] & 0xff;
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int index = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		switch (op2) {
		case FLOAD:

		case ILOAD: {
			int value = getLocalInt(index);
			pushInt(value);
			break;
		}
		case ALOAD: {
			int value = getLocalHandle(index);
			pushHandle(value);
			break;
		}
		case DLOAD:
		case LLOAD: {
			long value = getLocalLong(index);
			pushLong(value);
			break;
		}
		case FSTORE:

		case ISTORE: {
			int value = popInt();
			putLocalInt(index, value);
			break;
		}
		case ASTORE: {
			int aref = popType(tc);
			switch (tc.type) {
			case DescriptorAnalyzer.TYPE_HANDLE:
				putLocalHandle(index, aref);
				break;
			case DescriptorAnalyzer.TYPE_RETURN:
				putLocalReturn(index, aref);
				break;
			default:
				throw new VMException("java/lang/VirtualMachineError",
						"type check error!");
			}
			break;
		}
		case DSTORE:
		case LSTORE: {
			long value = popLong();
			putLocalLong(index, value);
			break;
		}
		case RET: {
			int addr = getLocalReturn(index);
			this.pc = addr;
			break;
		}
		case IINC: {
			byte cb1 = code[this.pc++];
			byte cb2 = code[this.pc++];
			int cb = TypeUtil.bytesToSignedInt(cb1, cb2);
			this.localVars[index] += cb;
			break;
		}
		default: {
			throw new VMException("java/lang/VirtualMachineError",
					"Unknown OPCode " + op2);
		}
		}
	}

	/**
	 * @throws VMCriticalException
	 * @throws VMException
	 * 
	 */
	private void opPutStatic() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int index = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ConstantFieldRef cfr = (ConstantFieldRef) method.getOwner()
				.getConstantPool()[index];
		char type = cfr.getNameAndType().getDescriptor().charAt(0);

		ClassBase targetClass = (ClassBase) cfr.getClazz().getClazz();
		if (clinit(targetClass)) {
			return;
		}
		long value;
		switch (type) {
		case 'D':
		case 'J':
			value = popLong();
			break;
		case 'L':
		case '[':
			value = popHandle();
			break;
		default:
			value = popInt();
			break;
		}
		ClassField field = cfr.getTargetField();
		if (AbstractClass.hasFlag(field.getAccessFlags(),
				AbstractClass.ACC_FINAL)
				&& method.getOwner() != field.getOwner()) {
			throw new VMException("java/lang/IllegalAccessError", "field "
					+ field.getUniqueName() + " is final");
		}
		switch (type) {
		case 'D':
		case 'J':
			heap.setStaticLong(field, value);
			break;
		default:
			heap.setStaticInt(field, (int) value);
			break;
		}
	}

	/**
	 * @throws VMException
	 * @throws VMCriticalException
	 * 
	 */
	private void opInvokeSpecial() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		ClassMethod invoke = getMethodFromConstant(m);
		ClassBase cb = invoke.getOwner();
		int count = invoke.getParamCount() + 1;
		int[] args = new int[count];
		byte[] types = new byte[count];
		for (int i = count - 1; i >= 0; i--) {
			args[i] = popType(tc);
			types[i] = tc.type;
		}
		if (((method.getOwner().getAccessFlags() & AbstractClass.ACC_SUPER) > 0)
				&& cb.isSuperClassOf(method.getOwner())
				&& !invoke.getName().equals("<init>")) {
			invoke = context.lookupMethodVirtual(method.getOwner()
					.getSuperClass(), invoke.getMethodName());
		}
		if (invoke == null) {
			throw new VMException("java/lang/AbstractMethodError", "");
		}
		if ("<init>".equals(invoke.getName()) && invoke.getOwner() != cb) {
			throw new VMException("java/lang/NoSuchMethodError",
					invoke.getUniqueName());
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_STATIC)) {
			throw new VMException("java/lang/IncompatibleClassChangeError",
					invoke.getUniqueName());
		}
		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_ABSTRACT)) {
			throw new VMException("java/lang/AbstractMethodError",
					invoke.getUniqueName());
		}

		if (AbstractClass.hasFlag(invoke.getAccessFlags(),
				AbstractClass.ACC_NATIVE)) {
			invoke.getNativeHandler().dealNative(args, this);
		} else {
			pushMethod(invoke, false, count, args, types);
		}
	}

	/**
	 * @throws VMException
	 * @throws VMCriticalException
	 * 
	 */
	private void opANewArray() throws VMException, VMCriticalException {
		byte ib1 = code[this.pc++];
		byte ib2 = code[this.pc++];
		int m = TypeUtil.bytesToUnsignedInt(ib1, ib2);
		int count = popInt();
		if (count < 0) {
			throw new VMException("java/lang/IndexOutOfBoundException", ""
					+ count);
		}
		ConstantClass cc = (ConstantClass) method.getOwner().getConstantPool()[m];
		ClassArray ca = (ClassArray) context.getClass("[L"
				+ cc.getClazz().getName() + ";");
		int a = heap.allocate(ca, count);
		pushHandle(a);

	}

	private void monitorEnter(int handle) {
		// context.getConsole().info(
		// getThreadId() + ">+++ Enter monitor " + handle);
		manager.monitorEnter(this, handle);
	}

	private void monitorExit(int handle) throws VMException,
			VMCriticalException {
		// context.getConsole().info(
		// getThreadId() + ">+++ Exit monitor " + handle);
		manager.monitorExit(this, handle);
	}

	private int mulitANewArray(String name, int dims, int[] count)
			throws VMException, VMCriticalException {
		ClassArray clazz = (ClassArray) context.getClass(name);
		int size = count[0];
		int ret = heap.allocate(clazz, size);
		if (dims > 1) {
			dims--;
			name = name.substring(1);
			int[] nc = new int[count.length - 1];
			System.arraycopy(count, 1, nc, 0, dims);
			for (int i = 0; i < size; i++) {
				heap.putArrayHandle(ret, i, mulitANewArray(name, dims, nc));
			}
		}
		return ret;
	}

	/**
	 * 
	 * @param targetClass
	 * @return will break?
	 */

	private boolean clinit(ClassBase targetClass) throws VMCriticalException {
		if (targetClass == null) {
			return false;
		}
		int tid = context.getClinitThreadId(targetClass);
		if (tid == VMContext.CLINIT_FINISHED || tid == getThreadId()) {
			return false;
		}
		if (targetClass.getClinit() == null) {
			boolean ret = clinit(targetClass.getSuperClass());
			if (!ret) {
				context.finishClinited(targetClass);
			}
			return ret;
		} else {
			if (tid == VMContext.CLINIT_NONE) {
				pc = lpc;
				pushFrame(targetClass.getClinit());
				context.setClinited(targetClass, this);
				return true;
			} else {
				pc = lpc;
				setYield(true);
				assert context.getConsole().debug(
						"Waiting for clinit of " + targetClass.getName());
				return true;
			}
		}
	}

	private Constant getConstant(int idx) {
		ClassBase cb = method.getOwner();
		return cb.getConstantPool()[idx];
	}

	private ClassField getFieldFromConstant(int idx) throws VMException,
			VMCriticalException {
		ClassBase cb = method.getOwner();
		ConstantFieldRef cf = (ConstantFieldRef) cb.getConstantPool()[idx];
		return cf.getTargetField();
	}

	private ClassMethod getMethodFromConstant(int idx) throws VMException,
			VMCriticalException {
		ClassBase cb = method.getOwner();
		assert cb.isLoaded();
		ConstantMethodRef cc = (ConstantMethodRef) cb.getConstantPool()[idx];
		return cc.getTargetMethod();
	}

	private ClassMethod getInterfaceMethodFromConstant(int idx)
			throws VMException, VMCriticalException {
		ClassBase cb = method.getOwner();
		ConstantInterfaceMethodRef cc = (ConstantInterfaceMethodRef) cb
				.getConstantPool()[idx];
		return cc.getTargetMethod();
	}

	private AbstractClass getClassFromConstant(int idx) throws VMException,
			VMCriticalException {
		ClassBase cb = method.getOwner();
		ConstantClass cc = (ConstantClass) cb.getConstantPool()[idx];
		return cc.getClazz();
	}

	public int popType(TypeContainer tc) {
		int tmp = --this.sp;
		tc.type = this.opStackTypes[tmp];
		return this.opStacks[tmp];
	}

	public void pushType(int value, byte type) {
		int tmp = this.sp++;
		this.opStacks[tmp] = value;
		this.opStackTypes[tmp] = type;
		assert type != DescriptorAnalyzer.TYPE_HANDLE
				|| (!(value < 0 || value > JHeap.MAX_OBJECTS)) : "Put a invalid handle!"
				+ value;
	}

	public int popHandle() {
		int tmp = --this.sp;
		assert this.opStackTypes[tmp] == DescriptorAnalyzer.TYPE_HANDLE : "Type mismatch!"
				+ this.opStackTypes[tmp]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_HANDLE;
		return this.opStacks[tmp];
	}

	public void pushHandle(int handle) {
		assert !(handle < 0 || handle > JHeap.MAX_OBJECTS) : "Put a invalid handle!"
				+ handle;
		int tmp = this.sp++;
		this.opStackTypes[tmp] = DescriptorAnalyzer.TYPE_HANDLE;
		this.opStacks[tmp] = handle;
	}

	public int popInt() {
		int tmp = --this.sp;
		assert this.opStackTypes[tmp] == DescriptorAnalyzer.TYPE_INT : "Type mismatch!"
				+ this.opStackTypes[tmp] + " should be " + DescriptorAnalyzer.TYPE_INT;
		return this.opStacks[tmp];
	}

	public void pushInt(int value) {
		int tmp = this.sp++;
		this.opStackTypes[tmp] = DescriptorAnalyzer.TYPE_INT;
		this.opStacks[tmp] = value;
	}

	public float popFloat() {
		int tmp = --this.sp;
		assert this.opStackTypes[tmp] == DescriptorAnalyzer.TYPE_INT : "Type mismatch!"
				+ this.opStackTypes[tmp] + " should be " + DescriptorAnalyzer.TYPE_INT;
		return Float.intBitsToFloat(this.opStacks[tmp]);
	}

	public void pushFloat(float value) {
		int tmp = this.sp++;
		this.opStackTypes[tmp] = DescriptorAnalyzer.TYPE_INT;
		this.opStacks[tmp] = Float.floatToRawIntBits(value);

	}

	public double popDouble() {
		int tmp = this.sp -= 2;
		assert this.opStackTypes[tmp] == DescriptorAnalyzer.TYPE_WIDE : "Type mismatch!"
				+ this.opStackTypes[tmp]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_WIDE;
		long lvalue = TypeUtil.intToLong(this.opStacks[tmp],
				this.opStacks[tmp + 1]);
		return Double.longBitsToDouble(lvalue);
	}

	public void pushDouble(double value) {
		long lvalue = Double.doubleToRawLongBits(value);
		int tmp = this.sp;
		this.sp += 2;
		this.opStackTypes[tmp] = DescriptorAnalyzer.TYPE_WIDE;
		this.opStackTypes[tmp + 1] = DescriptorAnalyzer.TYPE_WIDE2;
		this.opStacks[tmp] = TypeUtil.getHighInt(lvalue);
		this.opStacks[tmp + 1] = TypeUtil.getLowInt(lvalue);
	}

	public long popLong() {
		int tmp = this.sp -= 2;
		assert this.opStackTypes[tmp] == DescriptorAnalyzer.TYPE_WIDE : "Type mismatch!"
				+ this.opStackTypes[tmp]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_WIDE;
		long lvalue = TypeUtil.intToLong(this.opStacks[tmp],
				this.opStacks[tmp + 1]);
		return lvalue;
	}

	public void pushLong(long value) {
		int tmp = this.sp;
		this.sp += 2;
		this.opStackTypes[tmp] = DescriptorAnalyzer.TYPE_WIDE;
		this.opStackTypes[tmp + 1] = DescriptorAnalyzer.TYPE_WIDE2;
		this.opStacks[tmp] = (int) (value >>> 32);
		this.opStacks[tmp + 1] = (int) value;

	}

	public int getLocalReturn(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_RETURN : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_RETURN;
		return this.localVars[index];
	}

	public void putLocalReturn(int index, int value) {
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_RETURN;
		this.localVars[index] = value;
	}

	public int getLocalHandle(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_HANDLE : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_HANDLE;
		return this.localVars[index];
	}

	public void putLocalHandle(int index, int value) {
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_HANDLE;
		this.localVars[index] = value;
	}

	public int getLocalType(int index, TypeContainer tc) {
		tc.type = this.localVarTypes[index];
		return this.localVars[index];
	}

	public void putLocalType(int index, int value, byte type) {
		assert type != DescriptorAnalyzer.TYPE_HANDLE
				|| (!(value < 0 || value > JHeap.MAX_OBJECTS)) : "Put a invalid handle!"
				+ value;
		this.localVarTypes[index] = type;
		this.localVars[index] = value;
	}

	public int getLocalInt(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_INT : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_INT;
		return this.localVars[index];
	}

	public void putLocalInt(int index, int value) {
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_INT;
		this.localVars[index] = value;
	}

	public float getLocalFloat(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_INT : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_INT;
		return Float.intBitsToFloat(this.localVars[index]);
	}

	public void putLocalFloat(int index, float value) {
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_INT;
		this.localVars[index] = Float.floatToIntBits(value);
	}

	// public synchronized native void foo();

	public long getLocalLong(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_WIDE : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_WIDE;

		long lvalue = TypeUtil.intToLong(this.localVars[index],
				this.localVars[index + 1]);
		return lvalue;
	}

	public void putLocalLong(int index, long value) {
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_WIDE;
		this.localVarTypes[index + 1] = DescriptorAnalyzer.TYPE_WIDE2;
		this.localVars[index] = TypeUtil.getHighInt(value);
		this.localVars[index + 1] = TypeUtil.getLowInt(value);
	}

	public double getLocalDouble(int index) {
		assert this.localVarTypes[index] == DescriptorAnalyzer.TYPE_WIDE : "Type mismatch!"
				+ this.localVarTypes[index]
				+ " should be "
				+ DescriptorAnalyzer.TYPE_WIDE;

		long lvalue = TypeUtil.intToLong(this.localVars[index],
				this.localVars[index + 1]);
		return Double.longBitsToDouble(lvalue);
	}

	public void putLocalDouble(int index, double value) {
		long lvalue = Double.doubleToRawLongBits(value);
		this.localVarTypes[index] = DescriptorAnalyzer.TYPE_WIDE;
		this.localVarTypes[index + 1] = DescriptorAnalyzer.TYPE_WIDE2;
		this.localVars[index] = TypeUtil.getHighInt(lvalue);
		this.localVars[index + 1] = TypeUtil.getLowInt(lvalue);
	}

	public ClassMethod getCurrentMethod() {
		return context.getMethodById(this.methodId);
	}

	public void fillUsedHandles(BitSet tofill) {
		// int fpbak = getFP();
		assert heap.isHandleValid(getThreadHandle());
		saveFromCache();
		assert context.getConsole().info("SCAN INITT1->" + getThreadHandle());
		tofill.set(getThreadHandle());
		if (getCurrentThrowable() > 0) {
			assert heap.isHandleValid(getCurrentThrowable());
			assert context.getConsole().info(
					"SCAN INITT2->" + getCurrentThrowable());
			tofill.set(getCurrentThrowable());
		}
		StringBuilder out = new StringBuilder(64);
		for (int ii = frames.size() - 1; ii >= 0; ii--) {
			Frame frame = frames.get(ii);
			boolean assertion = false;
			assert assertion = true;
			if (assertion) {
				ClassMethod method = context.getMethodById(frame.methodId);

				out.append("GC Method=");
				out.append(method.getUniqueName());
				out.append(" ");
				for (int i = 0, max = frame.localVarTypes.length; i < max; i++) {
					out.append((char) frame.localVarTypes[i]);
				}
				out.append(" ");
				for (int i = 0, max = frame.sp; i < max; i++) {
					out.append((char) frame.opStackTypes[i]);
				}
				assert context.getConsole().debug(out.toString());
				out.setLength(0);
			}
			for (int i = 0, max = frame.localVars.length; i < max; i++) {
				if (frame.localVarTypes[i] == DescriptorAnalyzer.TYPE_HANDLE) {
					int handle = frame.localVars[i];
					if (handle > 0) {
						assert heap.isHandleValid(handle) : handle;
						assert context.getConsole().info(
								"SCAN INITT3->" + handle);
						tofill.set(handle);
					}
				}
			}

			for (int i = 0, max = frame.sp; i < max; i++) {
				if (frame.opStackTypes[i] == DescriptorAnalyzer.TYPE_HANDLE) {
					int handle = frame.opStacks[i];
					if (handle > 0) {
						assert heap.isHandleValid(handle);
						assert context.getConsole().info(
								"SCAN INITT4->" + handle);
						tofill.set(handle);
					}
				}
			}
		}
	}

	public void nativeReturnHandle(int handle) {
		pushHandle(handle);

	}

	public void nativeReturnInt(int value) {
		pushInt(value);

	}

	public void nativeReturnWide(long value) {
		pushLong(value);
	}

	public List<com.cirnoworks.fisce.vm.data.StackTraceElement> dumpStackTrace(
			List<com.cirnoworks.fisce.vm.data.StackTraceElement> list)
			throws VMException, VMCriticalException {
		if (list == null) {
			list = new ArrayList<com.cirnoworks.fisce.vm.data.StackTraceElement>();
		}
		saveFromCache();
		assert context.getConsole().debug(
				"######## DUMP STACK TRACE BEGIN #########");
		StackTraceElement ste;
		for (int ii = frames.size() - 1; ii >= 0; ii--) {
			Frame frame = frames.get(ii);

			ClassMethod mt = context.getMethodById(frame.methodId);
			ClassBase clazz = mt.getOwner();
			LineNumber[] lnt = mt.getLineNumberTable();
			AttributeSourceFile source = (AttributeSourceFile) Attribute
					.getAttributeByName(clazz, "SourceFile");

			String declaringClass = clazz.getName().replaceAll("/", ".");
			String methodName = mt.getName();
			String fileName = null;
			int lineNumber = -1;
			if (source != null) {
				fileName = source.sourceFile;
			}
			if (AbstractClass.hasFlag(clazz.getAccessFlags(),
					AbstractClass.ACC_NATIVE)) {
				lineNumber = -2;
			} else {
				int lpc = frame.lpc;
				if (lnt != null) {
					for (int i = 0, max = lnt.length; i < max; i++) {
						LineNumber ln = lnt[i];
						if (lpc >= ln.startPc) {
							lineNumber = ln.lineNumber;
							// break;
						} else {
							break;
						}
					}
				}
			}
			ste = new StackTraceElement(declaringClass, methodName, fileName,
					lineNumber);
			assert context.getConsole().debug(ste.toString());
			if (clazz.canCastTo(context.getClass("java/lang/Throwable"))
					&& (methodName.equals("<init>") || methodName
							.equals("fillInStackTrace"))) {
				continue;
			}
			list.add(ste);
		}
		assert context.getConsole().debug(
				"######## DUMP STACK TRACE END   #########");
		return list;
	}

}
