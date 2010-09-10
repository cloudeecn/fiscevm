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
package com.cirnoworks.fisce.vm;

import java.util.List;
import java.util.Set;

import com.cirnoworks.fisce.vm.data.ClassMethod;
import com.cirnoworks.fisce.vm.data.StackTraceElement;

/**
 * 
 * @author yuxuanhuang
 */
public interface IThread {
	/**
	 * 总线程栈的大小
	 */
	public static final int STACK_SIZE = 1048576;

	/**
	 * 线程可以被调度
	 */
	public static final int STAT_ALIVE = 0;
	/**
	 * 线程已经死亡
	 */
	public static final int STAT_DEAD = 1;

	/**
	 * 往当前执行栈中压入一个整形变量
	 * 
	 * @param value
	 *            值
	 */
	void pushInt(int value);

	/**
	 * 往当前执行栈中压入一个句柄
	 * 
	 * @param value
	 *            值
	 */
	void pushHandle(int handle);

	/**
	 * 往当前执行栈中压入一个长整形变量
	 * 
	 * @param value
	 *            值
	 */
	void pushLong(long value);

	/**
	 * 往当前执行栈中压入一个浮点变量
	 * 
	 * @param value
	 *            值
	 */
	void pushFloat(float value);

	/**
	 * 往当前执行栈中压入一个双精度浮点变量
	 * 
	 * @param value
	 *            值
	 */
	void pushDouble(double value);

	/**
	 * 从当前执行栈中弹出一个整形变量
	 * 
	 * @return 值
	 */
	int popInt();

	/**
	 * 从当前执行栈中弹出一个句柄
	 * 
	 * @return 值
	 */
	int popHandle();

	/**
	 * 从当前执行栈中弹出一个长整形变量
	 * 
	 * @return 值
	 */
	long popLong();

	/**
	 * 从当前执行栈中弹出一个浮点变量
	 * 
	 * @return 值
	 */
	float popFloat();

	/**
	 * 从当前执行栈中弹出一个双精度浮点变量
	 * 
	 * @return 值
	 */
	double popDouble();

	/**
	 * 从一个句柄型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	int getLocalHandle(int index);

	/**
	 * 给一个句柄型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalHandle(int index, int value);

	/**
	 * 从一个整型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	int getLocalInt(int index);

	/**
	 * 给一个整型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalInt(int index, int value);

	/**
	 * 从一个长整型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	long getLocalLong(int index);

	/**
	 * 给一个长整型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalLong(int index, long value);
	/**
	 * 从一个双精度浮点型局部变量取值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @return 值
	 */
	double getLocalDouble(int index);
	/**
	 * 给一个双精度浮点型局部变量赋值
	 * 
	 * @param index
	 *            局部变量的索引
	 * @param value
	 *            值
	 */
	void putLocalDouble(int index, double value);

	/**
	 * 压入线程栈，准备调用别的方法
	 * @param mt 要调用的方法
	 */
	void pushFrame(ClassMethod mt);

	/**
	 * 取得当前正在执行的方法
	 * @return 当前正在执行的方法
	 */
	ClassMethod getCurrentMethod();

	/**
	 * 为线程导出调用栈
	 * @param list 用于存放调用栈的列表，如果传入null的话就产生一个新的
	 * @return 到处后的调用栈列表
	 */
	List<StackTraceElement> dumpStackTrace(List<StackTraceElement> list);

	/**
	 * 取当前线程的状态
	 * @return
	 */
	int getStatus();

	/**
	 * 取线程对应的Thread对象的句柄
	 * 
	 * @return 线程对应的Thread对象的句柄
	 */
	int getThreadHandle();

	/**
	 * 设置执行完当前的指令后是否把控制权交回现成管理器
	 * @param yield
	 */
	void setYield(boolean yield);

	/**
	 * 将这个线程使用到的句柄填充到集合中
	 * @param tofill 待填充的集合
	 */
	void fillUsedHandles(Set<Integer> tofill);

	/**
	 * 取当前线程的线程id
	 * @return 当前线程的线程id
	 */
	int getThreadId();

	public static final int NOP = 0x00;
	public static final int ACONST_NULL = 0x01;
	public static final int ICONST_M1 = 0x02;
	public static final int ICONST_0 = 0x03;
	public static final int ICONST_1 = 0x04;
	public static final int ICONST_2 = 0x05;
	public static final int ICONST_3 = 0x06;
	public static final int ICONST_4 = 0x07;

