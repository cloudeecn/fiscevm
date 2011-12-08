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

static fy_int processThrowable(fy_context *context, fy_frame *frame,
		fy_int handle, fy_int lpc, fy_exception *exception) {
	fy_class *throwableClass;
	fy_class *handlerClass;
	fy_int i, imax;
	fy_exceptionHandler *handlers;
	fy_exceptionHandler *handler;
	fy_int target = -1;
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
					target = -1;
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
	//Jump after found the target is move to run() for further optimize
}

static fy_int threadmulitANewArray(fy_context *context, fy_class *clazz,
		fy_int layers, fy_int *elementsForEachLayer, fy_exception *exception) {
	int i;
	fy_int handle;
	int size = elementsForEachLayer[0];
	int ret = fy_heapAllocateArray(context, clazz, size, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	if (layers > 1) {
		for (i = 0; i < size; i++) {
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

static fy_int opLDC(fy_context *context, fy_class *owner, fy_char index,
		fy_uint *type, fy_exception *exception) {
	ConstantStringInfo *constantStringInfo;
	ConstantClass *constantClass;
	fy_class *clazz;
	fy_int hvalue;
	switch (owner->constantTypes[index]) {
	case CONSTANT_Integer:
	case CONSTANT_Float:
		*type = FY_TYPE_INT;
		return ((ConstantIntegerFloatInfo*) (owner->constantPools[index]))->value;
	case CONSTANT_String:
		*type = FY_TYPE_HANDLE;
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
		*type = FY_TYPE_HANDLE;
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

static fy_long opLDC2(fy_context *context, fy_class *owner, fy_char index,
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

void fy_threadMonitorEnter(fy_context *context, fy_thread *thread,
		fy_int handle) {
	fy_tmMonitorEnter(context, thread, handle);
}

void fy_threadMonitorExit(fy_context *context, fy_thread *thread, fy_int handle,
		fy_exception *exception) {
	fy_tmMonitorExit(context, thread, handle, exception);
}

void fy_threadDestroy(fy_context *context, fy_thread *thread) {
	fy_uint handle;
	fy_object *obj;
	obj = context->objects + thread->handle;
	obj->attachedId = 0;
	thread->handle = 0;

	thread->waitForLockId = 0;
	thread->waitForNotifyId = 0;
	thread->nextWakeTime = 0;
	thread->pendingLockCount = 0;
	thread->destroyPending = FALSE;
	for (handle = 1; handle < MAX_OBJECTS; handle++) {
		obj = context->objects + handle;
		if (obj->monitorOwnerId == thread->threadId) {
			obj->monitorOwnerId = 0;
			obj->monitorOwnerTimes = 0;
		}
	}
}

void fy_threadFillException(fy_context *context, fy_thread *thread,
		fy_uint handle, fy_exception *exception) {
#define FY_SIMPLE_ERROR_HANDLE if(exception->exceptionType != exception_none) return;

	fy_class *clazz, *array;
	fy_field *throwableStackTraceElements, *declaringClassField,
			*methodNameField, *fileNameField, *lineNumberField;
	fy_frame *frame;
	fy_method *method;
	fy_str *str;
	fy_uint arrayHandle, itemHandle, strHandle;
	fy_uint lpc;
	fy_int i, j, t;
	fy_int lineNumber;
	fy_lineNumber *ln;

	fy_vmLookupClass(context, context->sClassThrowable, exception);
	FY_SIMPLE_ERROR_HANDLE
	throwableStackTraceElements = fy_vmGetField(context,
			context->sThrowableStackTrace);
	if (throwableStackTraceElements == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		strcpy_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"Can't find nessessery sThrowableStackTrace");
		return;
	}

	clazz = fy_vmLookupClass(context, context->sStackTraceElement, exception);
	FY_SIMPLE_ERROR_HANDLE
	array = fy_vmLookupClass(context, context->sStackTraceElementArray,
			exception);
	FY_SIMPLE_ERROR_HANDLE

	declaringClassField = fy_vmGetField(context,
			context->sStackTraceElementDeclaringClass);
	if (declaringClassField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementDeclaringClass);
		return;
	}

	methodNameField = fy_vmGetField(context,
			context->sStackTraceElementMethodName);
	if (methodNameField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementMethodName);
		return;
	}

	fileNameField = fy_vmGetField(context, context->sStackTraceElementFileName);
	if (fileNameField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementFileName);
		return;
	}

	lineNumberField = fy_vmGetField(context,
			context->sStackTraceElementLineNumber);
	if (lineNumberField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/VirtualMachineError");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementLineNumber);
		return;
	}
	arrayHandle = fy_heapAllocateArray(context, array, thread->frameCount,
			exception);
	FY_SIMPLE_ERROR_HANDLE

	t = 0;
	for (i = thread->frameCount - 1; i >= 0; i--) {
		itemHandle = fy_heapAllocate(context, clazz, exception);
		FY_SIMPLE_ERROR_HANDLE
		frame = thread->frames + i;
		method = frame->method;
		str = fy_strCreateClone(context->memblocks, method->owner->className,
				exception);
		fy_exceptionCheckAndReturn(exception);
		fy_strReplaceOne(str, '/', '.');
		strHandle = fy_heapMakeString(context, str, exception);
		fy_strRelease(context->memblocks, str);
		FY_SIMPLE_ERROR_HANDLE
		fy_heapPutFieldHandle(context, itemHandle, declaringClassField,
				strHandle, exception);
		FY_SIMPLE_ERROR_HANDLE
		strHandle = fy_heapMakeString(context, method->name, exception);
		FY_SIMPLE_ERROR_HANDLE
		fy_heapPutFieldHandle(context, itemHandle, methodNameField, strHandle,
				exception);
		FY_SIMPLE_ERROR_HANDLE
		strHandle = fy_heapMakeString(context, method->owner->sourceFile,
				exception);
		FY_SIMPLE_ERROR_HANDLE
		fy_heapPutFieldHandle(context, itemHandle, fileNameField, strHandle,
				exception);
		FY_SIMPLE_ERROR_HANDLE

		lineNumber = -1;
		ln = method->line_number_table;
		if (method->access_flags & FY_ACC_NATIVE) {
			lineNumber = -2;
		} else {
			lpc = frame->lpc;
			for (j = method->line_number_table_length - 1; j >= 0; j--) {
				ln = method->line_number_table + j;
				if (lpc >= method->line_number_table[j].start_pc) {
					lineNumber = ln->line_number;
					break;
				}
			}
		}

		fy_heapPutFieldInt(context, itemHandle, lineNumberField, lineNumber,
				exception);
		FY_SIMPLE_ERROR_HANDLE

		fy_heapPutArrayHandle(context, arrayHandle, t, itemHandle, exception);
		FY_SIMPLE_ERROR_HANDLE
		t++;
	}
	fy_heapPutFieldHandle(context, handle, throwableStackTraceElements,
			arrayHandle, exception);
	/*The last line, so don't need error handle*/
}

#ifdef _FY_GOTO
# define FY_FALLOUT_INVOKE goto label_fallout_invoke;
# define FY_FALLOUT_NOINVOKE goto label_fallout_noinvoke;
#else
# define FY_FALLOUT_INVOKE fallout = fallout_invoke;
# define FY_FALLOUT_NOINVOKE fallout = fallout_noinvoke;
#endif

#ifdef FY_STRICT_CHECK
# define fy_checkPush(SIZE) { \
	if(sp>=sb+size+(SIZE)){ \
		fy_fault(exception,NULL,"Stack overflow! base=%d sp=%d localvars=%d",sb,sp,localCount); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	} \
}

# define fy_checkPop(SIZE,T) { \
	if(sp<=sb+localCount+(SIZE)){ \
		fy_fault(exception,NULL,"Stack underflow! base=%d sp=%d localvars=%d",sb,sp,localCount); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	} \
	if(typeStack[sp-1]!=(T)){ \
		fy_fault(exception,NULL,"Type mismatch, needs [%c] but got [%c]",(T),typeStack[sp-1]); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
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
		fy_fault(exception,NULL,"Local var out of range %d/%d",(P),localCount);\
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	} \
}

# define fy_checkGetLocal(P,SIZE,T) { \
	if((P)<0 || (P)+(SIZE)>=localCount) {\
		fy_fault(exception,NULL,"Local var out of range %d/%d",(P),localCount);\
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	} \
	if(typeStack[sb+(P)]!=(T)){\
		fy_fault(exception,NULL,"Type mismatch, needs [%c] but got [%c]",(T),typeStack[sb+(P)]); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	}\
}

