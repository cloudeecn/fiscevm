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
#define CMD_BREAK  1
#define CMD_GOON  2
#define CMD_BACK  3
#if 0
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
#endif

#ifdef FY_VERBOSE
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

#if 0
static jubyte fy_nextU1(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 1);
	return code[pc];
}

static jbyte fy_nextS1(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 1);
	return code[pc];
}

static jchar fy_nextU2(fy_VMContext *context, fy_thread *thread) {
	int pc = getPC(context, thread);
	jubyte *code = thread->code;
	setPC(context, thread, pc + 2);
	return (jchar) fy_B2TOI(code[pc],code[pc+1]);
}

static jshort fy_nextS2(fy_VMContext *context, fy_thread *thread) {
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

static jint fy_nextS4(fy_VMContext *context, fy_thread *thread) {
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
#endif

static jint processThrowable(fy_VMContext *context, fy_frame *frame,
		jint handle, jint lpc, fy_exception *exception) {
	fy_class *throwableClass;
	fy_class *handlerClass;
	jint i, imax;
	struct ExceptionTable *handlers;
	struct ExceptionTable *handler;
	jint target = 0;
	DLOG("EXCEPTION HANDLE LOOKUP: LPC=%ld", lpc);
	throwableClass = fy_heapGetClassOfObject(context, handle);
	handlers = frame->method->exception_table;
	imax = frame->method->exception_table_length;
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

#if 0
static void updateLocalBuf(fy_VMContext *context, fy_thread *thread) {
	if (thread->fp != 0) {
		thread->method = fy_threadGetCurrentMethod(context, thread);
		thread->code = thread->method->code;
	} else {
		thread->method = NULL;
		thread->code = NULL;
	}
}
#endif

static jint opLDC(fy_VMContext *context, fy_class *owner, jchar index,
		juint *type, fy_exception *exception) {
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

void fy_threadInit(fy_VMContext *context, fy_thread *thread) {

}

#if 0
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
	arrayHandle = fy_heapAllocateArray(context, array, thread->frameCount,
			exception);
	fy_simpleErrorHandle;

}
#endif

#ifdef FY_STRICT_CHECK
# define fy_checkPush(SIZE) { \
	if(sp>=sb+size+(SIZE)){ \
		vm_die("Stack overflow! base=%d sp=%d localvars=%d",sb,sp,localCount); \
	} \
}

# define fy_checkPop(SIZE,T) { \
	if(sp<=sb+localCount+(SIZE)){ \
		vm_die("Stack underflow! base=%d sp=%d localvars=%d",sb,sp,localCount); \
	} \
	if(typeStack[sp-1]!=(T)){ \
		vm_die("Type mismatch, needs [%c] but got [%c]",(T),typeStack[sp-1]); \
	} \
}

# define fy_frameToLocalCheck(ptrFrame) { \
	size = ptrFrame->size; \
	localCount = ptrFrame -> localCount; \
	codeSize = ptrFrame->codeSize; \
}

# define fy_localToFrameCheck(ptrFrame) { \
	ptrFrame->size = size; \
	ptrFrame -> localCount = localCount; \
	ptrFrame->codeSize = codeSize; \
}

# define fy_checkPutLocal(P,SIZE) { \
	if((P)<0 || (P)+(SIZE)>=localCount) {\
		vm_die("Local var out of range %d/%d",(P),localCount);\
	} \
}

# define fy_checkGetLocal(P,SIZE,T) { \
	if((P)<0 || (P)+(SIZE)>=localCount) {\
		vm_die("Local var out of range %d/%d",(P),localCount);\
	} \
	if(typeStack[sb+(P)]!=(T)){\
		vm_die("Type mismatch, needs [%c] but got [%c]",(T),typeStack[sb+(P)]); \
	}\
}

# define fy_checkCall(COUNT) \
	if (sp - (COUNT) - sb < localCount) { \
		vm_die("Buffer underflow! %d %d", sp - (COUNT) - sb, localCount); \
	}

#else
# define fy_checkPush(SIZE) {}
# define fy_checkPop(SIZE,T) {}
# define fy_frameToLocalCheck(F) {}
# define fy_localToFrameCheck(F) {}
# define fy_checkPutLocal(P,SIZE) {}
# define fy_checkGetLocal(P,SIZE,T) {}
# define fy_checkCall(COUNT) {}
#endif

#define fy_threadPushType(V,T) { \
	fy_checkPush(0) \
	typeStack[sp]=(T);stack[sp++]=V; \
}

#define fy_threadPushInt(V) { \
	fy_checkPush(0) \
	typeStack[sp]=TH_TYPE_INT;stack[sp++]=V; \
}

#define fy_threadPushHandle(V) { \
		fy_checkPush(0) \
	typeStack[sp]=TH_TYPE_HANDLE;stack[sp++]=V; \
}
#define fy_threadPushReturn(V) { \
	fy_checkPush(0) \
	typeStack[sp]=TH_TYPE_RETURN;stack[sp++]=V; \
}

#define fy_threadPushLong(W) { \
	fy_checkPush(1) \
	typeStack[sp]=TH_TYPE_WIDE;stack[sp++]=fy_HOFL((W)); \
	typeStack[sp]=TH_TYPE_WIDE2;stack[sp++]=fy_LOFL((W)); \
}

#define fy_threadPopType(O,T) { \
	fy_checkPop(0,typeStack[sp-1]) \
	O=stack[--sp]; \
	T=typeStack[sp]; \
}

#define fy_threadPopInt(O) { \
	fy_checkPop(0,typeStack[sp-1]) \
	O=stack[--sp]; \
}

#define fy_threadPopFloat(O) { \
	fy_checkPop(0,typeStack[sp-1]) \
	O=fy_intToFloat(stack[--sp]); \
}

#define fy_threadPopHandle(O) { \
	fy_checkPop(0,typeStack[sp-1]) \
	O=stack[--sp]; \
}

#define fy_threadPopReturn(O) { \
	fy_checkPop(0,typeStack[sp-1]) \
	O=stack[--sp]; \
}

#define fy_threadPopLong(O) { \
	fy_checkPop(1,typeStack[sp-1]) \
	O = stack[--sp]; \
	O += ((julong)stack[--sp])<<32; \
}

#define fy_threadPopDouble(O) { \
	fy_checkPop(1,typeStack[sp-1]) \
	sp-=2; \
	O=fy_longToDouble((((julong)stack[sp])<<32)+(julong)stack[sp+1]); \
}

