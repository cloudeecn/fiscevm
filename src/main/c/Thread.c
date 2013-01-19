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
#include "fyc/Instructions.h"

static fy_nh _NO_HANDLER;
static fy_nh *NO_HANDLER = &_NO_HANDLER;

static fy_int processThrowable(fy_context *context, fy_frame *frame,
		fy_int handle, fy_int lpc, fy_exception *exception) {
	fy_class *throwableClass;
	fy_class *handlerClass;
	fy_int i, imax;
	fy_exceptionHandler *handlers;
	fy_exceptionHandler *handler;
	fy_int target = -1;
	DLOG(context, "EXCEPTION HANDLE LOOKUP: LPC=%ld", lpc);
#ifdef FY_DEBUG
	context->logDVar(context,"Exception: ");
	context->logDStr(context,
			fy_heapGetClassOfObject(context, handle, exception)->className);
	context->logDVar(context,"\nat ");
	context->logDStr(context, frame->method->uniqueName);
	context->logDVar(context,"\n");
	context->logDVar(context,"LPC=%"FY_PRINT32"d ", lpc);
#endif
	throwableClass = fy_heapGetClassOfObject(context, handle, exception);
	FYEH()0;
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
				if (fy_classCanCastTo(context, throwableClass, handlerClass, TRUE)) {
					target = handler->handler_pc;
					break;
				}
			}
		}
	}
#ifdef FY_DEBUG
	context->logDVarLn(context,"target=%"FY_PRINT32"d", target);
#endif
	return target;
	//Jump after found the target is move to run() for further optimize
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
		*type = 0;
		return ((ConstantIntegerFloatInfo*) (owner->constantPools[index]))->value;
	case CONSTANT_String:
		*type = 1;
		constantStringInfo =
				((ConstantStringInfo*) (owner->constantPools[index]));
		hvalue = fy_heapLookupStringFromConstant(context, constantStringInfo,
				exception);
		return hvalue;
	case CONSTANT_Class:
		constantClass = (ConstantClass*) (owner->constantPools[index]);
		*type = 1;
		clazz = fy_vmLookupClassFromConstant(context, constantClass, exception);
		if (exception->exceptionType != exception_none) {
			return 0;
		}
		return fy_vmGetClassObjHandle(context, clazz, exception);
	default:
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC type wrong! %"FY_PRINT32"d,%d", index,
				owner->constantTypes[index]);
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
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"LDC2 type wrong! %"FY_PRINT32"d,%d", index,
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
	obj->object_data->threadId = 0;
	thread->handle = 0;

	thread->waitForLockId = 0;
	thread->waitForNotifyId = 0;
	thread->nextWakeTime = 0;
	thread->pendingLockCount = 0;
	thread->destroyPending = FALSE;
	for (handle = 1; handle < MAX_OBJECTS; handle++) {
		obj = context->objects + handle;
		if (obj->object_data
				&& obj->object_data->monitorOwnerId == thread->threadId) {
			obj->object_data->monitorOwnerId = 0;
			obj->object_data->monitorOwnerTimes = 0;
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
	fy_str str[1];
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
				FY_EXCEPTION_VM);
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
				FY_EXCEPTION_VM);
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementDeclaringClass);
		return;
	}

	methodNameField = fy_vmGetField(context,
			context->sStackTraceElementMethodName);
	if (methodNameField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementMethodName);
		return;
	}

	fileNameField = fy_vmGetField(context, context->sStackTraceElementFileName);
	if (fileNameField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementFileName);
		return;
	}

	lineNumberField = fy_vmGetField(context,
			context->sStackTraceElementLineNumber);
	if (lineNumberField == NULL) {
		exception->exceptionType = exception_normal;
		strcpy_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sStackTraceElementLineNumber);
		return;
	}
	arrayHandle = fy_heapAllocateArray(context, array, thread->frameCount,
			exception);
	FY_SIMPLE_ERROR_HANDLE
	fy_heapPutFieldHandle(context, handle, throwableStackTraceElements,
			arrayHandle, exception);
	FY_SIMPLE_ERROR_HANDLE

	t = 0;
	for (i = thread->frameCount - 1; i >= 0; i--) {
		itemHandle = fy_heapAllocate(context, clazz, exception);
		FY_SIMPLE_ERROR_HANDLE
		fy_heapPutArrayHandle(context, arrayHandle, t, itemHandle, exception);
		FY_SIMPLE_ERROR_HANDLE
		frame = FY_GET_FRAME(thread,i);
		method = frame->method;
		str->content = NULL;
		fy_strInit(context->memblocks, str, method->owner->className->length,
				exception);
		FYEH();
		fy_strAppend(context->memblocks, str, method->owner->className,
				exception);
		if (exception->exceptionType != exception_none) {
			fy_strDestroy(context->memblocks, str);
			return;
		}
		fy_strReplaceOne(str, '/', '.');
		strHandle = fy_heapMakeString(context, str, exception);
		FYEH();
		fy_strDestroy(context->memblocks, str);
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

		t++;
	}

	/*The last line, so don't need error handle*/
}

#ifdef FY_GOTO
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

# define fy_checkPopSize(SIZE) { \
	if(sp<=sb+localCount+(SIZE)){ \
		fy_fault(exception,NULL,"Stack underflow! base=%d sp=%d localvars=%d",sb,sp,localCount); \
		message->messageType=message_exception; \
		FY_FALLOUT_NOINVOKE \
		break; \
	} \
}

# define fy_checkPop(SIZE,T) { \
	fy_checkPopSize(SIZE) \
	if(T!=fy_bitGet(typeStack, sp-1)){ \
		fy_fault(exception,NULL,"Type mismatch, needs [%d] but got [%d]",(T),fy_bitGet(typeStack, sp-1)); \
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
	if((T)!=fy_bitGet(typeStack, sb+(P))){\
		fy_fault(exception,NULL,"Type mismatch, needs [%d] but got [%d]",\
				(T),fy_bitGet(typeStack, sb+(P))); \
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
# define fy_checkPopSize(SIZE)
# define fy_checkPop(SIZE,T)
# define fy_frameToLocalCheck(F)
# define fy_localToFrameCheck(F)
# define fy_checkPutLocal(P,SIZE)
# define fy_checkGetLocal(P,SIZE,T)
# define fy_checkCall(COUNT)
#endif

#define fy_threadPushType(V,T) { \
	fy_checkPush(0) \
	if(T)fy_bitSet(typeStack,sp);else fy_bitClear(typeStack,sp);\
	stack[sp++]=V; \
}

#define fy_threadPushInt(V) { \
	fy_checkPush(0) \
	fy_bitClear(typeStack,sp);stack[sp++]=V; \
}