# define fy_checkCall(COUNT) \
	if (sp - (COUNT) - sb < localCount) { \
		fy_fault(exception,NULL,"Buffer underflow! %d %d", sp - (COUNT) - sb, localCount); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	}

#else
# define fy_checkPush(SIZE)
# define fy_checkPop(SIZE,T)
# define fy_frameToLocalCheck(F)
# define fy_localToFrameCheck(F)
# define fy_checkPutLocal(P,SIZE)
# define fy_checkGetLocal(P,SIZE,T)
# define fy_checkCall(COUNT)
#endif

#define fy_threadPushType(V,T) { \
	fy_checkPush(0) \
	typeStack[sp]=(T);stack[sp++]=V; \
}

#define fy_threadPushInt(V) { \
	fy_checkPush(0) \
	typeStack[sp]=FY_TYPE_INT;stack[sp++]=V; \
}

#define fy_threadPushHandle(V) { \
		fy_checkPush(0) \
	typeStack[sp]=FY_TYPE_HANDLE;stack[sp++]=V; \
}
#define fy_threadPushReturn(V) { \
	fy_checkPush(0) \
	typeStack[sp]=FY_TYPE_RETURN;stack[sp++]=V; \
}

#define fy_threadPushLong(W) { \
	fy_checkPush(1) \
	typeStack[sp]=FY_TYPE_WIDE;stack[sp++]=fy_HOFL((W)); \
	typeStack[sp]=FY_TYPE_WIDE2;stack[sp++]=fy_LOFL((W)); \
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
	O += ((fy_ulong)stack[--sp])<<32; \
}

#define fy_threadPopDouble(O) { \
	fy_checkPop(1,typeStack[sp-1]) \
	sp-=2; \
	O=fy_longToDouble((((fy_ulong)stack[sp])<<32)+(fy_ulong)stack[sp+1]); \
}

#define fy_threadPutLocalInt(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	typeStack[sb+(P)]=FY_TYPE_INT; \
}

#define fy_threadPutLocalHandle(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	typeStack[sb+(P)]=FY_TYPE_HANDLE; \
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
	typeStack[sb+(P)]=FY_TYPE_WIDE; \
	typeStack[sb+(P)+1]=FY_TYPE_WIDE2; \
}

#define fy_threadGetLocalInt(P,O) { \
	fy_checkGetLocal(P,0,FY_TYPE_INT) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalHandle(P,O) { \
	fy_checkGetLocal(P,0,FY_TYPE_HANDLE) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalReturn(P,O) { \
	fy_checkGetLocal(P,0,FY_TYPE_RETURN) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalLong(P,O) { \
	fy_checkGetLocal(P,1,FY_TYPE_WIDE) \
	O=stack[sb+(P)]; \
	O=((fy_ulong)O<<32)+((fy_ulong)stack[sb+(P)+1]); \
}

#define fy_frameToLocal(ptrFrame) { \
	sp=ptrFrame->sp; \
	pc=ptrFrame->pc; \
	lpc=ptrFrame->lpc; \
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

static fy_frame *fy_threadCurrentFrame(fy_context *context, fy_thread *thread) {
	fy_uint frameCount = thread->frameCount;
	if (frameCount == 0) {
		return NULL;
	} else {
		return thread->frames + (frameCount - 1);
	}
}

static fy_frame *fy_threadPushFrame(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_exception *exception) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	fy_uint sb, sp;
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
		fy_fault(exception, NULL, "FRAME OVERFLOW! %d", MAX_FRAMES);
		return NULL;
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

static fy_frame *fy_threadPopFrame(fy_context *context, fy_thread *thread) {
	fy_uint fc = --(thread->frameCount);
	if (fc <= 0) {
		return NULL;
	} else {
		return thread->frames + fc - 1;
	}
}

static fy_class *clinit(fy_context *context, fy_thread *thread, fy_class *clazz) {
	fy_uint tid;
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

void fy_threadPushMethod(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_frame **localFrame, fy_exception *exception) {
	fy_frame *frame = fy_threadPushFrame(context, thread, invoke, exception);
	fy_exceptionCheckAndReturn(exception);
	if (localFrame != NULL) {
		*localFrame = frame;
	}

	if (invoke->access_flags & FY_ACC_SYNCHRONIZED) {
		fy_threadMonitorEnter(
				context,
				thread,
				(invoke->access_flags & FY_ACC_STATIC) ?
						invoke->owner->classObjId : thread->stack[frame->sb]);
	}

}

void fy_threadInitWithMethod(fy_context *context, fy_thread *thread,
		int threadHandle, fy_method *method, fy_exception *exception) {
	fy_class *clazz;
	fy_object *obj = context->objects + threadHandle;
	if (!fy_strEndsWith(method->uniqueName, context->sFMain)) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/InternalError");
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"The boot method must be static void main(String[])");
		return;
	}
	if ((method->access_flags & FY_ACC_STATIC) == 0) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/InternalError");
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"The first method of a thread must have no return value.");
		return;
	}
	thread->handle = threadHandle;
	obj->attachedId = thread->threadId;
	fy_threadPushFrame(context, thread, method, exception);
	fy_exceptionCheckAndReturn(exception);
	clazz = fy_vmLookupClass(context, context->sStringArray, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}

	thread->stack[0] = fy_heapAllocateArray(context, clazz, 0, exception);
	thread->typeStack[0] = FY_TYPE_HANDLE;
	if (exception->exceptionType != exception_none) {
		return;
	}
}