#define fy_threadPutLocalInt(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	typeStack[sb+(P)]=TH_TYPE_INT; \
}

#define fy_threadPutLocalHandle(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	typeStack[sb+(P)]=TH_TYPE_HANDLE; \
}

#define fy_threadPutLocalType(P,V,TYPE) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	typeStack[sb+(P)]=(TYPE); \
}

#define fy_threadPutLocalLong(P,W) { \
	fy_checkPutLocal(P,1) \
	stack[sb+(P)]=fy_HOFL(W); \
	stack[sb+(P)+1]=fy_LOFL(W); \
	typeStack[sb+(P)]=TH_TYPE_WIDE; \
	typeStack[sb+(P)+1]=TH_TYPE_WIDE2; \
}

#define fy_threadGetLocalInt(P,O) { \
	fy_checkGetLocal(P,0,TH_TYPE_INT) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalHandle(P,O) { \
	fy_checkGetLocal(P,0,TH_TYPE_HANDLE) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalReturn(P,O) { \
	fy_checkGetLocal(P,0,TH_TYPE_RETURN) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalLong(P,O) { \
	fy_checkGetLocal(P,1,TH_TYPE_WIDE) \
	O=stack[sb+(P)]; \
	O=((julong)O<<32)+((julong)stack[sb+(P)+1]); \
}

#define fy_frameToLocal(ptrFrame) { \
	sp=ptrFrame->sp; \
	pc=ptrFrame->pc; \
	lpc=ptrFrame->pc; \
	sb=ptrFrame->sb; \
	method = ptrFrame->method; \
	code = method->code; \
	fy_frameToLocalCheck(ptrFrame) \
}

#define fy_localToFrame(ptrFrame) { \
	ptrFrame->sp = sp; \
	ptrFrame->pc = pc; \
	ptrFrame->lpc = lpc; \
	ptrFrame->sb = sb; \
	ptrFrame->method = method; \
	ptrFrame->methodId = method->method_id; \
	fy_localToFrameCheck(ptrFrame) \
}

static fy_frame *fy_threadCurrentFrame(fy_VMContext *context, fy_thread *thread) {
	juint frameCount = thread->frameCount;
	if (frameCount == 0) {
		return NULL;
	} else {
		return thread->frames + (frameCount - 1);
	}
}
#if 0
void fy_threadGetFrameInfo(fy_VMContext *context, fy_thread *thread, char *out,
		size_t outSize) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	sprintf_s(out, outSize,
			"STACK=[%I32d-%I32d) VARS=[%I32d-%I32d) SIZE=%I32d %d",
			frame->sb + frame->localCount, frame->sb + frame->size, frame->sb,
			frame->sb + frame->localCount, frame->size,
			frame->method->access_flags & fy_ACC_STATIC);
}
#endif

static fy_frame *fy_threadPushFrame(fy_VMContext *context, fy_thread *thread,
		fy_method *invoke) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	juint sb, sp;
	if (frame == NULL) {
		sb = 0;
		sp = invoke->max_locals;
		frame = thread->frames;
	} else {
		sb = frame->sp;
		sp = sb + invoke->max_locals;
		frame++;
	}
	if ((++(thread->frameCount)) >= MAX_FRAMES) {
		vm_die("FRAME OVERFLOW! %d", MAX_FRAMES);
	}
	frame->method = invoke;
	frame->code = invoke->code;
	frame->sb = sb;
	frame->methodId = invoke->method_id;
	frame->sp = sp;
	frame->pc = 0;
	frame->lpc = 0;
#ifdef FY_STRICT_CHECK
	frame->size = invoke->max_locals + invoke->max_stack;
	frame->localCount = invoke->max_locals;
	frame->codeSize = invoke->codeLength;
#endif

	return frame;
}

static fy_frame *fy_threadPopFrame(fy_VMContext *context, fy_thread *thread) {
	juint fc = --(thread->frameCount);
	if (fc <= 0) {
		return NULL;
	} else {
		return thread->frames + fc - 1;
	}
}

static fy_class *clinit(fy_VMContext *context, fy_thread *thread,
		fy_class *clazz) {
	juint tid;
	fy_class *ret;
	if (clazz == NULL) {
		return NULL;
	}
	tid = clazz->clinitThreadId;
	if (tid == -1 || tid == thread->threadId) {
		return NULL;
	}
	if (clazz->clinit == NULL) {
		ret = clinit(context, thread, clazz->super);
		if (ret == NULL) {
			clazz->clinitThreadId = -1;
		}
		return ret;
	} else {
		return clazz;
	}
	return NULL;
}

