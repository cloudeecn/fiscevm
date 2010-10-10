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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.cirnoworks.fisce.vm.default_impl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import com.cirnoworks.fisce.vm.IHeap;
import com.cirnoworks.fisce.vm.INativeHandler;
import com.cirnoworks.fisce.vm.IThread;
import com.cirnoworks.fisce.vm.VMContext;
import com.cirnoworks.fisce.vm.VMCriticalException;
import com.cirnoworks.fisce.vm.VMException;
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
public final class DefaultThread implements IThread {

	public static final byte TYPE_INT = 'I';
	public static final byte TYPE_WIDE = 'W';
	public static final byte TYPE_HANDLE = 'H';
	public static final byte TYPE_RETURN = 'R';
	public static final byte TYPE_WIDE2 = '_';
	public static final byte TYPE_UNKNOWN = 0;

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
	private final DefaultThreadManager manager;
	private final IHeap heap;
	private byte[] code;
	private ClassMethod method;
	private boolean yield;
	private final Object statLock = new Object();

	class TypeContainer {
		byte type;
	}

	TypeContainer tc = new TypeContainer();

	/*
	 * ******************
	 * Persist data
	 */
	private final ByteBuffer frames = BufferUtil.createBuffer(STACK_SIZE);

	public byte[] getFullStack() {
		int size = getFP() + 4;
		byte[] buf = new byte[size];
		frames.clear();
		frames.get(buf);
		frames.clear();
		return buf;
	}

	public final int getThreadHandle() {
		return frames.getInt(pThreadHandle);
	}

	private final void setThreadHandle(int handle) throws VMException {
		assert context.getClass(handle) != null;
		frames.putInt(pThreadHandle, handle);
	}

	public final int getCurrentThrowable() {
		return frames.getInt(pCurrentThrowable);
	}