#define fy_threadPushHandle(V) { \
		fy_checkPush(0) \
	fy_bitSet(typeStack,sp);stack[sp++]=V; \
}
#define fy_threadPushReturn(V) { \
	fy_checkPush(0) \
	fy_bitClear(typeStack,sp);stack[sp++]=V; \
}

#define fy_threadPushLong(W) { \
	fy_checkPush(1) \
	fy_bitClear(typeStack,sp);stack[sp++]=fy_HOFL((W)); \
	fy_bitClear(typeStack,sp);stack[sp++]=fy_LOFL((W)); \
}

#define fy_threadPopType(O,T) { \
	fy_checkPopSize(0) \
	O=stack[--sp]; \
	T=fy_bitGet(typeStack,sp); \
}

#define fy_threadPopInt(O) { \
	fy_checkPop(0,0) \
	O=stack[--sp]; \
}

#define fy_threadPopFloat(O) { \
	fy_checkPop(0,0) \
	O=fy_intToFloat(stack[--sp]); \
}

#define fy_threadPopHandle(O) { \
	fy_checkPop(0,1) \
	O=stack[--sp]; \
}

#define fy_threadPopReturn(O) { \
	fy_checkPop(0,0) \
	O=stack[--sp]; \
}

#define fy_threadPopLong(O) { \
	fy_checkPop(1,0) \
	O = (fy_ulong)stack[--sp]; \
	O += ((fy_ulong)stack[--sp])<<32; \
}

#define fy_threadPopDouble(O) { \
	fy_checkPop(1,0) \
	sp-=2; \
	O=fy_longToDouble((((fy_ulong)stack[sp])<<32)+(fy_ulong)stack[sp+1]); \
}

#define fy_threadPutLocalInt(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	fy_bitClear(typeStack,sb+(P)); \
}

#define fy_threadPutLocalHandle(P,V) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	fy_bitSet(typeStack,sb+(P)); \
}

#define fy_threadPutLocalType(P,V,TYPE) { \
	fy_checkPutLocal(P,0) \
	stack[sb+(P)]=(V); \
	if(TYPE)fy_bitSet(typeStack,sb+(P)); else fy_bitClear(typeStack,sb+(P)); \
}

#define fy_threadPutLocalLong(P,W) { \
	fy_checkPutLocal(P,1) \
	stack[sb+(P)]=fy_HOFL(W); \
	stack[sb+(P)+1]=fy_LOFL(W); \
	fy_bitClear(typeStack,sb+(P)); \
	fy_bitClear(typeStack,sb+(P)+1); \
}

#define fy_threadGetLocalInt(P,O) { \
	fy_checkGetLocal(P,0,FALSE) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalHandle(P,O) { \
	fy_checkGetLocal(P,0,TRUE) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalReturn(P,O) { \
	fy_checkGetLocal(P,0,FALSE) \
	O=stack[sb+(P)]; \
}

#define fy_threadGetLocalLong(P,O) { \
	fy_checkGetLocal(P,1,FALSE) \
	O=stack[sb+(P)]; \
	O=((fy_ulong)O<<32)+((fy_ulong)stack[sb+(P)+1]); \
}

#define fy_frameToLocal(ptrFrame) { \
	sp=ptrFrame->sp; \
	pc=ptrFrame->pc; \
	lpc=ptrFrame->lpc; \
	sb=ptrFrame->sb; \
	method = ptrFrame->method; \
	instructions = method->instructions; \
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

fy_frame *fy_threadCurrentFrame(fy_context *context, fy_thread *thread) {
	fy_uint frameCount = thread->frameCount;
	if (frameCount == 0) {
		return NULL;
	} else {
		return FY_GET_FRAME(thread,thread->frameCount-1);
	}
}

static fy_frame *fy_threadPushFrame(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_exception *exception) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	fy_uint sb, sp;
	if (!(invoke->access_flags & FY_ACC_VERIFIED)) {
		fy_preverify(context, invoke, exception);
		FYEH()NULL;
	}
	if (frame == NULL) {
		sb = 0;
		sp = invoke->max_locals;
		frame = FY_GET_FRAME(thread,0);
	} else {
		sb = frame->sp;
		sp = sb + invoke->max_locals;
		frame--;
	}
	if (sp + invoke->max_stack + (++(thread->frameCount)) * FY_FRAME_ENTRIES
			>= STACK_SIZE) {
		fy_fault(exception, NULL, "STACK OVERFLOW! sp=%d frame_size=", sp,
				thread->frameCount * FY_FRAME_ENTRIES);
		return NULL;
	}
	frame->method = invoke;
	frame->instructions = invoke->instructions;
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
		return FY_GET_FRAME(thread,fc-1);
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
	FYEH();
	if (localFrame != NULL) {
		*localFrame = frame;
	}

	if (invoke->access_flags & FY_ACC_SYNCHRONIZED) {
		fy_threadMonitorEnter(context, thread,
				(invoke->access_flags & FY_ACC_STATIC) ?
						fy_vmGetClassObjHandle(context, invoke->owner,
								exception) :
						thread->stack[frame->sb]);
	}

}

void fy_threadInitWithMethod(fy_context *context, fy_thread *thread,
		int threadHandle, fy_method *method, fy_exception *exception) {
	fy_class *clazz;
	fy_object *obj = context->objects + threadHandle;
	if (!fy_strEndsWith(method->uniqueName, context->sFMain)) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"The boot method must be static void main(String[])");
		return;
	}
	if ((method->access_flags & FY_ACC_STATIC) == 0) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_VM);
		sprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				"The first method of a thread must have no return value.");
		return;
	}
	thread->handle = threadHandle;
	obj->object_data->threadId = thread->threadId;
	fy_threadPushFrame(context, thread, method, exception);
	FYEH();
	clazz = fy_vmLookupClass(context, context->sStringArray, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}

	thread->stack[0] = fy_heapAllocateArray(context, clazz, 0, exception);
	fy_bitSet(thread->typeStack, 0);
	if (exception->exceptionType != exception_none) {
		return;
	}
}

void fy_threadInitWithRun(fy_context *context, fy_thread *thread, int handle,
		fy_exception *exception) {
	fy_class *handleClass;
	fy_class *threadClass;
	fy_object *obj;
	fy_method *runner;
	handleClass = fy_heapGetClassOfObject(context, handle, exception);
	FYEH();
	threadClass = fy_vmLookupClass(context, context->sThread, exception);
	FYEH();
	if (!fy_classCanCastTo(context, handleClass, threadClass, TRUE)) {
		fy_fault(exception, NULL,
				"The create(int) is used to start a "FY_BASE_THREAD"!");
		return;
	}
	runner = fy_vmLookupMethodVirtual(context, handleClass, context->sFRun,
			exception);
	FYEH();

	obj = context->objects + handle;
	thread->handle = handle;
	obj->object_data->threadId = thread->threadId;
	fy_threadPushFrame(context, thread, runner, exception);
	FYEH();
	thread->stack[0] = handle;
	fy_bitSet(thread->typeStack, 0);
}