void fy_threadInitWithRun(fy_context *context, fy_thread *thread, int handle,
		fy_exception *exception) {
	fy_class *handleClass = fy_heapGetClassOfObject(context, handle);
	fy_class *threadClass = fy_vmLookupClass(context, context->sThread,
			exception);
	fy_object *obj;
	fy_method *runner;
	fy_exceptionCheckAndReturn(exception);
	if (!fy_classCanCastTo(context, handleClass, threadClass)) {
		fy_fault(exception, NULL,
				"The create(int) is used to start a java/lang/Thread!");
		return;
	}
	runner = fy_vmLookupMethodVirtual(context, handleClass, context->sFRun,
			exception);
	fy_exceptionCheckAndReturn(exception);

	obj = context->objects + handle;
	thread->handle = handle;
	obj->attachedId = thread->threadId;
	fy_threadPushFrame(context, thread, runner, exception);
	fy_exceptionCheckAndReturn(exception);
	thread->stack[0] = handle;
	thread->typeStack[0] = FY_TYPE_HANDLE;
}

void fy_threadInitWithData(fy_context *context, fy_thread *thread,
		fy_byte *data, fy_int length, fy_exception *exception) {
	/*TODO*/
}

#define fy_nextU1(CODE) (fy_uint)CODE[pc++]
#define fy_nextS1(CODE) ((fy_byte)CODE[pc++])
#define fy_nextU2(CODE) (fy_B2TOI(CODE[pc],CODE[pc+1])&0xffff);pc+=2
#define fy_nextS2(CODE) fy_B2TOI(CODE[pc],CODE[pc+1]);pc+=2
#define fy_nextS4(CODE) fy_B4TOI(CODE[pc],CODE[pc+1],CODE[pc+2],CODE[pc+3]);pc+=4

/**
 * DON'T USE RETURN HERE!!!!
 */
void fy_threadRun(fy_context *context, fy_thread *thread, fy_message *message,
		fy_int ops) {
	fy_uint sb, sp, pc, lpc, opCount, op;
#ifndef _FY_GOTO
	enum FallOut {
		fallout_none, fallout_invoke, fallout_noinvoke
	}fallout;
#endif
#ifdef FY_STRICT_CHECK
	fy_uint size, localCount, codeSize;
#endif
	fy_ubyte *code;
	fy_method *method = NULL;
	fy_frame *frame = NULL;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	fy_memblock *block = context->memblocks;
	fy_exception *exception = &(message->body.exception);

#ifndef _FY_LATE_DECLARATION
	fy_uint ivalue, ivalue2, ivalue3, ivalue4;
	int i;
	fy_ulong lvalue, lvalue2;
	fy_double dvalue, dvalue2;
	fy_float fvalue, fvalue2;
	fy_method *mvalue;
	fy_field *field = NULL;
	fy_class *clazz1 = NULL, *clazz2 = NULL, *clinitClazz;
	fy_char ch1;
	fy_str str1;
	fy_uint type, type2, type3, type4;
	fy_int *pivalue, *pivalue2;
	fy_str *pstr1;
	fy_boolean bvalue1;
	fy_nh *nh;
	char msg[256];
	fy_exception exceptionToPrepare;

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
				frame = fy_threadPushFrame(context, thread, method, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				stack[frame->sb] = thread->currentThrowable;
				typeStack[frame->sb] = FY_TYPE_HANDLE;
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
			fy_uint ivalue;
#endif
			ivalue = processThrowable(context, frame, thread->currentThrowable,
					lpc, exception);
			if (exception->exceptionType != exception_none) {
				/*Drop to thread manager with critical exception*/
				message->messageType = message_exception;
				break;
			} else {
				if (ivalue & 0x80000000) {
					/*Not found, will return*/
					if (method->access_flags & FY_ACC_SYNCHRONIZED) {
						if (method->access_flags & FY_ACC_STATIC) {
							fy_threadMonitorExit(context, thread,
									method->owner->classObjId, exception);
						} else {
							fy_threadMonitorExit(context, thread, stack[sb],
									exception);
						}
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							break;
						}
					}
					frame = fy_threadPopFrame(context, thread);
					continue;
				} else {

					/*Found*/
					sp = sb + method->max_locals;
					lpc = pc = ivalue;
					fy_threadPushHandle(thread->currentThrowable);
					thread->currentThrowable = 0;
				}
			}
		}
#ifndef _FY_GOTO
		fallout = fallout_none;