	public final void setCurrentThrowable(int handle)
			throws VMCriticalException {
		if (handle != 0) {

			try {
				AbstractClass clazz = context.getClass(handle);
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
		frames.putInt(pCurrentThrowable, handle);
	}

	public final int getStatus() {
		return frames.getInt(pStatus);
	}

	private final void setStatus(int value) {
		frames.putInt(pStatus, value);
	}

	public final int getPriority() {
		return frames.getInt(pPriority);
	}

	public final void setPriority(int value) {
		frames.putInt(pPriority, value);
	}

	public final int getThreadId() {
		return frames.getInt(pThreadId);
	}

	public final void setThreadId(int value) {
		frames.putInt(pThreadId, value);
	}

	// public final int getWaitForLock() {
	// return frames.getInt(pWaitForLock);
	// }
	//
	// public final void setWaitForLock(int value) {
	// frames.putInt(pWaitForLock, value);
	// }

	private final int getFP() {
		return frames.getInt(pFP);
	}

	private final void setFP(int value) {
		frames.putInt(pFP, value);
	}

	private final int getSIZE() {
		return frames.getInt(getFP() + dSIZE);
	}

	private final void setSIZE(int value) {
		frames.putInt(getFP() + dSIZE, value);
	}

	private final int getMID() {
		return frames.getInt(getFP() + dMID);
	}

	private final void setMID(int value) {
		frames.putInt(getFP() + dMID, value);
	}

	private final int getLC() {
		return frames.getInt(getFP() + dLC);
	}

	private final void setLC(int value) {
		frames.putInt(getFP() + dLC, value);
	}

	private final int getLB() {
		return frames.getInt(getFP() + dLB);
	}

	private final void setLB(int value) {
		frames.putInt(getFP() + dLB, value);
	}

	private final int getSC() {
		return frames.getInt(getFP() + dSC);
	}

	private final void setSC(int value) {
		frames.putInt(getFP() + dSC, value);
	}

	private final int getSB() {
		return frames.getInt(getFP() + dSB);
	}

	private final void setSB(int value) {
		frames.putInt(getFP() + dSB, value);
	}

	private final int getSR() {
		return frames.getInt(getFP() + dSR);
	}

	private final void setSR(int value) {
		frames.putInt(getFP() + dSR, value);
	}

	private final int getLTB() {
		return frames.getInt(getFP() + dLTB);
	}

	private final void setLTB(int value) {
		frames.putInt(getFP() + dLTB, value);
	}

	private final int getSTB() {
		return frames.getInt(getFP() + dSTB);
	}

	private final void setSTB(int value) {
		frames.putInt(getFP() + dSTB, value);
	}

	private final int getLPC() {
		return frames.getInt(getFP() + dLPC);
	}

	private final void setLPC(int value) {
		frames.putInt(getFP() + dLPC, value);
	}

	private final int getPC() {
		return frames.getInt(getFP() + dPC);
	}

	private final void setPC(int value) {
		frames.putInt(getFP() + dPC, value);
	}

	public boolean isYield() {
		return yield;
	}

	public void setYield(boolean yield) {
		this.yield = yield;
	}

	public DefaultThread(VMContext context, DefaultThreadManager manager) {
		this.context = context;
		this.manager = manager;
		heap = context.getHeap();
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
		setFP(pFP);
		pushFrame(method);
		putLocalHandle(0, heap.allocate(
				(ClassArray) context.getClass("[Ljava/lang/String;"), 0));
		clinit(method.getOwner());
	}

	public void create(int handle) throws VMException, VMCriticalException {
		AbstractClass clazz = context.getClass(handle);
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
		setFP(pFP);
		pushFrame(runner);
		// "this"
		putLocalHandle(0, handle);
	}

	public void createFromData(byte[] data) {
		frames.clear();
		frames.put(data);
		frames.clear();
		yield = true;
		updateLocalBuf();
	}

	private byte nextOP() {
		int pc = getPC();
		setPC(pc + 1);
		return code[pc];
	}

	private void movePC(int ofs) {
		setPC(getPC() + ofs);
	}

	private void updateLocalBuf() {
		if (getFP() != pFP) {
			method = getCurrentMethod();
			code = method.getCode();
		} else {
			method = null;
			code = null;
		}
	}

	private String getFrameInfo() {
		return "STACK=["
				+ getSB()
				+ "-"
				+ (getSB() + 4 * getSC())
				+ "] VARS=["
				+ getLB()
				+ "-"
				+ (getLB() + 4 * getLC())
				+ ") FP="
				+ getFP()
				+ " SIZE="
				+ getSIZE()
				+ (!AbstractClass.hasFlag(method.getAccessFlags(),
						AbstractClass.ACC_STATIC));
	}

	public void pushFrame(ClassMethod mt) {
		int ltsize = mt.getMaxLocals();
		int stsize = mt.getMaxStack();
		int lsize = mt.getMaxLocals() * 4;
		int ssize = mt.getMaxStack() * 4;
		int size = -dPC + 4 + ssize + lsize + stsize + ltsize;
		if (getFP() > pFP) {
			assert context.getConsole().debug(
					">>>Push frame from " + getFrameInfo());
		} else {
			assert context.getConsole().debug(">>>Pop frame prepare to enter!");
		}
		setFP(getFP() + size);
		setSIZE(size);
		setMID(context.getMethodId(mt));
		setLC(mt.getMaxLocals());
		setLB(getFP() + dPC - lsize);
		setSC(mt.getMaxStack());
		setSB(getLB() - ssize);
		setLTB(getSB() - ltsize);
		setSTB(getLTB() - stsize);
		setSR(0);
		setPC(0);
		int ltb = getLTB();
		for (int i = 0, max = getLC(); i < max; i++) {
			frames.put(ltb + i, (byte) 0);
		}
		updateLocalBuf();
		assert context.getConsole().debug(">>>Push frame to " + getFrameInfo());

		// switchIn();
	}

	private void popFrame() {
		// switchOut();
		if (getFP() > pFP) {
			assert context.getConsole().debug(
					">>>Pop frame from " + getFrameInfo());
		}
		setFP(getFP() - getSIZE());

		updateLocalBuf();
		if (getFP() > pFP) {
			assert context.getConsole().debug(
					">>>Pop frame to " + getFrameInfo());
		} else {
			assert context.getConsole().debug(">>>Pop frame prepare to exit!");
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
			if (getFP() == pFP) {
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
				"*EXCEPTION HANDLE LOOKUP: LPC=" + getLPC());
		AbstractClass throwableClass;
		try {
			throwableClass = (ClassBase) context.getClass(th);
		} catch (VMException e) {
			context.getConsole().error(
					"Error in processing exception. Thread dead.", e);
			throw new VMCriticalException(e);
		}

		ExceptionHandler[] handlers = method.getExceptionTable();
		int lpc = getLPC();
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
			setSR(0);
			pushHandle(th);
			setCurrentThrowable(0);
			setPC(target);
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

	/**
	 * 
	 * @return yield
	 * @throws VMCriticalException
	 */
	private void runOneInst() throws VMCriticalException {
		setLPC(getPC());
		int op = nextOP() & 0xff;
		if (method.isClinit()) {
			ClassBase targetClass = method.getOwner().getSuperClass();

			if (targetClass != null
					&& context.getClinitThreadId(targetClass) != VMContext.CLINIT_FINISHED) {
				int cmd = clinit(targetClass);
				if (cmd == CMD_BREAK) {
					return;
				} else if (cmd == CMD_GOON) {

				} else if (cmd == CMD_BACK) {
					setPC(getLPC());
					return;
				}
			}
		}

		assert context.getConsole().debug(
				method.getUniqueName() + " " + getLPC() + " " + OP_NAME[op]
						+ " SR=" + getSR());
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
				if (value != 0 && !context.getClass(value).canCastTo(content)) {
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
				int index = nextOP() & 0xff;
				int value = getLocalInt(index);
				pushInt(value);
				break;
			}
			case ALOAD: {
				int index = nextOP() & 0xff;
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				int count = popInt();
				if (count < 0) {
					throw new VMException("java/lang/IndexOutOfBoundException",
							"" + count);
				}
				ConstantClass cc = (ConstantClass) method.getOwner()
						.getConstantPool()[m];
				ClassArray ca = (ClassArray) context.getClass("[L"
						+ cc.getClazz().getName() + ";");
				int a = heap.allocate(ca, count);
				pushHandle(a);
				break;
			}
			case IRETURN:
			case FRETURN: {
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
				break;
			}
			case ARETURN: {
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
				break;
			}
			case ARRAYLENGTH: {
				int aref = popHandle();
				pushInt(heap.getArrayLength(aref));
				break;
			}
			case ISTORE:
			case FSTORE: {
				int index = nextOP() & 0xff;
				int value = popInt();

				putLocalInt(index, value);
				break;
			}
			case ASTORE: {
				int index = nextOP() & 0xff;
				int aref = popType(tc);
				switch (tc.type) {
				case TYPE_HANDLE:
					putLocalHandle(index, aref);
					break;
				case TYPE_RETURN:
					putLocalReturn(index, aref);
					break;
				default:
					throw new VMException("java/lang/VirtualMachineError",
							"type check error!" + (char)tc.type);
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
				case TYPE_HANDLE:
					putLocalHandle(0, aref);
					break;
				case TYPE_RETURN:
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
				case TYPE_HANDLE:
					putLocalHandle(1, aref);
					break;
				case TYPE_RETURN:
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
				case TYPE_HANDLE:
					putLocalHandle(2, aref);
					break;
				case TYPE_RETURN:
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
				case TYPE_HANDLE:
					putLocalHandle(3, aref);
					break;
				case TYPE_RETURN:
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
				int value = nextOP();
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				int handle = popHandle();
				if (handle == 0) {
					pushHandle(handle);
					break;
				}
				AbstractClass clazzS = context.getClass(handle);
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
				int index = nextOP() & 0xff;
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
				int index = nextOP() & 0xff;

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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				int handle = popHandle();
				ClassField field = ((ConstantFieldRef) method.getOwner()
						.getConstantPool()[m]).getTargetField();
				if (AbstractClass.hasFlag(field.getAccessFlags(),
						AbstractClass.ACC_STATIC)) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError", "field "
									+ field.getUniqueName() + " is static");
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
				break;
			}
			case GETSTATIC: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				ConstantFieldRef cfr = (ConstantFieldRef) method.getOwner()
						.getConstantPool()[m];
				ClassBase targetClass = (ClassBase) cfr.getClazz().getClazz();
				if (context.getClinitThreadId(targetClass) != VMContext.CLINIT_FINISHED) {
					int cmd = clinit(targetClass);
					if (cmd == CMD_BREAK) {
						break;
					} else if (cmd == CMD_GOON) {

					} else if (cmd == CMD_BACK) {
						setPC(getLPC());
						break;
					}
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
				break;
			}
			case GOTO: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				setPC(getLPC() + m);
				break;
			}
			case GOTO_W: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				byte ib3 = nextOP();
				byte ib4 = nextOP();
				// ib1 = ((ib1 << 24) & ib2) & 0xffff;
				setPC(getLPC() + mergeb(ib1, ib2, ib3, ib4));
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
				int value = popInt();
				pushInt(value + popInt());
				break;
			}
			case IAND: {
				int value = popInt();
				pushInt(value & popInt());
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 == value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ACMPEQ: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popHandle();
				int value1 = popHandle();
				if (value1 == value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ICMPNE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 != value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ACMPNE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popHandle();
				int value1 = popHandle();
				if (value1 != value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ICMPLT: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 < value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ICMPLE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 <= value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ICMPGT: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 > value2) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IF_ICMPGE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value2 = popInt();
				int value1 = popInt();
				if (value1 >= value2) {
					setPC(getLPC() + m);
				}
				break;
			}

			case IFEQ: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value == 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IFNULL: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popHandle();
				if (value == 0) {
					setPC(getLPC() + m);
				}
				break;
			}

			case IFNE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value != 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IFNONNULL: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popHandle();
				if (value != 0) {
					setPC(getLPC() + m);
				}
				break;
			}

			case IFLT: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value < 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IFLE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value <= 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IFGT: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value > 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IFGE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = mergeb(ib1, ib2);
				int value = popInt();
				if (value >= 0) {
					setPC(getLPC() + m);
				}
				break;
			}
			case IINC: {
				int index = nextOP() & 0xff;

				int value = nextOP();
				putLocalInt(index, getLocalInt(index) + value);
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				int handle = popHandle();
				AbstractClass clazz = context.getClass(handle);
				AbstractClass castto = getClassFromConstant(m);
				pushInt(clazz.canCastTo(castto) ? 1 : 0);
				break;
			}
			case INVOKEINTERFACE: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int count = nextOP() & 0xff;
				nextOP();
				int[] args = new int[count];
				byte[] types = new byte[count];
				for (int i = count - 1; i >= 0; i--) {
					args[i] = popType(tc);
					types[i] = tc.type;
				}
				int m = merge(ib1, ib2);
				AbstractClass clazz = context.getClass(args[0]);
				ClassMethod lookup = getInterfaceMethodFromConstant(m);
				if (!clazz.canCastTo(lookup.getOwner())) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError", "");
				}
				ClassMethod invoke = context.lookupMethodVirtual(
						(ClassBase) context.getClass(args[0]),
						lookup.getMethodName());
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
					INativeHandler inh = context.getNativeHandler(invoke
							.getUniqueName());
					if (inh == null) {
						throw new VMCriticalException(
								"java/lang/UnsatisfiedLinkError "
										+ invoke.getUniqueName());
					} else {
						inh.dealNative(args, context, this);
					}
				} else {
					pushFrame(invoke);
					for (int i = 0; i < count; i++) {
						putLocalType(i, args[i], types[i]);
					}
					if (AbstractClass.hasFlag(invoke.getAccessFlags(),
							AbstractClass.ACC_SYNCHRONIZED)) {
						monitorEnter(args[0]);
					}
				}
				break;
			}
			case INVOKESPECIAL: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
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
				if ("<init>".equals(invoke.getName())
						&& invoke.getOwner() != cb) {
					throw new VMException("java/lang/NoSuchMethodError",
							invoke.getUniqueName());
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_STATIC)) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError",
							invoke.getUniqueName());
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_ABSTRACT)) {
					throw new VMException("java/lang/AbstractMethodError",
							invoke.getUniqueName());
				}

				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_NATIVE)) {
					INativeHandler inh = context.getNativeHandler(invoke
							.getUniqueName());
					if (inh == null) {
						throw new VMCriticalException(
								"java/lang/UnsatisfiedLinkError"
										+ invoke.getUniqueName());
					} else {
						inh.dealNative(args, context, this);
					}
				} else {
					pushFrame(invoke);
					for (int i = 0; i < count; i++) {
						putLocalType(i, args[i], types[i]);
					}
					if (AbstractClass.hasFlag(invoke.getAccessFlags(),
							AbstractClass.ACC_SYNCHRONIZED)) {
						monitorEnter(args[0]);
					}
				}
				break;
			}
			case INVOKESTATIC: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				ClassMethod invoke = getMethodFromConstant(m);
				if (!AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_STATIC)) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError", "");
				}
				ClassBase targetClass = invoke.getOwner();
				if (context.getClinitThreadId(targetClass) != VMContext.CLINIT_FINISHED) {
					int cmd = clinit(targetClass);
					if (cmd == CMD_BREAK) {
						break;
					} else if (cmd == CMD_GOON) {

					} else if (cmd == CMD_BACK) {
						setPC(getLPC());
						break;
					}
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
					INativeHandler inh = context.getNativeHandler(invoke
							.getUniqueName());
					if (inh == null) {
						throw new VMCriticalException(
								"java/lang/UnsatisfiedLinkError"
										+ invoke.getUniqueName());
					} else {
						inh.dealNative(args, context, this);
					}
				} else {
					pushFrame(invoke);
					for (int i = 0; i < count; i++) {
						putLocalType(i, args[i], types[i]);
					}
					if (AbstractClass.hasFlag(invoke.getAccessFlags(),
							AbstractClass.ACC_SYNCHRONIZED)) {
						monitorEnter(context
								.getClassObjectHandleForClass(invoke.getOwner()));
					}
				}
				break;
			}
			case INVOKEVIRTUAL: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int m = merge(ib1, ib2);
				ClassMethod lookup = getMethodFromConstant(m);

				int count = lookup.getParamCount() + 1;
				int[] args = new int[count];
				byte[] types = new byte[count];
				for (int i = count - 1; i >= 0; i--) {
					args[i] = popType(tc);
					types[i] = tc.type;
				}
				ClassMethod invoke = context.lookupMethodVirtual(
						(ClassBase) context.getClass(args[0]),
						lookup.getMethodName());
				if (invoke == null) {
					throw new VMException("java/lang/AbstractMethodError", "");
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_STATIC)) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError",
							invoke.getUniqueName());
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_STATIC)) {
					throw new VMException(
							"java/lang/IncompatibleClassChangeError",
							invoke.getUniqueName());
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_ABSTRACT)) {
					throw new VMException("java/lang/AbstractMethodError",
							invoke.getUniqueName());
				}
				if (AbstractClass.hasFlag(invoke.getAccessFlags(),
						AbstractClass.ACC_NATIVE)) {
					INativeHandler inh = context.getNativeHandler(invoke
							.getUniqueName());
					if (inh == null) {
						throw new VMCriticalException(
								"java/lang/UnsatisfiedLinkError"
										+ invoke.getUniqueName());
					} else {
						inh.dealNative(args, context, this);
					}
				} else {
					pushFrame(invoke);
					for (int i = 0; i < count; i++) {
						putLocalType(i, args[i], types[i]);
					}
					if (AbstractClass.hasFlag(invoke.getAccessFlags(),
							AbstractClass.ACC_SYNCHRONIZED)) {
						monitorEnter(args[0]);
					}
				}
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
				byte bb1 = nextOP();
				byte bb2 = nextOP();
				int target = mergeb(bb1, bb2);
				pushType(getPC(), TYPE_RETURN);
				setPC(getLPC() + target);
				break;
			}
			case JSR_W: {
				byte bb1 = nextOP();
				byte bb2 = nextOP();
				byte bb3 = nextOP();
				byte bb4 = nextOP();
				int target = mergeb(bb1, bb2, bb3, bb4);
				pushType(getPC(), TYPE_RETURN);
				setPC(getLPC() + target);
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
				char index = (char) (nextOP() & 0xff);
				Constant con = getConstant(index);
				if (con instanceof ConstantInteger) {
					pushInt(((ConstantInteger) con).getData());
				} else if (con instanceof ConstantFloat) {
					pushFloat(((ConstantFloat) con).getData());
				} else if (con instanceof ConstantString) {
					pushHandle(((ConstantString) con).getHandle());
				} else {
					throw new VMException("java/lang/VirtualMachineError",
							"LDC type wrong!" + con);
				}
				break;
			}
			case LDC_W: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int index = merge(ib1, ib2);
				Constant con = getConstant(index);
				if (con instanceof ConstantInteger) {
					pushInt(((ConstantInteger) con).getData());
				} else if (con instanceof ConstantFloat) {
					pushFloat(((ConstantFloat) con).getData());
				} else if (con instanceof ConstantString) {
					pushHandle(((ConstantString) con).getHandle());
				} else {
					throw new VMException("java/lang/VirtualMachineError",
							"LDCW type wrong!" + con);
				}
				break;
			}
			case LDC2_W: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int index = merge(ib1, ib2);
				Constant con = getConstant(index);
				if (con instanceof ConstantLong) {
					pushLong(((ConstantLong) con).getData());
				} else if (con instanceof ConstantDouble) {
					pushDouble(((ConstantDouble) con).getData());
				} else {
					throw new VMException("java/lang/VirtualMachineError",
							"LDC2_W type wrong!" + con);
				}
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
				int index = nextOP() & 0xff;
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
				int padSize = (65536 - getPC()) % 4;
				movePC(padSize);
				byte db1 = nextOP();
				byte db2 = nextOP();
				byte db3 = nextOP();
				byte db4 = nextOP();
				int db = mergeb(db1, db2, db3, db4);
				byte np1 = nextOP();
				byte np2 = nextOP();
				byte np3 = nextOP();
				byte np4 = nextOP();
				int np = mergeb(np1, np2, np3, np4);
				int[] match = new int[np];
				int[] offset = new int[np];
				for (int i = 0; i < np; i++) {
					byte ma1 = nextOP();
					byte ma2 = nextOP();
					byte ma3 = nextOP();
					byte ma4 = nextOP();
					byte of1 = nextOP();
					byte of2 = nextOP();
					byte of3 = nextOP();
					byte of4 = nextOP();
					match[i] = mergeb(ma1, ma2, ma3, ma4);
					offset[i] = mergeb(of1, of2, of3, of4);
				}
				int key = popInt();
				boolean matched = false;
				for (int i = 0; i < np; i++) {
					if (key == match[i]) {
						setPC(getLPC() + offset[i]);
						matched = true;
						break;
					}
				}
				if (!matched) {
					setPC(getLPC() + db);
				}
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
				int index = nextOP() & 0xff;
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int dims = nextOP() & 0xff;
				int m = merge(ib1, ib2);
				ClassArray clazz = (ClassArray) getClassFromConstant(m);
				int[] count = new int[dims];
				for (int i = dims - 1; i >= 0; i--) {
					count[i] = popInt();
				}
				pushHandle(mulitANewArray(clazz.getName(), dims, count));
				break;
			}
			case NEW: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int idx = merge(ib1, ib2);
				try {
					ClassBase targetClass = (ClassBase) getClassFromConstant(idx);
					if (AbstractClass.hasFlag(targetClass.getAccessFlags(),
							AbstractClass.ACC_INTERFACE
									| AbstractClass.ACC_ABSTRACT)) {
						throw new VMException("java/lang/InstantiationError",
								targetClass.getName());
					}
					if (context.getClinitThreadId(targetClass) != VMContext.CLINIT_FINISHED) {
						int cmd = clinit(targetClass);
						if (cmd == CMD_BREAK) {
							break;
						} else if (cmd == CMD_GOON) {

						} else if (cmd == CMD_BACK) {
							setPC(getLPC());
							break;
						}
					}
					pushHandle(heap.allocate(targetClass));
				} catch (VMException e) {
					throw new VMCriticalException(e);
				}
				break;
			}
			case NEWARRAY: {
				byte atype = nextOP();
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
					throw new VMException(
							"java/lang/NegativeArraySizeException", "" + count);
				}
				int handle = heap.allocate((ClassArray) context.getClass(name),
						count);
				pushHandle(handle);
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
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int index = merge(ib1, ib2);
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
					throw new VMException(
							"java/lang/IncompatibleClassChangeError", "field "
									+ field.getUniqueName() + " is static");
				}
				if (AbstractClass.hasFlag(field.getAccessFlags(),
						AbstractClass.ACC_FINAL)
						&& method.getOwner() != field.getOwner()) {
					throw new VMException("java/lang/IllegalAccessError",
							"field " + field.getUniqueName() + " is final");
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
				break;
			}
			case PUTSTATIC: {
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int index = merge(ib1, ib2);
				ConstantFieldRef cfr = (ConstantFieldRef) method.getOwner()
						.getConstantPool()[index];
				char type = cfr.getNameAndType().getDescriptor().charAt(0);

				ClassBase targetClass = (ClassBase) cfr.getClazz().getClazz();
				if (context.getClinitThreadId(targetClass) != VMContext.CLINIT_FINISHED) {
					int cmd = clinit(targetClass);
					if (cmd == CMD_BREAK) {
						break;
					} else if (cmd == CMD_GOON) {

					} else if (cmd == CMD_BACK) {
						setPC(getLPC());
						break;
					}
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
					throw new VMException("java/lang/IllegalAccessError",
							"field " + field.getUniqueName() + " is final");
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

				break;
			}
			case RET: {
				int index = nextOP() & 0xff;
				int addr = getLocalReturn(index);
				setPC(addr);
				break;
			}
			case RETURN: {
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
				byte bb1 = nextOP();
				byte bb2 = nextOP();
				int value = mergeb(bb1, bb2);
				pushInt(value);
				break;
			}
			case SWAP: {
				int value1 = popType(tc);
				byte type1 = tc.type;
				int value2 = popType(tc);
				byte type2 = tc.type;
				pushType(value1, type1);
				pushType(value2, type2);
				break;
			}
			case TABLESWITCH: {
				int pad = (65536 - getPC()) % 4;
				movePC(pad);
				byte db1 = nextOP();
				byte db2 = nextOP();
				byte db3 = nextOP();
				byte db4 = nextOP();
				byte lb1 = nextOP();
				byte lb2 = nextOP();
				byte lb3 = nextOP();
				byte lb4 = nextOP();
				byte hb1 = nextOP();
				byte hb2 = nextOP();
				byte hb3 = nextOP();
				byte hb4 = nextOP();
				int db = mergeb(db1, db2, db3, db4);
				int lb = mergeb(lb1, lb2, lb3, lb4);
				int hb = mergeb(hb1, hb2, hb3, hb4);
				int count = hb - lb + 1;
				int[] address = new int[count];
				for (int i = 0; i < count; i++) {
					byte ab1 = nextOP();
					byte ab2 = nextOP();
					byte ab3 = nextOP();
					byte ab4 = nextOP();
					address[i] = mergeb(ab1, ab2, ab3, ab4);
				}
				int target;
				int index = popInt();
				if (index < lb || index > hb) {
					target = db;
				} else {
					target = address[index - lb];
				}
				setPC(getLPC() + target);
				break;
			}
			case WIDE: {
				int op2 = nextOP() & 0xff;
				byte ib1 = nextOP();
				byte ib2 = nextOP();
				int index = merge(ib1, ib2);
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
					case TYPE_HANDLE:
						putLocalHandle(index, aref);
						break;
					case TYPE_RETURN:
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
					setPC(addr);
					break;
				}
				case IINC: {
					byte cb1 = nextOP();
					byte cb2 = nextOP();
					int cb = mergeb(cb1, cb2);
					putLocalInt(index, getLocalInt(index) + cb);
					break;
				}
				default: {
					throw new VMException("java/lang/VirtualMachineError",
							"Unknown OPCode " + op);
				}
				}
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
		}
		return;
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

	private int clinit(ClassBase targetClass) {
		if (targetClass.getClinit() == null) {
			context.finishClinited(targetClass);
			return CMD_GOON;
		} else {
			int tid = context.getClinitThreadId(targetClass);
			if (tid == VMContext.CLINIT_FINISHED) {
				return CMD_GOON;
			} else if (tid == VMContext.CLINIT_NONE) {
				setPC(getLPC());
				pushFrame(targetClass.getClinit());
				context.setClinited(targetClass, this);
				return CMD_BREAK;
			} else if (tid == getThreadId()) {
				return CMD_GOON;
			} else {
				setYield(true);
				assert context.getConsole().debug(
						"Waiting for clinit of " + targetClass.getName());
				return CMD_BACK;
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

	private int merge(byte ib1, byte ib2) {
		return ((ib1 << 8) + (ib2 & 0xff)) & 0xffff;
	}

	private int mergeb(byte bb1, byte bb2) {
		return ((bb1 << 8) + (bb2 & 0xff));
	}

	private int merge(byte ib1, byte ib2, byte ib3, byte ib4) {
		return ((ib1 & 0xff) << 24) + ((ib2 & 0xff) << 16)
				+ ((ib3 & 0xff) << 8) + (ib4 & 0xff);
	}

	private int mergeb(byte bb1, byte bb2, byte bb3, byte bb4) {
		return ((bb1 & 0xff) << 24) + ((bb2 & 0xff) << 16)
				+ ((bb3 & 0xff) << 8) + (bb4 & 0xff);
	}

	public void athrow(int handle) {

	}

	public int popType(TypeContainer tc) {
		setSR(getSR() - 1);
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		tc.type = frames.get(getSTB() + getSR());
		return frames.getInt(getSB() + (getSR() << 2));
	}

	public void pushType(int value, byte type) {
		assert getSR() < getSC() : "Stack overflow!" + getSR() + ">=" + getSC();
		assert type != TYPE_HANDLE
				|| (!(value < 0 || value > IHeap.MAX_OBJECTS)) : "Put a invalid handle!"
				+ value;
		frames.put(getSTB() + getSR(), type);
		frames.putInt(getSB() + (getSR() << 2), value);
		setSR(getSR() + 1);
	}

	public int popHandle() {
		setSR(getSR() - 1);
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		assert frames.get(getSTB() + getSR()) == TYPE_HANDLE : "Type mismatch!"
				+ frames.get(getSTB() + getSR()) + " should be " + TYPE_HANDLE;
		return frames.getInt(getSB() + (getSR() << 2));
	}

	public void pushHandle(int handle) {
		assert getSR() < getSC() : "Stack overflow!" + getSR() + ">=" + getSC();
		assert !(handle < 0 || handle > IHeap.MAX_OBJECTS) : "Put a invalid handle!"
				+ handle;
		frames.put(getSTB() + getSR(), TYPE_HANDLE);
		frames.putInt(getSB() + (getSR() << 2), handle);
		setSR(getSR() + 1);
	}

	public int popInt() {
		setSR(getSR() - 1);
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		assert frames.get(getSTB() + getSR()) == TYPE_INT : "Type mismatch!"
				+ frames.get(getSTB() + getSR()) + " should be " + TYPE_INT;
		return frames.getInt(getSB() + (getSR() << 2));
	}

	public void pushInt(int value) {
		assert getSR() < getSC() : "Stack overflow!" + getSR() + ">=" + getSC();
		frames.put(getSTB() + getSR(), TYPE_INT);
		frames.putInt(getSB() + (getSR() << 2), value);
		setSR(getSR() + 1);

	}

	public float popFloat() {
		setSR(getSR() - 1);
		assert frames.get(getSTB() + getSR()) == TYPE_INT : "Type mismatch!"
				+ frames.get(getSTB() + getSR()) + " should be " + TYPE_INT;
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		return frames.getFloat(getSB() + (getSR() << 2));
	}

	public void pushFloat(float value) {
		assert getSR() < getSC() : "Stack overflow!" + getSR() + ">=" + getSC();
		frames.put(getSTB() + getSR(), TYPE_INT);
		frames.putFloat(getSB() + (getSR() << 2), value);
		setSR(getSR() + 1);

	}

	public double popDouble() {
		setSR(getSR() - 2);
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		assert frames.get(getSTB() + getSR()) == TYPE_WIDE : "Type mismatch!"
				+ frames.get(getSTB() + getSR()) + " should be " + TYPE_WIDE;
		return frames.getDouble(getSB() + (getSR() << 2));
	}

	public void pushDouble(double value) {
		assert getSR() < (getSC() - 1) : "Stack overflow!" + getSR() + ">="
				+ (getSC() - 1);
		frames.put(getSTB() + getSR(), TYPE_WIDE);
		frames.put(getSTB() + getSR() + 1, TYPE_WIDE2);
		frames.putDouble(getSB() + (getSR() << 2), value);
		setSR(getSR() + 2);

	}

	public long popLong() {
		setSR(getSR() - 2);
		assert getSR() >= 0 : "Stack underflow!" + getSR() + "<" + 0;
		assert frames.get(getSTB() + getSR()) == TYPE_WIDE : "Type mismatch!"
				+ frames.get(getSTB() + getSR()) + " should be " + TYPE_WIDE;
		return frames.getLong(getSB() + (getSR() << 2));
	}

	public void pushLong(long value) {
		assert getSR() < getSC() - 1 : "Stack overflow!" + getSR() + ">="
				+ (getSC() - 1);
		frames.put(getSTB() + getSR(), TYPE_WIDE);
		frames.put(getSTB() + getSR() + 1, TYPE_WIDE2);
		frames.putLong(getSB() + (getSR() << 2), value);
		setSR(getSR() + 2);

	}

	public int getLocalReturn(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert frames.get(getLTB() + index) == TYPE_RETURN : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_RETURN;
		return frames.getInt(pos);
	}

	public void putLocalReturn(int index, int value) {
		int pos = getLB() + (index << 2);

		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		frames.put(getLTB() + index, TYPE_RETURN);
		frames.putInt(pos, value);
	}

	public int getLocalHandle(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert frames.get(getLTB() + index) == TYPE_HANDLE : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_HANDLE;
		return frames.getInt(pos);
	}

	public void putLocalHandle(int index, int value) {
		int pos = getLB() + (index << 2);

		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert !(value < 0 || value > IHeap.MAX_OBJECTS) : "Put a invalid handle!"
				+ value + " " + IHeap.MAX_OBJECTS;
		frames.put(getLTB() + index, TYPE_HANDLE);
		frames.putInt(pos, value);
	}

	public int getLocalType(int index, TypeContainer tc) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		tc.type = frames.get(getLTB() + index);
		return frames.getInt(pos);
	}

	public void putLocalType(int index, int value, byte type) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert type != TYPE_HANDLE
				|| (!(value < 0 || value > IHeap.MAX_OBJECTS)) : "Put a invalid handle!"
				+ value;
		frames.put(getLTB() + index, type);
		frames.putInt(pos, value);
	}

	public int getLocalInt(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert frames.get(getLTB() + index) == TYPE_INT : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_INT;
		return frames.getInt(pos);
	}

	public void putLocalInt(int index, int value) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		frames.put(getLTB() + index, TYPE_INT);
		frames.putInt(pos, value);
	}

	public float getLocalFloat(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		assert frames.get(getLTB() + index) == TYPE_INT : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_INT;
		return frames.getFloat(pos);
	}

	public void putLocalFloat(int index, float value) {
		int pos = getLB() + (index << 2);
		assert index < getLC() : "Local var overflow!" + pos + ">"
				+ (getLC() - 1);
		frames.put(getLTB() + index, TYPE_INT);
		frames.putFloat(pos, value);
	}

	// public synchronized native void foo();

	public long getLocalLong(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() - 1 : "Local var overflow!" + pos + ">"
				+ (getLC() - 2);
		assert frames.get(getLTB() + index) == TYPE_WIDE : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_WIDE;
		return frames.getLong(pos);
	}

	public void putLocalLong(int index, long value) {
		int pos = getLB() + (index << 2);
		assert index < getLC() - 1 : "Local var overflow!" + pos + ">"
				+ (getLC() - 2);
		frames.put(getLTB() + index, TYPE_WIDE);
		frames.put(getLTB() + index + 1, TYPE_WIDE2);
		frames.putLong(pos, value);
	}

	public double getLocalDouble(int index) {
		int pos = getLB() + (index << 2);
		assert index < getLC() - 1 : "Local var overflow!" + pos + ">"
				+ (getLC() - 2);
		assert frames.get(getLTB() + index) == TYPE_WIDE : "Type mismatch!"
				+ frames.get(getLTB() + index) + " should be " + TYPE_WIDE;
		return frames.getDouble(pos);
	}

	public void putLocalDouble(int index, double value) {
		int pos = getLB() + (index << 2);
		assert index < getLC() - 1 : "Local var overflow!" + pos + ">"
				+ (getLC() - 2);
		frames.put(getLTB() + index, TYPE_WIDE);
		frames.put(getLTB() + index + 1, TYPE_WIDE2);
		frames.putDouble(pos, value);
	}

	public ClassMethod getCurrentMethod() {
		return context.getMethodById(getMID());
	}

	public List<StackTraceElement> dumpStackTrace(List<StackTraceElement> list) {
		if (list == null) {
			list = new ArrayList<StackTraceElement>();
		}
		assert context.getConsole().debug(
				"######## DUMP STACK TRACE BEGIN #########");
		final int fp = getFP();
		StackTraceElement ste;
		while (getFP() > pFP) {
			ClassMethod mt = context.getMethodById(getMID());
			ClassBase clazz = mt.getOwner();
			LineNumber[] lnt = mt.getLineNumberTable();
			AttributeSourceFile source = (AttributeSourceFile) Attribute
					.getAttributeByName(clazz, "SourceFile");

			String declaringClass = clazz.getName();
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
				int lpc = getLPC();
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
			list.add(ste);
			setFP(getFP() - getSIZE());
		}
		assert context.getConsole().debug(
				"######## DUMP STACK TRACE END   #########");
		setFP(fp);
		return list;
	}

	public void fillUsedHandles(Set<Integer> tofill) {
		int fpbak = getFP();
		assert heap.isHandleValid(getThreadHandle());
		// context.getConsole().info("SCAN INITT->" + getThreadHandle());
		tofill.add(getThreadHandle());
		if (getCurrentThrowable() > 0) {
			assert heap.isHandleValid(getCurrentThrowable());
			// context.getConsole().info("SCAN INITT->" +
			// getCurrentThrowable());
			tofill.add(getCurrentThrowable());
		}
		StringBuilder out = new StringBuilder(64);
		while (getFP() > pFP) {
			int lb = getLB();
			int sb = getSB();
			int ltb = getLTB();
			int stb = getSTB();
			boolean assertion = false;
			assert assertion = true;
			if (assertion) {
				ClassMethod method = context.getMethodById(getMID());

				out.append("GC Method=");
				out.append(method.getUniqueName());
				out.append(" ");
				for (int i = 0, max = getLC(); i < max; i++) {
					out.append((char) frames.get(ltb + i));
				}
				out.append(" ");
				for (int i = 0, max = getSR(); i < max; i++) {
					out.append((char) frames.get(stb + i));
				}
				context.getConsole().debug(out.toString());
				out.setLength(0);
			}
			for (int i = 0, max = getLC(); i < max; i++) {
				if (frames.get(ltb + i) == TYPE_HANDLE) {
					int handle = frames.getInt(lb + (i << 2));
					if (handle > 0) {
						assert heap.isHandleValid(handle);
						// context.getConsole().info("SCAN INITT->" + handle);
						tofill.add(handle);
					}
				}
			}

			for (int i = 0, max = getSR(); i < max; i++) {
				if (frames.get(stb + i) == TYPE_HANDLE) {
					int handle = frames.getInt(sb + (i << 2));
					if (handle > 0) {
						assert heap.isHandleValid(handle);
						// context.getConsole().info("SCAN INITT->" + handle);
						tofill.add(handle);
					}
				}
			}

			setFP(getFP() - getSIZE());
		}
		setFP(fpbak);
	}
}