static void doInvoke(fy_context *context, fy_thread *thread, fy_frame *frame,
		fy_method *method, fy_uint paramsCount, fy_message *message,
		fy_exception *exception) {
	fy_nh *nh;
	if (method->access_flags & FY_ACC_NATIVE) {
		nh = method->nh;
		if (nh == NULL) {
			nh = fy_hashMapGet(context->memblocks, context->mapMUNameToNH,
					method->uniqueName);
			nh = method->nh = (nh == NULL ? NO_HANDLER : nh);
		}
		if (nh == NO_HANDLER) {
			message->messageType = message_invoke_native;
			message->body.call.method = method;
			message->body.call.paramCount = paramsCount;
			message->body.call.params = thread->stack + frame->sp;
		} else {
			fy_heapBeginProtect(context);
			(nh->handler)(context, thread, nh->data, thread->stack + frame->sp,
					paramsCount, message, exception);
			FYEH();
		}
	} else {
		fy_threadPushMethod(context, thread, method, &frame, exception);
		FYEH();
	}
}

static void invokeVirtual(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_exception *exception,
		fy_message *message) {
	fy_int count = method->paramCount + 1;
	fy_method *actureMethod;
	fy_class *clazz;
	fy_uint *stack;
	fy_uint sp;
#ifdef FY_VERBOSE
	context->logDVar(context,"Invoke virtual: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context,"\n");
#endif
#ifdef FY_STRICT_CHECK
	if (frame->sp - (count) - frame->sb < frame->localCount) {
		fy_fault(exception, NULL, "Buffer underflow! %d %d",
				frame->sp - (count) - frame->sb, frame->localCount);
		return;
	}
#endif
	frame->sp -= count;
	sp = frame->sp;
	ASSERT(fy_bitGet(thread->typeStack,frame->sp));
	stack = thread->stack;
	if (stack[frame->sp] == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	if (method->access_flags & FY_ACC_FINAL) {
		actureMethod = method;
	} else {
		clazz = fy_heapGetClassOfObject(context, stack[sp], exception);
		FYEH();
		actureMethod = fy_vmLookupMethodVirtualByMethod(context, clazz, method,
				exception);
		FYEH();
#ifdef FY_VERBOSE
		context->logDStr(context, actureMethod->uniqueName);
		context->logDVar(context,"\n");
#endif
	}
#ifdef FY_VERBOSE
	context->logDVar(context,"\tmethod is: ");
	context->logDStr(context, actureMethod->uniqueName);
	context->logDVar(context,"\n");
#endif
	doInvoke(context, thread, frame, actureMethod, count, message, exception);
}

static void invokeDirect(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_exception *exception,
		fy_message *message) {
	fy_int count = method->paramCount + 1;
	fy_class *clazz;
	fy_uint *stack;
	fy_object *object;
	fy_uint sp;
#ifdef FY_VERBOSE
	context->logDVar(context,"Invoke direct: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context,"\n");
#endif
#ifdef FY_STRICT_CHECK
	if (frame->sp - (count) - frame->sb < frame->localCount) {
		fy_fault(exception, NULL, "Buffer underflow! %d %d",
				frame->sp - (count) - frame->sb, frame->localCount);
		return;
	}
#endif
	frame->sp -= count;
	sp = frame->sp;
	ASSERT(fy_bitGet(thread->typeStack,frame->sp));
	stack = thread->stack;
	if (stack[frame->sp] == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return;
	}
	object = fy_heapGetObject(context,stack[sp]);
	if (!fy_classCanCastTo(context, object->object_data->clazz,
			method->owner, TRUE)) {
		fy_fault(exception, FY_EXCEPTION_CAST, "");
		return;
	}
#ifdef FY_VERBOSE
	context->logDVar(context,"\tmethod is: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context,"\n");
#endif
	doInvoke(context, thread, frame, method, count, message, exception);
}

static void invokeStatic(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_exception *exception,
		fy_message *message) {
	char msg[256];
	fy_class *owner = method->owner;
	fy_class *clinitClazz;
	fy_uint count = method->paramCount;
#ifdef FY_VERBOSE
	context->logDVar(context,"Invoke static: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context,"\n");
#endif

	if (!(method->access_flags & FY_ACC_STATIC)) {
		fy_strSPrint(msg, 256, method->uniqueName);
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "%s is not static",
				msg);
		return;
	}

	{
		clinitClazz = clinit(context, thread, owner);
		if (clinitClazz) {
			if (clinitClazz->clinitThreadId == 0) {
				frame->pc--;
				clinitClazz->clinitThreadId = thread->threadId;
				frame = fy_threadPushFrame(context, thread, clinitClazz->clinit,
						exception);
				FYEH();
				return;
			} else {
				frame->pc--;
				thread->yield = TRUE;
				return;
			}
		}
	}
#ifdef FY_STRICT_CHECK
	if (frame->sp - (count) - frame->sb < frame->localCount) {
		fy_fault(exception, NULL, "Buffer underflow! %d %d",
				frame->sp - (count) - frame->sb, frame->localCount);
		return;
	}
#endif
	frame->sp -= count;
#ifdef FY_VERBOSE
	context->logDStr(context, method->uniqueName);
	context->logDVar(context,"\n");
#endif
	doInvoke(context, thread, frame, method, count, message, exception);
}

void fy_threadInvoke(fy_context *context, fy_thread *thread, fy_method *method,
		fy_message *message, fy_exception *exception) {
	if (method->access_flags & FY_ACC_STATIC) {
		invokeStatic(context, thread, fy_threadCurrentFrame(context, thread),
				method, exception, message);
	} else {
		invokeVirtual(context, thread, fy_threadCurrentFrame(context, thread),
				method, exception, message);
	}
}
/**
 * DON'T USE RETURN HERE!!!!
 */
void fy_threadRun(fy_context *context, fy_thread *thread, fy_message *message,
		fy_int ops) {
	fy_uint sb, sp, pc, lpc, opCount;
#ifndef FY_GOTO
	enum FallOut {
		fallout_none, fallout_invoke, fallout_noinvoke
	}fallout;
#endif
#ifdef FY_STRICT_CHECK
	fy_uint size, localCount, codeSize;
#endif
	fy_instruction *instructions;
	fy_instruction *instruction;
	fy_method *method = NULL;
	fy_frame *frame = NULL;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	fy_memblock *block = context->memblocks;
	fy_exception *exception = &(message->body.exception);

#ifndef FY_LATE_DECLARATION
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
				DLOG(context, "XXXXXXXXXXUnhandled Exception!!!XXXXXXXXXXXX");
				method = fy_vmGetMethod(context,
						context->sThrowablePrintStacktrace);
				ASSERT( method != NULL);
				frame = fy_threadPushFrame(context, thread, method, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					break;
				}
				stack[frame->sb] = thread->currentThrowable;
				fy_bitSet(typeStack, frame->sb);
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
#ifdef FY_LATE_DECLARATION
			fy_uint ivalue, ivalue2;
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
							ivalue2 = fy_vmGetClassObjHandle(context,
									method->owner, exception);
							if (exception->exceptionType != exception_none) {
								message->messageType = message_exception;
								break;
							}
							fy_threadMonitorExit(context, thread, ivalue2,
									exception);
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
#ifndef FY_GOTO
		fallout = fallout_none;
#endif
		lpc = pc;
		if (method->access_flags & FY_ACC_CLINIT) {
#ifdef FY_LATE_DECLARATION
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
#ifndef FY_GOTO
				&& fallout == fallout_none
#endif
				; opCount++) {
			/*RUN_ONE_INST!!!!!*/
			lpc = pc;
			instruction = instructions + (pc++);
#ifdef FY_PROFILE
			context->opUsage[instruction->op].count++;
			context->opCombine[(context->lastOp << 8) + instruction->op].count++;
			context->lastOp = instruction->op;
#endif
#ifdef FY_VERBOSE
#ifdef FY_LATE_DECLARATION
			char msg[256];
#endif
			fy_strSPrint(msg, 256, method->uniqueName);
			context->logDVar(context,"##%2d %6d/%6d %s %d %s SB=%d SP=%d\n", thread->threadId,
					opCount, ops, msg, lpc, FY_OP_NAME[instruction->op], sb,
					sp);
#endif

			switch (instruction->op) {
			case AALOAD: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				fy_class *clazz1, *clazz2;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPopHandle(ivalue3);
				clazz1 = fy_heapGetClassOfObject(context, ivalue3, exception);
				clazz2 = clazz1->ci.arr.contentClass;
				if (ivalue != 0
						&& !fy_classCanCastTo(context,
								fy_heapGetClassOfObject(context, ivalue,
										exception), clazz2, TRUE)) {
					exception->exceptionType = exception_normal;
					strcpy_s(exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_STORE);
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
#endif
				fy_threadGetLocalInt(instruction->params.int_params.param1,
						ivalue2);
				fy_threadPushInt(ivalue2);
				break;
			}
			case ALOAD: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
#endif
				fy_threadGetLocalHandle( instruction->params.int_params.param1,
						ivalue2);
				fy_threadPushHandle(ivalue2);
				break;
			}
			case ILOAD_0:
			case FLOAD_0: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(0, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_0: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(0, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_1:
			case FLOAD_1: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(1, ivalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%d\n", ivalue);
#endif
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_1: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(1, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_2:
			case FLOAD_2: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(2, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_2: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(2, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ILOAD_3:
			case FLOAD_3: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalInt(3, ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case ALOAD_3: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalHandle(3, ivalue);
				fy_threadPushHandle(ivalue);
				break;
			}
			case ANEWARRAY: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_class *clazz1, *clazz2;
				fy_str str1;
#endif
				fy_threadPopInt(ivalue);
				if (((fy_int)ivalue) < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_AIOOB);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc), "%d<0", ivalue);
					FY_FALLOUT_NOINVOKE
					break;
				}

				clazz1 = instruction->params.clazz;
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
				if (clazz1->type == object_class) {
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
				} else if (clazz1->type == array_class) {
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						ivalue2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_threadMonitorExit(context, thread, ivalue2,
								exception);
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						ivalue2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_threadMonitorExit(context, thread, ivalue2,
								exception);
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue);
				ivalue2 = fy_heapArrayLength(context, ivalue, exception);
#ifdef FY_VERBOSE
				context->logDVar(context,"%d\n", ivalue2);
#endif
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPutLocalInt(instruction->params.int_params.param1,
						ivalue2);
				break;
			}
			case ASTORE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
				fy_uint type;
#endif
				fy_threadPopType(ivalue2, type);
				fy_threadPutLocalType(instruction->params.int_params.param1,
						ivalue2, type);
				break;
			}
			case ISTORE_0:
			case FSTORE_0: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(0, ivalue);
				break;
			}
			case ASTORE_0: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(0, ivalue, type);
				break;
			}
			case ISTORE_1:
			case FSTORE_1: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(1, ivalue);
				break;
			}
			case ASTORE_1: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(1, ivalue, type);
				break;
			}
			case ISTORE_2:
			case FSTORE_2: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(2, ivalue);
				break;
			}
			case ASTORE_2: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(2, ivalue, type);
				break;
			}
			case ISTORE_3:
			case FSTORE_3: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPutLocalInt(3, ivalue);
				break;
			}
			case ASTORE_3: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPutLocalType(3, ivalue, type);
				break;
			}
			case ATHROW: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				thread->currentThrowable = ivalue;
				FY_FALLOUT_NOINVOKE
				break;
			}
			case BALOAD: {
#ifdef FY_LATE_DECLARATION
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
				}
				fy_threadPushInt(ivalue);
				break;
			}
			case BASTORE: {
#ifdef FY_LATE_DECLARATION
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
				fy_threadPushInt(instruction->params.int_params.param1);
				break;
			}
			case CALOAD: {
#ifdef FY_LATE_DECLARATION
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
				}
				fy_threadPushInt(ivalue);
				break;
			}
			case CASTORE: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
				fy_class *clazz1, *clazz2;
				fy_str str1;