#endif
		lpc = pc;
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
							clinitClazz->clinit, exception);
					if (exception->exceptionType != exception_none) {
						FY_FALLOUT_NOINVOKE
					}
					continue;
				} else {
					thread->yield = TRUE;
					FY_FALLOUT_NOINVOKE
				}
			}
		}

		for (; opCount < ops
#ifndef _FY_GOTO
				&& fallout == fallout_none
#endif
				; opCount++) {
			/*RUN_ONE_INST!!!!!*/
			lpc = pc;
			op = fy_nextU1(code);
#ifdef _DEBUG
#ifdef FY_VERBOSE
#ifdef _FY_LATE_DECLARATION
			char msg[256];
#endif
			fy_strSPrint(msg, 256, method->uniqueName);
			DLOG("##%2d %6d/%6d %s %d %s SB=%d SP=%d\n", thread->threadId,
					opCount, ops, msg, lpc, OP_NAME[op], sb, sp);
#endif
#endif

			switch (op) {
			case AALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopHandle(ivalue2);
				fy_threadPushHandle(
						fy_heapGetArrayHandle(context, ivalue2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case FALOAD:
			case IALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopHandle(ivalue2);
				fy_threadPushInt(
						fy_heapGetArrayHandle(context, ivalue2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case AASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				fy_class *clazz1, *clazz2;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPopHandle(ivalue3);
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
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				fy_heapPutArrayHandle(context, ivalue3, ivalue2, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case FASTORE:
			case IASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPopHandle(ivalue3);
				fy_heapPutArrayInt(context, ivalue3, ivalue2, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
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
				fy_uint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalInt(ivalue, ivalue2);
				fy_threadPushInt(ivalue2);
				break;
			}
			case ALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalHandle(ivalue, ivalue2);
				fy_threadPushHandle(ivalue2);
				break;
			}
			case ILOAD_0:
			case FLOAD_0: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(0, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_0: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(0, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_1:
			case FLOAD_1: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(1, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_1: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(1, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_2:
			case FLOAD_2: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(2, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_2: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(2, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_3:
			case FLOAD_3: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(3, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_3: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(3, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ANEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ch1, ivalue;
				fy_class *clazz1, *clazz2;
				fy_str str1;
#endif
				ch1 = fy_nextU2(code);
				fy_threadPopInt(ivalue);
				if (ivalue < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IndexOutOfBoundException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d<0", ivalue);
					FY_FALLOUT_NOINVOKE
					break;
				}

				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ch1],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				str1.content = NULL;
				fy_strInit(block, &str1, 64, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				if (clazz1->type == obj) {
					fy_strAppendUTF8(block, &str1, "[L", 3, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
					fy_strAppend(block, &str1, clazz1->className, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
					fy_strAppendUTF8(block, &str1, ";", 3, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				} else if (clazz1->type == arr) {
					fy_strAppendUTF8(block, &str1, "[", 3, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
					fy_strAppend(block, &str1, clazz1->className, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				clazz2 = fy_vmLookupClass(context, &str1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_strDestroy(block, &str1);
				fy_threadPushHandle(
						fy_heapAllocateArray(context, clazz2, ivalue, exception));
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case IRETURN:
			case FRETURN: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId, exception);
					} else {
						/*CUSTOM*/
						fy_threadMonitorExit(context, thread, stack[sb],
								exception);
					}
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				/*CUSTOM*/
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnInt(context, thread, ivalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case ARETURN: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId, exception);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb],
								exception);
					}
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				/*CUSTOM*/
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnHandle(context, thread, ivalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case ARRAYLENGTH: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue);
				ivalue2 = fy_heapArrayLength(context, ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}

				fy_threadPushInt(ivalue2);
				break;
			}
			case ISTORE:
			case FSTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopInt(ivalue2);
				fy_threadPutLocalInt(ivalue, ivalue2);
				break;
			}
			case ASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
				fy_uint type;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopType(ivalue2, type);
				fy_threadPutLocalType(ivalue, ivalue2, type);
				break;
			}
			case ISTORE_0:
			case FSTORE_0: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(0, ivalue);
				break;
			}
			case ASTORE_0: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(0, ivalue, type);
				break;
			}
			case ISTORE_1:
			case FSTORE_1: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(1, ivalue);
				break;
			}
			case ASTORE_1: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(1, ivalue, type);
				break;
			}
			case ISTORE_2:
			case FSTORE_2: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(2, ivalue);
				break;
			}
			case ASTORE_2: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(2, ivalue, type);
				break;
			}
			case ISTORE_3:
			case FSTORE_3: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(3, ivalue);
				break;
			}
			case ASTORE_3: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(3, ivalue, type);
				break;
			}
			case ATHROW: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				thread->currentThrowable = ivalue;
				FY_FALLOUT_NOINVOKE
				break;
			}
			case BALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayByte(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case BASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayByte(context, ivalue2, ivalue3, (fy_byte) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case BIPUSH: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS1(code);
				fy_threadPushInt(ivalue);
				break;
			}
			case CALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayChar(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case CASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayChar(context, ivalue2, ivalue3, ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case CHECKCAST: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
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
						FY_FALLOUT_NOINVOKE
						break;
					}
					if (!fy_classCanCastTo(context, clazz1, clazz2)) {
						message->messageType = message_exception;
						exception->exceptionType = exception_normal;
						strcpy_s( exception->exceptionName,
								sizeof(exception->exceptionName),
								"java/lang/ClassCastException");
						fy_strInit(block, &str1, 64, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_strAppendUTF8(block, &str1, "from ", 99, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_strAppend(block, &str1, clazz1->className,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_strAppendUTF8(block, &str1, "to ", 99, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_strAppend(block, &str1, clazz2->className,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_strSPrint(exception->exceptionDesc,
								sizeof(exception->exceptionDesc), &str1);
						fy_strDestroy(block, &str1);
						FY_FALLOUT_NOINVOKE
						break;
					}
				}
				break;
			}
			case D2F: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt(
						fy_floatToInt((fy_float)fy_longToDouble(lvalue)));
				break;
			}
			case D2I: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((fy_int)fy_longToDouble(lvalue));
				break;
			}
			case D2L: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = (fy_long) fy_longToDouble(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DADD: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue, dvalue2;
				fy_ulong lvalue;
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
				fy_ulong lvalue;
				fy_uint ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				lvalue = fy_heapGetArrayLong(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushLong(lvalue);
				break;
			}
			case LASTORE:
			case DASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue2, ivalue3;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayLong(context, ivalue2, ivalue3, lvalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case DCMPG: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue, dvalue2;
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
				fy_double dvalue, dvalue2;
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
				fy_double dvalue, dvalue2;
				fy_ulong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue / dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DMUL: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue, dvalue2;
				fy_ulong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue * dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DNEG: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue;
				fy_ulong lvalue;
#endif
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(-dvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DREM: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue, dvalue2;
				fy_ulong lvalue;
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
				fy_double dvalue;
#endif
				fy_threadPopDouble(dvalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId, exception);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb],
								exception);
					}
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnDouble(context, thread, dvalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case DSUB: {
#ifdef _FY_LATE_DECLARATION
				fy_double dvalue, dvalue2;
				fy_ulong lvalue;
#endif
				fy_threadPopDouble(dvalue2);
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(dvalue - dvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case DUP: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP_X1: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
				fy_uint type, type2;
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
				fy_uint ivalue, ivalue2, ivalue3;
				fy_uint type, type2, type3;
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
				fy_uint ivalue, ivalue2;
				fy_uint type, type2;
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
				fy_uint ivalue, ivalue2, ivalue3;
				fy_uint type, type2, type3;
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
				fy_uint ivalue, ivalue2, ivalue3, ivalue4;
				fy_uint type, type2, type3, type4;
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
				fy_float fvalue;
				fy_ulong lvalue;
#endif
				fy_threadPopFloat(fvalue);
				lvalue = fy_doubleToLong(fvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case F2I: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue = (fy_uint) (fy_int) fy_intToFloat(ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case F2L: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_ulong lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = (fy_ulong) (fy_long) fy_intToFloat(ivalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case FADD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
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
				fy_float fvalue, fvalue2;
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
				fy_float fvalue, fvalue2;
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
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue / fvalue2));
				break;
			}
			case FMUL: {
#ifdef _FY_LATE_DECLARATION
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue * fvalue2));
				break;
			}
			case FNEG: {
#ifdef _FY_LATE_DECLARATION
				fy_float fvalue;
#endif
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(-fvalue));
				break;
			}
			case FREM: {
#ifdef _FY_LATE_DECLARATION
				fy_float fvalue, fvalue2;
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
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue - fvalue2));
				break;
			}
			case GETFIELD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue2;
				fy_field *field;
				fy_uint type;
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
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (field->access_flags & FY_ACC_STATIC) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"field %s is static", msg);
					FY_FALLOUT_NOINVOKE
					break;
				}
				type = (fy_byte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					lvalue = fy_heapGetFieldLong(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushLong( lvalue);
					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetFieldHandle(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushHandle(ivalue);
				}
					break;
				default: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetFieldInt(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushInt( ivalue);
					break;
				}
				}
				break;
			}
			case GETSTATIC: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3;
				fy_uint type;
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
					FY_FALLOUT_NOINVOKE
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
									clinitClazz->clinit, exception);
							if (exception->exceptionType != exception_none) {
								message->messageType = message_exception;
								FY_FALLOUT_NOINVOKE
								break;/*EXCEPTION_THROWN*/
							}
							FY_FALLOUT_INVOKE
							break;
						} else {
							pc = lpc;
							thread->yield = TRUE;
							FY_FALLOUT_NOINVOKE
							break;
						}
					}
				}
				type = (fy_byte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					lvalue = fy_heapGetStaticLong(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushLong(lvalue);

					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue2;
#endif
					ivalue2 = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushHandle(ivalue2);

					break;
				}
				default: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}fy_threadPushInt(ivalue);

					break;
				}
				}
				break;
			}
			case GOTO: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS2(code);
				pc = lpc + ivalue;
				break;
			}
			case GOTO_W: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS4(code);
				pc = lpc + ivalue;
				break;
			}
			case I2B: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2C: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xffff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2D: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_long lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = fy_doubleToLong((fy_double) ivalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case I2F: {
				/*CUSTOM*/
				fy_checkPop(0, FY_TYPE_INT);
				stack[sp - 1] = fy_floatToInt((fy_float) stack[sp - 1]);
				break;
			}
			case I2L: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_long lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case I2S: {
				/*CUSTOM*/
				fy_checkPop(0, FY_TYPE_INT);
				stack[sp - 1] = (fy_short) stack[sp - 1];
				break;
			}
			case IADD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue+ivalue2);
				break;
			}
			case IAND: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
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
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPopInt(ivalue);
				ivalue = (fy_int) ivalue / (fy_int) ivalue2;
				fy_threadPushInt(ivalue);
				break;
			}
			case IF_ICMPEQ: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
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
				fy_uint ivalue, ivalue2, ivalue3;
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
				fy_uint ivalue, ivalue2, ivalue3;
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
				fy_uint ivalue, ivalue2, ivalue3;
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
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue < (fy_int) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPLE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue <= (fy_int) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPGT: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue > (fy_int) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IF_ICMPGE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue >= (fy_int) ivalue2) {
					pc = lpc + ivalue3;
				}
				break;
			}

			case IFEQ: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
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
				fy_uint ivalue, ivalue3;
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
				fy_uint ivalue, ivalue3;
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
				fy_uint ivalue, ivalue3;
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
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue < 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFLE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue <= 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFGT: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue > 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IFGE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextS2(code);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue >= 0) {
					pc = lpc + ivalue3;
				}
				break;
			}
			case IINC: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextU1(code);
				ivalue = fy_nextS1(code);
				/*CUSTOM*/
				stack[sb + ivalue3] += ivalue;
				break;
			}
			case IMUL: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPushInt((fy_uint)((fy_int)ivalue2) * ((fy_int)ivalue));
				break;
			}
			case INEG: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPushInt(-ivalue);
				break;
			}
			case INSTANCEOF: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue2, ivalue3;
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
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushInt(
						fy_classCanCastTo(context, clazz1, clazz2) ? 1 : 0);
				break;
			}
			case INVOKEINTERFACE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
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
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
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
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (!fy_classCanCastTo(context, clazz1, mvalue->owner)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}
				mvalue = fy_vmLookupMethodVirtual(context, clazz1,
						mvalue->fullName, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}
				if ((mvalue->access_flags & FY_ACC_PUBLIC) == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}
				if ((mvalue->access_flags & FY_ACC_ABSTRACT)) {
					message->messageType = message_exception;

					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & FY_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(block, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;
						}
						frame = fy_threadCurrentFrame(context, thread);
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case INVOKESPECIAL: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
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
				if ((clazz1->accessFlags & FY_ACC_SUPER)
						&& fy_classIsSuperClassOf(context, clazz2, clazz1)
						&& fy_strCmp(mvalue->name, context->sInit)) {
					mvalue = fy_vmLookupMethodVirtual(context, clazz1->super,
							mvalue->fullName, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
				}
				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					exception->exceptionDesc[0] = 0;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (fy_strCmp(mvalue->name, context->sInit) > 0
						&& mvalue->owner != clazz2) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_NO_METHOD);
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (mvalue->access_flags & FY_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (mvalue->access_flags & FY_ACC_ABSTRACT) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & FY_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(block, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;
						}
						frame = fy_threadCurrentFrame(context, thread);
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case INVOKESTATIC: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
				fy_class *clazz1, *clazz2, *clinitClazz;
				fy_method *mvalue;
#endif
				clazz1 = method->owner;
				ivalue3 = fy_nextU2(code);/*m*/
				mvalue = fy_vmLookupMethodFromConstant(context,
						(ConstantMethodRef*) (clazz1->constantPools[ivalue3]),
						exception);
				clazz2 = mvalue->owner;
				if ((mvalue->access_flags & FY_ACC_STATIC) == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
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
									clinitClazz->clinit, exception);
							if (exception->exceptionType != exception_none) {
								message->messageType = message_exception;
								FY_FALLOUT_NOINVOKE
								break;/*EXCEPTION_THROWN*/
							}
							FY_FALLOUT_INVOKE
							break;
						} else {
							pc = lpc;
							thread->yield = TRUE;
							FY_FALLOUT_NOINVOKE
							break;
						}
					}
				}
				ivalue = mvalue->paramCount;
				fy_checkCall(ivalue);
				sp -= ivalue;
				fy_localToFrame(frame);
				if (mvalue->access_flags & FY_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(block, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;
						}
						frame = fy_threadCurrentFrame(context, thread);
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case INVOKEVIRTUAL: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue3;
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
				ASSERT(typeStack[sp]==FY_TYPE_HANDLE);
				if (stack[sp] == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NullPointerException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}
				clazz2 = fy_heapGetObject(context, stack[sp])->clazz;
				mvalue = fy_vmLookupMethodVirtual(context, clazz2,
						mvalue->fullName, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}

				if (mvalue == NULL) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "");
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (mvalue->access_flags & FY_ACC_STATIC) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}
				if ((mvalue->access_flags & FY_ACC_ABSTRACT)) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/AbstractMethodError");
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}fy_localToFrame(frame);
				if (mvalue->access_flags & FY_ACC_NATIVE) {
#ifdef _FY_LATE_DECLARATION
					fy_nh *nh;
#endif
					nh = fy_hashMapGet(block, context->mapMUNameToNH,
							mvalue->uniqueName);
					if (nh == NULL) {
						message->messageType = message_invoke_native;
						message->body.nativeMethod = mvalue;
					} else {
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;
						}
						frame = fy_threadCurrentFrame(context, thread);
					}
				} else {
					fy_threadPushMethod(context, thread, mvalue, &frame,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case IOR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue|ivalue2);
				break;
				break;
			}
			case IREM: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					FY_FALLOUT_NOINVOKE
					break;
				}
				ivalue = (fy_int) ivalue % (fy_int) ivalue2;
				fy_threadPushInt(ivalue);
				break;
			}
			case ISHL: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue << ivalue2);
				break;
			}
			case ISHR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(((fy_int)ivalue) >> ((fy_int)ivalue2));
				break;
			}
			case ISUB: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue - ivalue2);
				break;
				break;
			}
			case IUSHR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue >> ivalue2);
				break;
			}
			case IXOR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue ^ ivalue2);
				break;
			}
			case JSR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS2(code);
				fy_threadPushReturn(pc);
				pc = lpc + ivalue;
				break;
			}
			case JSR_W: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS4(code);
				fy_threadPushReturn(pc);
				pc = lpc + ivalue;
				break;
			}
			case L2D: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = fy_doubleToLong(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case L2F: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue;
#endif
				fy_threadPopLong(lvalue);
				ivalue = fy_floatToInt((float) lvalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case L2I: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((fy_uint)lvalue);
				break;
			}
			case LADD: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue += lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LAND: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue &= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LCMP: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				fy_threadPushInt(
						lvalue == lvalue2 ? 0 : ((fy_long)lvalue > (fy_long)lvalue2 ? 1 : -1));
				break;
			}
			case LCONST_0: {
				fy_threadPushLong((fy_ulong)0);
				break;
			}
			case LCONST_1: {
				fy_threadPushLong((fy_ulong)1);
				break;
			}
			case LDC: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue;
				fy_uint type;
