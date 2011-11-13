/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
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

#include "fyc/Thread.h"
#if 1
#define CMD_BREAK  1
#define CMD_GOON  2
#define CMD_BACK  3

#define dSIZE  0
#define dMID  -1
#define dLC  -2
#define dLB  -3
#define dSC  -4
#define dSB  -5
#define dSR  -6
#define dLTB  -7
#define dSTB  -8
#define dLPC  -9
#define dPC  -10

#define iframe(thread,OFFSET) (*((thread)->frames+(OFFSET)))
#define lframe(thread,OFFSET) (*(_s8*)((thread)->frames+(OFFSET)))
#define fframe(thread,OFFSET) (*(float*)((thread)->frames+(OFFSET)))
#define dframe(thread,OFFSET) (*(double*)((thread)->frames+(OFFSET)))

#ifdef VERBOSE_LOG
static char *OP_NAME[] = { /**/
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
	"BREAKPOINT" /* 0xCA */};
#endif

void fy_threadSetCurrentThrowable(fy_VMContext *context, fy_thread *thread,
		jint handle, fy_exception *exception) {
	fy_class *clazz;
	if (handle != 0) {
		clazz = fy_heapGetClassOfObject(context, handle);
		if (fy_classCanCastTo(context, clazz, context->TOP_THROWABLE)) {
			thread->currentThrowable = handle;
		} else {
			char name[64];
			fy_strSPrint(name, 64, clazz->className);
			exception->exceptionType = exception_normal;
			strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
					"Java/lang/VirtualMachineError");
			sprintf_s(exception->exceptionDesc, 64, "Wrong object thrown: %s",
					name);

		}
	}
}

static jint getSIZE(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSIZE);
}

static void setSIZE(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dSIZE) = value;
}

static jint getMID(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dMID);
}

static void setMID(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dMID) = value;
}

static jint getLC(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dLC);
}

static void setLC(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dLC) = value;
}

static jint getLB(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dLB);
}

static void setLB(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dLB) = value;
}

static jint getSC(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSC);
}

static void setSC(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dSC) = value;
}

static jint getSB(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSB);
}

static void setSB(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dSB) = value;
}

static jint getSR(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSR);
}

static void setSR(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dSR) = value;
}

static jint incSR(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSR)++;
}

static jint decSR(fy_VMContext *context, fy_thread *thread) {
	return --iframe(thread,thread->fp+dSR);
}

static jint getLTB(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dLTB);
}

static void setLTB(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dLTB) = value;
}

static jint getSTB(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dSTB);
}

static void setSTB(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dSTB) = value;
}

static jint getLPC(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dLPC);
}

static void setLPC(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dLPC) = value;
}

static jint getPC(fy_VMContext *context, fy_thread *thread) {
	return iframe(thread,thread->fp+dPC);
}

static void setPC(fy_VMContext *context, fy_thread *thread, jint value) {
	iframe(thread,thread->fp+dPC) = value;
}

static jubyte nextU1(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 1);
	return code[pc];
}

static jbyte nextS1(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 1);
	return code[pc];
}

static jchar nextU2(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 2);
	return (jchar) fy_B2TOI(code[pc],code[pc+1]);
}

static jshort nextS2(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 2);
	return (jshort) fy_B2TOI(code[pc],code[pc+1]);
}

#if 0
static juint nextU4(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 4);
	return (juint) fy_B4TOI(code[pc],code[pc+1],code[pc+2],code[pc+3]);
}
#endif

static jint nextS4(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 4);
	return fy_B4TOI(code[pc],code[pc+1],code[pc+2],code[pc+3]);
}

#if 0

static julong nextU8(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 8);
	return (((julong) code[pc - 7]) << 56) + (((julong) code[pc - 6]) << 48)
	+ (((julong) code[pc - 5]) << 40) + (((julong) code[pc - 4]) << 32)
	+ (((julong) code[pc - 3]) << 24) + (((julong) code[pc - 2]) << 16)
	+ (((julong) code[pc - 1]) << 8) + code[pc];
}

static jlong nextS8(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 8);
	return (((julong) code[pc - 7]) << 56) + (((julong) code[pc - 6]) << 48)
	+ (((julong) code[pc - 5]) << 40) + (((julong) code[pc - 4]) << 32)
	+ (((julong) code[pc - 3]) << 24) + (((julong) code[pc - 2]) << 16)
	+ (((julong) code[pc - 1]) << 8) + code[pc];
}

#endif

static void movePC(fy_VMContext *context, fy_thread *thread, int ofs) {
	setPC(context, thread, getPC(context, thread) + ofs);
}

static void opGOTO(fy_VMContext *context, fy_thread *thread, int ofs) {
	setPC(context, thread, getLPC(context, thread) + ofs);
}

static jint processThrowable(fy_VMContext *context, fy_thread *thread,
		jint handle, jint lpc, fy_exception *exception) {
	fy_class *throwableClass;
	fy_class *handlerClass;
	jint i, imax;
	struct ExceptionTable *handlers;
	struct ExceptionTable *handler;
	jint target;
	DLOG("EXCEPTION HANDLE LOOKUP: LPC=%ld",lpc);
	throwableClass = fy_heapGetClassOfObject(context, handle);
	handlers = thread->method->exception_table;
	imax = thread->method->exception_table_length;
	for (i = 0; i < imax; i++) {
		handler = handlers + i;
		if (lpc >= handler->start_pc && lpc < handler->end_pc) {
			if (handler->ci.constantClass == NULL) {
				target = handler->handler_pc;
				break;
			} else {
				handlerClass = fy_vmLookupClassFromExceptionHandler(context,
						handler, exception);
				if (exception->exceptionType != exception_none) {
					target = 0;
					break;
				}
				if (fy_classCanCastTo(context, throwableClass, handlerClass)) {
					target = handler->handler_pc;
					break;
				}
			}
		}
	}
	return target;
	//Jump after found the target is move to run() for futher optimize
}

static jboolean clinit(fy_VMContext *context, fy_thread *thread,
		fy_class *clazz) {
	int tid;
	jboolean ret;
	if (clazz == NULL) {
		return FALSE;
	}
	tid = clazz->clinitThreadId;
	if (tid == -1 || tid == thread->threadId) {
		return FALSE;
	}
	if (clazz->clinit == NULL) {
		ret = clinit(context, thread, clazz->super);
		if (!ret) {
			clazz->clinitThreadId = -1;
		}
		return ret;
	} else {
		if (tid == 0) {
			setPC(context, thread, getLPC(context, thread));
			fy_threadPushFrame(context, thread, clazz->clinit);
			clazz->clinitThreadId = thread->threadId;
			return TRUE;
		} else {
			setPC(context, thread, getLPC(context, thread));
			thread->yield = TRUE;
#ifdef VERBOSE_LOG
			DLOG("Waiting for clinit...");
#endif
			return TRUE;
		}
	}
	return 0;
}

static jint threadmulitANewArray(fy_VMContext *context, fy_class *clazz,
		jint layers, jint *elementsForEachLayer, fy_exception *exception) {
	int i;
	jint handle;
	int size = elementsForEachLayer[0];
	int ret = fy_heapAllocateArray(context, clazz, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	if (layers > 1) {
		for (i = 0; i < layers; i++) {
			handle = threadmulitANewArray(context, clazz->ci.arr.contentClass,
					layers - 1, elementsForEachLayer + 1, exception);
			if (exception->exceptionType != exception_none) {
				return 0;
			}
			fy_heapPutArrayHandle(context, ret, i, handle, exception);
			if (exception->exceptionType != exception_none) {
				return 0;
			}
		}
	}
	return ret;
}

static void updateLocalBuf(fy_VMContext *context, fy_thread *thread) {
	if (thread->fp != 0) {
		thread->method = fy_threadGetCurrentMethod(context, thread);
		thread->code = thread->method->code;
	} else {
		thread->method = NULL;
		thread->code = NULL;
	}
}

static jint opLDC(fy_VMContext *context, fy_class *owner, jchar index,
		jbyte *type, fy_exception *exception) {
	ConstantStringInfo *constantStringInfo;
	ConstantClass *constantClass;
	fy_class *clazz;
	jint hvalue;
	switch (owner->constantTypes[index]) {
	case CONSTANT_Integer:
	case CONSTANT_Float:
		*type = TH_TYPE_INT;
		return ((ConstantIntegerFloatInfo*) (owner->constantPools[index]))->value;
	case CONSTANT_String:
		*type = TH_TYPE_HANDLE;
		constantStringInfo =
				((ConstantStringInfo*) (owner->constantPools[index]));
		if (!constantStringInfo->derefed) {
			hvalue = fy_heapLiteral(context, constantStringInfo->ci.string,
					exception);
			if (exception->exceptionType != exception_none) {
				return 0;
			}
			constantStringInfo->derefed = TRUE;
			constantStringInfo->ci.handle = hvalue;
		} else {
			hvalue = constantStringInfo->ci.handle;
		}
		return hvalue;
	case CONSTANT_Class:
		constantClass = (ConstantClass*) (owner->constantPools[index]);
		*type = TH_TYPE_HANDLE;
		clazz = fy_vmLookupClassFromConstant(context, constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return 0;
		}
		return clazz->classObjId;
	default:
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC type wrong! %I32d,%d", index, owner->constantTypes[index]);
		return 0;
	}
}

static jlong opLDC2(fy_VMContext *context, fy_class *owner, jchar index,
		fy_exception *exception) {
	switch (owner->constantTypes[index]) {
	case CONSTANT_Double:
	case CONSTANT_Long:
		return ((ConstantLongDoubleInfo*) (owner->constantPools[index]))->value;
	default:
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC2 type wrong! %I32d,%d", index,
				owner->constantTypes[index]);
		return 0;
	}
}