	public static final int ICONST_5 = 0x08;
	public static final int LCONST_0 = 0x09;
	public static final int LCONST_1 = 0x0A;
	public static final int FCONST_0 = 0x0B;
	public static final int FCONST_1 = 0x0C;
	public static final int FCONST_2 = 0x0D;
	public static final int DCONST_0 = 0x0E;
	public static final int DCONST_1 = 0x0F;

	public static final int BIPUSH = 0x10;
	public static final int SIPUSH = 0x11;
	public static final int LDC = 0x12;
	public static final int LDC_W = 0x13;
	public static final int LDC2_W = 0x14;
	public static final int ILOAD = 0x15;
	public static final int LLOAD = 0x16;
	public static final int FLOAD = 0x17;

	public static final int DLOAD = 0x18;
	public static final int ALOAD = 0x19;
	public static final int ILOAD_0 = 0x1A;
	public static final int ILOAD_1 = 0x1B;
	public static final int ILOAD_2 = 0x1C;
	public static final int ILOAD_3 = 0x1D;
	public static final int LLOAD_0 = 0x1E;
	public static final int LLOAD_1 = 0x1F;

	public static final int LLOAD_2 = 0x20;
	public static final int LLOAD_3 = 0x21;
	public static final int FLOAD_0 = 0x22;
	public static final int FLOAD_1 = 0x23;
	public static final int FLOAD_2 = 0x24;
	public static final int FLOAD_3 = 0x25;
	public static final int DLOAD_0 = 0x26;
	public static final int DLOAD_1 = 0x27;

	public static final int DLOAD_2 = 0x28;
	public static final int DLOAD_3 = 0x29;
	public static final int ALOAD_0 = 0x2A;
	public static final int ALOAD_1 = 0x2B;
	public static final int ALOAD_2 = 0x2C;
	public static final int ALOAD_3 = 0x2D;
	public static final int IALOAD = 0x2E;
	public static final int LALOAD = 0x2F;

	public static final int FALOAD = 0x30;
	public static final int DALOAD = 0x31;
	public static final int AALOAD = 0x32;
	public static final int BALOAD = 0x33;
	public static final int CALOAD = 0x34;
	public static final int SALOAD = 0x35;
	public static final int ISTORE = 0x36;
	public static final int LSTORE = 0x37;

	public static final int FSTORE = 0x38;
	public static final int DSTORE = 0x39;
	public static final int ASTORE = 0x3A;
	public static final int ISTORE_0 = 0x3B;
	public static final int ISTORE_1 = 0x3C;
	public static final int ISTORE_2 = 0x3D;
	public static final int ISTORE_3 = 0x3E;
	public static final int LSTORE_0 = 0x3F;

	public static final int LSTORE_1 = 0x40;
	public static final int LSTORE_2 = 0x41;
	public static final int LSTORE_3 = 0x42;
	public static final int FSTORE_0 = 0x43;
	public static final int FSTORE_1 = 0x44;
	public static final int FSTORE_2 = 0x45;
	public static final int FSTORE_3 = 0x46;
	public static final int DSTORE_0 = 0x47;

	public static final int DSTORE_1 = 0x48;
	public static final int DSTORE_2 = 0x49;
	public static final int DSTORE_3 = 0x4A;
	public static final int ASTORE_0 = 0x4B;
	public static final int ASTORE_1 = 0x4C;
	public static final int ASTORE_2 = 0x4D;
	public static final int ASTORE_3 = 0x4E;
	public static final int IASTORE = 0x4F;

	public static final int LASTORE = 0x50;
	public static final int FASTORE = 0x51;
	public static final int DASTORE = 0x52;
	public static final int AASTORE = 0x53;
	public static final int BASTORE = 0x54;
	public static final int CASTORE = 0x55;
	public static final int SASTORE = 0x56;
	public static final int POP = 0x57;

	public static final int POP2 = 0x58;
	public static final int DUP = 0x59;
	public static final int DUP_X1 = 0x5A;
	public static final int DUP_X2 = 0x5B;
	public static final int DUP2 = 0x5C;
	public static final int DUP2_X1 = 0x5D;
	public static final int DUP2_X2 = 0x5E;
	public static final int SWAP = 0x5F;

	public static final int IADD = 0x60;
	public static final int LADD = 0x61;
	public static final int FADD = 0x62;
	public static final int DADD = 0x63;
	public static final int ISUB = 0x64;
	public static final int LSUB = 0x65;
	public static final int FSUB = 0x66;
	public static final int DSUB = 0x67;