#endif
				/*CUSTOM*/
				ivalue2 = stack[sp - 1];
				if (ivalue2 != 0) {
					clazz1 = fy_heapGetClassOfObject(context, ivalue2,
							exception);
					clazz2 = instruction->params.clazz;
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					if (!fy_classCanCastTo(context, clazz1, clazz2, TRUE)) {
						message->messageType = message_exception;
						strcpy_s( exception->exceptionName,
								sizeof(exception->exceptionName),
								FY_EXCEPTION_CAST);
						str1.content = NULL;
						fy_strInit(block, &str1, 64, exception);
						fy_strAppendUTF8(block, &str1, "from ", 99, exception);
						fy_strAppend(block, &str1, clazz1->className,
								exception);
						fy_strAppendUTF8(block, &str1, " to ", 99, exception);
						fy_strAppend(block, &str1, clazz2->className,
								exception);
						fy_strSPrint(exception->exceptionDesc,
								sizeof(exception->exceptionDesc), &str1);
						fy_strDestroy(block, &str1);
						exception->exceptionType = exception_normal;
						FY_FALLOUT_NOINVOKE
						break;
					}
				}
				break;
			}
			case D2F: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt(
						fy_floatToInt((fy_float)fy_longToDouble(lvalue)));
				break;
			}
			case D2I: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((fy_int)fy_longToDouble(lvalue));
				break;
			}
			case D2L: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = (fy_long) fy_longToDouble(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DADD: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
				}
				fy_threadPushLong(lvalue);
				break;
			}
			case LASTORE:
			case DASTORE: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_double dvalue;
				fy_ulong lvalue;