#endif
				ivalue3 = fy_nextU1(code);

				ivalue = opLDC(context, method->owner, ivalue3, &type,
						exception);

				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}

				fy_threadPushType(ivalue, type);
				break;
			}
			case LDC_W: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue;
				fy_uint type;
#endif
				ivalue3 = fy_nextU2(code);
				ivalue = opLDC(context, method->owner, ivalue3, &type,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushType(ivalue, type);
				break;
			}
			case LDC2_W: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3;
				fy_ulong lvalue;
#endif
				ivalue3 = fy_nextU2(code );
				lvalue = opLDC2(context, method->owner, ivalue3, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushLong(lvalue);
				break;
			}
			case LDIV: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					FY_FALLOUT_NOINVOKE
					break;
				}
				lvalue = (fy_ulong) ((fy_long) lvalue / (fy_long) lvalue2);
				fy_threadPushLong( lvalue);
				break;
			}
			case DLOAD:
			case LLOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue;
#endif
				ivalue = fy_nextU1(code);
				fy_threadGetLocalLong(ivalue, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_0:
			case LLOAD_0: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(0, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_1:
			case LLOAD_1: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(1, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_2:
			case LLOAD_2: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(2, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_3:
			case LLOAD_3: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(3, lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case LMUL: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue = (fy_ulong) ((fy_long) lvalue * (fy_long) lvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LNEG: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushLong(-lvalue);
				break;
			}
			case LOOKUPSWITCH: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				fy_int *pivalue, *pivalue2;
				fy_uint i;
				fy_boolean bvalue1;
#endif
				ivalue3 = (65536 - pc) % 4;
				pc += ivalue3;
				ivalue = fy_nextS4(code);/*db*/
				ivalue2 = fy_nextS4(code);/*np*/
				pivalue = fy_allocate(sizeof(fy_int) * ivalue2, exception); /*match*/
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				pivalue2 = fy_allocate(sizeof(fy_int) * ivalue2, exception); /*offset*/
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
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
				fy_free(pivalue);
				fy_free(pivalue2);
				if (!bvalue1) {
					pc = lpc + ivalue;
				}
				break;
			}
			case LOR: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue |= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LREM: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;

					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/ArithmeticException");
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					FY_FALLOUT_NOINVOKE
					break;
				}
				lvalue = (fy_ulong) ((fy_long) lvalue % (fy_long) lvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LRETURN: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId, exception);
					} else {
						/*CUSTOM*/
						fy_threadMonitorExit(context, thread, stack[sb],
								exception);
					}
				}
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnLong(context, thread, lvalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case LSHL: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_ulong lvalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopLong(lvalue);
				lvalue <<= ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case LSHR: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_ulong lvalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopLong(lvalue);
				lvalue = ((fy_long) lvalue) >> ivalue;
				fy_threadPushLong(lvalue);
				break;
			}
			case LUSHR: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue;
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
				fy_ulong lvalue;
				fy_uint ivalue;
#endif
				ivalue = fy_nextU1(code);
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(ivalue, lvalue);
				break;
			}
			case DSTORE_0:
			case LSTORE_0: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(0, lvalue);
				break;
			}
			case DSTORE_1:
			case LSTORE_1: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(1, lvalue);
				break;
			}
			case DSTORE_2:
			case LSTORE_2: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(2, lvalue);
				break;
			}
			case DSTORE_3:
			case LSTORE_3: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPutLocalLong(3, lvalue);
				break;
			}
			case LSUB: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue -= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LXOR: {
#ifdef _FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue ^= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case MONITORENTER: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadMonitorEnter(context, thread, ivalue);
				break;
			}
			case MONITOREXIT: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadMonitorExit(context, thread, ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				break;
			}
			case MULTIANEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				fy_int *pivalue;
				fy_class *clazz1;
				fy_int i;