void fy_threadPushMethod(fy_VMContext *context, fy_thread *thread,
		fy_method *invoke, fy_frame **localFrame) {
	fy_frame *frame = fy_threadPushFrame(context, thread, invoke);
	if (localFrame != NULL) {
		*localFrame = frame;
	}

	if (invoke->access_flags & fy_ACC_SYNCHRONIZED) {
		fy_threadMonitorEnter(
				context,
				thread,
				(invoke->access_flags & fy_ACC_STATIC) ?
						invoke->owner->classObjId : thread->stack[frame->sb]);
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
		fy_threadPushFrame(context, thread, method);
		clazz = fy_vmLookupClass(context, context->sStringArray, exception);
		if (exception->exceptionType != exception_none) {
			break;
		}

		thread->stack[0] = fy_heapAllocateArray(context, clazz, 0, exception);
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

#define fy_nextU1(CODE) (juint)CODE[pc++]
#define fy_nextS1(CODE) ((jbyte)CODE[pc++])
#define fy_nextU2(CODE) (fy_B2TOI(CODE[pc],CODE[pc+1])&0xffff);pc+=2
#define fy_nextS2(CODE) fy_B2TOI(CODE[pc],CODE[pc+1]);pc+=2
#define fy_nextS4(CODE) fy_B4TOI(CODE[pc],CODE[pc+1],CODE[pc+2],CODE[pc+3]);pc+=4

enum FallOut {
	fallout_none, fallout_invoke, fallout_noinvoke
};

/**
 * DON'T USE RETURN HERE!!!!
 */
void fy_threadRun(fy_VMContext *context, fy_thread *thread, fy_message *message,
		jint ops) {
	juint sb, sp, pc, lpc, opCount, op;
	enum FallOut fallout;
#ifdef FY_STRICT_CHECK
	juint size, localCount, codeSize;
#endif
	jubyte *code;
	fy_method *method = NULL;
	fy_frame *frame = NULL;
	juint *stack = thread->stack;
	juint *typeStack = thread->typeStack;
	fy_exception *exception = &(message->body.exception);

#ifndef _FY_LATE_DECLARATION
	juint ivalue, ivalue2, ivalue3, ivalue4;
	int i;
	julong lvalue, lvalue2;
	jdouble dvalue, dvalue2;
	jfloat fvalue, fvalue2;
	fy_method *mvalue;
	fy_field *field = NULL;
	fy_class *clazz1 = NULL, *clazz2 = NULL, *clinitClazz;
	jchar ch1;
	fy_str str1;
	juint type, type2, type3, type4;
	jint *pivalue, *pivalue2;
	fy_str *pstr1;
	jboolean bvalue1;
	fy_nh *nh;
	char msg[256];

	ivalue = 0x6789abcd;
	lvalue = 0x6789abcdef1234LL;
	pstr1 = NULL;
#endif

	message->messageType = message_continue;
	exception->exceptionType = exception_none;
	opCount = 0;

	frame = fy_threadCurrentFrame(context, thread);
	while (opCount < ops) {
		if (frame == NULL) {
			/*Time to quit!*/
			if (thread->currentThrowable) {
				DLOG("XXXXXXXXXXUnhandled Exception!!!XXXXXXXXXXXX");
				method = fy_vmGetMethod(context,
						context->sThrowablePrintStacktrace);
				ASSERT( method != NULL);
				frame = fy_threadPushFrame(context, thread, method);
				stack[frame->sb] = thread->currentThrowable;
				thread->currentThrowable = 0;
				continue;
			} else {
				message->messageType = message_thread_dead;
				break;
			}
		}
		/**/
		fy_frameToLocal(frame);
		if (thread->currentThrowable) {
#ifdef _FY_LATE_DECLARATION
			juint ivalue;
#endif
			ivalue = processThrowable(context, frame, thread->currentThrowable,
					lpc, exception);
			if (exception->exceptionType != exception_none) {
				/*Drop to thread manager with critical exception*/
				message->messageType = message_exception;
				break;
			} else {
				if (ivalue >= 0) {
					/*Found*/
					sp = 0;
					lpc = pc = ivalue;
					thread->currentThrowable = 0;
				} else {
					/*Not found, will return*/
					if (method->access_flags & fy_ACC_SYNCHRONIZED) {
						if (method->access_flags & fy_ACC_STATIC) {
							fy_threadMonitorExit(context, thread,
									method->owner->classObjId);
						} else {
							fy_threadMonitorExit(context, thread, stack[sb]);
						}
					}
					fy_threadPopFrame(context, thread);
					continue;
				}
			}
		}
		fallout = fallout_none;
		if (method->clinit) {
#ifdef _FY_LATE_DECLARATION
			fy_class *clinitClazz;
#endif
			clinitClazz = clinit(context, thread, method->owner->super);
			if (clinitClazz) {
				if (clinitClazz->clinitThreadId == 0) {
					fy_localToFrame(frame);
					clinitClazz->clinitThreadId = thread->threadId;
					frame = fy_threadPushFrame(context, thread,
							clinitClazz->clinit);
					continue;
				} else {
					thread->yield = TRUE;
					fallout = fallout_noinvoke;
				}
			}
		}

		for (; opCount < ops && fallout == fallout_none; opCount++) {
			/*RUN_ONE_INST!!!!!*/
			op = fy_nextU1(code);
#ifdef _DEBUG
#ifdef FY_VERBOSE
#ifdef _FY_LATE_DECLARATION
			char msg[256];
#endif
			fy_strSPrint(msg, 256, method->uniqueName);
			DLOG("##%6d/%6d %s %d %s SB=%d SP=%d\n", opCount, ops, msg, lpc,
					OP_NAME[op], sb, sp);
#endif
#endif

			switch (op) {
			case AALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopHandle(ivalue2);
				fy_threadPushHandle(
						fy_heapGetArrayHandle(context, ivalue2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case IALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopHandle(ivalue2);
				fy_threadPushInt(
						fy_heapGetArrayHandle(context, ivalue2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case AASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				fy_class *clazz1, *clazz2;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPopHandle(ivalue3);
				DLOG("AASTORE %d[%d]=%d", ivalue3, ivalue2, ivalue);
				clazz1 = fy_heapGetClassOfObject(context, ivalue3);
				clazz2 = clazz1->ci.arr.contentClass;
				if (ivalue != 0
						&& !fy_classCanCastTo(context,
								fy_heapGetClassOfObject(context, ivalue),
								clazz2)) {
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArrayStoreException");
					strcpy_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Data type not compatable!");
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;/*EXCEPTION_THROWN*/
				}
				fy_heapPutArrayHandle(context, ivalue3, ivalue2, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case IASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPopHandle(ivalue3);
				fy_heapPutArrayInt(context, ivalue3, ivalue2, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case ACONST_NULL: {
				fy_threadPushHandle(0);
				break;
			}

			case ILOAD:
			case FLOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalInt(ivalue, ivalue2);
				fy_threadPushInt(ivalue2);
				break;
			}
			case ALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalHandle(ivalue, ivalue2);
				fy_threadPushHandle(ivalue2);
				break;
			}
			case ILOAD_0:
			case FLOAD_0: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalInt(0, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_0: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalHandle(0, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_1:
			case FLOAD_1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalInt(1, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalHandle(1, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_2:
			case FLOAD_2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalInt(2, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalHandle(2, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_3:
			case FLOAD_3: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalInt(3, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_3: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadGetLocalHandle(3, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ANEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				juint ch1, ivalue;
				fy_class *clazz1, *clazz2;
				fy_str str1;
#endif
				ch1 = fy_nextU2(code);
				fy_threadPopInt(ivalue);
				if (ivalue < 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IndexOutOfBoundException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d<0", ivalue);
					break;
				}

				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ch1],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
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
					fallout = fallout_noinvoke;
					break;
				}
				fy_strDestroy(context, &str1);
				fy_threadPushHandle(
						fy_heapAllocateArray(context, clazz2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case IRETURN:
			case FRETURN: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						/*CUSTOM*/
						fy_threadMonitorExit(context, thread, stack[sb]);
					}
				}
				/*CUSTOM*/
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_nativeReturnInt(context, thread, ivalue);
				fallout = fallout_invoke;
				break;
			}
			case ARETURN: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb]);
					}
				}
				/*CUSTOM*/
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_nativeReturnHandle(context, thread, ivalue);
				fallout = fallout_invoke;
				break;
			}
			case ARRAYLENGTH: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue);
				ivalue2 = fy_heapArrayLength(context, ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}

				fy_threadPushInt(ivalue2);
				break;
			}
			case ISTORE:
			case FSTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopInt(ivalue2);
				fy_threadPutLocalInt(ivalue, ivalue2);
				break;
			}
			case ASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
				juint type;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopType(ivalue2, type);
				fy_threadPutLocalType(ivalue, ivalue2, type);
				break;
			}
			case ISTORE_0:
			case FSTORE_0: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(0, ivalue);
				break;
			}
			case ASTORE_0: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				juint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(0, ivalue, type);
				break;
			}
			case ISTORE_1:
			case FSTORE_1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(1, ivalue);
				break;
			}
			case ASTORE_1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				juint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(1, ivalue, type);
				break;
			}
			case ISTORE_2:
			case FSTORE_2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(2, ivalue);
				break;
			}
			case ASTORE_2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				juint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(2, ivalue, type);
				break;
			}
			case ISTORE_3:
			case FSTORE_3: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(3, ivalue);
				break;
			}
			case ASTORE_3: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				juint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(3, ivalue, type);
				break;
			}
			case ATHROW: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				thread->currentThrowable = ivalue;
				break;
			}
			case BALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayByte(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case BASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayByte(context, ivalue2, ivalue3, (jbyte) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case BIPUSH: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS1(code);
				fy_threadPushInt(ivalue);
				break;
			}
			case FALOAD:
			case CALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayInt(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case FASTORE:
			case CASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayInt(context, ivalue2, ivalue3, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case CHECKCAST: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
				fy_class *clazz1, *clazz2;
				fy_str str1;
#endif
				ivalue = fy_nextU2(code);
				/*CUSTOM*/
				ivalue2 = stack[sp - 1];
				if (ivalue2 != 0) {
					clazz1 = fy_heapGetClassOfObject(context, ivalue2);
					clazz2 = fy_vmLookupClassFromConstant(context,
							method->owner->constantPools[ivalue], exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}
					if (!fy_classCanCastTo(context, clazz1, clazz2)) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						exception->exceptionType = exception_normal;
						strcpy_s( exception->exceptionName,
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
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt(
						fy_floatToInt((jfloat)fy_longToDouble(lvalue)));
				break;
			}
			case D2I: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((jint)fy_longToDouble(lvalue));
				break;
			}
			case D2L: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = (jlong) fy_longToDouble(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DADD: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue);
				fy_threadPopDouble(dvalue2);
				lvalue = fy_doubleToLong(dvalue + dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LALOAD:
			case DALOAD: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				lvalue = fy_heapGetArrayLong(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushLong(lvalue);
				break;
			}
			case LASTORE:
			case DASTORE: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue2, ivalue3;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayLong(context, ivalue2, ivalue3, lvalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case DCMPG: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				if (fy_isnand(dvalue) || fy_isnand(dvalue2)) {
					fy_threadPushInt( 1);
				} else {
					fy_threadPushInt(
							dvalue == dvalue2 ? 0 : (dvalue - dvalue2 > 0) ? 1 : -1);

				}
				break;
			}
			case DCMPL: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				if (fy_isnand(dvalue) || fy_isnand(dvalue2)) {
					fy_threadPushInt( -1);
				} else {
					fy_threadPushInt(
							dvalue == dvalue2 ? 0 : (dvalue - dvalue2 > 0) ? 1 : -1);

				}
				break;
			}
			case DCONST_0: {
				fy_threadPushLong(fy_doubleToLong(0));
				break;
			}
			case DCONST_1: {
				fy_threadPushLong(fy_doubleToLong(1));
				break;
			}
			case DDIV: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue / dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DMUL: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue * dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DNEG: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(-dvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DREM: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				if (dvalue2 == 0) {
					lvalue = fy_doubleToLong(0.0 / dvalue2);
					fy_threadPushLong(lvalue);
				} else {
					lvalue = fy_doubleToLong(
							dvalue - floor(dvalue / dvalue2) * dvalue2);
					fy_threadPushLong( lvalue);
				}

				break;
			}
			case DRETURN: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue;
#endif
				fy_threadPopDouble(dvalue);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb]);
					}
				}fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_nativeReturnDouble(context, thread, dvalue);
				fallout = fallout_invoke;
				break;
			}
			case DSUB: {
#ifdef _FY_LATE_DECLARATION
				jdouble dvalue, dvalue2;
				julong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue - dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DUP: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				juint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP_X1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
				juint type, type2;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);

				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP_X2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				juint type, type2, type3;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);
				fy_threadPopType(ivalue3, type3);

				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue3, type3);
				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
				juint type, type2;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);

				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP2_X1: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				juint type, type2, type3;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);
				fy_threadPopType(ivalue3, type3);

				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue3, type3);
				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP2_X2: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3, ivalue4;
				juint type, type2, type3, type4;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);
				fy_threadPopType(ivalue3, type3);
				fy_threadPopType(ivalue4, type4);

				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue4, type4);
				fy_threadPushType(ivalue3, type3);
				fy_threadPushType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				break;
			}
			case F2D: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue;
				julong lvalue;