#endif
				fy_threadPopDouble(dvalue);
				lvalue = fy_doubleToLong(-dvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case DREM: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_double dvalue;
				fy_uint ivalue2;
#endif
				fy_threadPopDouble(dvalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						ivalue2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_threadMonitorExit(context, thread, ivalue2,
								exception);
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
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnDouble(context, thread, dvalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case DSUB: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				fy_threadPopType(ivalue, type);
				fy_threadPushType(ivalue, type);
				fy_threadPushType(ivalue, type);
				break;
			}
			case DUP_X1: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_float fvalue;
				fy_ulong lvalue;
#endif
				fy_threadPopFloat(fvalue);
				lvalue = fy_doubleToLong(fvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case F2I: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue = (fy_uint) (fy_int) fy_intToFloat(ivalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case F2L: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_ulong lvalue;
#endif
				fy_threadPopInt(ivalue);
				lvalue = (fy_ulong) (fy_long) fy_intToFloat(ivalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case FADD: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue / fvalue2));
				break;
			}
			case FMUL: {
#ifdef FY_LATE_DECLARATION
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue * fvalue2));
				break;
			}
			case FNEG: {
#ifdef FY_LATE_DECLARATION
				fy_float fvalue;
#endif
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(-fvalue));
				break;
			}
			case FREM: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_float fvalue, fvalue2;
#endif
				fy_threadPopFloat(fvalue2);
				fy_threadPopFloat(fvalue);
				fy_threadPushInt(fy_floatToInt(fvalue - fvalue2));
				break;
			}
			case GETFIELD: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
				fy_field *field;
				fy_uint type;
#endif
				fy_threadPopHandle(ivalue2);
				field = instruction->params.field;
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (field->access_flags & FY_ACC_STATIC) {
#ifdef FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_INCOMPAT_CHANGE);
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
#ifdef FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					lvalue = fy_heapGetFieldLong(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushLong( lvalue);
#ifdef FY_VERBOSE
					context->logDVar(context,"Long field:[");
					context->logDStr(context, field->uniqueName);
					context->logDVar(context,"] = %"FY_PRINT64"d\n", lvalue);
#endif
					break;
				}
				case 'L':
				case '[': {
#ifdef FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetFieldHandle(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushHandle(ivalue);
#ifdef FY_VERBOSE
					context->logDVar(context,"Handle field:[");
					context->logDStr(context, field->uniqueName);
					context->logDVar(context,"] = %d\n", ivalue);
#endif
				}
					break;
				default: {
#ifdef FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetFieldInt(context, ivalue2, field,
							exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushInt( ivalue);
#ifdef FY_VERBOSE
					context->logDVar(context,"Integer field:[");
					context->logDStr(context, field->uniqueName);
					context->logDVar(context,"] = %d\n", ivalue);
#endif
					break;
				}
				}
				break;
			}
			case GETSTATIC: {
#ifdef FY_LATE_DECLARATION
				fy_uint type;
				fy_field *field;
				fy_class *clazz1, *clinitClazz;
#endif

				field = instruction->params.field;
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
#ifdef FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					lvalue = fy_heapGetStaticLong(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushLong(lvalue);

					break;
				}
				case 'L':
				case '[': {
#ifdef FY_LATE_DECLARATION
					fy_uint ivalue2;
#endif
					ivalue2 = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushHandle(ivalue2);

					break;
				}
				default: {
#ifdef FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					ivalue = fy_heapGetStaticInt(context, field, exception);
					if (exception->exceptionType != exception_none) {
						message->messageType = message_exception;
						FY_FALLOUT_NOINVOKE
						break;
					}
					fy_threadPushInt(ivalue);

					break;
				}
				}
				break;
			}
			case GOTO_W:
			case GOTO: {
				pc = instruction->params.int_params.param1;
				break;
			}
			case I2B: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2C: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				ivalue &= 0xffff;
				fy_threadPushInt(ivalue);
				break;
			}
			case I2D: {
#ifdef FY_LATE_DECLARATION
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
				fy_checkPop(0, 0);
				stack[sp - 1] = fy_floatToInt((fy_float) stack[sp - 1]);
				break;
			}
			case I2L: {
#ifdef FY_LATE_DECLARATION
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
				fy_checkPop(0, 0);
				stack[sp - 1] = (fy_short) stack[sp - 1];
				break;
			}
			case IADD: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%d+%d=%d\n", ivalue, ivalue2, ivalue + ivalue2);
#endif
				fy_threadPushInt(ivalue+ivalue2);
				break;
			}
			case IAND: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ARITHMETIC);
					strcpy_s( exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Divided by zero!");
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_threadPopInt(ivalue);
				ivalue = (fy_int) ivalue / (fy_int) ivalue2;
				fy_threadPushInt(ivalue);
				break;
			}
			case IF_ICMPEQ: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue == ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ACMPEQ: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue2);
				fy_threadPopHandle(ivalue);
				if (ivalue == ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ICMPNE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue != ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ACMPNE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopHandle(ivalue2);
				fy_threadPopHandle(ivalue);
				if (ivalue != ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ICMPLT: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue < (fy_int) ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ICMPLE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue <= (fy_int) ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ICMPGT: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue > (fy_int) ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IF_ICMPGE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue >= (fy_int) ivalue2) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}

			case IFEQ: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if (ivalue == 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IFNULL: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				if (ivalue == 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}

			case IFNE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if (ivalue != 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IFNONNULL: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				if (ivalue != 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}

			case IFLT: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue < 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IFLE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue <= 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IFGT: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue > 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IFGE: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				if ((fy_int) ivalue >= 0) {
					pc = instruction->params.int_params.param1;
				}
				break;
			}
			case IINC: {
				/*CUSTOM*/
				stack[sb + instruction->params.int_params.param1] +=
						instruction->params.int_params.param2;
				break;
			}
			case IMUL: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPopInt(ivalue2);
				fy_threadPushInt(
						(fy_uint)((fy_int)ivalue2) * ((fy_int)ivalue));
				break;
			}
			case INEG: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopInt(ivalue);
				fy_threadPushInt(-ivalue);
				break;
			}
			case INSTANCEOF: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
				fy_class *clazz1, *clazz2;