#endif
				ivalue3 = fy_nextU2(code );
				ivalue = fy_nextU1(code );
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ivalue3],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				pivalue = fy_allocate(sizeof(int) * ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				for (i = ivalue - 1; i >= 0; i--) {
					fy_threadPopInt(pivalue[i]);
				}
				ivalue2 = threadmulitANewArray(context, clazz1, ivalue, pivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushHandle(ivalue2);
				fy_free(pivalue);
				break;
			}
			case NEW: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue;
				fy_class *clazz1, *clinitClazz;
#endif
				ivalue3 = fy_nextU2(code);
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ivalue3],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (clazz1->accessFlags
						& (FY_ACC_INTERFACE | FY_ACC_ABSTRACT)) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					fy_strSPrint(msg, 256, clazz1->className);
					fy_fault(exception, NULL, "InstantiationErro %s", msg);
					FY_FALLOUT_NOINVOKE
					break;
				}
				{
					clinitClazz = clinit(context, thread, clazz1);
					if (clinitClazz) {
						if (clinitClazz->clinitThreadId == 0) {
							pc = lpc;
							fy_localToFrame(frame);
							clinitClazz->clinitThreadId = thread->threadId;
							frame = fy_threadPushFrame(context, thread,
									clinitClazz->clinit, exception);
							if (exception->exceptionType != exception_none) {
								message->messageType = message_exception;
								FY_FALLOUT_NOINVOKE
								break;/*EXCEPTION_THROWN*/
							}
							FY_FALLOUT_INVOKE
							break;
						} else {
							pc = lpc;
							thread->yield = TRUE;
							FY_FALLOUT_NOINVOKE
							break;
						}
					}
				}
				ivalue = fy_heapAllocate(context, clazz1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushHandle(ivalue);
				break;
			}
			case NEWARRAY: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue2;
				fy_uint type;
				fy_str *pstr1;
				fy_class *clazz1;