#endif
				fy_threadPopFloat(fvalue);
				lvalue = fy_doubleToLong(fvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case F2I: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue = (juint) (jint) fy_intToFloat(ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case F2L: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				julong lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = (julong) (jlong) fy_intToFloat(ivalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case FADD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				ivalue = fy_floatToInt(
						fy_intToFloat(ivalue) + fy_intToFloat(ivalue2));
				fy_threadPushInt( ivalue);
				break;
			}
			case FCMPG: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				if (fy_isnanf(fvalue) || fy_isnanf(fvalue2)) {
					fy_threadPushInt(1);
				} else {
					fy_threadPushInt(
							fvalue == fvalue2 ? 0 : (fvalue - fvalue2 > 0) ? 1 : -1);
				}
				break;
			}
			case FCMPL: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				if (fy_isnanf(fvalue) || fy_isnanf(fvalue2)) {
					fy_threadPushInt(-1);
				} else {
					fy_threadPushInt(
							fvalue == fvalue2 ? 0 : (fvalue - fvalue2 > 0) ? 1 : -1);
				}
				break;
			}
			case FCONST_0: {
				fy_threadPushInt(fy_floatToInt(0.0f));
				break;
			}
			case FCONST_1: {
				fy_threadPushInt(fy_floatToInt(1.0f));
				break;
			}
			case FCONST_2: {
				fy_threadPushInt(fy_floatToInt(2.0f));
				break;
			}
			case FDIV: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue / fvalue2));
				break;
			}
			case FMUL: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue * fvalue2));
				break;
			}
			case FNEG: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue;