#endif
				fy_threadPopHandle(ivalue2);
				clazz1 = fy_heapGetClassOfObject(context, ivalue2, exception);
				clazz2 = instruction->params.clazz;
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_threadPushInt(
						fy_classCanCastTo(context, clazz1, clazz2, TRUE) ? 1 : 0);
				break;
			}
			case INVOKESPECIAL: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_class *clazz1, *clazz2;
				fy_method *mvalue;
#endif
				clazz1 = method->owner;
				mvalue = instruction->params.method;
#ifdef FY_VERBOSE
				context->logDVar(context,"Invoke special: ");
				context->logDStr(context, mvalue->uniqueName);
				context->logDVar(context,"\n");
#endif
				clazz2 = mvalue->owner;
				ivalue = mvalue->paramCount + 1;/*count*/
				fy_checkCall(ivalue);
				sp -= ivalue;
				if ((clazz1->accessFlags & FY_ACC_SUPER)
						&& fy_classIsSuperClassOf(context, clazz2, clazz1)
						&& fy_strCmp(mvalue->name, context->sInit)) {
					mvalue = fy_vmLookupMethodVirtualByMethod(context,
							clazz1->super, mvalue, exception);
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
							FY_EXCEPTION_ABSTRACT);
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
							FY_EXCEPTION_INCOMPAT_CHANGE);
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
							FY_EXCEPTION_ABSTRACT);
					fy_strSPrint(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							mvalue->uniqueName);
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_localToFrame(frame);
#ifdef FY_VERBOSE
				context->logDStr(context, mvalue->uniqueName);
				context->logDVar(context,"\n");
#endif
				if (mvalue->access_flags & FY_ACC_NATIVE) {
#ifdef FY_LATE_DECLARATION
					fy_nh *nh = mvalue->nh;
#endif
					if (nh == NULL) {
						nh = fy_hashMapGet(block, context->mapMUNameToNH,
								mvalue->uniqueName);
						nh = mvalue->nh = (nh == NULL ? NO_HANDLER : nh);
					}
					if (nh == NO_HANDLER) {
						message->messageType = message_invoke_native;
						message->body.call.method = mvalue;
						message->body.call.paramCount = ivalue;
						message->body.call.params = stack + sp;
					} else {
						fy_heapBeginProtect(context);
						(nh->handler)(context, thread, nh->data, stack + sp,
								ivalue, message, exception);
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
				fy_localToFrame(frame);
				invokeStatic(context, thread, frame, instruction->params.method,
						exception, message);
				frame = fy_threadCurrentFrame(context, thread);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case INVOKEINTERFACE:
			case INVOKEVIRTUAL: {
				fy_localToFrame(frame);
				invokeVirtual(context, thread, frame,
						instruction->params.method, exception, message);
				frame = fy_threadCurrentFrame(context, thread);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
				}
				FY_FALLOUT_INVOKE
				break;
			}
			case IOR: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue|ivalue2);
				break;
				break;
			}
			case IREM: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				if (ivalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ARITHMETIC);
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue << ivalue2);
				break;
			}
			case ISHR: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(((fy_int)ivalue) >> ((fy_int)ivalue2));
				break;
			}
			case ISUB: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue - ivalue2);
				break;
				break;
			}
			case IUSHR: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue >> ivalue2);
				break;
			}
			case IXOR: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
#endif
				fy_threadPopInt(ivalue2);
				fy_threadPopInt(ivalue);
				fy_threadPushInt(ivalue ^ ivalue2);
				break;
			}
			case JSR:
			case JSR_W: {
				fy_threadPushReturn(pc);
				pc = instruction->params.int_params.param1;
				break;
			}
			case L2D: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				lvalue = fy_doubleToLong(lvalue);
				fy_threadPushLong(lvalue);
				break;
			}
			case L2F: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue;
#endif
				fy_threadPopLong(lvalue);
				ivalue = fy_floatToInt((float) lvalue);
				fy_threadPushInt(ivalue);
				break;
			}
			case L2I: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushInt((fy_uint)lvalue);
				break;
			}
			case LADD: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d+%"FY_PRINT64"d=%"FY_PRINT64"d\n", lvalue,
						lvalue2, lvalue + lvalue2);
#endif
				lvalue += lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LAND: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue &= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LCMP: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				ivalue = opLDC(context, method->owner,
						instruction->params.ldc.value, &type, exception);

				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}

				fy_threadPushType(ivalue, type);
				break;
			}
			case LDC_W: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_uint type;
#endif
				ivalue = opLDC(context, method->owner,
						instruction->params.ldc.value, &type, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_threadPushType(ivalue, type);
				break;
			}
			case LDC2_W: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				lvalue = opLDC2(context, method->owner,
						instruction->params.ldc.value, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_threadPushLong(lvalue);
				break;
			}
			case LDIV: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ARITHMETIC);
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
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(instruction->params.int_params.param1,
						lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"LOCAL[%d]=%"FY_PRINT64"d\n",
						instruction->params.int_params.param1, lvalue);
#endif
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_0:
			case LLOAD_0: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(0, lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"LOCAL[0]=%"FY_PRINT64"d\n", lvalue);
#endif
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_1:
			case LLOAD_1: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(1, lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"LOCAL[1]=%"FY_PRINT64"d\n", lvalue);
#endif
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_2:
			case LLOAD_2: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(2, lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"LOCAL[2]=%"FY_PRINT64"d\n", lvalue);
#endif
				fy_threadPushLong(lvalue);
				break;
			}
			case DLOAD_3:
			case LLOAD_3: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadGetLocalLong(3, lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"LOCAL[3]=%"FY_PRINT64"d\n", lvalue);
#endif
				fy_threadPushLong(lvalue);
				break;
			}
			case LMUL: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue = (fy_ulong) ((fy_long) lvalue * (fy_long) lvalue2);
				fy_threadPushLong(lvalue);
				break;
			}
			case LNEG: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
				fy_threadPushLong(-lvalue);
				break;
			}
			case LOOKUPSWITCH: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2;
				fy_uint i;
				fy_boolean bvalue1;
				fy_switch_lookup *swlookup;
