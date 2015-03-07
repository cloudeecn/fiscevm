/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/Thread.h"

#include "fy_util/Debug.h"
#include "fyc/Debug.h"

#include "fy_util/ArrList.h"
#include "fy_util/HashMap.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"

#include "fyc/Instructions.h"
#include "fyc/Constants.h"
#include "fyc/Class.h"
#include "fyc/Heap.h"
#include "fyc/Preverifier.h"
#include "fyc/ThreadManager.h"
#include "fyc/VMContext.h"

#include <math.h>
#include <string.h>
#include <time.h>

static fy_nh _NO_HANDLER;
fy_nh *FY_NH_NO_HANDLER = &_NO_HANDLER;

static fy_int processThrowable(fy_context *context, fy_frame *frame,
		fy_int handle, fy_int lpc, fy_exception *exception) {
	fy_class *throwableClass;
	fy_class *handlerClass;
	fy_int i, imax;
	fy_exceptionHandler *handlers;
	fy_exceptionHandler *handler;
	fy_int target = -1;
#ifdef FY_DEBUG
	DLOG(context, "EXCEPTION HANDLE LOOKUP: LPC=%ld", lpc); /**/
	context->logDVar(context, "Exception: ");
	context->logDStr(context,
			fy_heapGetClassOfObject(context, handle, exception)->className);
	context->logDVar(context, "\nat ");
	context->logDStr(context, frame->method->uniqueName);
	context->logDVar(context, "\n");
	context->logDVar(context, "LPC=%"FY_PRINT32"d ", lpc);
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
				if (fy_classCanCastTo(context, throwableClass, handlerClass,
				TRUE)) {
					target = handler->handler_pc;
					break;
				}
			}
		}
	}
#ifdef FY_DEBUG
	context->logDVarLn(context, "target=%"FY_PRINT32"d", target);
#endif
	return target;
}

fy_int fy_threadMonitorEnter(fy_context *context, fy_thread *thread,
		fy_int handle, fy_int ops) {
	return fy_tmMonitorEnter(context, thread, handle, ops);
}

void fy_threadMonitorExit(fy_context *context, fy_thread *thread, fy_int handle,
		fy_exception *exception) {
	fy_tmMonitorExit(context, thread, handle, exception);
}