	public static final int IMUL = 0x68;
	public static final int LMUL = 0x69;
	public static final int FMUL = 0x6A;
	public static final int DMUL = 0x6B;
	public static final int IDIV = 0x6C;
	public static final int LDIV = 0x6D;
	public static final int FDIV = 0x6E;
	public static final int DDIV = 0x6F;

	public static final int IREM = 0x70;
	public static final int LREM = 0x71;
	public static final int FREM = 0x72;
	public static final int DREM = 0x73;
	public static final int INEG = 0x74;
	public static final int LNEG = 0x75;
	public static final int FNEG = 0x76;
	public static final int DNEG = 0x77;

	public static final int ISHL = 0x78;
	public static final int LSHL = 0x79;
	public static final int ISHR = 0x7A;
	public static final int LSHR = 0x7B;
	public static final int IUSHR = 0x7C;
	public static final int LUSHR = 0x7D;
	public static final int IAND = 0x7E;
	public static final int LAND = 0x7F;

	public static final int IOR = 0x80;
	public static final int LOR = 0x81;
	public static final int IXOR = 0x82;
	public static final int LXOR = 0x83;
	public static final int IINC = 0x84;
	public static final int I2L = 0x85;
	public static final int I2F = 0x86;
	public static final int I2D = 0x87;

	public static final int L2I = 0x88;
	public static final int L2F = 0x89;
	public static final int L2D = 0x8A;
	public static final int F2I = 0x8B;
	public static final int F2L = 0x8C;
	public static final int F2D = 0x8D;
	public static final int D2I = 0x8E;
	public static final int D2L = 0x8F;

	public static final int D2F = 0x90;
	public static final int I2B = 0x91;
	public static final int I2C = 0x92;
	public static final int I2S = 0x93;
	public static final int LCMP = 0x94;
	public static final int FCMPL = 0x95;
	public static final int FCMPG = 0x96;
	public static final int DCMPL = 0x97;

	public static final int DCMPG = 0x98;
	public static final int IFEQ = 0x99;
	public static final int IFNE = 0x9A;
	public static final int IFLT = 0x9B;
	public static final int IFGE = 0x9C;
	public static final int IFGT = 0x9D;
	public static final int IFLE = 0x9E;
	public static final int IF_ICMPEQ = 0x9F;

	public static final int IF_ICMPNE = 0xA0;
	public static final int IF_ICMPLT = 0xA1;
	public static final int IF_ICMPGE = 0xA2;
	public static final int IF_ICMPGT = 0xA3;
	public static final int IF_ICMPLE = 0xA4;
	public static final int IF_ACMPEQ = 0xA5;
	public static final int IF_ACMPNE = 0xA6;
	public static final int GOTO = 0xA7;

	public static final int JSR = 0xA8;
	public static final int RET = 0xA9;
	public static final int TABLESWITCH = 0xAA;
	public static final int LOOKUPSWITCH = 0xAB;
	public static final int IRETURN = 0xAC;
	public static final int LRETURN = 0xAD;
	public static final int FRETURN = 0xAE;
	public static final int DRETURN = 0xAF;

	public static final int ARETURN = 0xB0;
	public static final int RETURN = 0xB1;
	public static final int GETSTATIC = 0xB2;
	public static final int PUTSTATIC = 0xB3;
	public static final int GETFIELD = 0xB4;
	public static final int PUTFIELD = 0xB5;
	public static final int INVOKEVIRTUAL = 0xB6;
	public static final int INVOKESPECIAL = 0xB7;

	public static final int INVOKESTATIC = 0xB8;
	public static final int INVOKEINTERFACE = 0xB9;
	public static final int UNUSED_BA = 0xBA;
	public static final int NEW = 0xBB;
	public static final int NEWARRAY = 0xBC;
	public static final int ANEWARRAY = 0xBD;
	public static final int ARRAYLENGTH = 0xBE;
	public static final int ATHROW = 0xBF;

	public static final int CHECKCAST = 0xC0;
	public static final int INSTANCEOF = 0xC1;
	public static final int MONITORENTER = 0xC2;
	public static final int MONITOREXIT = 0xC3;
	public static final int WIDE = 0xC4;
	public static final int MULTIANEWARRAY = 0xC5;
	public static final int IFNULL = 0xC6;
	public static final int IFNONNULL = 0xC7;

	public static final int GOTO_W = 0xC8;
	public static final int JSR_W = 0xC9;
	public static final int BREAKPOINT = 0xCA;