void fy_threadMonitorEnter(fy_VMContext *context, fy_thread *thread,
		jint handle) {
	/*TODO*/
}

void fy_threadMonitorExit(fy_VMContext *context, fy_thread *thread, jint handle) {
	/*TODO*/
}

static void fy_threadPutLocalType(fy_VMContext *context, fy_thread *thread,
		int pos, jint value, jbyte type) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);ASSERT(type!=TH_TYPE_HANDLE||(value>=0&&value<MAX_OBJECTS));
	iframe(thread,framepos) = value;
	iframe(thread,typepos) = type;

}

static void fy_threadPutLocalHandle(fy_VMContext *context, fy_thread *thread,
		int pos, jint handle) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);
	iframe(thread,framepos) = handle;
	iframe(thread,typepos) = TH_TYPE_HANDLE;
}

static void fy_threadPutLocalInt(fy_VMContext *context, fy_thread *thread,
		int pos, jint value) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);
	iframe(thread,framepos) = value;
	iframe(thread,typepos) = TH_TYPE_INT;
}

static void fy_threadPutLocalLong(fy_VMContext *context, fy_thread *thread,
		int pos, jlong value) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT((pos+1)<getLC(context,thread)&&pos>=0);
	iframe(thread,framepos) = fy_HOFL(value);
	iframe(thread,framepos+1) = fy_LOFL(value);
	iframe(thread,typepos) = TH_TYPE_WIDE;
	iframe(thread,typepos+1) = TH_TYPE_WIDE2;
}
#if 0
static void fy_threadPutLocalReturn(fy_VMContext *context, fy_thread *thread, int pos,
		jint value) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);
	iframe(thread,framepos) = value;
	iframe(thread,typepos) = TH_TYPE_RETURN;
}

static jint fy_threadGetLocalType(fy_VMContext *context, fy_thread *thread, int pos,
		jbyte *type) {
	int framepos = getLB(context, thread) + pos;
	int typepos = getLTB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);
	*type = iframe(thread,typepos);
	return iframe(thread,framepos);
}
#endif
static jint fy_threadGetLocalHandle(fy_VMContext *context, fy_thread *thread,
		int pos) {
	int framepos = getLB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);ASSERT(iframe(thread,getLTB(context, thread) + pos)==TH_TYPE_HANDLE);
	return iframe(thread,framepos);
}

static jint fy_threadGetLocalInt(fy_VMContext *context, fy_thread *thread,
		int pos) {
	int framepos = getLB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);ASSERT(iframe(thread,getLTB(context, thread) + pos)==TH_TYPE_INT);
	return iframe(thread,framepos);
}

static jlong fy_threadGetLocalLong(fy_VMContext *context, fy_thread *thread,
		int pos) {
	int framepos = getLB(context, thread) + pos;
	ASSERT((pos<getLC(context,thread))&&(pos>=0));ASSERT(iframe(thread,getLTB(context, thread) + pos)==TH_TYPE_WIDE);
	return fy_I2TOL(iframe(thread,framepos), iframe(thread,framepos+1));
}
static jint fy_threadGetLocalReturn(fy_VMContext *context, fy_thread *thread,
		int pos) {
	int framepos = getLB(context, thread) + pos;
	ASSERT(pos<getLC(context,thread)&&pos>=0);ASSERT(iframe(thread,getLTB(context, thread) + pos)==TH_TYPE_RETURN);
	return iframe(thread,framepos);
}

void fy_threadPushType(fy_VMContext *context, fy_thread *thread, jint value,
		jbyte type) {
	ASSERT(getSR(context,thread)<getSC(context,thread));ASSERT(type!=TH_TYPE_HANDLE || (value>=0||value<MAX_OBJECTS));
	iframe(thread,getSB(context,thread)+getSR(context,thread)) = value;
	iframe(thread,getSTB(context,thread)+incSR(context,thread)) = type;
}

void fy_threadPushHandle(fy_VMContext *context, fy_thread *thread, jint value) {
	ASSERT(getSR(context,thread)<getSC(context,thread));ASSERT(value>=0||value<MAX_OBJECTS);
	iframe(thread,getSB(context,thread)+getSR(context,thread)) = value;
	iframe(thread,getSTB(context,thread)+incSR(context,thread)) =
			TH_TYPE_HANDLE;
}

void fy_threadPushInt(fy_VMContext *context, fy_thread *thread, jint value) {
	ASSERT(getSR(context,thread)<getSC(context,thread));
	iframe(thread,getSB(context,thread)+getSR(context,thread)) = value;
	iframe(thread,getSTB(context,thread)+incSR(context,thread)) = TH_TYPE_INT;
}

void fy_threadPushFloat(fy_VMContext *context, fy_thread *thread, jfloat value) {
	fy_threadPushInt(context, thread, fy_floatToInt(value));
}

void fy_threadPushLong(fy_VMContext *context, fy_thread *thread, jlong value) {
	ASSERT(getSR(context,thread)<getSC(context,thread));
	iframe(thread,getSB(context,thread)+getSR(context,thread)) = fy_HOFL(value);
	iframe(thread,getSTB(context,thread)+incSR(context,thread)) = TH_TYPE_WIDE;
	iframe(thread,getSB(context,thread)+getSR(context,thread)) = fy_LOFL(value);
	iframe(thread,getSTB(context,thread)+incSR(context,thread)) = TH_TYPE_WIDE2;
}

void fy_threadPushDouble(fy_VMContext *context, fy_thread *thread,
		jdouble value) {
	fy_threadPushLong(context, thread, fy_doubleToLong(value));
}

jint fy_threadPopType(fy_VMContext *context, fy_thread *thread, jbyte *type) {
	*type = iframe(thread,getSTB(context,thread)+decSR(context,thread));
	ASSERT(getSR(context,thread)>=0);
	return iframe(thread,getSB(context,thread)+getSR(context,thread));
}

jint fy_threadPopHandle(fy_VMContext *context, fy_thread *thread) {
	ASSERT(getSR(context,thread)>0);ASSERT(
			iframe(thread,getSTB(context,thread)+getSR(context,thread)-1)==TH_TYPE_HANDLE);
	return iframe(thread,getSB(context,thread)+decSR(context,thread));
}

jint fy_threadPopInt(fy_VMContext *context, fy_thread *thread) {
	ASSERT(getSR(context,thread)>0);ASSERT(
			iframe(thread,getSTB(context,thread)+getSR(context,thread)-1)==TH_TYPE_INT);
	return iframe(thread,getSB(context,thread)+decSR(context,thread));
}

jfloat fy_threadPopFloat(fy_VMContext *context, fy_thread *thread) {
	ASSERT(getSR(context,thread)>0);ASSERT(
			iframe(thread,getSTB(context,thread)+getSR(context,thread)-1)==TH_TYPE_HANDLE);
	return fy_intToFloat(
			iframe(thread,getSB(context,thread)+decSR(context,thread)));
}

jlong fy_threadPopLong(fy_VMContext *context, fy_thread *thread) {
	jint value1, value2;
	ASSERT(getSR(context,thread)>0);ASSERT(
			iframe(thread,getSTB(context,thread)+getSR(context,thread)-2)==TH_TYPE_WIDE);ASSERT(
			iframe(thread,getSTB(context,thread)+getSR(context,thread)-1)==TH_TYPE_WIDE2);
	value2 = iframe(thread,getSB(context,thread)+decSR(context,thread));
	value1 = iframe(thread,getSB(context,thread)+decSR(context,thread));
	return fy_I2TOL(value1, value2);
}