#endif
				swlookup = instruction->params.swlookup;
				ivalue = swlookup->count;
				fy_threadPopInt(ivalue2);
				bvalue1 = FALSE;
				for (i = 0; i < ivalue; i++) {
					if (swlookup->targets[i].value == ivalue2) {
						pc = swlookup->targets[i].target;
						bvalue1 = TRUE;
					}
				}
				if (!bvalue1) {
					pc = swlookup->defaultJump;
				}
				break;
			}
			case LOR: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue |= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LREM: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				if (lvalue2 == 0) {
					message->messageType = message_exception;

					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ARITHMETIC);
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
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
				fy_uint ivalue2;
#endif
				fy_threadPopLong(lvalue);
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						ivalue2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_threadMonitorExit(context, thread, ivalue2,
								exception);
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
				}
				fy_localToFrame(frame);
				frame = fy_threadPopFrame(context, thread);
				fy_threadReturnLong(context, thread, lvalue);
				FY_FALLOUT_INVOKE
				break;
			}
			case LSHL: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d->LOCAL[%d]\n", lvalue,
						instruction->params.int_params.param1);
#endif
				fy_threadPutLocalLong(instruction->params.int_params.param1,
						lvalue);
				break;
			}
			case DSTORE_0:
			case LSTORE_0: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d->LOCAL[%d]\n", lvalue, 0);
#endif
				fy_threadPutLocalLong(0, lvalue);
				break;
			}
			case DSTORE_1:
			case LSTORE_1: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d->LOCAL[%d]\n", lvalue, 1);
#endif
				fy_threadPutLocalLong(1, lvalue);
				break;
			}
			case DSTORE_2:
			case LSTORE_2: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d->LOCAL[%d]\n", lvalue, 2);
#endif
				fy_threadPutLocalLong(2, lvalue);
				break;
			}
			case DSTORE_3:
			case LSTORE_3: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue;
#endif
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d->LOCAL[%d]\n", lvalue, 3);
#endif
				fy_threadPutLocalLong(3, lvalue);
				break;
			}
			case LSUB: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
#ifdef FY_VERBOSE
				context->logDVar(context,"%"FY_PRINT64"d-%"FY_PRINT64"d=%"FY_PRINT64"d\n", lvalue,
						lvalue2, lvalue - lvalue2);
#endif
				lvalue -= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case LXOR: {
#ifdef FY_LATE_DECLARATION
				fy_ulong lvalue, lvalue2;
#endif
				fy_threadPopLong(lvalue2);
				fy_threadPopLong(lvalue);
				lvalue ^= lvalue2;
				fy_threadPushLong(lvalue);
				break;
			}
			case MONITORENTER: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadPopHandle(ivalue);
				fy_threadMonitorEnter(context, thread, ivalue);
				break;
			}
			case MONITOREXIT: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				fy_int *pivalue;
				fy_class *clazz1;
				fy_int i;
#endif
				ivalue3 = instruction->params.int_params.param1;
				ivalue = instruction->params.int_params.param2;
				clazz1 = fy_vmLookupClassFromConstant(context,
						(ConstantClass*) method->owner->constantPools[ivalue3],
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				pivalue = /*TEMP*/fy_allocate(sizeof(int) * ivalue, exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;/*EXCEPTION_THROWN*/
				}
				for (i = ivalue - 1; i >= 0; i--) {
					fy_threadPopInt(pivalue[i]);
				}
				fy_heapBeginProtect(context);
				ivalue2 = fy_heapMultiArray(context, clazz1, ivalue, pivalue,
						exception);
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				fy_threadPushHandle(ivalue2);
				fy_free(pivalue);
				break;
			}
			case NEW: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
				fy_class *clazz1, *clinitClazz;
#endif
				clazz1 = instruction->params.clazz;
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if (clazz1->accessFlags
						& (FY_ACC_INTERFACE | FY_ACC_ABSTRACT)) {
#ifdef FY_LATE_DECLARATION
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
				}
				fy_threadPushHandle(ivalue);
				break;
			}
			case NEWARRAY: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue3, ivalue2;
				fy_uint type;
				fy_str *pstr1;
				fy_class *clazz1;
#endif
				type = instruction->params.int_params.param1;
				fy_threadPopInt(ivalue3);
				if (((fy_int)ivalue3) < 0) {
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_NASE);
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
							sizeof(exception->exceptionName), FY_EXCEPTION_VM);
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
				}
				fy_threadPushHandle(ivalue2);
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue = 0, ivalue2;
				fy_ulong lvalue = 0;
				fy_uint type;
				fy_field *field;
#endif
				field = instruction->params.field;
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
				}
				fy_threadPopHandle(ivalue2);
				if (field->access_flags & FY_ACC_STATIC) {
#ifdef FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_INCOMPAT_CHANGE);
					fy_strSPrint(msg, 256, field->uniqueName);
					sprintf_s(exception->exceptionDesc,
							sizeof(exception->exceptionDesc),
							"Field %s is static", msg);
					FY_FALLOUT_NOINVOKE
					break;
				}

				if ((field->access_flags & FY_ACC_FINAL)
						&& method->owner != field->owner) {
#ifdef FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ACCESS);
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
#ifdef FY_VERBOSE
					context->logDVar(context,"Long field:[");
					context->logDStr(context, field->uniqueName);
					context->logDVar(context,"] = %"FY_PRINT64"d(%"FY_PRINT64"x)\n", lvalue,lvalue);
#endif
					break;
				default:
					fy_heapPutFieldInt(context, ivalue2, field, ivalue,
							exception);
#ifdef FY_VERBOSE
					context->logDVar(context,"Field:[");
					context->logDStr(context, field->uniqueName);
					context->logDVar(context,"] = %d(%x)\n", ivalue,ivalue);
#endif
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
#ifdef FY_LATE_DECLARATION
				fy_uint type;
				fy_field *field;
				fy_class *clazz1, *clinitClazz;
#endif
				field = instruction->params.field;
				if (exception->exceptionType != exception_none) {
					message->messageType = message_exception;
					FY_FALLOUT_NOINVOKE
					break;
				}
				if ((field->access_flags & FY_ACC_FINAL)
						&& (field->owner != method->owner)) {
#ifdef FY_LATE_DECLARATION
					char msg[256];
#endif
					message->messageType = message_exception;
					exception->exceptionType = exception_normal;
					strcpy_s( exception->exceptionName,
							sizeof(exception->exceptionName),
							FY_EXCEPTION_ACCESS);
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
#ifdef FY_LATE_DECLARATION
					fy_ulong lvalue;
#endif
					fy_threadPopLong(lvalue);
					fy_heapPutStaticLong(context, field, lvalue, exception);
					break;
				}
				case 'L':
				case '[': {
#ifdef FY_LATE_DECLARATION
					fy_uint ivalue;
#endif
					fy_threadPopHandle(ivalue)
					fy_heapPutStaticHandle(context, field, ivalue, exception);
					break;
				}
				default: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				fy_threadGetLocalReturn( instruction->params.int_params.param1,
						ivalue);
				pc = ivalue;
				break;
			}
			case RETURN: {
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue2;
#endif
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						ivalue2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						if (exception->exceptionType != exception_none) {
							message->messageType = message_exception;
							FY_FALLOUT_NOINVOKE
							break;/*EXCEPTION_THROWN*/
						}
						fy_threadMonitorExit(context, thread, ivalue2,
								exception);
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
				if (method->access_flags & FY_ACC_CLINIT) {
					method->owner->clinitThreadId = -1;
				}
				frame = fy_threadPopFrame(context, thread);
				FY_FALLOUT_INVOKE
				break;
			}
			case SALOAD: {
#ifdef FY_LATE_DECLARATION
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
				}
				fy_threadPushInt(ivalue);
				break;
			}
			case SASTORE: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue;