#endif
				type = fy_nextU1(code);
				fy_threadPopInt(ivalue3);
				if (ivalue3 < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/NegativeArraySizeException");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d", ivalue3);
					FY_FALLOUT_NOINVOKE
					break;
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
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/VirtualMachineError");
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Unknown array type in NEWARRAY type=%d", type);
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (message->messageType == message_exception) {
					break;
				}
				clazz1 = fy_vmLookupClass(context, pstr1, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				ivalue2 = fy_heapAllocateArray(context, clazz1, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
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
				fy_uint ivalue = 0, ivalue2, ivalue3;
				fy_ulong lvalue = 0;
				fy_uint type;
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
					FY_FALLOUT_NOINVOKE
					break;
				}
				type = (fy_byte) field->descriptor->content[0];
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
				if (field->access_flags & FY_ACC_STATIC) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IncompatibleClassChangeError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is static", msg);
					FY_FALLOUT_NOINVOKE
					break;
				}

				if ((field->access_flags & FY_ACC_FINAL)
						&& method->owner != field->owner) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is final", msg);
					FY_FALLOUT_NOINVOKE
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
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case PUTSTATIC: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3;
				fy_uint type;
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
					FY_FALLOUT_NOINVOKE
					break;
				}
				if ((field->access_flags & FY_ACC_FINAL)
						&& (field->owner != method->owner)) {
#ifdef _FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							"java/lang/IllegalAccessError");
					fy_strSPrint(msg, sizeof(msg), field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is final", msg);
					FY_FALLOUT_NOINVOKE
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
									clinitClazz->clinit, exception);
							if (exception->exceptionType != exception_none) {
								message->messageType = message_exception;
								FY_FALLOUT_NOINVOKE
								break;/*EXCEPTION_THROWN*/
							}
							FY_FALLOUT_INVOKE
							break;
						} else {
							pc = lpc;
							thread->yield = TRUE;
							FY_FALLOUT_NOINVOKE
							break;
						}
					}
				}
				type = (fy_byte) field->descriptor->content[0];
				switch (type) {
				case 'D':
				case 'J': {
#ifdef _FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					fy_threadPopLong(lvalue);
					fy_heapPutStaticLong(context, field, lvalue, exception);
					break;
				}
				case 'L':
				case '[': {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadPopInt(ivalue)
					fy_heapPutStaticHandle(context, field, ivalue, exception);
					break;
				}
				default: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
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
				fy_uint ivalue, ivalue3;
#endif
				ivalue3 = fy_nextU1(code);
				fy_threadGetLocalReturn(ivalue3, ivalue);
				pc = ivalue;
				break;
			}
			case RETURN: {
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						fy_threadMonitorExit(context, thread,
								method->owner->classObjId, exception);
					} else {
						fy_threadMonitorExit(context, thread, stack[sb],
								exception);
					}
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;/*EXCEPTION_THROWN*/
					}
				}
				if (method->clinit) {
					method->owner->clinitThreadId = -1;
				}
				frame = fy_threadPopFrame(context, thread);
				FY_FALLOUT_INVOKE
				break;
			}
			case SALOAD: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				ivalue = fy_heapGetArrayShort(context, ivalue2, ivalue3,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}fy_threadPushInt(ivalue);
				break;
			}
			case SASTORE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue3);
				fy_threadPopHandle(ivalue2);
				fy_heapPutArrayShort(context, ivalue2, ivalue3, (short) ivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				break;
			}
			case SIPUSH: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = fy_nextS2(code);
				fy_threadPushInt(ivalue);
				break;
			}
			case SWAP: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
				fy_uint type, type2;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPopType(ivalue2, type2);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue2, type2);
				break;
			}
			case TABLESWITCH: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3, ivalue4;
				fy_uint i;
				fy_int *pivalue;