#endif
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(-fvalue));
				break;
			}
			case FREM: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				if (fvalue2 == 0) {
					fy_threadPushInt(fy_floatToInt( 0.0f / fvalue2));
				} else {
					fy_threadPushInt(
							fy_floatToInt( fvalue - (float) floor(fvalue / fvalue2) * fvalue2));
				}
				break;
			}
			case FSUB: {
#ifdef _FY_LATE_DECLARATION
				jfloat fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue - fvalue2));
				break;
			}
			case GETFIELD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue2;
				fy_field *field;
				juint type;
#endif
				ivalue3 = fy_nextU2(code);
				fy_threadPopHandle(ivalue2);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[ivalue3]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				if (field->access_flags & fy_ACC_STATIC) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
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
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					julong lvalue;
#endif
					lvalue = fy_heapGetFieldLong(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushLong( lvalue);
					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					ivalue = fy_heapGetFieldHandle(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushHandle(ivalue);
				}
					break;
				default: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					ivalue = fy_heapGetFieldInt(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushInt( ivalue);
					break;
				}
				}
				break;
			}
			case GETSTATIC: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3;
				juint type;
				fy_field *field;
				fy_class *clazz1, *clinitClazz;
#endif
				ivalue3 = fy_nextU2(code);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[ivalue3]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				clazz1 = field->owner;
				{
					clinitClazz = clinit(context, thread, clazz1);
					if (clinitClazz) {
						if (clinitClazz->clinitThreadId == 0) {
							pc = lpc;
							fy_localToFrame(frame);
							clinitClazz->clinitThreadId = thread->threadId;
							frame = fy_threadPushFrame(context, thread,
									clinitClazz->clinit);
							fallout = fallout_invoke;
						} else {
							thread->yield = TRUE;
						}
						break;
					}
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					julong lvalue;
#endif
					lvalue = fy_heapGetStaticLong(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushLong(lvalue);

					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					juint ivalue2;
#endif
					ivalue2 = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushHandle(ivalue2);

					break;
				}
				default: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					ivalue = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						fallout = fallout_noinvoke;
						break;
					}fy_threadPushInt(ivalue);

					break;
				}
				}
				break;
			}
			case GOTO: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS2(code);
				pc = lpc + ivalue;
				break;
			}
			case GOTO_W: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS4(code);
				pc = lpc + ivalue;
				break;
			}
			case I2B: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2C: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xffff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2D: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				jlong lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = fy_doubleToLong((jdouble) ivalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case I2F: {
				/*CUSTOM*/
				fy_checkPop(0, TH_TYPE_INT);
				stack[sp - 1] = fy_floatToInt((jfloat) stack[sp - 1]);
				break;
			}
			case I2L: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				jlong lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case I2S: {
				/*CUSTOM*/
				fy_checkPop(0, TH_TYPE_INT);
				stack[sp - 1] = (jshort) stack[sp - 1];
				break;
			}
			case IADD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue+ivalue2);
				break;
			}
			case IAND: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue&ivalue2);
				break;
			}
			case ICONST_M1: {
				fy_threadPushInt(-1);
				break;
			}
			case ICONST_0: {
				fy_threadPushInt(0);
				break;
			}
			case ICONST_1: {
				fy_threadPushInt(1);
				break;
			}
			case ICONST_2: {
				fy_threadPushInt(2);
				break;
			}
			case ICONST_3: {
				fy_threadPushInt(3);
				break;
			}
			case ICONST_4: {
				fy_threadPushInt(4);
				break;
			}
			case ICONST_5: {
				fy_threadPushInt(5);
				break;
			}
			case IDIV: {
#ifdef _FY_LATE_DECLARATION
				jint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}fy_threadPopInt(ivalue);
				ivalue /= ivalue2;
				fy_threadPushInt(ivalue);
				break;
			}
			case IF_ICMPEQ: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue == ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ACMPEQ: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopHandle(ivalue2);
				fy_threadPopHandle(ivalue);
				if (ivalue == ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPNE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue != ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ACMPNE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopHandle(ivalue2);
				fy_threadPopHandle(ivalue);
				if (ivalue != ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPLT: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue < (jint) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPLE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue <= (jint) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPGT: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue > (jint) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPGE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue >= (jint) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}

			case IFEQ: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if (ivalue == 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFNULL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopHandle(ivalue);
				if (ivalue == 0) {
					pc = lpc + ivalue3;
				}
				break;
			}

			case IFNE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if (ivalue != 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFNONNULL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopHandle(ivalue);
				if (ivalue != 0) {
					pc = lpc + ivalue3;
				}
				break;
			}

			case IFLT: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue < 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFLE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue <= 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFGT: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue > 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFGE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((jint) ivalue >= 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IINC: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextU1(code);
				ivalue = fy_nextS1(code);
				/*CUSTOM*/
				stack[sb + ivalue3] += ivalue;
				break;
			}
			case IMUL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPushInt((juint)((jint)ivalue2) * ((jint)ivalue));
				break;
			}
			case INEG: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPushInt(-ivalue);
				break;
			}
			case INSTANCEOF: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue2, ivalue3;
				fy_class *clazz1, *clazz2;