void fy_threadDestroy(fy_context *context, fy_thread *thread) {
	fy_uint handle;
	fy_object *obj;
	obj = context->objects + thread->handle;
	obj->object_data->m.threadId = 0;
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
	fy_int i, j, t, begin;
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

	for (begin = thread->frameCount - 1; begin >= 0; begin--) {
		frame = FY_GET_FRAME(thread, begin);
		if ((frame->method->access_flags & FY_ACC_STATIC)
				|| (frame->baseSpp->uvalue != handle)) {
			break;
		}
	}

	arrayHandle = fy_heapAllocateArray(context, array, begin + 1, exception);
	FY_SIMPLE_ERROR_HANDLE
	fy_heapPutFieldHandle(context, handle, throwableStackTraceElements,
			arrayHandle, exception);
	FY_SIMPLE_ERROR_HANDLE

	t = 0;
	for (i = begin; i >= 0; i--) {
		itemHandle = fy_heapAllocate(context, clazz, exception);
		FY_SIMPLE_ERROR_HANDLE
		fy_heapPutArrayHandle(context, arrayHandle, t, itemHandle, exception);
		FY_SIMPLE_ERROR_HANDLE
		frame = FY_GET_FRAME(thread, i);
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

fy_frame *fy_threadCurrentFrame(fy_context *context, fy_thread *thread) {
	fy_uint frameCount = thread->frameCount;
	if (frameCount == 0) {
		return NULL;
	} else {
		return FY_GET_FRAME(thread, thread->frameCount - 1);
	}
}

fy_frame *fy_threadPushFrame(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_stack_item *spp, fy_exception *exception) {
	fy_frame *frame;
#ifdef FY_STRICT_CHECK
	fy_frame *currentFrame = fy_threadCurrentFrame(context, thread);
	if (currentFrame == NULL) {

	} else {
		if (spp < currentFrame->baseSpp + currentFrame->method->max_locals) {
			fy_fault(exception, NULL, "Illegal spp: %p < %p[%"FY_PRINT32"d]",
					spp, currentFrame->baseSpp,
					currentFrame->method->max_locals);
		}
	}
#endif
	if (!(invoke->access_flags & FY_ACC_VERIFIED)) {
		fy_preverify(context, invoke, exception);
		FYEH()NULL;
	}
	frame = ((fy_frame*) ((thread)->stack
			+ (STACK_SIZE - (++thread->frameCount) * FY_FRAME_ENTRIES)));
	if ((void*) (spp + invoke->max_locals + invoke->max_stack)
			> (void*) frame) {
		fy_fault(exception, NULL, "STACK OVERFLOW! spp=%p frame=%p", spp,
				frame);
		return NULL;
	}

	frame->method = invoke;
	frame->baseSpp = spp;
	frame->lpc = FY_IP_begin;
	frame->pcofs = 0;
#ifdef FY_STRICT_CHECK
	frame->size = invoke->max_locals + invoke->max_stack;
	frame->localCount = invoke->max_locals;
	frame->codeSize = invoke->codeLength;
#endif
	return frame;
}

fy_frame *fy_threadPopFrame(fy_context *context, fy_thread *thread) {
	fy_uint fc = --(thread->frameCount);
	if (fc <= 0) {
		return NULL;
	} else {
		return FY_GET_FRAME(thread, fc - 1);
	}
}

static fy_class *clinit(fy_context *context, fy_thread *thread, fy_class *clazz) {
	fy_uint tid;
	fy_class *ret;
	if (clazz == NULL) {
		return NULL;
	}
	tid = clazz->clinitThreadId;
	if (likely(tid == -1 || (thread!= NULL && tid == thread->threadId))) {
		return NULL;
	}
	if (clazz->clinit == NULL) {
		ret = clinit(context, thread, clazz->s.super);
		if (ret == NULL) {
			clazz->clinitThreadId = -1;
		}
		return ret;
	} else {
		return clazz;
	}
	return NULL;
}

fy_class *fy_threadCheckClinit(fy_context *context, fy_thread *thread, fy_class *clazz) {
	return clinit(context, thread, clazz);
}

fy_int fy_threadPushMethod(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	fy_frame *frame = fy_threadPushFrame(context, thread, invoke, spp,
			exception);
	FYEH()0;

	if (invoke->access_flags & FY_ACC_SYNCHRONIZED) {
		return fy_threadMonitorEnter(context, thread,
				(invoke->access_flags & FY_ACC_STATIC) ?
						fy_vmGetClassObjHandle(context, invoke->owner,
								exception) :
						frame->baseSpp->uvalue, ops);
	} else {
		return ops;
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
	obj->object_data->m.threadId = thread->threadId;
	fy_threadPushFrame(context, thread, method, thread->stack, exception);
	FYEH();
	clazz = fy_vmLookupClass(context, context->sStringArray, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}

	thread->stack->uvalue = fy_heapAllocateArray(context, clazz, 0, exception);
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
	obj->object_data->m.threadId = thread->threadId;
#ifdef FY_VERBOSE
	context->logDVar(context, "Init thread with class ");
	context->logDStr(context, obj->object_data->clazz->className);
	context->logDVarLn(context, "With thread #%"FY_PRINT32"d",
			thread->threadId);
#endif
	fy_threadPushFrame(context, thread, runner, thread->stack, exception);
	FYEH();
	thread->stack->uvalue = handle;
}

static fy_int doInvoke(fy_context *context, fy_thread *thread, fy_frame *frame,
		fy_method *method, fy_uint paramsCount, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	fy_nh *nh;
	frame->pcofs = 1;
	if (method->access_flags & FY_ACC_NATIVE) {
		nh = method->c.nh;
		if (unlikely(nh == NULL)) {
			nh = fy_hashMapGet(context->memblocks, context->mapMUNameToNH,
					method->uniqueName);
			if (nh == NULL) {
				nh = FY_NH_NO_HANDLER;
			} else {
				nh->stack_count = paramsCount;
			}
			method->c.nh = nh;
		}
		if (nh == FY_NH_NO_HANDLER) {
			thread->pendingNative.methodName = method->utf8Name;
			thread->pendingNative.paramCount = paramsCount;
			thread->pendingNative.params = spp;
			ops = 0;
		} else {
			fy_heapBeginProtect(context);
			ops = (nh->handler)(context, thread, nh->data, spp, paramsCount,
					ops, exception);
			FYEH()0;
			fy_heapEndProtect(context);
		}
	} else {
		ops = fy_threadPushMethod(context, thread, method, spp, ops, exception);
		FYEH()0;
		if (ops <= 0) {
			return 0;
		}
		ops = (*(method->engine->runner))(context, thread,
				fy_threadCurrentFrame(context, thread), ops, exception, NULL);
	}
	return ops;
}

fy_int fy_threadInvokeSpecial(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	fy_int count = method->paramStackUsage + 1;
	fy_method *actureMethod = NULL;
#ifdef FY_VERBOSE
	context->logDVar(context, "Invoke special: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context, "\n");
#endif
#ifdef FY_STRICT_CHECK
	if (spp - count < frame->baseSpp + frame->localCount) {
		fy_fault(exception, NULL,
				"Buffer underflow! %p - %"FY_PRINT32"d < %p + %"FY_PRINT32"d",
				spp, count, frame->baseSpp, frame->localCount);
		return 0;
	}
#endif
	spp -= count;
	if (spp->uvalue == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return 0;
	}
	if (fy_strCmp(method->name, context->sInit)
			&& fy_classIsSuperClassOf(context, method->owner,
					frame->method->owner)) {
		/* call to method is not init and owned by super = call super.XXX() */
		actureMethod = fy_vmLookupMethodVirtualByMethod(context,
				frame->method->owner->s.super, method, exception);
		FYEH()0;
		if (actureMethod->owner != method->owner) {
			fy_fault(exception, FY_EXCEPTION_NO_METHOD, "");
			fy_strSPrint(exception->exceptionDesc,
					sizeof(exception->exceptionDesc), method->uniqueName);
			FYEH()0;
		}
	} else {
		/*Else call private or <init>*/
		actureMethod = method;
	}
	if (actureMethod == NULL) {
		fy_fault(exception, FY_EXCEPTION_ABSTRACT, "");
		FYEH()0;
	}
	if (actureMethod->access_flags & FY_ACC_STATIC) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				actureMethod->uniqueName);
		FYEH()0;
	}
	if (actureMethod->access_flags & FY_ACC_ABSTRACT) {
		fy_fault(exception, FY_EXCEPTION_ABSTRACT, "");
		fy_strSPrint(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				actureMethod->uniqueName);
		FYEH()0;
	}
#ifdef FY_VERBOSE
	context->logDVar(context, "\tmethod is: ");
	context->logDStr(context, actureMethod->uniqueName);
	context->logDVar(context, "\n");
#endif
	return doInvoke(context, thread, frame, actureMethod, count, spp, ops,
			exception);
}

fy_int fy_threadInvokeVirtual(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	fy_int count = method->paramStackUsage + 1;
	fy_method *actureMethod;
	fy_class *clazz;
#ifdef FY_VERBOSE
	context->logDVar(context, "Invoke virtual: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context, "\n");
#endif
#ifdef FY_STRICT_CHECK
	if (spp - count < frame->baseSpp + frame->localCount) {
		fy_fault(exception, NULL,
				"Buffer underflow! %p - %"FY_PRINT32"d < %p + %"FY_PRINT32"d",
				spp, count, frame->baseSpp, frame->localCount);
		return 0;
	}
#endif
	spp -= count;
	if (spp->uvalue == 0) {
		fy_fault(exception, FY_EXCEPTION_NPT, "");
		return 0;
	}
	if (method->access_flags & FY_ACC_FINAL) {
		actureMethod = method;
	} else {
		clazz = fy_heapGetClassOfObject(context, spp->uvalue, exception);
		FYEH()0;
		actureMethod = fy_vmLookupMethodVirtualByMethod(context, clazz, method,
				exception);
		FYEH()0;
#ifdef FY_VERBOSE
		context->logDStr(context, actureMethod->uniqueName);
		context->logDVar(context, "\n");
#endif
	}
#ifdef FY_VERBOSE
	context->logDVar(context, "\tmethod is: ");
	context->logDStr(context, actureMethod->uniqueName);
	context->logDVar(context, "\n");
#endif
	return doInvoke(context, thread, frame, actureMethod, count, spp, ops,
			exception);
}

fy_int fy_threadInvokeStatic(fy_context *context, fy_thread *thread,
		fy_frame *frame, fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	char msg[256];
#ifdef FY_DEBUG
	fy_class *owner = method->owner;
#endif
	fy_uint count = method->paramStackUsage;
#ifdef FY_VERBOSE
	context->logDVar(context, "Invoke static: ");
	context->logDStr(context, method->uniqueName);
	context->logDVar(context, "\n");
#endif

	if (!(method->access_flags & FY_ACC_STATIC)) {
		fy_strSPrint(msg, 256, method->uniqueName);
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "%s is not static",
				msg);
		return 0;
	}

#ifdef FY_DEBUG
	if (clinit(context, thread, owner) && method != owner->clinit) {
		context->logDStr(context, owner->clinit->uniqueName);
		context->logDVar(context, " / ");
		context->logDStr(context, method->uniqueName);
		context->logDVarLn(context, "");
		fy_fault(exception, NULL,
				"Clinit of class should be finished before invoke");
		return 0;
	}
#endif

#ifdef FY_STRICT_CHECK
	if (spp - count < frame->baseSpp + frame->localCount) {
		fy_fault(exception, NULL,
				"Buffer underflow! %p - %"FY_PRINT32"d < %p + %"FY_PRINT32"d",
				spp, count, frame->baseSpp, frame->localCount);
		return 0;
	}
#endif
	spp -= count;
#ifdef FY_VERBOSE
	context->logDStr(context, method->uniqueName);
	context->logDVar(context, "\n");
#endif
	return doInvoke(context, thread, frame, method, count, spp, ops, exception);
}

fy_int fy_threadInvoke(fy_context *context, fy_thread *thread,
		fy_method *method, fy_stack_item *spp, fy_int ops,
		fy_exception *exception) {
	if (method->access_flags & FY_ACC_STATIC) {
		return fy_threadInvokeStatic(context, thread,
				fy_threadCurrentFrame(context, thread), method, spp, ops,
				exception);
	} else {
		return fy_threadInvokeVirtual(context, thread,
				fy_threadCurrentFrame(context, thread), method, spp, ops,
				exception);
	}
}

fy_int fy_threadClinit(fy_context *context, fy_thread *thread, fy_class *clazz,
		fy_stack_item *spp, fy_int ops, fy_exception *exception) {
	/*!CLINIT*/
	fy_class *clinitClazz = NULL;
	fy_frame *frame = fy_threadCurrentFrame(context, thread);
	clinitClazz = clinit(context, thread, clazz);
	if (unlikely(clinitClazz)) {
		frame->pcofs = 0;
		if (clinitClazz->clinitThreadId == 0) {
#ifdef FY_VERBOSE
			context->logDVar(context,
					"Prepare invoke <clinit> rollback pcofs");
#endif
			clinitClazz->clinitThreadId = thread->threadId;
			fy_threadPushMethod(context, thread, clinitClazz->clinit, spp, ops,
					exception);
			return 0;
		} else {
#ifdef FY_VERBOSE
			context->logDVar(context,
					"Wait for <clinit> thread #%"FY_PRINT32"d rollback ip %p",
					clinitClazz->clinitThreadId, frame);
#endif
			return 0;
		}
	} else {
		return ops > 0 ? ops : 1;
	}
}

void fy_threadRun(fy_context *context, fy_thread *thread, fy_message *message,
		fy_int ops, fy_exception *exception) {
	fy_frame *frame;
	fy_method *method;
	fy_exception intrenalException[1];
#ifndef FY_LATE_DECLARATION
	fy_exception exceptionToPrepare;
	fy_uint i1, i2;
#endif

	intrenalException->exceptionType = exception_none;
	while (ops > 0) {
		frame = fy_threadCurrentFrame(context, thread);

		if (frame == NULL) {
			/*Time to quit!*/
			if (thread->currentThrowable) {
				DLOG(context, "XXXXXXXXXXUnhandled Exception!!!XXXXXXXXXXXX");
				method = fy_vmGetMethod(context,
						context->sThrowablePrintStacktrace);
				ASSERT(method != NULL);
				frame = fy_threadPushFrame(context, thread, method,
						thread->stack, exception);
				FYEH();
				frame->baseSpp->uvalue = thread->currentThrowable;
				thread->currentThrowable = 0;
			} else {
				message->messageType = message_thread_dead;
				break;
			}
		}
		method = frame->method;
		/**/
		if (thread->currentThrowable) {
#ifdef FY_LATE_DECLARATION
			fy_uint i1, i2;
#endif
			i1 = processThrowable(context, frame, thread->currentThrowable,
					frame->lpc, exception);
			FYEH();
			if (i1 & 0x80000000) {
				/*Not found, will return*/
				if (method->access_flags & FY_ACC_SYNCHRONIZED) {
					if (method->access_flags & FY_ACC_STATIC) {
						i2 = fy_vmGetClassObjHandle(context, method->owner,
								exception);
						FYEH();
						fy_threadMonitorExit(context, thread, i2, exception);
						FYEH();
					} else {
						fy_threadMonitorExit(context, thread,
								frame->baseSpp->uvalue,
								exception);
						FYEH();
					}
				}
				fy_threadPopFrame(context, thread);
				continue;
			} else {
				/*Found*/
				frame->lpc = i1;
				frame->pcofs = 0;
				(frame->baseSpp+method->max_locals)->uvalue = thread->currentThrowable;
				thread->currentThrowable = 0;
			}
		}

		ops = (*(method->engine->runner))(context, thread, frame, ops,
				intrenalException, NULL);

		if (intrenalException->exceptionType != exception_none) {
			if (intrenalException->exceptionName[0] == 0) {
				*exception = *intrenalException;
				FYEH();
			} else {
#ifdef FY_LATE_DECLARATION
				fy_exception exceptionToPrepare;
#endif
				exceptionToPrepare = *intrenalException;
				intrenalException->exceptionType = exception_none;
				intrenalException->exceptionName[0] = 0;
				intrenalException->exceptionDesc[0] = 0;
				fy_heapBeginProtect(context);
				thread->currentThrowable = fy_threadPrepareThrowable(context,
						thread, &exceptionToPrepare, intrenalException);
				FYEH();
			}
		}
		if (thread->pendingNative.methodName) {
			message->messageType = message_invoke_native;
			message->threadId = thread->threadId;
			message->body.call = thread->pendingNative;
			thread->pendingNative.methodName = NULL;
		}
	}
}

fy_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
		fy_exception *toPrepare, fy_exception *exception) {
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

void fy_threadReturnInt(fy_stack_item *spp, fy_int value) {
	spp->ivalue = value;
}

void fy_threadReturnLong(fy_stack_item *spp, fy_long value) {
	spp->uvalue = fy_HOFL(value);
	(spp + 1)->uvalue = fy_LOFL(value);
}

void fy_threadScanRef(fy_context *context, fy_thread *thread,
		fy_arrayList *from, fy_exception *exception) {
	fy_uint handle;
	fy_stack_item *sbase;
	fy_frame *frame;
	fy_method *method;
	fy_object *object;
	fy_int i, maxSp;
	fy_int frameId, frameIdMax;
	fy_instruction_extra *ipp, *nipp;

	frameIdMax = thread->frameCount - 1;
	if (frameIdMax == 0) {
		/*Thread already dead but not removed*/
		return;
	}
	for (frameId = 0; frameId < frameIdMax; frameId++) {
		/*not last frame, must be an a invoke, or clinit by invoke, new, get/put static op, or lpc=0, pcofs=1, or thread is holding. It's ok to use nipp for all situation */
		frame = FY_GET_FRAME(thread, frameId);
		method = frame->method;
		ipp = method->c.i.instruction_extras + frame->lpc + frame->pcofs;
		maxSp = ipp->sp;
		sbase = frame->baseSpp;
		for (i = 0; i < maxSp; i++) {
			if (fy_instGetStackItem(ipp, i)) {
				handle = sbase[i].uvalue;
				if (handle
						== 0|| handle >= MAX_OBJECTS || fy_heapGetObject(context,handle)->object_data == NULL) {
					continue;
				}
				/**/
				object = fy_heapGetObject(context, handle);
				if (object->object_data->data != NULL) {
					/*Valid handle*/
					fy_arrayListAdd(context->memblocks, from, &handle,
							exception);
					FYEH();
				}
			}
		}
	}
	frame = FY_GET_FRAME(thread, frameId);
	method = frame->method;
	ipp = method->c.i.instruction_extras + frame->lpc;
	nipp = ipp + frame->pcofs
			- ((frame->lpc + frame->pcofs >= method->codeLength) & 1);
	maxSp = fy_maxi(ipp->sp, nipp->sp);
	sbase = frame->baseSpp;
	for (i = 0; i < maxSp; i++) {
		if (fy_instGetStackItem(ipp, i) || fy_instGetStackItem(nipp, i)) {
			handle = sbase[i].uvalue;
			if (handle
					== 0|| handle >= MAX_OBJECTS || fy_heapGetObject(context,handle)->object_data == NULL) {
				continue;
			}
			/**/
			object = fy_heapGetObject(context, handle);
			if (object->object_data->data != NULL) {
				/*Valid handle*/
				fy_arrayListAdd(context->memblocks, from, &handle, exception);
				FYEH();
			}
		}
	}
}