jdouble fy_threadPopDouble(fy_VMContext *context, fy_thread *thread) {
	return fy_longToDouble(fy_threadPopLong(context, thread));
}

void fy_threadInit(fy_VMContext *context, fy_thread *thread) {

}

void fy_threadGetFrameInfo(fy_VMContext *context, fy_thread *thread, char *out,
		size_t outSize) {
	sprintf_s(out, outSize,
			"STACK=[%I32d-%I32d) VARS=[%I32d-%I32d) FP=%I32d SIZE=%I32d %d",
			getSB(context, thread),
			getSB(context, thread) + getSC(context, thread),
			getLB(context, thread),
			getLB(context, thread) + getLC(context, thread), thread->fp,
			getSIZE(context, thread),
			thread->method->access_flags & fy_ACC_STATIC);
}

fy_method *fy_threadGetCurrentMethod(fy_VMContext *context, fy_thread *thread) {
	return context->methods[getMID(context, thread)];
}

void fy_threadPushFrame(fy_VMContext *context, fy_thread *thread,
		fy_method *method) {
	int ltsize = method->max_locals;
	int stsize = method->max_stack;
	int lsize = method->max_locals;
	int ssize = method->max_stack;
	int size = -dPC + 1 + ssize + lsize + stsize + ltsize;
	int ltb;
	int i, maxi;
#ifdef _DEBUG
	char buf[256];
	if (thread->fp > 0) {
		fy_threadGetFrameInfo(context, thread, buf, 256);
#ifdef VERBOSE_LOG
		DLOG
		(">>>Push frame from %s ", buf);
#endif
	} else {
#ifdef VERBOSE_LOG
		DLOG
		(">>>Pop frame prepare to enter!");
#endif
	}
#endif
	thread->fp += size;
	setSIZE(context, thread, size);
	setMID(context, thread, method->method_id);
	setLC(context, thread, method->max_locals);
	setLB(context, thread, thread->fp + dPC - lsize);
	setSC(context, thread, method->max_stack);
	setSB(context, thread, getLB(context, thread) - ssize);
	setLTB(context, thread, getSB(context, thread) - ltsize);
	setSTB(context, thread, getLTB(context, thread) - stsize);
	setSR(context, thread, 0);
	setPC(context, thread, 0);
	ltb = getLTB(context, thread);
	maxi = getLC(context, thread);
	for (i = 0; i < maxi; i++) {
		iframe(thread,ltb+i) = 0;
	}
	thread->frameCount++;
	updateLocalBuf(context, thread);
#ifdef _DEBUG
	if (thread->fp > 0) {
		fy_threadGetFrameInfo(context, thread, buf, 256);
#ifdef VERBOSE_LOG
		DLOG
		(">>>Push frame to %s ", buf);
#endif
	} else {
		vm_die("????");
	}
#endif
}

void fy_threadPopFrame(fy_VMContext *context, fy_thread *thread) {
#ifdef _DEBUG
	char msg[256];
	if (thread->fp > 0) {
		fy_threadGetFrameInfo(context, thread, msg, 256);
#ifdef VERBOSE_LOG
		DLOG
		(">>>Pop frame from %s", msg);
#endif
	}
#endif
	thread->fp -= getSIZE(context, thread);
	thread->frameCount--;
	updateLocalBuf(context, thread);
#ifdef _DEBUG
	if (thread->fp > 0) {
		fy_threadGetFrameInfo(context, thread, msg, 256);
#ifdef VERBOSE_LOG
		DLOG
		(">>>Pop frame to %s", msg);
#endif
	} else {
		printf(">>>Pop frame prepare to exit!");
	}
#endif
}

void fy_threadPushMethod(fy_VMContext *context, fy_thread *thread,
		fy_method *invoke, jboolean isStatic, int argsCount, jint *args,
		jbyte *types) {
	int i;
	fy_threadPushFrame(context, thread, invoke);
	for (i = 0; i < argsCount; i++) {
		fy_threadPutLocalType(context, thread, i, args[i], types[i]);
	}

	if (invoke->access_flags & fy_ACC_SYNCHRONIZED) {
		fy_threadMonitorEnter(context, thread,
				isStatic ? invoke->owner->classObjId : args[0]);
	}

}

void fy_threadCreateWithMethod(fy_VMContext *context, fy_thread *thread,
		int threadHandle, fy_method *method, fy_exception *exception) {
	fy_class *clazz;
	do {
		if (!fy_strEndsWith(method->uniqueName, context->sMainPostfix)) {
			fy_strPrint(method->uniqueName);
			printf("\n");
			fy_strPrint(context->sMainPostfix);
			printf("\n");
			vm_die("The boot method must be static void main(String[])");
		}
		if ((method->access_flags & fy_ACC_STATIC) == 0) {
			vm_die("The first method of a thread must have no return value.");
			break;
		}
		thread->handle = threadHandle;
		thread->fp = 0;
		fy_threadPushFrame(context, thread, method);
		clazz = fy_vmLookupClass(context, context->sStringArray, exception);
		if (exception->exceptionType != exception_none) {
			break;
		}
		fy_threadPutLocalHandle(context, thread, 0,
				fy_heapAllocateArray(context, clazz, 0, exception));
		if (exception->exceptionType != exception_none) {
			break;
		}

	} while (0);
}

void fy_threadCreateWithRun(fy_VMContext *context, fy_thread *thread,
		int handle, fy_exception *exception) {
	/*TODO*/
}

void fy_threadCreateWithData(fy_VMContext *context, fy_thread *thread,
		jbyte *data, jint length, fy_exception *exception) {
	/*TODO*/
}