#endif
				ivalue3 = fy_nextU2(code);
				fy_threadPopHandle(ivalue2);
				clazz1 = fy_heapGetClassOfObject(context, ivalue2);
				clazz2 =
						fy_vmLookupClassFromConstant(
								context,
								(ConstantClass*) (method->owner->constantPools[ivalue3]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushInt(
						fy_classCanCastTo(context, clazz1, clazz2) ? 1 : 0);
				break;
			}
			case INVOKEINTERFACE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
				fy_class *clazz1;
				fy_method *mvalue;
#endif
				ivalue3 = fy_nextU2(code); /*m*/
				ivalue = fy_nextU1(code); /*count*/
				fy_nextU1(code);
				/*CusTOM*/
				fy_checkCall(ivalue);
				sp -= ivalue;
				if (stack[sp]/*this*/== 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				clazz1 = fy_heapGetObject(context, stack[sp])->clazz;
				mvalue =
						fy_vmLookupMethodFromConstant(
								context,
								(ConstantMethodRef*) (method->owner->constantPools[ivalue3]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				if (!fy_classCanCastTo(context, clazz1, mvalue->owner)) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				mvalue = fy_vmLookupMethodVirtual(context, clazz1,
						mvalue->fullName);
				if (mvalue == NULL) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				if ((mvalue->access_flags & fy_ACC_PUBLIC) == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				if ((mvalue->access_flags & fy_ACC_ABSTRACT)) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & fy_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							fallout = fallout_noinvoke;
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame);
				}
				fallout = fallout_invoke;
				break;
			}
			case INVOKESPECIAL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
				fy_class *clazz1, *clazz2;
				fy_method *mvalue;
#endif
				ivalue3 = fy_nextU2(code);/*m*/
				clazz1 = method->owner;
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[ivalue3]),
						exception);
				clazz2 = mvalue->owner;
				ivalue = mvalue->paramCount + 1;/*count*/
				fy_checkCall(ivalue);
				sp -= ivalue;
				if ((clazz1->accessFlags & fy_ACC_SUPER)
						&& fy_classIsSuperClassOf(context, clazz2, clazz1)
						&& fy_strCmp(mvalue->name, context->sInit)) {
					mvalue = fy_vmLookupMethodVirtual(context, clazz1->super,
							mvalue->fullName);
				}
				if (mvalue == NULL) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					exception->exceptionDesc[0] = 0;
					break;
				}
				if (fy_strCmp(mvalue->name, context->sInit) > 0
						&& mvalue->owner != clazz2) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NoSuchMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}
				if (mvalue->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}
				if (mvalue->access_flags & fy_ACC_ABSTRACT) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & fy_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							fallout = fallout_noinvoke;
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame);
				}
				fallout = fallout_invoke;
				break;
			}
			case INVOKESTATIC: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
				fy_class *clazz1, *clazz2, *clinitClazz;
				fy_method *mvalue;
#endif
				clazz1 = method->owner;
				ivalue3 = fy_nextU2(code);/*m*/
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[ivalue3]),
						exception);
				clazz2 = mvalue->owner;
				if ((mvalue->access_flags & fy_ACC_STATIC) == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}

				{
					clinitClazz = clinit(context, thread, clazz2);
					if (clinitClazz) {
						if (clinitClazz->clinitThreadId == 0) {
							pc = lpc;
							fy_localToFrame(frame);
							clinitClazz->clinitThreadId = thread->threadId;
							frame = fy_threadPushFrame(context, thread,
									clinitClazz->clinit);
							fallout = fallout_invoke;
						} else {
							thread->yield = TRUE;
						}
						break;
					}
				}
				ivalue = mvalue->paramCount;
				fy_checkCall(ivalue);
				sp -= ivalue;
				fy_localToFrame(frame);
				if (mvalue->access_flags & fy_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							fallout = fallout_noinvoke;
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame);
				}
				fallout = fallout_invoke;
				break;
			}
			case INVOKEVIRTUAL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
				fy_class *clazz1, *clazz2;
				fy_method *mvalue;
#endif
				clazz1 = method->owner;
				ivalue3 = fy_nextU2(code);/*m*/
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[ivalue3]),
						exception);
				ivalue = mvalue->paramCount + 1;
				fy_checkCall(ivalue);
				sp -= ivalue;
				ASSERT(typeStack[sp]==TH_TYPE_HANDLE);
				if (stack[sp] == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				clazz2 = fy_heapGetObject(context, stack[sp])->clazz;
				mvalue = fy_vmLookupMethodVirtual(context, clazz2,
						mvalue->fullName);

				if (mvalue == NULL) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					break;
				}
				if (mvalue->access_flags & fy_ACC_STATIC) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}
				if ((mvalue->access_flags & fy_ACC_ABSTRACT)) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & fy_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(context, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							fallout = fallout_noinvoke;
							break;
						}
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame);
				}
				fallout = fallout_invoke;
				break;
			}
			case IOR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue|ivalue2);
				break;
				break;
			}
			case IREM: {
#ifdef _FY_LATE_DECLARATION
				jint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					break;
				}
				ivalue %= ivalue2;
				ivalue += ivalue >> 31 & abs(ivalue2);
				fy_threadPushInt(ivalue);
				break;
			}
			case ISHL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue << ivalue2);
				break;
			}
			case ISHR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(((jint)ivalue) >> ((jint)ivalue2));
				break;
			}
			case ISUB: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue - ivalue2);
				break;
				break;
			}
			case IUSHR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue >> ivalue2);
				break;
			}
			case IXOR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue ^ ivalue2);
				break;
			}
			case JSR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS2(code);
				fy_threadPushReturn(pc);
				pc = lpc + ivalue;
				break;
			}
			case JSR_W: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS4(code);
				fy_threadPushReturn(pc);
				pc = lpc + ivalue;
				break;
			}
			case L2D: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = fy_doubleToLong(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case L2F: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue;
#endif
				fy_threadPopLong(lvalue);
				ivalue = fy_floatToInt((float) lvalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case L2I: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((juint)lvalue);
				break;
			}
			case LADD: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue += lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LAND: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue &= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LCMP: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				fy_threadPushInt(
						lvalue == lvalue2 ? 0 : ((jlong)lvalue > (jlong)lvalue2 ? 1 : -1));
				break;
			}
			case LCONST_0: {
				fy_threadPushLong((julong)0);
				break;
			}
			case LCONST_1: {
				fy_threadPushLong((julong)1);
				break;
			}
			case LDC: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue;
				juint type;
#endif
				ivalue3 = fy_nextU1(code);

				ivalue = opLDC(context, method->owner, ivalue3, &type,
						exception);

				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}

				fy_threadPushType(ivalue, type);
				break;
			}
			case LDC_W: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue;
				juint type;
#endif
				ivalue3 = fy_nextU2(code);
				ivalue = opLDC(context, method->owner, ivalue3, &type,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushType(ivalue, type);
				break;
			}
			case LDC2_W: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3;
				julong lvalue;