#endif
				ivalue4 = (65536 - pc) % 4;
				pc += ivalue4;
				ivalue = fy_nextS4(code );/*db*/
				ivalue2 = fy_nextS4(code );/*lb*/
				ivalue3 = fy_nextS4(code );/*hb*/
				ivalue4 = ivalue3 - ivalue2 + 1;/*count*/
				pivalue = fy_allocate(sizeof(fy_int) * ivalue4, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				for (i = 0; i < ivalue4; i++) {
					pivalue[i] = fy_nextS4(code)
					;
				}fy_threadPopInt(ivalue4);
				if (ivalue4 < ivalue2 || ivalue4 > ivalue3) {
					ivalue4 = ivalue;
				} else {
					ivalue4 = pivalue[ivalue4 - ivalue2];
				}
				fy_free(pivalue);
				pc = lpc + ivalue4;
				break;
			}
			case WIDE: {
#ifdef _FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue4;
#endif
				ivalue4 = fy_nextU1(code);
				ivalue3 = fy_nextU2(code);
				switch (ivalue4) {
				case FLOAD:
				case ILOAD: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadGetLocalInt(ivalue3, ivalue);
					fy_threadPushInt(ivalue);
					break;
				}
				case ALOAD: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadGetLocalHandle(ivalue3, ivalue);
					fy_threadPushHandle(ivalue);
					break;
				}
				case DLOAD:
				case LLOAD: {
#ifdef _FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					fy_threadGetLocalLong(ivalue3, lvalue);
					fy_threadPushLong(lvalue);
					break;
				}
				case FSTORE:
				case ISTORE: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadPopInt(ivalue);
					fy_threadPutLocalInt( ivalue3, ivalue);
					break;
				}
				case ASTORE: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
					fy_uint type;
#endif
					fy_threadPopType(ivalue, type);
					fy_threadPutLocalType( ivalue3, ivalue, type);
					break;
				}
				case DSTORE:
				case LSTORE: {
#ifdef _FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					fy_threadPopLong(lvalue);
					fy_threadPutLocalLong( ivalue3, lvalue);
					break;
				}
				case RET: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadGetLocalReturn( ivalue3, ivalue);
					pc = ivalue;
					break;
				}
				case IINC: {
#ifdef _FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_nextS2(code);
					/*CuSTOM*/
					stack[sb + ivalue3] += ivalue;
					break;
				}
				default: {
					fy_fault(exception, NULL, "Unknown OPCode %d", ivalue4);
					FY_FALLOUT_NOINVOKE
					break;
				}
				}
				break;
			}
			}
			/*lpc = pc;*/
		}
#ifdef _FY_GOTO
		label_fallout_noinvoke:
#else
		if (fallout != fallout_invoke) {
#endif
		fy_localToFrame(frame);
#ifdef _FY_GOTO
		label_fallout_invoke:
#else
	}
#endif
		/*The only exit point, from here, the frame is always valid*/
		if (thread->yield) {
			thread->yield = FALSE;
			message->messageType = message_none;
			break;
		}
		if (message->messageType == message_exception) {
			//send the exception to the VM caller for PI handle.
#ifdef _FY_LATE_DECLARATION
			fy_exception exceptionToPrepare;
#endif
			if (exception->exceptionName[0] == 0) {
				break;
			}
			exceptionToPrepare = *exception;
			exception->exceptionType = exception_none;
			exception->exceptionName[0] = 0;
			exception->exceptionDesc[0] = 0;
			thread->currentThrowable = fy_threadPrepareThrowable(context,
					thread, &exceptionToPrepare, exception);
			if (exception->exceptionType != exception_none) {
				break;
			}
			message->messageType = message_continue;
		}
		if (message->messageType != message_continue) {
			break;
		}
	}
	if (message->messageType == message_continue) {
		message->messageType = message_none;
	}
}

fy_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
		_FY_RESTRICT fy_exception *toPrepare,
		_FY_RESTRICT fy_exception *exception) {
	fy_class *clazz1;
	fy_str *str1;
	fy_uint ivalue, ivalue2;
	fy_field *field;
	fy_memblock *block = context->memblocks;
	str1 = fy_strCreateFromUTF8(block, toPrepare->exceptionName, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	clazz1 = fy_vmLookupClass(context, str1, exception);
	fy_strRelease(block, str1);
	if (exception->exceptionType != exception_none) {
		return 0;
	}

	if (!fy_classCanCastTo(context, clazz1, context->TOP_THROWABLE)) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ClassCastException");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				clazz1->className);
		return 0;
	}
	ivalue = fy_heapAllocate(context, clazz1, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	field = fy_vmGetField(context, context->sThrowableDetailMessage);
	if (field == NULL) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				"java/lang/ClassNotFoundException");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sThrowableDetailMessage);
		return 0;
	}
	str1 = fy_strCreateFromUTF8(block, toPrepare->exceptionDesc, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	ivalue2 = fy_heapMakeString(context, str1, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}fy_strRelease(block, str1);
	fy_heapPutFieldHandle(context, ivalue, field, ivalue2, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	fy_threadFillException(context, thread, ivalue, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	return ivalue;
}

void fy_threadReturnInt(fy_context *context, fy_thread *thread, fy_int value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	fy_uint sp = frame->sp;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	stack[sp] = value;
	typeStack[sp] = FY_TYPE_INT;
	frame->sp = sp + 1;
}

void fy_threadReturnHandle(fy_context *context, fy_thread *thread, fy_int value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	fy_uint sp = frame->sp;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	stack[sp] = value;
	typeStack[sp] = FY_TYPE_HANDLE;
	frame->sp = sp + 1;
}

void fy_threadReturnLong(fy_context *context, fy_thread *thread, fy_long value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	fy_uint sp = frame->sp;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	stack[sp] = fy_HOFL(value);
	stack[sp + 1] = fy_LOFL(value);
	typeStack[sp] = FY_TYPE_WIDE;
	typeStack[sp + 1] = FY_TYPE_WIDE2;
	frame->sp = sp + 2;
}