void fy_threadFillException(fy_VMContext *context, fy_thread *thread, juint lpc,
		juint handle, fy_exception *exception) {
	//TODO
#define fy_simpleErrorHandle if(exception->exceptionType != exception_none) return
	fy_class *clazz, *array;
	fy_field *declaringClass, *methodName, *fileName, *lineNumber;
	juint arrayHandle;

	clazz = fy_vmLookupClass(context, context.sStackTraceElement, exception);
	fy_simpleErrorHandle;
	array = fy_vmLookupClass(context, context.sStackTraceElementArray,
			exception);
	fy_simpleErrorHandle;

	declaringClass = fy_vmGetField(context,
			context->sStackTraceElementDeclaringClass);
	if (declaringClass == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_sf(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find field %s.",
				context->sStackTraceElementDeclaringClass);
	}

	methodName = fy_vmGetField(context, context->sStackTraceElementMethodName);
	if (methodName == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_sf(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find field %s.", context->sStackTraceElementMethodName);
	}

	fileName = fy_vmGetField(context, context->sStackTraceElementFileName);
	if (fileName == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_sf(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find field %s.", context->sStackTraceElementFileName);
	}

	lineNumber = fy_vmGetField(context, context->sStackTraceElementLineNumber);
	if (lineNumber == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		sprintf_sf(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find field %s.", context->sStackTraceElementLineNumber);
	}
	arrayHandle=fy_heapAllocateArray(context,array,thread->frameCount,exception);
	fy_simpleErrorHandle;


}

void fy_threadRun(fy_VMContext *context, fy_thread *thread, fy_message *message,
		jint ops) {

	jint index;
	jint aref;
	jint ivalue, ivalue2, ivalue3, ivalue4;
	jint hvalue;
	jint op;
	jshort svalue;

	fy_method *method = NULL;
	fy_exception *exception = &(message->body.exception);

	int i, opCount;
	jlong lvalue, lvalue2;
	jdouble dvalue1, dvalue2;
	jfloat fvalue1, fvalue2;
	fy_method *mvalue;
	fy_field *field = NULL;
	fy_class *clazz1 = NULL, *clazz2 = NULL;
	jchar ch1;
	fy_str str1;
	jbyte type, type2, type3, type4;
	jint *pivalue, *pivalue2;
	jbyte *pbvalue;
	fy_str *pstr1;
	jubyte op2;
	jboolean bvalue1;
	fy_nh *nh;
	char msg[256];

	ivalue = 0x6789abcd;
	lvalue = 0x6789abcdef1234LL;
	pstr1 = NULL;

	message->messageType = message_continue;
	exception->exceptionType = exception_none;
	for (opCount = 0; opCount < ops; opCount++) {
		method = thread->method;
		if (thread->fp == 0) {
			/*Time to quit!*/
			if (thread->currentThrowable) {
				DLOG("XXXXXXXXXXUnhandled Exception!!!XXXXXXXXXXXX");
				method = fy_vmGetMethod(context,
						context->sThrowablePrintStacktrace);
				ASSERT( method != NULL);
				fy_threadPushFrame(context, thread, method);
				fy_threadPutLocalHandle(context, thread, 0,
						thread->currentThrowable);
				thread->currentThrowable = 0;
				continue;
			} else {
				message->messageType = message_thread_dead;
				break;
			}
		}
		if (thread->currentThrowable) {
			ivalue = processThrowable(context, thread, thread->currentThrowable,
					getLPC(context, thread), exception);
			if (exception->exceptionType != exception_none) {
				message->messageType = message_exception;
			} else {
				if (ivalue >= 0) {
					setSR(context, thread, 0);
					fy_threadPushHandle(context, thread, ivalue);
					thread->currentThrowable = 0;
					setPC(context, thread, 0);
				} else {
					if (method->access_flags & fy_ACC_SYNCHRONIZED) {
						if (method->access_flags & fy_ACC_STATIC) {
							fy_threadMonitorExit(context, thread,
									method->owner->classObjId);
						} else {
							fy_threadMonitorExit(
									context,
									thread,
									fy_threadGetLocalHandle(context, thread,
											0));
						}
					}
					fy_threadPopFrame(context, thread);
				}
			}
		} else {

			/*RUN_ONE_INST!!!!!*/

			setLPC(context, thread, getPC(context, thread));
			op = nextU1(context, thread);
			if (method->clinit) {
				if (clinit(context, thread, method->owner->super)) {
					continue;
				}
			}

#ifdef _DEBUG
#ifdef VERBOSE_LOG
			fy_strSPrint(msg, 256, method->uniqueName);
			DLOG("##%6d/%6d %s %d %s SR=%d\n", opCount, ops, msg,
					getLPC(context, thread), OP_NAME[op], getSR(context, thread));
#endif
#endif

			switch (op) {
			case AALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_threadPushHandle(context, thread,
						fy_heapGetArrayHandle(context, aref, index, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case IALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_threadPushInt(context, thread,
						fy_heapGetArrayInt(context, aref, index, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case AASTORE: {
				hvalue = fy_threadPopHandle(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				DLOG("AASTORE %d[%d]=%d", aref, index, hvalue);
				clazz1 = fy_heapGetClassOfObject(context, aref);
				clazz2 = clazz1->ci.arr.contentClass;
				if (hvalue != 0
						&& !fy_classCanCastTo(context,
								fy_heapGetClassOfObject(context, hvalue),
								clazz2)) {
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArrayStoreException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Data type not compatable!");
					message->messageType = message_exception;
					break;/*EXCEPTION_THROWN*/
				}
				fy_heapPutArrayHandle(context, aref, index, hvalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case IASTORE: {
				ivalue = fy_threadPopInt(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);

				fy_heapPutArrayInt(context, aref, index, ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case ACONST_NULL: {
				fy_threadPushHandle(context, thread, 0);
				break;
			}

			case ILOAD:
			case FLOAD: {

				index = nextU1(context, thread);
				ivalue = fy_threadGetLocalInt(context, thread, index);
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case ALOAD: {
				index = nextU1(context, thread);
				ivalue = fy_threadGetLocalHandle(context, thread, index);
				fy_threadPushHandle(context, thread, ivalue);
				break;
			}
			case ILOAD_0:
			case FLOAD_0: {
				fy_threadPushInt(context, thread,
						fy_threadGetLocalInt(context, thread, 0));
				break;
			}
			case ALOAD_0: {
				fy_threadPushHandle(context, thread,
						fy_threadGetLocalHandle(context, thread, 0));
				break;
			}
			case ILOAD_1:
			case FLOAD_1: {
				fy_threadPushInt(context, thread,
						fy_threadGetLocalInt(context, thread, 1));
				break;
			}
			case ALOAD_1: {
				fy_threadPushHandle(context, thread,
						fy_threadGetLocalHandle(context, thread, 1));
				break;
			}
			case ILOAD_2:
			case FLOAD_2: {
				fy_threadPushInt(context, thread,
						fy_threadGetLocalInt(context, thread, 2));
				break;
			}
			case ALOAD_2: {
				fy_threadPushHandle(context, thread,
						fy_threadGetLocalHandle(context, thread, 2));
				break;
			}
			case ILOAD_3:
			case FLOAD_3: {
				ivalue = fy_threadGetLocalInt(context, thread, 3);
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case ALOAD_3: {
				fy_threadPushHandle(context, thread,
						fy_threadGetLocalHandle(context, thread, 3));
				break;
			}
			case ANEWARRAY: {
				ch1 = nextU2(context, thread);
				index = fy_threadPopInt(context, thread);
				if (index < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IndexOutOfBoundException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d", index);
					break;
				}

				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ch1],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;/*EXCEPTION_THROWN*/
				}
				fy_strInit(context, &str1, 64);
				if (clazz1->type == obj) {
					fy_strAppendUTF8(context, &str1, "[L", 3);
					fy_strAppend(context, &str1, clazz1->className);
					fy_strAppendUTF8(context, &str1, ";", 3);
				} else if (clazz1->type == arr) {
					fy_strAppendUTF8(context, &str1, "[", 3);
					fy_strAppend(context, &str1, clazz1->className);
				}
				clazz2 = fy_vmLookupClass(context, &str1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_strDestroy(context, &str1);
				fy_threadPushHandle(
						context,
						thread,
						fy_heapAllocateArray(context, clazz2, index,
								exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case IRETURN:
			case FRETURN: {
				ivalue = fy_threadPopInt(context, thread);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread,
								fy_threadGetLocalHandle(context, thread, 0));
					}
				}
				fy_threadPopFrame(context, thread);
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case ARETURN: {
				aref = fy_threadPopHandle(context, thread);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread,
								fy_threadGetLocalHandle(context, thread, 0));
					}
				}
				fy_threadPopFrame(context, thread);
				fy_threadPushHandle(context, thread, aref);
				break;
			}
			case ARRAYLENGTH: {
				aref = fy_threadPopHandle(context, thread);
				index = fy_heapArrayLength(context, aref, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushInt(context, thread, index);
				break;
			}
			case ISTORE:
			case FSTORE: {
				index = nextU1(context, thread);
				ivalue = fy_threadPopInt(context, thread);

				fy_threadPutLocalInt(context, thread, index, ivalue);
				break;
			}
			case ASTORE: {
				index = nextU1(context, thread);
				aref = fy_threadPopType(context, thread, &type);
				fy_threadPutLocalType(context, thread, index, aref, type);
				break;
			}
			case ISTORE_0:
			case FSTORE_0: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPutLocalInt(context, thread, 0, ivalue);
				break;
			}
			case ASTORE_0: {
				aref = fy_threadPopType(context, thread, &type);
				fy_threadPutLocalType(context, thread, 0, aref, type);
				break;
			}
			case ISTORE_1:
			case FSTORE_1: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPutLocalInt(context, thread, 1, ivalue);
				break;
			}
			case ASTORE_1: {
				aref = fy_threadPopType(context, thread, &type);
				fy_threadPutLocalType(context, thread, 1, aref, type);
				break;
			}
			case ISTORE_2:
			case FSTORE_2: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPutLocalInt(context, thread, 2, ivalue);
				break;
			}
			case ASTORE_2: {
				aref = fy_threadPopType(context, thread, &type);
				fy_threadPutLocalType(context, thread, 2, aref, type);
				break;
			}
			case ISTORE_3:
			case FSTORE_3: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPutLocalInt(context, thread, 3, ivalue);
				break;
			}
			case ASTORE_3: {
				aref = fy_threadPopType(context, thread, &type);
				ASSERT(type == TH_TYPE_HANDLE || type == TH_TYPE_RETURN);
				fy_threadPutLocalType(context, thread, 3, aref, type);
				break;
			}
			case ATHROW: {
				aref = fy_threadPopHandle(context, thread);
				thread->currentThrowable = aref;
				break;
			}
			case BALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				ivalue = fy_heapGetArrayByte(context, aref, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case BASTORE: {
				ivalue = fy_threadPopInt(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_heapPutArrayByte(context, aref, index, (jbyte) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case BIPUSH: {
				ivalue = nextS1(context, thread);
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case FALOAD:
			case CALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				ivalue = fy_heapGetArrayInt(context, aref, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case FASTORE:
			case CASTORE: {
				ivalue = fy_threadPopInt(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_heapPutArrayInt(context, aref, index, ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case CHECKCAST: {
				index = nextU2(context, thread);
				aref = fy_threadPopHandle(context, thread);
				if (aref == 0) {
					fy_threadPushHandle(context, thread, aref);
				} else {
					clazz1 = fy_heapGetClassOfObject(context, aref);
					clazz2 = fy_vmLookupClassFromConstant(context,
							method->owner->constantPools[index], exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					if (fy_classCanCastTo(context, clazz1, clazz2)) {
						fy_threadPushHandle(context, thread, aref);
					} else {
						message->messageType = message_exception;
						exception->exceptionType = exception_normal;
						strcpy_s(exception->exceptionName,
								sizeof(exception->exceptionName),
								"java/lang/ClassCastException");
						fy_strInit(context, &str1, 64);
						fy_strAppendUTF8(context, &str1, "from ", 99);
						fy_strAppend(context, &str1, clazz1->className);
						fy_strAppendUTF8(context, &str1, "to ", 99);
						fy_strAppend(context, &str1, clazz2->className);
						fy_strSPrint(exception->exceptionDesc,
								sizeof(exception->exceptionDesc), &str1);
						fy_strDestroy(context, &str1);
					}
				}
				break;
			}
			case D2F: {
				fy_threadPushFloat(context, thread,
						(float) fy_threadPopDouble(context, thread));
				break;
			}
			case D2I: {
				fy_threadPushInt(context, thread,
						(int) fy_threadPopDouble(context, thread));
				break;
			}
			case D2L: {
				fy_threadPushLong(context, thread,
						(long) fy_threadPopDouble(context, thread));
				break;
			}
			case DADD: {
				fy_threadPushDouble(
						context,
						thread,
						fy_threadPopDouble(context, thread)
								+ fy_threadPopDouble(context, thread));
				break;
			}
			case DALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				dvalue1 = fy_heapGetArrayDouble(context, aref, index,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushDouble(context, thread, dvalue1);
				break;
			}
			case DASTORE: {
				dvalue1 = fy_threadPopDouble(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_heapPutArrayDouble(context, aref, index, dvalue1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case DCMPG:
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				if (fy_isnand(dvalue1) || fy_isnand(dvalue2)) {
					fy_threadPushInt(context, thread, 1);
				} else {
					fy_threadPushInt(
							context,
							thread,
							dvalue1 == dvalue2 ? 0 :
							(dvalue1 - dvalue2 > 0) ? 1 : -1);
					;;
				}
				break;
			case DCMPL: {
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				if (fy_isnand(dvalue1) || fy_isnand(dvalue2)) {
					fy_threadPushInt(context, thread, -1);
				} else {
					fy_threadPushInt(
							context,
							thread,
							dvalue1 == dvalue2 ? 0 :
							(dvalue1 - dvalue2 > 0) ? 1 : -1);
					;;
				}
				break;
			}
			case DCONST_0: {
				fy_threadPushDouble(context, thread, 0);
				break;
			}
			case DCONST_1: {
				fy_threadPushDouble(context, thread, 1);
				break;
			}
			case DDIV: {
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				fy_threadPushDouble(context, thread, dvalue1 / dvalue2);
				break;
			}
			case DMUL: {
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				fy_threadPushDouble(context, thread, dvalue1 * dvalue2);
				break;
			}
			case DNEG: {
				fy_threadPushDouble(context, thread,
						-fy_threadPopDouble(context, thread));
				break;
			}
			case DREM: {
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				if (dvalue2 == 0) {
					fy_threadPushDouble(context, thread, 0.0 / dvalue2);
				} else {
					fy_threadPushDouble(context, thread,
							dvalue1 - floor(dvalue1 / dvalue2) * dvalue2);
				}

				break;
			}
			case DRETURN: {
				dvalue1 = fy_threadPopDouble(context, thread);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread,
								fy_threadGetLocalHandle(context, thread, 0));
					}
				}
				fy_threadPopFrame(context, thread);
				fy_threadPushDouble(context, thread, dvalue1);
				break;
			}
			case DSUB: {
				dvalue2 = fy_threadPopDouble(context, thread);
				dvalue1 = fy_threadPopDouble(context, thread);
				fy_threadPushDouble(context, thread, dvalue1 - dvalue2);
				break;
			}
			case DUP: {
				ivalue = fy_threadPopType(context, thread, &type);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case DUP_X1: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case DUP_X2: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				ivalue3 = fy_threadPopType(context, thread, &type3);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue3, type3);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case DUP2: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case DUP2_X1: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				ivalue3 = fy_threadPopType(context, thread, &type3);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue3, type3);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case DUP2_X2: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				ivalue3 = fy_threadPopType(context, thread, &type3);
				ivalue4 = fy_threadPopType(context, thread, &type4);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue4, type4);
				fy_threadPushType(context, thread, ivalue3, type3);
				fy_threadPushType(context, thread, ivalue2, type2);
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case F2D: {
				fy_threadPushDouble(context, thread,
						fy_threadPopFloat(context, thread));
				break;
			}
			case F2I: {
				fy_threadPushInt(context, thread,
						(int) fy_threadPopFloat(context, thread));
				break;
			}
			case F2L: {
				fy_threadPushLong(context, thread,
						(long) fy_threadPopFloat(context, thread));
				break;
			}
			case FADD: {
				fy_threadPushFloat(
						context,
						thread,
						fy_threadPopFloat(context, thread)
								+ fy_threadPopFloat(context, thread));
				break;
			}
			case FCMPG: {
				fvalue2 = fy_threadPopFloat(context, thread);
				fvalue1 = fy_threadPopFloat(context, thread);
				if (fy_isnanf(fvalue1) || fy_isnanf(fvalue2)) {
					fy_threadPushInt(context, thread, 1);
				} else {
					fy_threadPushInt(
							context,
							thread,
							fvalue1 == fvalue2 ? 0 :
							(fvalue1 - fvalue2 > 0) ? 1 : -1);
					;;
				}
				break;
			}
			case FCMPL: {
				fvalue2 = fy_threadPopFloat(context, thread);
				fvalue1 = fy_threadPopFloat(context, thread);
				if (fy_isnanf(fvalue1) || fy_isnanf(fvalue2)) {
					fy_threadPushInt(context, thread, -1);
				} else {
					fy_threadPushInt(
							context,
							thread,
							fvalue1 == fvalue2 ? 0 :
							(fvalue1 - fvalue2 > 0) ? 1 : -1);
					;;
				}
				break;
			}
			case FCONST_0: {
				fy_threadPushFloat(context, thread, 0.0f);
				break;
			}
			case FCONST_1: {
				fy_threadPushFloat(context, thread, 1.0f);
				break;
			}
			case FCONST_2: {
				fy_threadPushFloat(context, thread, 2.0f);
				break;
			}
			case FDIV: {
				fvalue2 = fy_threadPopFloat(context, thread);
				fy_threadPushFloat(context, thread,
						fy_threadPopFloat(context, thread) / fvalue2);
				break;
			}
			case FMUL: {
				fy_threadPushFloat(
						context,
						thread,
						fy_threadPopFloat(context, thread)
								* fy_threadPopFloat(context, thread));
				break;
			}
			case FNEG: {
				fy_threadPushFloat(context, thread,
						-fy_threadPopFloat(context, thread));
				break;
			}
			case FREM: {
				fvalue2 = fy_threadPopFloat(context, thread);
				fvalue1 = fy_threadPopFloat(context, thread);
				if (fvalue2 == 0) {
					fy_threadPushFloat(context, thread, 0.0f / fvalue2);
				} else {
					fy_threadPushFloat(
							context,
							thread,
							fvalue1
									- (float) floor(fvalue1 / fvalue2) * fvalue2);
				}
				break;
			}
			case FSUB: {
				fvalue2 = fy_threadPopFloat(context, thread);
				fvalue1 = fy_threadPopFloat(context, thread);
				fy_threadPushFloat(context, thread, fvalue1 - fvalue2);
				break;
			}
			case GETFIELD: {
				index = nextU2(context, thread);
				aref = fy_threadPopHandle(context, thread);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[index]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				if (field->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"field %s is static", msg);
					break;
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J':
					lvalue = fy_heapGetFieldLong(context, aref, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushLong(context, thread, lvalue);
					break;
				case 'L':
				case '[':
					aref = fy_heapGetFieldHandle(context, aref, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushHandle(context, thread, aref);
					break;
				default:
					ivalue = fy_heapGetFieldInt(context, aref, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushInt(context, thread, ivalue);
					break;
				}
				break;
			}
			case GETSTATIC: {
				index = nextU2(context, thread);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[index]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				clazz1 = field->owner;
				if (clinit(context, thread, clazz1)) {
					break;
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J':
					lvalue = fy_heapGetStaticLong(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushLong(context, thread, lvalue);
					break;
				case 'L':
				case '[':
					aref = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushHandle(context, thread, aref);
					break;
				default:
					ivalue = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						break;
					}
					fy_threadPushInt(context, thread, ivalue);
					break;
				}
				break;
			}
			case GOTO: {
				opGOTO(context, thread, nextS2(context, thread));
				break;
			}
			case GOTO_W: {
				opGOTO(context, thread, nextS4(context, thread));
				break;
			}
			case I2B: {
				fy_threadPushInt(context, thread,
						(jbyte) fy_threadPopInt(context, thread));
				break;
			}
			case I2C: {
				fy_threadPushInt(context, thread,
						(jchar) fy_threadPopInt(context, thread));
				break;
			}
			case I2D: {
				fy_threadPushDouble(context, thread,
						(jdouble) fy_threadPopInt(context, thread));
				break;
			}
			case I2F: {
				fy_threadPushFloat(context, thread,
						(jfloat) fy_threadPopInt(context, thread));
				break;
			}
			case I2L: {
				fy_threadPushLong(context, thread,
						(jlong) fy_threadPopInt(context, thread));
				break;
			}
			case I2S: {
				fy_threadPushInt(context, thread,
						(jshort) fy_threadPopInt(context, thread));
				break;
			}
			case IADD: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(
						context,
						thread,
						(juint) ivalue
								+ (juint) fy_threadPopInt(context, thread));
				break;
			}
			case IAND: {
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread,
						ivalue & fy_threadPopInt(context, thread));
				break;
			}
			case ICONST_M1: {
				fy_threadPushInt(context, thread, -1);
				break;
			}
			case ICONST_0: {
				fy_threadPushInt(context, thread, 0);
				break;
			}
			case ICONST_1: {
				fy_threadPushInt(context, thread, 1);
				break;
			}
			case ICONST_2: {
				fy_threadPushInt(context, thread, 2);
				break;
			}
			case ICONST_3: {
				fy_threadPushInt(context, thread, 3);
				break;
			}
			case ICONST_4: {
				fy_threadPushInt(context, thread, 4);
				break;
			}
			case ICONST_5: {
				fy_threadPushInt(context, thread, 5);
				break;
			}
			case IDIV: {
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}

				fy_threadPushInt(context, thread,
						fy_threadPopInt(context, thread) / ivalue);
				break;
			}
			case IF_ICMPEQ: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue == ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ACMPEQ: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopHandle(context, thread);
				ivalue = fy_threadPopHandle(context, thread);
				if (ivalue != ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ICMPNE: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue != ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ACMPNE: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopHandle(context, thread);
				ivalue = fy_threadPopHandle(context, thread);
				if (ivalue != ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ICMPLT: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue < ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ICMPLE: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue <= ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ICMPGT: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue > ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IF_ICMPGE: {
				index = nextS2(context, thread);
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue >= ivalue2) {
					opGOTO(context, thread, index);
				}
				break;
			}

			case IFEQ: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue == 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IFNULL: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopHandle(context, thread);
				if (ivalue == 0) {
					opGOTO(context, thread, index);
				}
				break;
			}

			case IFNE: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue != 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IFNONNULL: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopHandle(context, thread);
				if (ivalue != 0) {
					opGOTO(context, thread, index);
				}
				break;
			}

			case IFLT: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue < 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IFLE: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue <= 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IFGT: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue > 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IFGE: {
				index = nextS2(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue >= 0) {
					opGOTO(context, thread, index);
				}
				break;
			}
			case IINC: {
				index = nextU1(context, thread);

				ivalue = nextS1(context, thread);
				fy_threadPutLocalInt(
						context,
						thread,
						index,
						(juint) fy_threadGetLocalInt(context, thread, index)
								+ ivalue);
				break;
			}
			case IMUL: {
				fy_threadPushInt(
						context,
						thread,
						(juint) fy_threadPopInt(context, thread)
								* fy_threadPopInt(context, thread));
				break;
			}
			case INEG: {
				fy_threadPushInt(context, thread,
						-fy_threadPopInt(context, thread));
				break;
			}
			case INSTANCEOF: {
				index = nextU2(context, thread);
				aref = fy_threadPopHandle(context, thread);
				clazz1 = fy_heapGetClassOfObject(context, aref);
				clazz2 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) (method->owner->constantPools[index]),
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushInt(context, thread,
						fy_classCanCastTo(context, clazz1, clazz2) ? 1 : 0);
				break;
			}
			case INVOKEINTERFACE: {
				index = nextU2(context, thread); /*m*/
				ivalue = nextU1(context, thread); /*count*/
				nextU1(context, thread);
				pivalue = vm_allocate(ivalue * sizeof(jint));/*args*/
				pbvalue = vm_allocate(ivalue * sizeof(jbyte)); /*types*/
				for (i = ivalue - 1; i >= 0; i--) {
					pivalue[i] = fy_threadPopType(context, thread, pbvalue + i);
				}
				if (pivalue[0] == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				clazz1 = context->objects[pivalue[0]].clazz;
				mvalue =
						fy_vmLookupMethodFromConstant(
								context,
								(ConstantMethodRef*) (method->owner->constantPools[index]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (!fy_classCanCastTo(context, clazz1, mvalue->owner)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				mvalue = fy_vmLookupMethodVirtual(context, clazz1,
						mvalue->fullName);
				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if ((mvalue->access_flags & fy_ACC_PUBLIC) == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if ((mvalue->access_flags & fy_ACC_ABSTRACT)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_NATIVE) {
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, pivalue,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							vm_free(pivalue);
							vm_free(pbvalue);
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, FALSE, ivalue,
							pivalue, pbvalue);
				}
				vm_free(pivalue);
				vm_free(pbvalue);
				break;
			}
			case INVOKESPECIAL: {
				index = nextU2(context, thread);/*m*/
				clazz1 = method->owner;
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[index]),
						exception);
				clazz2 = mvalue->owner;
				ivalue = mvalue->paramCount + 1;/*count*/
				pivalue = vm_allocate(sizeof(jint) * ivalue);
				pbvalue = vm_allocate(sizeof(jbyte) * ivalue);
				for (i = ivalue - 1; i >= 0; i--) {
					pivalue[i] = fy_threadPopType(context, thread, pbvalue + i);
				}
				if ((clazz1->accessFlags & fy_ACC_SUPER)
						&& fy_classIsSuperClassOf(context, clazz2, clazz1)
						&& fy_strCmp(mvalue->name, context->sInit)) {
					mvalue = fy_vmLookupMethodVirtual(context, clazz1->super,
							mvalue->fullName);
				}
				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					exception->exceptionDesc[0] = 0;
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (fy_strCmp(mvalue->name, context->sInit) > 0
						&& mvalue->owner != clazz2) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NoSuchMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_ABSTRACT) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_NATIVE) {
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, pivalue,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							vm_free(pivalue);
							vm_free(pbvalue);
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, FALSE, ivalue,
							pivalue, pbvalue);
				}
				vm_free(pivalue);
				vm_free(pbvalue);
				break;
			}
			case INVOKESTATIC: {
				clazz1 = method->owner;
				index = nextU2(context, thread);/*m*/
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[index]),
						exception);
				clazz2 = mvalue->owner;
				if ((mvalue->access_flags & fy_ACC_STATIC) == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}

				if (clinit(context, thread, clazz2)) {
					break;
				}
				ivalue = mvalue->paramCount;
				pivalue = vm_allocate(sizeof(jint) * ivalue);
				pbvalue = vm_allocate(sizeof(jbyte) * ivalue);
				for (i = ivalue - 1; i >= 0; i--) {
					pivalue[i] = fy_threadPopType(context, thread, pbvalue + i);
				}
				if (mvalue->access_flags & fy_ACC_NATIVE) {
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, pivalue,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							vm_free(pivalue);
							vm_free(pbvalue);
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, TRUE, ivalue,
							pivalue, pbvalue);
				}
				vm_free(pivalue);
				vm_free(pbvalue);
				break;
			}
			case INVOKEVIRTUAL: {
				clazz1 = method->owner;
				index = nextU2(context, thread);/*m*/
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[index]),
						exception);
				ivalue = mvalue->paramCount + 1;

				pivalue = vm_allocate(ivalue * sizeof(jint));/*args*/
				pbvalue = vm_allocate(ivalue * sizeof(jbyte)); /*types*/
				for (i = ivalue - 1; i >= 0; i--) {
					pivalue[i] = fy_threadPopType(context, thread, pbvalue + i);
				}ASSERT(pbvalue[0]==TH_TYPE_HANDLE);
				if (pivalue[0] == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				clazz2 = context->objects[pivalue[0]].clazz;
				mvalue = fy_vmLookupMethodVirtual(context, clazz2,
						mvalue->fullName);

				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if ((mvalue->access_flags & fy_ACC_ABSTRACT)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					vm_free(pivalue);
					vm_free(pbvalue);
					break;
				}
				if (mvalue->access_flags & fy_ACC_NATIVE) {
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, pivalue,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							vm_free(pivalue);
							vm_free(pbvalue);
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, FALSE, ivalue,
							pivalue, pbvalue);
				}
				vm_free(pivalue);
				vm_free(pbvalue);
				break;
			}
			case IOR: {
				fy_threadPushInt(
						context,
						thread,
						fy_threadPopInt(context, thread)
								| fy_threadPopInt(context, thread));
				break;
			}
			case IREM: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					break;
				}
				fy_threadPushInt(context, thread, ivalue % ivalue2);
				break;
			}
			case ISHL: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread, ivalue << ivalue2);
				break;
			}
			case ISHR: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread, ivalue >> ivalue2);
				break;
			}
			case ISUB: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread, (juint) ivalue - ivalue2);
				break;
			}
			case IUSHR: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread, ((juint) ivalue) >> ivalue2);
				break;
			}
			case IXOR: {
				ivalue2 = fy_threadPopInt(context, thread);
				ivalue = fy_threadPopInt(context, thread);
				fy_threadPushInt(context, thread, ivalue ^ ivalue2);
				break;
			}
			case JSR: {
				index = nextS2(context, thread);
				fy_threadPushType(context, thread, getPC(context, thread),
						TH_TYPE_RETURN);
				opGOTO(context, thread, index);
				break;
			}
			case JSR_W: {
				index = nextS4(context, thread);
				fy_threadPushType(context, thread, getPC(context, thread),
						TH_TYPE_RETURN);
				opGOTO(context, thread, index);
				break;
			}
			case L2D: {
				fy_threadPushDouble(context, thread,
						(jdouble) fy_threadPopLong(context, thread));
				break;
			}
			case L2F: {
				fy_threadPushFloat(context, thread,
						(jfloat) fy_threadPopLong(context, thread));
				break;
			}
			case L2I: {
				fy_threadPushInt(context, thread,
						(jint) fy_threadPopLong(context, thread));
				break;
			}
			case LADD: {
				fy_threadPushLong(
						context,
						thread,
						(julong) fy_threadPopLong(context, thread)
								+ fy_threadPopLong(context, thread));
				break;
			}
			case LALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				lvalue = fy_heapGetArrayLong(context, aref, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushLong(context, thread, lvalue);
				break;
			}
			case LAND: {
				fy_threadPushLong(
						context,
						thread,
						fy_threadPopLong(context, thread)
								& fy_threadPopLong(context, thread));
				break;
			}
			case LASTORE: {
				lvalue = fy_threadPopLong(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_heapPutArrayLong(context, aref, index, lvalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case LCMP: {
				lvalue2 = fy_threadPopLong(context, thread);
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushInt(context, thread,
						lvalue == lvalue2 ? 0 : (lvalue > lvalue2 ? 1 : -1));
				break;
			}
			case LCONST_0: {
				fy_threadPushLong(context, thread, 0);
				break;
			}
			case LCONST_1: {
				fy_threadPushLong(context, thread, 1);
				break;
			}
			case LDC: {
				index = nextU1(context, thread);

				ivalue = opLDC(context, method->owner, index, &type, exception);

				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}

				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case LDC_W: {
				index = nextU2(context, thread);
				ivalue = opLDC(context, method->owner, index, &type, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushType(context, thread, ivalue, type);
				break;
			}
			case LDC2_W: {
				index = nextU2(context, thread);
				lvalue = opLDC2(context, method->owner, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushLong(context, thread, lvalue);
				break;
			}
			case LDIV: {
				lvalue2 = fy_threadPopLong(context, thread);
				lvalue = fy_threadPopLong(context, thread);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}
				fy_threadPushLong(context, thread, lvalue / lvalue2);
				break;
			}
			case DLOAD:
			case LLOAD: {
				index = nextU1(context, thread);
				fy_threadPushLong(context, thread,
						fy_threadGetLocalLong(context, thread, index));
				break;
			}
			case DLOAD_0:
			case LLOAD_0: {
				fy_threadPushLong(context, thread,
						fy_threadGetLocalLong(context, thread, 0));
				break;
			}
			case DLOAD_1:
			case LLOAD_1: {
				fy_threadPushLong(context, thread,
						fy_threadGetLocalLong(context, thread, 1));
				break;
			}
			case DLOAD_2:
			case LLOAD_2: {
				fy_threadPushLong(context, thread,
						fy_threadGetLocalLong(context, thread, 2));
				break;
			}
			case DLOAD_3:
			case LLOAD_3: {
				fy_threadPushLong(context, thread,
						fy_threadGetLocalLong(context, thread, 3));
				break;
			}
			case LMUL: {
				fy_threadPushLong(
						context,
						thread,
						(julong) fy_threadPopLong(context, thread)
								* fy_threadPopLong(context, thread));
				break;
			}
			case LNEG: {
				fy_threadPushLong(context, thread,
						-fy_threadPopLong(context, thread));
				break;
			}
			case LOOKUPSWITCH: {
				index = (65536 - getPC(context, thread)) % 4;
				movePC(context, thread, index);
				ivalue = nextS4(context, thread);/*db*/
				ivalue2 = nextS4(context, thread);/*np*/
				pivalue = vm_allocate(sizeof(jint) * ivalue2); /*match*/
				pivalue2 = vm_allocate(sizeof(jint) * ivalue2); /*offset*/
				for (i = 0; i < ivalue2; i++) {
					pivalue[i] = nextS4(context, thread);
					pivalue2[i] = nextS4(context, thread);
				}
				index = fy_threadPopInt(context, thread);
				bvalue1 = FALSE; /*matched*/
				for (i = 0; i < ivalue2; i++) {
					if (index == pivalue[i]) {
						opGOTO(context, thread, pivalue2[i]);
						bvalue1 = TRUE;
						break;
					}
				}
				vm_free(pivalue);
				vm_free(pivalue2);
				if (!bvalue1) {
					opGOTO(context, thread, ivalue);
				}
				break;
			}
			case LOR: {
				fy_threadPushLong(
						context,
						thread,
						fy_threadPopLong(context, thread)
								| fy_threadPopLong(context, thread));
				break;
			}
			case LREM: {
				lvalue2 = fy_threadPopLong(context, thread);
				lvalue = fy_threadPopLong(context, thread);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}
				fy_threadPushLong(context, thread, lvalue % lvalue2);
				break;
			}
			case LRETURN: {
				lvalue = fy_threadPopLong(context, thread);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread,
								fy_threadGetLocalHandle(context, thread, 0));
					}
				}
				fy_threadPopFrame(context, thread);
				fy_threadPushLong(context, thread, lvalue);

				break;
			}
			case LSHL: {
				lvalue2 = fy_threadPopInt(context, thread) & 0x3f;
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushLong(context, thread, lvalue << lvalue2);
				break;
			}
			case LSHR: {
				lvalue2 = fy_threadPopInt(context, thread) & 0x3f;
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushLong(context, thread, lvalue >> lvalue2);
				break;
			}
			case DSTORE:
			case LSTORE: {
				index = nextU1(context, thread);
				fy_threadPutLocalLong(context, thread, index,
						fy_threadPopLong(context, thread));
				break;
			}
			case DSTORE_0:
			case LSTORE_0: {
				fy_threadPutLocalLong(context, thread, 0,
						fy_threadPopLong(context, thread));
				break;
			}
			case DSTORE_1:
			case LSTORE_1: {
				fy_threadPutLocalLong(context, thread, 1,
						fy_threadPopLong(context, thread));
				break;
			}
			case DSTORE_2:
			case LSTORE_2: {
				fy_threadPutLocalLong(context, thread, 2,
						fy_threadPopLong(context, thread));
				break;
			}
			case DSTORE_3:
			case LSTORE_3: {
				fy_threadPutLocalLong(context, thread, 3,
						fy_threadPopLong(context, thread));
				break;
			}
			case LSUB: {
				lvalue2 = fy_threadPopLong(context, thread);
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushLong(context, thread, (julong) lvalue - lvalue2);
				break;
			}
			case LUSHR: {
				lvalue2 = fy_threadPopInt(context, thread) & 0x3f;
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushLong(context, thread,
						((julong) lvalue) >> lvalue2);
				break;
			}
			case LXOR: {
				lvalue2 = fy_threadPopLong(context, thread);
				lvalue = fy_threadPopLong(context, thread);
				fy_threadPushLong(context, thread, lvalue ^ lvalue2);
				break;
			}
			case MONITORENTER: {
				aref = fy_threadPopHandle(context, thread);
				fy_threadMonitorEnter(context, thread, aref);
				break;
			}
			case MONITOREXIT: {
				aref = fy_threadPopHandle(context, thread);
				fy_threadMonitorExit(context, thread, aref);
				break;
			}
			case MULTIANEWARRAY: {
				index = nextU2(context, thread);
				ivalue = nextU1(context, thread);
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[index],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				pivalue = vm_allocate(sizeof(int) * ivalue);
				for (i = ivalue - 1; i >= 0; i--) {
					pivalue[i] = fy_threadPopInt(context, thread);
				}
				aref = threadmulitANewArray(context, clazz1, ivalue, pivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushHandle(context, thread, aref);
				vm_free(pivalue);
				break;
			}
			case NEW: {
				index = nextU2(context, thread);
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[index],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				if (clazz1->accessFlags
						& (fy_ACC_INTERFACE | fy_ACC_ABSTRACT)) {
					fy_strSPrint(msg, 256, clazz1->className);
					vm_die("InstantiationErro %s", msg);
				}
				if (clinit(context, thread, clazz1)) {
					break;
				}
				aref = fy_heapAllocate(context, clazz1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushHandle(context, thread, aref);
				break;
			}
			case NEWARRAY: {
				type = nextU1(context, thread);
				index = fy_threadPopInt(context, thread);
				if (index < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NegativeArraySizeException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d", index);
				}
				switch (type) {
				case 4:
					pstr1 = context->sArrayBoolean;
					break;
				case 5:
					pstr1 = context->sArrayChar;
					break;
				case 6:
					pstr1 = context->sArrayFloat;
					break;
				case 7:
					pstr1 = context->sArrayDouble;
					break;
				case 8:
					pstr1 = context->sArrayByte;
					break;
				case 9:
					pstr1 = context->sArrayShort;
					break;
				case 10:
					pstr1 = context->sArrayInteger;
					break;
				case 11:
					pstr1 = context->sArrayLong;
					break;
				default:
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/VirtualMachineError");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Unknown array type in NEWARRAY type=%d", type);
					break;
				}
				if (message->messageType == message_exception) {
					break;
				}
				clazz1 = fy_vmLookupClass(context, pstr1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				aref = fy_heapAllocateArray(context, clazz1, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushHandle(context, thread, aref);
				break;
			}
			case NOP: {
				break;
			}
			case POP: {
				fy_threadPopType(context, thread, &type);
				break;
			}
			case POP2: {
				fy_threadPopType(context, thread, &type);
				fy_threadPopType(context, thread, &type);
				break;
			}
			case PUTFIELD: {
				index = nextU2(context, thread);
				field = fy_vmLookupFieldFromConstant(context,
						(ConstantFieldRef*) method->owner->constantPools[index],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J':
					lvalue = fy_threadPopLong(context, thread);
					break;
				case 'L':
				case '[':
					ivalue = fy_threadPopHandle(context, thread);
					break;
				default:
					ivalue = fy_threadPopInt(context, thread);
				}
				aref = fy_threadPopHandle(context, thread);
				if (field->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is static", msg);
					break;
				}

				if ((field->access_flags & fy_ACC_FINAL)
						&& method->owner != field->owner) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is final", msg);
					break;
				}

				switch (type) {
				case 'D':
				case 'J':
					fy_heapPutFieldLong(context, aref, field, lvalue,
							exception);
					break;
				default:
					fy_heapPutFieldInt(context, aref, field, ivalue, exception);
					break;
				}
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case PUTSTATIC: {
				index = nextU2(context, thread);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[index]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				if ((field->access_flags & fy_ACC_FINAL)
						&& (field->owner != method->owner)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					fy_strSPrint(msg, sizeof(msg), field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is final", msg);
					break;
				}
				clazz1 = field->owner;
				if (clinit(context, thread, clazz1)) {
					break;
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J':
					fy_heapPutStaticLong(context, field,
							fy_threadPopLong(context, thread), exception);
					break;
				case 'L':
				case '[':
					fy_heapPutStaticHandle(context, field,
							fy_threadPopHandle(context, thread), exception);
					break;
				default:
					fy_heapPutStaticInt(context, field,
							fy_threadPopInt(context, thread), exception);
					break;
				}
				break;
			}
			case RET: {
				index = nextU1(context, thread);
				ivalue = fy_threadGetLocalReturn(context, thread, index);
				setPC(context, thread, ivalue);
				break;
			}
			case RETURN: {
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread,
								fy_threadGetLocalHandle(context, thread, 0));
					}
				}
				if (fy_strCmp(method->name, context->sClinit) == 0) {
					method->owner->clinitThreadId = -1;
				}
				fy_threadPopFrame(context, thread);
				break;
			}
			case SALOAD: {
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				svalue = fy_heapGetArrayShort(context, aref, index, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				fy_threadPushInt(context, thread, svalue);
				break;
			}
			case SASTORE: {
				ivalue = fy_threadPopInt(context, thread);
				index = fy_threadPopInt(context, thread);
				aref = fy_threadPopHandle(context, thread);
				fy_heapPutArrayShort(context, aref, index, (short) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				break;
			}
			case SIPUSH: {
				ivalue = nextS2(context, thread);
				fy_threadPushInt(context, thread, ivalue);
				break;
			}
			case SWAP: {
				ivalue = fy_threadPopType(context, thread, &type);
				ivalue2 = fy_threadPopType(context, thread, &type2);
				fy_threadPushType(context, thread, ivalue, type);
				fy_threadPushType(context, thread, ivalue2, type2);
				break;
			}
			case TABLESWITCH: {
				index = (65536 - getPC(context, thread)) % 4;
				movePC(context, thread, index);
				ivalue = nextS4(context, thread);/*db*/
				ivalue2 = nextS4(context, thread);/*lb*/
				ivalue3 = nextS4(context, thread);/*hb*/
				index = ivalue3 - ivalue2 + 1;/*count*/
				pivalue = vm_allocate(sizeof(jint) * index);
				for (i = 0; i < index; i++) {
					pivalue[i] = nextS4(context, thread);
				}

				index = fy_threadPopInt(context, thread);
				if (index < ivalue2 || index > ivalue3) {
					ivalue4 = ivalue;
				} else {
					ivalue4 = pivalue[index - ivalue2];
				}
				vm_free(pivalue);
				opGOTO(context, thread, ivalue4);
				break;
			}
			case WIDE: {
				op2 = nextU1(context, thread);
				index = nextU2(context, thread);
				switch (op2) {
				case FLOAD:
				case ILOAD: {
					ivalue = fy_threadGetLocalInt(context, thread, index);
					fy_threadPushInt(context, thread, ivalue);
					break;
				}
				case ALOAD: {
					aref = fy_threadGetLocalHandle(context, thread, index);
					fy_threadPushHandle(context, thread, aref);
					break;
				}
				case DLOAD:
				case LLOAD: {
					lvalue = fy_threadGetLocalLong(context, thread, index);
					fy_threadPushLong(context, thread, lvalue);
					break;
				}
				case FSTORE:
				case ISTORE: {
					ivalue = fy_threadPopInt(context, thread);
					fy_threadPutLocalInt(context, thread, index, ivalue);
					break;
				}
				case ASTORE: {
					aref = fy_threadPopType(context, thread, &type);
					fy_threadPutLocalType(context, thread, index, aref, type);
					break;
				}
				case DSTORE:
				case LSTORE: {
					lvalue = fy_threadPopLong(context, thread);
					fy_threadPutLocalLong(context, thread, index, lvalue);
					break;
				}
				case RET: {
					ivalue = fy_threadGetLocalReturn(context, thread, index);
					setPC(context, thread, ivalue);
					break;
				}
				case IINC: {
					ivalue = nextS2(context, thread);
					fy_threadPutLocalInt(
							context,
							thread,
							index,
							fy_threadGetLocalInt(context, thread, index)
									+ ivalue);
					break;
				}
				default: {
					vm_die("Unknown OPCode %d", op2);
				}
				}
				break;
			}
			}
		}
		if (thread->yield) {
			thread->yield = FALSE;
			message->messageType = message_none;
			break;
		}
		if (message->messageType == message_exception) {
			//send the exception to the VM caller for PI handle.
			break;
		}
		if (message->messageType != message_continue) {
			break;
		}
	}
}

#endif