#endif
				ivalue3 = fy_nextU2(code );
				lvalue = opLDC2(context, method->owner, ivalue3, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushLong(lvalue);
				break;
			}
			case LDIV: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}
				lvalue = (julong) ((jlong) lvalue / (jlong) lvalue2);
				fy_threadPushLong( lvalue);
				break;
			}
			case DLOAD:
			case LLOAD: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalLong(ivalue, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_0:
			case LLOAD_0: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadGetLocalLong(0, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_1:
			case LLOAD_1: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadGetLocalLong(1, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_2:
			case LLOAD_2: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadGetLocalLong(2, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_3:
			case LLOAD_3: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadGetLocalLong(3, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case LMUL: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue = (julong) ((jlong) lvalue * (jlong) lvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LNEG: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushLong(-lvalue);
				break;
			}
			case LOOKUPSWITCH: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				jint *pivalue, *pivalue2;
				juint i;
				jboolean bvalue1;
#endif
				ivalue3 = (65536 - pc) % 4;
				pc += ivalue3;
				ivalue = fy_nextS4(code);/*db*/
				ivalue2 = fy_nextS4(code);/*np*/
				pivalue = vm_allocate(sizeof(jint) * ivalue2); /*match*/
				pivalue2 = vm_allocate(sizeof(jint) * ivalue2); /*offset*/
				for (i = 0; i < ivalue2; i++) {
					pivalue[i] = fy_nextS4(code);
					pivalue2[i] = fy_nextS4(code)
					;
				}fy_threadPopInt(ivalue3);
				bvalue1 = FALSE; /*matched*/
				for (i = 0; i < ivalue2; i++) {
					if (ivalue3 == pivalue[i]) {
						pc = lpc + pivalue2[i];
						bvalue1 = TRUE;
						break;
					}
				}
				vm_free(pivalue);
				vm_free(pivalue2);
				if (!bvalue1) {
					pc = lpc + ivalue;
				}
				break;
			}
			case LOR: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue |= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LREM: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
				}
				lvalue = (julong) ((jlong) lvalue % (jlong) lvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LRETURN: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						/*CUSTOM*/
						fy_threadMonitorExit(context, thread, stack[sb]);
					}
				}fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_nativeReturnLong(context, thread, lvalue);
				fallout = fallout_invoke;
				break;
			}
			case LSHL: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				julong lvalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopLong(lvalue);
				lvalue <<= ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case LSHR: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
				julong lvalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopLong(lvalue);
				lvalue = ((jlong) lvalue) >> ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case LUSHR: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopLong(lvalue);
				lvalue >>= ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case DSTORE:
			case LSTORE: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
				juint ivalue;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(ivalue, lvalue);
				break;
			}
			case DSTORE_0:
			case LSTORE_0: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(0, lvalue);
				break;
			}
			case DSTORE_1:
			case LSTORE_1: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(1, lvalue);
				break;
			}
			case DSTORE_2:
			case LSTORE_2: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(2, lvalue);
				break;
			}
			case DSTORE_3:
			case LSTORE_3: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(3, lvalue);
				break;
			}
			case LSUB: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue -= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LXOR: {
#ifdef _FY_LATE_DECLARATION
				julong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue ^= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case MONITORENTER: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadMonitorEnter(context, thread, ivalue);
				break;
			}
			case MONITOREXIT: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadMonitorExit(context, thread, ivalue);
				break;
			}
			case MULTIANEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				jint *pivalue;
				fy_class *clazz1;
				jint i;
#endif
				ivalue3 = fy_nextU2(code );
				ivalue = fy_nextU1(code );
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ivalue3],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				pivalue = vm_allocate(sizeof(int) * ivalue);
				for (i = ivalue - 1; i >= 0; i--) {
					fy_threadPopInt(pivalue[i]);
				}
				ivalue2 = threadmulitANewArray(context, clazz1, ivalue, pivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushHandle(ivalue2);
				vm_free(pivalue);
				break;
			}
			case NEW: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue;
				fy_class *clazz1, *clinitClazz;
#endif
				ivalue3 = fy_nextU2(code);
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ivalue3],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				if (clazz1->accessFlags
						& (fy_ACC_INTERFACE | fy_ACC_ABSTRACT)) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					fy_strSPrint(msg, 256, clazz1->className);
					vm_die("InstantiationErro %s", msg);
				}
				{
					clinitClazz = clinit(context, thread, clazz1);
					if (clinitClazz) {
						if (clinitClazz->clinitThreadId == 0) {
							pc = lpc;
							fy_localToFrame(frame);
							clinitClazz->clinitThreadId = thread->threadId;
							frame = fy_threadPushFrame(context, thread,
									clinitClazz->clinit);
							fallout = fallout_invoke;
						} else {
							thread->yield = TRUE;
						}
						break;
					}
				}
				ivalue = fy_heapAllocate(context, clazz1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushHandle(ivalue);
				break;
			}
			case NEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue2;
				juint type;
				fy_str *pstr1;
				fy_class *clazz1;
#endif
				type = fy_nextU1(code);
				fy_threadPopInt(ivalue3);
				if (ivalue3 < 0) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NegativeArraySizeException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d", ivalue3);
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
					pstr1 = NULL; /*make compiler happy*/
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
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
					fallout = fallout_noinvoke;
					break;
				}
				ivalue2 = fy_heapAllocateArray(context, clazz1, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushHandle(ivalue2);
				break;
			}
			case NOP: {
				break;
			}
			case POP: {
				sp--;
				break;
			}
			case POP2: {
				sp--;
				sp--;
				break;
			}
			case PUTFIELD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
				julong lvalue;
				juint type;
				fy_field *field;
#endif
				ivalue3 = fy_nextU2(code);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) method->owner->constantPools[ivalue3],
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J':
					fy_threadPopLong(lvalue);
					break;
				case 'L':
				case '[':
					fy_threadPopHandle(ivalue);
					break;
				default:
					fy_threadPopInt(ivalue);
				}fy_threadPopHandle(ivalue2);
				if (field->access_flags & fy_ACC_STATIC) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
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
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
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
					fy_heapPutFieldLong(context, ivalue2, field, lvalue,
							exception);
					break;
				default:
					fy_heapPutFieldInt(context, ivalue2, field, ivalue,
							exception);
					break;
				}
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case PUTSTATIC: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3;
				juint type;
				fy_field *field;
				fy_class *clazz1, *clinitClazz;