	public static final String[] OP_NAME = new String[] {//
	"NOP", /* 0x00 */
	"ACONST_NULL", /* 0x01 */
	"ICONST_M1", /* 0x02 */
	"ICONST_0", /* 0x03 */
	"ICONST_1", /* 0x04 */
	"ICONST_2", /* 0x05 */
	"ICONST_3", /* 0x06 */
	"ICONST_4", /* 0x07 */
	"ICONST_5", /* 0x08 */
	"LCONST_0", /* 0x09 */
	"LCONST_1", /* 0x0A */
	"FCONST_0", /* 0x0B */
	"FCONST_1", /* 0x0C */
	"FCONST_2", /* 0x0D */
	"DCONST_0", /* 0x0E */
	"DCONST_1", /* 0x0F */
	"BIPUSH", /* 0x10 */
	"SIPUSH", /* 0x11 */
	"LDC", /* 0x12 */
	"LDC_W", /* 0x13 */
	"LDC2_W", /* 0x14 */
	"ILOAD", /* 0x15 */
	"LLOAD", /* 0x16 */
	"FLOAD", /* 0x17 */
	"DLOAD", /* 0x18 */
	"ALOAD", /* 0x19 */
	"ILOAD_0", /* 0x1A */
	"ILOAD_1", /* 0x1B */
	"ILOAD_2", /* 0x1C */
	"ILOAD_3", /* 0x1D */
	"LLOAD_0", /* 0x1E */
	"LLOAD_1", /* 0x1F */
	"LLOAD_2", /* 0x20 */
	"LLOAD_3", /* 0x21 */
	"FLOAD_0", /* 0x22 */
	"FLOAD_1", /* 0x23 */
	"FLOAD_2", /* 0x24 */
	"FLOAD_3", /* 0x25 */
	"DLOAD_0", /* 0x26 */
	"DLOAD_1", /* 0x27 */
	"DLOAD_2", /* 0x28 */
	"DLOAD_3", /* 0x29 */
	"ALOAD_0", /* 0x2A */
	"ALOAD_1", /* 0x2B */
	"ALOAD_2", /* 0x2C */
	"ALOAD_3", /* 0x2D */
	"IALOAD", /* 0x2E */
	"LALOAD", /* 0x2F */
	"FALOAD", /* 0x30 */
	"DALOAD", /* 0x31 */
	"AALOAD", /* 0x32 */
	"BALOAD", /* 0x33 */
	"CALOAD", /* 0x34 */
	"SALOAD", /* 0x35 */
	"ISTORE", /* 0x36 */
	"LSTORE", /* 0x37 */
	"FSTORE", /* 0x38 */
	"DSTORE", /* 0x39 */
	"ASTORE", /* 0x3A */
	"ISTORE_0", /* 0x3B */
	"ISTORE_1", /* 0x3C */
	"ISTORE_2", /* 0x3D */
	"ISTORE_3", /* 0x3E */
	"LSTORE_0", /* 0x3F */
	"LSTORE_1", /* 0x40 */
	"LSTORE_2", /* 0x41 */
	"LSTORE_3", /* 0x42 */
	"FSTORE_0", /* 0x43 */
	"FSTORE_1", /* 0x44 */
	"FSTORE_2", /* 0x45 */
	"FSTORE_3", /* 0x46 */
	"DSTORE_0", /* 0x47 */
	"DSTORE_1", /* 0x48 */
	"DSTORE_2", /* 0x49 */
	"DSTORE_3", /* 0x4A */
	"ASTORE_0", /* 0x4B */
	"ASTORE_1", /* 0x4C */
	"ASTORE_2", /* 0x4D */
	"ASTORE_3", /* 0x4E */
	"IASTORE", /* 0x4F */
	"LASTORE", /* 0x50 */
	"FASTORE", /* 0x51 */
	"DASTORE", /* 0x52 */
	"AASTORE", /* 0x53 */
	"BASTORE", /* 0x54 */
	"CASTORE", /* 0x55 */
	"SASTORE", /* 0x56 */
	"POP", /* 0x57 */
	"POP2", /* 0x58 */
	"DUP", /* 0x59 */
	"DUP_X1", /* 0x5A */
	"DUP_X2", /* 0x5B */
	"DUP2", /* 0x5C */
	"DUP2_X1", /* 0x5D */
	"DUP2_X2", /* 0x5E */
	"SWAP", /* 0x5F */
	"IADD", /* 0x60 */
	"LADD", /* 0x61 */
	"FADD", /* 0x62 */
	"DADD", /* 0x63 */
	"ISUB", /* 0x64 */
	"LSUB", /* 0x65 */
	"FSUB", /* 0x66 */
	"DSUB", /* 0x67 */
	"IMUL", /* 0x68 */
	"LMUL", /* 0x69 */
	"FMUL", /* 0x6A */
	"DMUL", /* 0x6B */
	"IDIV", /* 0x6C */
	"LDIV", /* 0x6D */
	"FDIV", /* 0x6E */
	"DDIV", /* 0x6F */
	"IREM", /* 0x70 */
	"LREM", /* 0x71 */
	"FREM", /* 0x72 */
	"DREM", /* 0x73 */
	"INEG", /* 0x74 */
	"LNEG", /* 0x75 */
	"FNEG", /* 0x76 */
	"DNEG", /* 0x77 */
	"ISHL", /* 0x78 */
	"LSHL", /* 0x79 */
	"ISHR", /* 0x7A */
	"LSHR", /* 0x7B */
	"IUSHR", /* 0x7C */
	"LUSHR", /* 0x7D */
	"IAND", /* 0x7E */
	"LAND", /* 0x7F */
	"IOR", /* 0x80 */
	"LOR", /* 0x81 */
	"IXOR", /* 0x82 */
	"LXOR", /* 0x83 */
	"IINC", /* 0x84 */
	"I2L", /* 0x85 */
	"I2F", /* 0x86 */
	"I2D", /* 0x87 */
	"L2I", /* 0x88 */
	"L2F", /* 0x89 */
	"L2D", /* 0x8A */
	"F2I", /* 0x8B */
	"F2L", /* 0x8C */
	"F2D", /* 0x8D */
	"D2I", /* 0x8E */
	"D2L", /* 0x8F */
	"D2F", /* 0x90 */
	"I2B", /* 0x91 */
	"I2C", /* 0x92 */
	"I2S", /* 0x93 */
	"LCMP", /* 0x94 */
	"FCMPL", /* 0x95 */
	"FCMPG", /* 0x96 */
	"DCMPL", /* 0x97 */
	"DCMPG", /* 0x98 */
	"IFEQ", /* 0x99 */
	"IFNE", /* 0x9A */
	"IFLT", /* 0x9B */
	"IFGE", /* 0x9C */
	"IFGT", /* 0x9D */
	"IFLE", /* 0x9E */
	"IF_ICMPEQ", /* 0x9F */
	"IF_ICMPNE", /* 0xA0 */
	"IF_ICMPLT", /* 0xA1 */
	"IF_ICMPGE", /* 0xA2 */
	"IF_ICMPGT", /* 0xA3 */
	"IF_ICMPLE", /* 0xA4 */
	"IF_ACMPEQ", /* 0xA5 */
	"IF_ACMPNE", /* 0xA6 */
	"GOTO", /* 0xA7 */
	"JSR", /* 0xA8 */
	"RET", /* 0xA9 */
	"TABLESWITCH", /* 0xAA */
	"LOOKUPSWITCH", /* 0xAB */
	"IRETURN", /* 0xAC */
	"LRETURN", /* 0xAD */
	"FRETURN", /* 0xAE */
	"DRETURN", /* 0xAF */
	"ARETURN", /* 0xB0 */
	"RETURN", /* 0xB1 */
	"GETSTATIC", /* 0xB2 */
	"PUTSTATIC", /* 0xB3 */
	"GETFIELD", /* 0xB4 */
	"PUTFIELD", /* 0xB5 */
	"INVOKEVIRTUAL", /* 0xB6 */
	"INVOKESPECIAL", /* 0xB7 */
	"INVOKESTATIC", /* 0xB8 */
	"INVOKEINTERFACE", /* 0xB9 */
	"UNUSED_BA", /* 0xBA */
	"NEW", /* 0xBB */
	"NEWARRAY", /* 0xBC */
	"ANEWARRAY", /* 0xBD */
	"ARRAYLENGTH", /* 0xBE */
	"ATHROW", /* 0xBF */
	"CHECKCAST", /* 0xC0 */
	"INSTANCEOF", /* 0xC1 */
	"MONITORENTER", /* 0xC2 */
	"MONITOREXIT", /* 0xC3 */
	"WIDE", /* 0xC4 */
	"MULTIANEWARRAY", /* 0xC5 */
	"IFNULL", /* 0xC6 */
	"IFNONNULL", /* 0xC7 */
	"GOTO_W", /* 0xC8 */
	"JSR_W", /* 0xC9 */
	"BREAKPOINT"/* 0xCA */};
}