#endif
				ivalue = instruction->params.int_params.param1;
				fy_threadPushInt(ivalue);
				break;
			}
			case SWAP: {
#ifdef FY_LATE_DECLARATION
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
#ifdef FY_LATE_DECLARATION
				fy_uint ivalue, ivalue2, ivalue3;
				/*fy_uint i;*/
#endif
				fy_threadPopInt(ivalue);
				ivalue2 = instruction->params.swtable->lowest;/*lb*/
				ivalue3 = instruction->params.swtable->highest;/*hb*/
				if ((fy_int) ivalue < (fy_int) ivalue2
						|| (fy_int) ivalue > (fy_int) ivalue3) {
					pc = instruction->params.swtable->defaultJump;
				} else {
					pc = instruction->params.swtable->targets[ivalue - ivalue2];
				}
				break;
			}
			default: {
				fy_fault(exception, NULL, "Unknown OPCode %d", instruction->op);
				FY_FALLOUT_NOINVOKE
				break;
			}
			}
			/*lpc = pc;*/
		}
#ifdef FY_GOTO
		label_fallout_noinvoke:
#else
		if (fallout != fallout_invoke) {
#endif
		fy_localToFrame(frame);
#ifdef FY_GOTO
		label_fallout_invoke:
#else
	}
#endif

		if (message->messageType == message_continue) {
			if (thread->yield) {
				thread->yield = FALSE;
				message->messageType = message_none;
				break;
			}
		} else if (message->messageType == message_invoke_native) {
			break;
		} else if (message->messageType == message_exception) {
			//send the exception to the VM caller for PI handle.
#ifdef FY_LATE_DECLARATION
			fy_exception exceptionToPrepare;
#endif
			if (exception->exceptionName[0] == 0) {
				break;
			}
			exceptionToPrepare = *exception;
			exception->exceptionType = exception_none;
			exception->exceptionName[0] = 0;
			exception->exceptionDesc[0] = 0;
			fy_heapBeginProtect(context);
			thread->currentThrowable = fy_threadPrepareThrowable(context,
					thread, &exceptionToPrepare, exception);
			if (exception->exceptionType != exception_none) {
				break;
			}
			message->messageType = message_continue;
		} else if (message->messageType == message_thread_dead) {
			break;
		} else {
			fy_fault(NULL, NULL, "Fault! Invalid message type %d\n",
					message->messageType);
		}
	}
	if (message->messageType == message_continue) {
		message->messageType = message_none;
	}
}

fy_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
		FY_ATTR_RESTRICT fy_exception *toPrepare,
		FY_ATTR_RESTRICT fy_exception *exception) {
	fy_class *clazz1;
	fy_str str1[1];
	fy_uint ivalue, ivalue2;
	fy_field *field;
	fy_memblock *block = context->memblocks;
	str1->content = NULL;
	fy_strInitWithUTF8(block, str1, toPrepare->exceptionName, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	clazz1 = fy_vmLookupClass(context, str1, exception);
	fy_strDestroy(block, str1);
	if (exception->exceptionType != exception_none) {
		return 0;
	}

	if (!fy_classCanCastTo(context, clazz1, context->TOP_THROWABLE, TRUE)) {
		exception->exceptionType = exception_normal;
		sprintf_s(exception->exceptionName, sizeof(exception->exceptionName),
				FY_EXCEPTION_CAST);
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
				FY_EXCEPTION_CLASSNOTFOUND);
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				context->sThrowableDetailMessage);
		return 0;
	}
	fy_strInitWithUTF8(block, str1, toPrepare->exceptionDesc, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	ivalue2 = fy_heapMakeString(context, str1, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	fy_strDestroy(block, str1);
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
	fy_bitClear(typeStack, sp);
	frame->sp = sp + 1;
}

void fy_threadReturnHandle(fy_context *context, fy_thread *thread, fy_int value) {
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	/*As both run and main are void, no need to consider frame is NULL*/
	fy_uint sp = frame->sp;
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	stack[sp] = value;
	fy_bitSet(typeStack, sp);
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
	fy_bitClear(typeStack, sp);
	fy_bitClear(typeStack, sp+1);
	frame->sp = sp + 2;
}

void fy_threadScanRef(fy_context *context, fy_thread *thread, fy_uint *marks) {
	fy_uint i, imax, handle;
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	fy_uint *stack = thread->stack;
	fy_uint *typeStack = thread->typeStack;
	fy_object *object;

	if (frame == NULL) {
		/*Thread already dead but not removed*/
		return;
	}

	imax = frame->sb + frame->method->max_locals + frame->method->max_stack;
	ASSERT(imax<=STACK_SIZE-thread->frameCount*FY_FRAME_ENTRIES);
	for (i = 0; i < imax; i++) {
		/*TODO we can scan 32bits a time first and skip all-zero blocks*/
		if (fy_bitGet(typeStack, i)) {
			handle = stack[i];
			if (handle == 0) {
				continue;
			}
			/**/
			ASSERT(
					i>=frame->sb || (handle > 0 && handle < MAX_OBJECTS && fy_heapGetObject(context,handle)->object_data!= NULL));
			if (handle > 0 && handle < MAX_OBJECTS) {
				object = fy_heapGetObject(context,handle);
				if (object->object_data != NULL
						&& object->object_data->data != NULL) {
					/*Valid handle*/
					fy_bitSet(marks, handle);
				}
			}
		}
	}
}