#endif
				ivalue3 = fy_nextU2(code);
				field =
						fy_vmLookupFieldFromConstant(
								context,
								(ConstantFieldRef*) (method->owner->constantPools[ivalue3]),
								exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				if ((field->access_flags & fy_ACC_FINAL)
						&& (field->owner != method->owner)) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					fy_strSPrint(msg, sizeof(msg), field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is final", msg);
					break;
				}
				clazz1 = field->owner;
				{
					clinitClazz = clinit(context, thread, clazz1);
					if (clinitClazz) {
						if (clinitClazz->clinitThreadId == 0) {
							pc = lpc;
							fy_localToFrame(frame);
							clinitClazz->clinitThreadId = thread->threadId;
							frame = fy_threadPushFrame(context, thread,
									clinitClazz->clinit);
							fallout = fallout_invoke;
						} else {
							thread->yield = TRUE;
						}
						break;
					}
				}
				type = (jbyte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					julong lvalue;
#endif
					fy_threadPopLong(lvalue);
					fy_heapPutStaticLong(context, field, lvalue, exception);
					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadPopInt(ivalue)
					fy_heapPutStaticHandle(context, field, ivalue, exception);
					break;
				}
				default: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadPopInt(ivalue)
					fy_heapPutStaticHandle(context, field, ivalue, exception);
					break;
				}
				}
				break;
			}
			case RET: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextU1(code);
				fy_threadGetLocalReturn(ivalue3, ivalue);
				pc = ivalue;
				break;
			}
			case RETURN: {
				if (method->access_flags & fy_ACC_SYNCHRONIZED) {
					if (method->access_flags & fy_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb]);
					}
				}
				if (method->clinit) {
					method->owner->clinitThreadId = -1;
				}
				frame = fy_threadPopFrame(context, thread);
				fallout = fallout_invoke;
				break;
			}
			case SALOAD: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayShort(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case SASTORE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayShort(context, ivalue2, ivalue3, (short) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					fallout = fallout_noinvoke;
					break;
				}
				break;
			}
			case SIPUSH: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue;
#endif
				ivalue = fy_nextS2(code);
				fy_threadPushInt(ivalue);
				break;
			}
			case SWAP: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2;
				juint type, type2;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue2, type2);
				break;
			}
			case TABLESWITCH: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue, ivalue2, ivalue3, ivalue4;
				juint i;
				jint *pivalue;
#endif
				ivalue4 = (65536 - pc) % 4;
				pc += ivalue4;
				ivalue = fy_nextS4(code );/*db*/
				ivalue2 = fy_nextS4(code );/*lb*/
				ivalue3 = fy_nextS4(code );/*hb*/
				ivalue4 = ivalue3 - ivalue2 + 1;/*count*/
				pivalue = vm_allocate(sizeof(jint) * ivalue4);
				for (i = 0; i < ivalue4; i++) {
					pivalue[i] = fy_nextS4(code)
					;
				}fy_threadPopInt(ivalue4);
				if (ivalue4 < ivalue2 || ivalue4 > ivalue3) {
					ivalue4 = ivalue;
				} else {
					ivalue4 = pivalue[ivalue4 - ivalue2];
				}
				vm_free(pivalue);
				pc = lpc + ivalue4;
				break;
			}
			case WIDE: {
#ifdef _FY_LATE_DECLARATION
				juint ivalue3, ivalue4;
#endif
				ivalue4 = fy_nextU1(code);
				ivalue3 = fy_nextU2(code);
				switch (ivalue4) {
				case FLOAD:
				case ILOAD: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadGetLocalInt(ivalue3, ivalue);
					fy_threadPushInt(ivalue);
					break;
				}
				case ALOAD: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadGetLocalHandle(ivalue3, ivalue);
					fy_threadPushHandle(ivalue);
					break;
				}
				case DLOAD:
				case LLOAD: {
#ifdef _FY_LATE_DECLARATION
					julong lvalue;
#endif
					fy_threadGetLocalLong(ivalue3, lvalue);
					fy_threadPushLong(lvalue);
					break;
				}
				case FSTORE:
				case ISTORE: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadPopInt(ivalue);
					fy_threadPutLocalInt( ivalue3, ivalue);
					break;
				}
				case ASTORE: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
					juint type;
#endif
					fy_threadPopType(ivalue, type);
					fy_threadPutLocalType( ivalue3, ivalue, type);
					break;
				}
				case DSTORE:
				case LSTORE: {
#ifdef _FY_LATE_DECLARATION
					julong lvalue;
#endif
					fy_threadPopLong(lvalue);
					fy_threadPutLocalLong( ivalue3, lvalue);
					break;
				}
				case RET: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					fy_threadGetLocalReturn( ivalue3, ivalue);
					pc = ivalue;
					break;
				}
				case IINC: {
#ifdef _FY_LATE_DECLARATION
					juint ivalue;
#endif
					ivalue = fy_nextS2(code);
					/*CuSTOM*/
					stack[sb + ivalue3] += ivalue;
					break;
				}
				default: {
					vm_die("Unknown OPCode %d", ivalue4);
				}
				}
				break;
			}
			}
			lpc = pc;
		}
		if (fallout != fallout_invoke) {
			fy_localToFrame(frame);
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

void fy_nativeReturnInt(fy_VMContext *context, fy_thread *thread, jint value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	juint sp = frame->sp;
	juint *stack = thread->stack;
	juint *typeStack = thread->typeStack;
	stack[sp] = value;
	typeStack[sp] = TH_TYPE_INT;
	frame->sp = sp + 1;
}

void fy_nativeReturnHandle(fy_VMContext *context, fy_thread *thread, jint value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	juint sp = frame->sp;
	juint *stack = thread->stack;
	juint *typeStack = thread->typeStack;
	stack[sp] = value;
	typeStack[sp] = TH_TYPE_HANDLE;
	frame->sp = sp + 1;
}

void fy_nativeReturnLong(fy_VMContext *context, fy_thread *thread, jlong value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	juint sp = frame->sp;
	juint *stack = thread->stack;
	juint *typeStack = thread->typeStack;
	stack[sp] = fy_HOFL(value);
	stack[sp + 1] = fy_LOFL(value);
	typeStack[sp] = TH_TYPE_WIDE;
	typeStack[sp + 1] = TH_TYPE_WIDE2;
	frame->sp = sp + 2;
}

