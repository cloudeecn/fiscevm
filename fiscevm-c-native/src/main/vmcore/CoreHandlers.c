/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 * fiscevm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * fiscevm is distributed in the hope that it will be useful,
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

#include "fyc/CoreHandlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "fiscedev.h"
#include "fy_util/Portable.h"
#include "fy_util/ArrList.h"
#include "fy_util/HashMap.h"
#include "fy_util/HashMapI.h"
#include "fy_util/MemMan.h"
#include "fy_util/String.h"
#include "fy_util/Utf8.h"
#include "fyc/Config.h"
#include "fyc/Constants.h"
#include "fyc/ClassStruct.h"
#include "fyc/Class.h"
#include "fyc/ClassLoader.h"
#include "fyc/Heap.h"
#include "fyc/Thread.h"
#include "fyc/ThreadManager.h"
#include "fyc/VMContext.h"

static fy_int FiScEVMSave(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	context->saveloadParam = "save.dat";
	fy_vmSave(context, exception);
	return ops;
}

static fy_int FiScEVMStoreParams(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
/*We don't have a frame for native function currently, so the current frame is the caller frame.*/
	fy_frame *frame = FY_GET_FRAME(thread, thread->frameCount - 1);
	fy_int begin = args[0].ivalue;
	fy_int targetHandle = args[1].ivalue;
	fy_int targetPos = args[2].ivalue;
	fy_int count = args[3].ivalue;
	fy_int *targetData = (fy_int*) fy_heapGetArrayBytes(context, targetHandle,
			exception);
	if (fy_heapArrayLength(context, targetHandle, exception)
			< count + targetPos) {
		fy_fault(exception, FY_EXCEPTION_IOOB,
				"Target array is too small to fit param size");
		FYEH()0;
	}
	if (frame->method->max_locals < count + begin) {
		fy_fault(exception, FY_EXCEPTION_IOOB,
				"Current method doesn't have %d params.", count);
		FYEH()0;
	}
	memcpy(targetData + targetPos, frame->baseSpp + begin,
			count * sizeof(fy_int));
	return ops;
}

static fy_int SISRead0(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {/*TODO stub*/
	fy_threadReturnInt(args, 0);
	return ops;
}

static fy_int SystemSetIn(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {/*TODO stub*/
	return ops;
}

static fy_int SystemSetOut(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {/*TODO stub*/
	return ops;
}

static fy_int SystemSetErr(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {/*TODO stub*/
	return ops;
}

static fy_int SystemGetProperty(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {/*TODO stub*/
	fy_threadReturnInt(args, 0);
	return ops;
}

static fy_int SystemSetProperty(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {/*TODO stub*/
	fy_threadReturnInt(args, 0);
	return ops;
}

static fy_int SystemGC(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_heapGC(context, FALSE, exception);
	return ops;
}

static fy_int SystemExit(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	context->exitCode = args[0].ivalue;
	return 0;
}

static fy_int RuntimeFreeMemory(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnLong(args,
			(context->memblocks->oldTop - context->memblocks->posInOld)
					* sizeof(fy_int));
	return ops;
}

static fy_int RuntimeTotalMemory(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnLong(args, sizeof(fy_memblock));
	return ops;
}

static fy_int RuntimeMaxMemory(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnLong(args, sizeof(fy_context));
	return ops;
}

static fy_int StringIntern(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_str str;
	fy_uint ret;

	str.content = NULL;
	fy_strInit(context->memblocks, &str, 512, exception);
	FYEH()0;
	fy_heapGetString(context, args[0].ivalue, &str, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return 0;
	}
	ret = fy_heapLiteral(context, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	fy_threadReturnInt(args, ret);
	return ops;
}
static fy_int doubleLongBitsToDouble(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnLong(args, fy_I2TOL(args[0].ivalue, args[1].ivalue));
	return ops;
}
static fy_int floatIntBitsToFloat(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnInt(args, args[0].ivalue);
	return ops;
}

static fy_int SystemArrayCopy(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_int srcHandle = args[0].ivalue;
	fy_int srcPos = args[1].ivalue;
	fy_int dstHandle = args[2].ivalue;
	fy_int dstPos = args[3].ivalue;
	fy_int len = args[4].ivalue;
	fy_heapArrayCopy(context, srcHandle, srcPos, dstHandle, dstPos, len,
			exception);
	return ops;
}

static fy_int SystemTimeMS(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_threadReturnLong(args, fy_portTimeMillSec(context->port));
	return ops;
}

static fy_int SystemTimeNS(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_threadReturnLong(args, fy_portTimeNanoSec(context->port));
	return ops;
}

static fy_int SystemIdentityHashCode(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnInt(args, args[0].ivalue);
	return ops;
}

static fy_int ObjectGetClass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_heapGetClassOfObject(context, args[0].ivalue,
			exception);
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, clazz, exception));
	return ops;
}
static fy_int ObjectClone(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_int ret = fy_heapClone(context, args[0].ivalue, exception);
	FYEH()0;
	fy_threadReturnInt(args, ret);
	return ops;
}
static fy_int ObjectWait(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_uint monitorId = args[0].uvalue;
	fy_uint high = args[1].uvalue;
	fy_uint low = args[2].uvalue;
	fy_long time = fy_I2TOL(high, low);
	fy_tmWait(context, thread, monitorId, time, exception);
	return 0;
}

static fy_int ObjectNotify(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_uint monitorId = args[0].uvalue;
	fy_tmNotify(context, thread, monitorId, FALSE, exception);
	return ops;
}

static fy_int ObjectNotifyAll(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_uint monitorId = args[0].uvalue;
	fy_tmNotify(context, thread, monitorId, TRUE, exception);
	return ops;
}

static fy_int ThreadCurrentThread(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnInt(args, thread->handle);
	return ops;
}

static fy_int ThreadSetPriority(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_thread *target;
	fy_object *obj = context->objects + args[0].uvalue;
	target = context->threads[obj->object_data->m.threadId];
	if (target != NULL) {
		target->priority = args[1].uvalue;
	}
	return ops;
}

static fy_int ThreadIsAlive(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnInt(args,
			fy_tmIsAlive(context, args[0].uvalue, exception) ? 1 : 0);
	return ops;
}

static fy_int ThreadInterrupt(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_tmInterrupt(context, args[0].uvalue, exception);
	return ops;
}

static fy_int ThreadInterrupted(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_boolean ret;
	ret = fy_tmIsInterrupted(context, args[0].uvalue, args[1].uvalue,
			exception);
	fy_threadReturnInt(args, ret ? 1 : 0);
	return ops;
}

static fy_int ThreadStart(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_tmPushThread(context, args[0].uvalue, exception);
	return ops;
}

static fy_int ThreadSleep(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_tmSleep(context, thread, fy_I2TOL(args[0].uvalue, args[1].uvalue));
	return 0;
}

static fy_int ThreadYield(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	return 0;
}

static fy_int VMLogOut(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_str str[1];
	fy_memblock *block = context->memblocks;
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	str->content = NULL;
	fy_strInit(block, str, 128, exception);
	FYEH()0;
	fy_heapGetString(context, args[1].uvalue, str, exception);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	switch (args[0].uvalue) {
	case 0:
		context->logDStr(context, str);
		context->logDVar(context, "\n");
		break;
	case 1:
		context->logIStr(context, str);
		context->logIVar(context, "\n");
		break;
	case 2:
		context->logWStr(context, str);
		context->logWVar(context, "\n");
		break;
	case 3:
		context->logEStr(context, str);
		context->logEVar(context, "\n");
		break;
	}
	fy_strDestroy(block, str);
	return ops;
}

static fy_int VMThrowOut(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	/*TODO */
	thread->currentThrowable = args[0].uvalue;
	return ops;
}

static fy_int VMExit(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	/*TODO */
	return ops;
}

static fy_int SOSWrite(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	putchar(args[1].ivalue);
	return ops;
}

static fy_int VMDecode(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_int handleSrc = args[1].ivalue;
	fy_int ofs = args[2].ivalue;
	fy_int len = args[3].ivalue;
	int i, maxi;
	fy_byte *array;
	fy_byte *newArray;
	fy_str str[1];
	fy_int handleRet;
	fy_class *charArray;
	fy_memblock *block = context->memblocks;
	charArray = fy_vmLookupClass(context, context->sArrayChar, exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		return 0;
	}
	array = fy_heapGetArrayBytes(context, handleSrc, exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		return 0;
	}

	newArray = /*TEMP*/fy_allocate(len + 1, exception);
	FYEH()0;
	memcpy(newArray, array + ofs, len);
	newArray[len] = 0;
	str->content = NULL;
	fy_strInitWithUTF8(block, str, (char*) newArray, exception);
	fy_free(newArray);
	FYEH()0;

	handleRet = fy_heapAllocateArray(context, charArray, maxi = str->length,
			exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		fy_strDestroy(block, str);
		return 0;
	}
	for (i = 0; i < maxi; i++) {
		fy_heapPutArrayChar(context, handleRet, i, fy_strGet(str, i),
				exception);
		if (exception->exceptionType != exception_none) {
			fy_threadReturnInt(args, 0);
			fy_strDestroy(block, str);
			return 0;
		}
	}
	fy_strDestroy(block, str);
	fy_threadReturnInt(args, handleRet);
	return ops;
}

static fy_int VMEncode(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_int handleSrc = args[1].ivalue;
	fy_int ofs = args[2].ivalue;
	fy_int len = args[3].ivalue;
	fy_class *byteClass;
	fy_int handleRet;
	fy_byte *out;
	fy_byte *outTmp;
	fy_int olen;
	fy_int left;
	fy_int read;
	fy_int i;
	fy_char ch;
	byteClass = fy_vmLookupClass(context, context->sArrayByte, exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		return 0;
	}
	left = len * 3 + 1;
	olen = 0;
	out = outTmp = /*TEMP*/fy_allocate(left, exception);
	FYEH()0;
	for (i = 0; i < len; i++) {
		ch = fy_heapGetArrayChar(context, handleSrc, i + ofs, exception);
		if (exception->exceptionType != exception_none) {
			fy_threadReturnInt(args, 0);
			fy_free(out);
			return 0;
		}
		read = fy_utf8Write(ch, (char**) &outTmp, &left);
		if (read <= 0) {
			break;
		}
		olen += read;
	}
	handleRet = fy_heapAllocateArray(context, byteClass, olen, exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		fy_free(out);
		return 0;
	}

	outTmp = fy_heapGetArrayBytes(context, handleRet, exception);
	if (exception->exceptionType != exception_none) {
		fy_threadReturnInt(args, 0);
		fy_free(out);
		return 0;
	}

	memcpy(outTmp, out, olen);
	fy_free(out);
	fy_threadReturnInt(args, handleRet);
	return ops;
}

static fy_int VMGetDoubleRaw(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnLong(args,
			((fy_ulong) args[0].uvalue << 32) | ((fy_uint) args[1].uvalue));
	return ops;
}

static fy_int VMGetFloatRaw(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_threadReturnInt(args, args[0].ivalue);
	return ops;
}

static fy_int VMStringToDouble(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	char ch[64];
	fy_str str[1];
	fy_double value;
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH()0;
	fy_heapGetString(context, args[0].uvalue, str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	fy_strDestroy(context->memblocks, str);
	FYEH()0;
	value = atof(ch);
	fy_threadReturnLong(args, fy_doubleToLong(value));
	return ops;
}

static fy_int VMDoubleToString(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_long lvalue = ((fy_ulong) args[0].uvalue << 32)
			| ((fy_uint) args[1].uvalue);
	fy_double dvalue = fy_longToDouble(lvalue);
	fy_int handleRet;
	char ch[64];
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH()0;
	sprintf_s(ch, sizeof(ch), "%f", dvalue);
	fy_strAppendUTF8(context->memblocks, str, ch, 64, exception);
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strDestroy(context->memblocks, str);
	FYEH()0;
	fy_threadReturnInt(args, handleRet);
	return ops;
}

static fy_int VMStringToFloat(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	char ch[64];
	fy_double value;
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH()0;
	fy_heapGetString(context, args[0].uvalue, str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	fy_strDestroy(context->memblocks, str);
	FYEH()0;
	value = atof(ch);
	fy_threadReturnInt(args, fy_floatToInt((fy_float) value));
	return ops;
}

static fy_int VMFloatToString(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_int value = args[0].ivalue;
	fy_float fvalue = fy_intToFloat(value);
	fy_int handleRet;
	char ch[64];
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH()0;
	sprintf_s(ch, sizeof(ch), "%f", fvalue);
	fy_strAppendUTF8(block, str, ch, 64, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, str);
		return 0;
	}
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strDestroy(block, str);
	if (exception->exceptionType != exception_none) {
		return 0;
	}
	fy_threadReturnInt(args, handleRet);
	return ops;
}

static fy_int VMBreakpoint(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_breakpoint();
	return ops;
}

static fy_int throwableFillInStackTrace(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_int value = args[0].ivalue;
	fy_threadFillException(context, thread, value, exception);
	return ops;
}

static fy_int finalizerGetFinalizee(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_uint ret;
	fy_int storage, i, len = context->toFinalize->length;
	fy_object *object;

	fy_class *clazz = fy_vmLookupClass(context, context->sArrayObject,
			exception);
	FYEH()0;

#if 0
	if (len > 0) {
		context->logDVar(context, "#Finalizer %d objects need finalize\n", len);
	}
#endif
	ret = fy_heapAllocateArray(context, clazz, len, exception);
	FYEH()0;

	for (i = 0; i < len; i++) {
		fy_arrayListGet(context->memblocks, context->toFinalize, i, &storage);
#if 0
		context->logDVar(context, "GET %d need finalize\n", storage);
#endif
		fy_heapPutArrayHandle(context, ret, i, storage, exception);
		FYEH()0;
		object = fy_heapGetObject(context, storage);
		object->object_data->finalizeStatus = finalized;
	}
	fy_arrayListClear(context->memblocks, context->toFinalize);
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int finalizerGetReferencesToEnqueue(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_uint ret;
	fy_int storage, i, len = context->toEnqueue->length;

	fy_class *clazz = fy_vmLookupClass(context, context->sArrayObject,
			exception);
	FYEH()0;

#if 0
	if (len > 0) {
		context->logDVar(context, "#Finalizer %d references need enqueue\n",
				len);
	}
#endif
	ret = fy_heapAllocateArray(context, clazz, len, exception);
	FYEH()0;

	for (i = 0; i < len; i++) {
		fy_arrayListGet(context->memblocks, context->toEnqueue, i, &storage);
		fy_heapPutArrayHandle(context, ret, i, storage, exception);
		FYEH()0;
	}
	fy_arrayListClear(context->memblocks, context->toEnqueue);
	fy_threadReturnInt(args, ret);
	return ops;
}

/*
 #     #  #######  #######  #     #    ###    #####
 ##   ##  #           #     #     #   #   #   #    #
 # # # #  #           #     #     #  #     #  #     #
 #  #  #  #####       #     #######  #     #  #     #
 #     #  #           #     #     #  #     #  #     #
 #     #  #           #     #     #   #   #   #    #
 #     #  #######     #     #     #    ###    #####
 */
static fy_int methodIsBridge(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			(FY_ACC_BRIDGE & method->access_flags) != 0 ? 1 : 0);
	return ops;
}

static fy_int methodIsVarArgs(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			(FY_ACC_VARARGS & method->access_flags) != 0 ? 1 : 0);
	return ops;
}

static fy_int methodIsSynthetic(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			(method->access_flags & FY_ACC_SYNTHETIC) != 0 ? 1 : 0);
	return ops;
}

static fy_int methodGetDeclaringClass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, method->owner, exception));
	return ops;
}

static fy_int methodExceptionTypes(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_int i, max, ret, handler;
	fy_exceptionHandler *eh;
	fy_class *handlerClass, *classClass;
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	classClass = fy_vmLookupClass(context, context->sArrayClass, exception);
	FYEH()0;

	max = method->exception_table_length;
	ret = fy_heapAllocateArray(context, classClass, max, exception);
	FYEH()0;
	for (i = 0; i < max; i++) {
		eh = method->exception_table + i;
		handlerClass = fy_vmLookupClassFromExceptionHandler(context, eh,
				exception);
		FYEH()0;
		handler = fy_vmGetClassObjHandle(context, handlerClass, exception);
		FYEH()0;
		fy_heapPutArrayHandle(context, ret, i, handler, exception);
		FYEH()0;
	}
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int methodGetModifiers(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args, method->access_flags);
	return ops;
}

static fy_int methodGetName(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	fy_int ret;
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	ret = fy_heapLiteral(context, method->name, exception);
	FYEH()0;
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int methodGetParameterTypes(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	fy_class *clazz;
	fy_int ret, i, max, handle;
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	clazz = fy_vmLookupClass(context, context->sArrayClass, exception);
	FYEH()0;
	max = method->parameterTypes->length;
	ret = fy_heapAllocateArray(context, clazz, max, exception);
	for (i = 0; i < max; i++) {
		fy_arrayListGet(context->memblocks, method->parameterTypes, i, &clazz);
		handle = fy_vmGetClassObjHandle(context, clazz, exception);
		FYEH()0;
		fy_heapPutArrayHandle(context, ret, i, handle, exception);
		FYEH()0;
	}
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int methodGetReturnType(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, method->returnTypeClass,
					exception));
	return ops;
}

static fy_int methodInvoke(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	fy_uint count =
			args[2].uvalue ?
					fy_heapArrayLength(context, args[2].uvalue, exception) : 0;
	fy_class *paramType;
	/*fy_int clinitStatus;*/
	fy_uint i;
	fy_uint paramsHandle = args[2].uvalue;
	fy_uint paramHandle;
	FYEH()0;
#ifdef FY_VERBOSE
	context->logDVarLn(context,
			"#REFINVOKE# Prepare to invoke method #%"FY_PRINT32"d(@%"FY_PRINT32"d).",
			methodObj->object_data->m.methodId, args[0]);
#endif
	if (method == NULL) {
		context->logEVarLn(context,
				"Can't find method #%"FY_PRINT32"d for handle #%"FY_PRINT32"d",
				methodObj->object_data->m.methodId, args[0]);
		fy_fault(exception, NULL, "Method not found!");
		FYEH()0;
	}
#ifdef FY_VERBOSE
	context->logDVar(context, "#REFINVOKE# %s",
			(method->access_flags & FY_ACC_STATIC) ? "static" : "");
	context->logDStr(context, method->uniqueName);
	context->logDVarLn(context, "");
#endif
	if (count != method->parameterCount) {
		context->logEVar(context, "Error occored in invoking ");
		context->logEStr(context, method->uniqueName);
		context->logEVarLn(context, "");
		context->logEVarLn(context,
				"Parameter count mismatch method:%"FY_PRINT32"d params:%"FY_PRINT32"d",
				method->parameterCount, count);
		fy_fault(exception, NULL, "parameter count mismatch");
	}
	if (!(method->access_flags & FY_ACC_STATIC)) {
		fy_threadReturnInt(args, args[1].uvalue);
		args++;
	} else {
		ops = fy_threadClinit(context, thread, method->owner, args + argsCount,
				ops, exception);
		FYEH()0;
		if (ops <= 0) {
			return 0;
		}
	}
	for (i = 0; i < count; i++) {
		fy_arrayListGet(context->memblocks, method->parameterTypes, i,
				&paramType);
		paramHandle = fy_heapGetArrayHandle(context, paramsHandle, i,
				exception);
		FYEH()0;
		if (paramType->type == primitive_class) {
			/*Unwrap*/
			switch (paramType->ci.prm.pType) {
			case FY_TYPE_BOOLEAN: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapBoolean(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_BYTE: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapByte(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_SHORT: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapShort(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_CHAR: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapChar(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_INT: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapInt(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_FLOAT: {
				fy_threadReturnFloat(args++,
						fy_heapUnwrapFloat(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_LONG: {
				fy_threadReturnLong(args++,
						fy_heapUnwrapLong(context, paramHandle, exception));
				FYEH()0;
				args++;
				break;
			}
			case FY_TYPE_DOUBLE: {
				fy_threadReturnDouble(args++,
						fy_heapUnwrapDouble(context, paramHandle, exception));
				FYEH()0;
				args++;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_ARGU,
						"Illegal parameter type %c",
						(char) paramType->ci.prm.pType);
				break;
			}
			}
		} else {
			fy_threadReturnInt(args++, paramHandle);
		}
	}
	context->logDVar(context, "Invoking: ");
	context->logDStr(context, method->uniqueName);
	context->logDVarLn(context, "");
	return fy_threadInvoke(context, thread, method, args, ops, exception);
}

static fy_int methodGetUniqueName(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			fy_heapLiteral(context, method->uniqueName, exception));
	return ops;
}

static void registerMethod(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context, FY_REFLECT_METHOD".isBridge.()Z", NULL,
			methodIsBridge, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_REFLECT_METHOD".isVarArgs.()Z", NULL,
			methodIsVarArgs, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REFLECT_METHOD".isSynthetic.()Z",
	NULL, methodIsSynthetic, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getDeclaringClass.()L"FY_BASE_CLASS";", NULL,
			methodGetDeclaringClass, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getExceptionTypes.()[L"FY_BASE_CLASS";", NULL,
			methodExceptionTypes, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REFLECT_METHOD".getModifiers.()I",
	NULL, methodGetModifiers, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getName.()L"FY_BASE_STRING";", NULL, methodGetName,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getParameterTypes.()[L"FY_BASE_CLASS";", NULL,
			methodGetParameterTypes, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getReturnType.()L"FY_BASE_CLASS";", NULL,
			methodGetReturnType, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeZ.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)Z",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeB.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)B",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeS.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)S",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeC.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)C",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeI.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)I",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeF.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)F",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeD.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)D",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeJ.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)J",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_REFLECT_METHOD".invokeL.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
			NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".invokeV.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)V",
	NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_METHOD".getUniqueName.()L"FY_BASE_STRING";", NULL,
			methodGetUniqueName, exception);
	FYEH();
}

static fy_int constructorNewInstance(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context, args[0].uvalue);
	fy_method *method = context->methods[methodObj->object_data->m.methodId];
	fy_uint count =
			args[1].uvalue ?
					fy_heapArrayLength(context, args[1].uvalue, exception) : 0;
	fy_class *paramType;
	fy_uint i;
	fy_uint paramsHandle = args[1].uvalue;
	fy_uint paramHandle;
	FYEH()0;

	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	if (count != method->parameterCount) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
	}
	if (method->access_flags & FY_ACC_STATIC) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"This is not a constructor!!");
	}
	paramHandle = fy_heapAllocate(context, method->owner, exception);
	FYEH()0;
	fy_threadReturnInt(args++, paramHandle);
	fy_threadReturnInt(args++, paramHandle);
	for (i = 0; i < count; i++) {
		fy_arrayListGet(context->memblocks, method->parameterTypes, i,
				&paramType);
		paramHandle = fy_heapGetArrayHandle(context, paramsHandle, i,
				exception);
		FYEH()0;
		if (paramType->type == primitive_class) {
			/*Unwrap*/
			switch (paramType->ci.prm.pType) {
			case FY_TYPE_BOOLEAN: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapBoolean(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_BYTE: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapByte(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_SHORT: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapShort(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_CHAR: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapChar(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_INT: {
				fy_threadReturnInt(args++,
						fy_heapUnwrapInt(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_FLOAT: {
				fy_threadReturnFloat(args++,
						fy_heapUnwrapFloat(context, paramHandle, exception));
				FYEH()0;
				break;
			}
			case FY_TYPE_LONG: {
				fy_threadReturnLong(args++,
						fy_heapUnwrapLong(context, paramHandle, exception));
				FYEH()0;
				args++;
				break;
			}
			case FY_TYPE_DOUBLE: {
				fy_threadReturnDouble(args++,
						fy_heapUnwrapDouble(context, paramHandle, exception));
				FYEH()0;
				args++;
				break;
			}
			default: {
				fy_fault(exception, FY_EXCEPTION_ARGU,
						"Illegal parameter type %c",
						(char) paramType->ci.prm.pType);
				break;
			}
			}
		} else {
			fy_threadReturnInt(args++, paramHandle);
		}
	}
	return fy_threadInvokeSpecial(context, thread,
			fy_threadCurrentFrame(context, thread), method, args, ops,
			exception);
}

static void registerConstructor(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context, FY_REFLECT_CONSTRUCTOR".isBridge.()Z",
	NULL, methodIsBridge, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_REFLECT_CONSTRUCTOR".isVarArgs.()Z",
	NULL, methodIsVarArgs, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".isSynthetic.()Z", NULL, methodIsSynthetic,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getDeclaringClass.()L"FY_BASE_CLASS";",
	NULL, methodGetDeclaringClass, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getExceptionTypes.()[L"FY_BASE_CLASS";",
	NULL, methodExceptionTypes, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getModifiers.()I", NULL, methodGetModifiers,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getName.()L"FY_BASE_STRING";", NULL, methodGetName,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getParameterTypes.()[L"FY_BASE_CLASS";",
	NULL, methodGetParameterTypes, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_CONSTRUCTOR".getUniqueName.()L"FY_BASE_STRING";", NULL,
			methodGetUniqueName, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
			FY_REFLECT_CONSTRUCTOR".newInstance0.([L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
			NULL, constructorNewInstance, exception);
	FYEH();
}

/*
 #######   #####   #######  #        #####
 #           #     #        #        #    #
 #           #     #        #        #     #
 #####       #     #####    #        #     #
 #           #     #        #        #     #
 #           #     #        #        #    #
 #         #####   #######  ######   #####
 */

static fy_int fieldIsSynthetic(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args, field->access_flags & FY_ACC_SYNTHETIC);
	return ops;
}

static fy_int fieldIsEnumconstant(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args, (field->access_flags & FY_ACC_ENUM) ? 1 : 0);
	return ops;
}

static fy_int fieldGetObject(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type == primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU,
				"This field is not object or array");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		if (field->access_flags & FY_ACC_FINAL) {
			if (field->constant_value_index > 0) {
				/*This must be string*/
				fy_threadReturnInt(args,
						fy_heapLookupStringFromConstant(context,
								field->owner->constantPools[field->constant_value_index],
								exception));
			} else {
				fy_threadReturnInt(args,
						fy_heapGetStaticHandle(context, field, exception));
			}
		} else {
			fy_threadReturnInt(args,
					fy_heapGetStaticHandle(context, field, exception));
		}
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldHandle(context, args[1].uvalue, field,
						exception));
	}
	return ops;
}

static fy_int fieldGetBoolean(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not boolean");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_BOOLEAN) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not boolean");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticBoolean(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldBoolean(context, args[1].uvalue, field,
						exception));
	}
	return ops;
}

static fy_int fieldGetByte(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not byte");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_BYTE) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not byte");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticByte(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldByte(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetShort(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not short");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_SHORT) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not short");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticShort(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldShort(context, args[1].uvalue, field,
						exception));
	}
	return ops;
}

static fy_int fieldGetChar(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not char");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_CHAR) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not char");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticChar(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldChar(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetInt(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not int");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_INT) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not int");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticInt(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldInt(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetFloat(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not float");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_FLOAT) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not float");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnInt(args,
				fy_heapGetStaticInt(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnInt(args,
				fy_heapGetFieldInt(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetLong(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not long");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_LONG) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not long");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnLong(args,
				fy_heapGetStaticLong(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnLong(args,
				fy_heapGetFieldLong(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetDouble(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_class *type;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not double");
		FYEH()0;
	}

	if (field->type->ci.prm.pType != FY_TYPE_DOUBLE) {
		fy_fault(exception, FY_EXCEPTION_ARGU, "This field is not double");
		FYEH()0;
	}

	if (field->access_flags & FY_ACC_STATIC) {
		fy_threadReturnLong(args,
				fy_heapGetStaticLong(context, field, exception));
	} else {
		type = fy_heapGetClassOfObject(context, args[1].uvalue, exception);
		FYEH()0;
		if (!fy_classCanCastTo(context, type, field->owner, TRUE)) {
			fy_fault(exception, FY_EXCEPTION_ARGU, "Class cast exception");
			FYEH()0;
		}
		fy_threadReturnLong(args,
				fy_heapGetFieldLong(context, args[1].uvalue, field, exception));
	}
	return ops;
}

static fy_int fieldGetModifiers(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args, field->access_flags);
	return ops;
}

static fy_int fieldGetName(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_uint ret;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	ret = fy_heapLiteral(context, field->name, exception);
	FYEH()0;
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int fieldGetType(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, field->type, exception));
	return ops;
}

static fy_int fieldGetDeclaringClass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, field->owner, exception));
	return ops;
}

static fy_int fieldSetObject(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type == primitive_class) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field is primitive");
		FYEH()0;
	}
	if (field->access_flags & FY_ACC_STATIC) {
		fy_heapPutStaticHandle(context, field, args[2].uvalue, exception);
	} else {
		fy_heapPutFieldHandle(context, args[1].uvalue, field, args[2].uvalue,
				exception);
	}
	return ops;
}

static fy_int fieldSetPrim(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Field is not primitive");
		FYEH()0;
	}
	if (field->access_flags & FY_ACC_STATIC) {
		fy_heapPutStaticInt(context, field, args[2].uvalue, exception);
	} else {
		fy_heapPutFieldInt(context, args[1].uvalue, field, args[2].ivalue,
				exception);
	}
	return ops;
}

static fy_int fieldSetWidePrim(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Field not found!");
		FYEH()0;
	}
	if (field->type->type != primitive_class) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Field is not primitive");
		FYEH()0;
	}
	if (field->access_flags & FY_ACC_STATIC) {
		fy_heapPutStaticLong(context, field,
				fy_I2TOL(args[2].uvalue, args[3].uvalue), exception);
	} else {
		fy_heapPutFieldLong(context, args[1].uvalue, field,
				fy_I2TOL(args[2].uvalue, args[3].uvalue), exception);
	}
	return ops;
}

static fy_int fieldGetUniqueName(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context, args[0].uvalue);
	fy_field *field = context->fields[fieldObj->object_data->m.fieldId];
	fy_uint ret;
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH()0;
	}
	ret = fy_heapLiteral(context, field->uniqueName, exception);
	FYEH()0;
	fy_threadReturnInt(args, ret);
	return ops;
}

static void registerFields(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getName.()L"FY_BASE_STRING";", NULL, fieldGetName,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getUniqueName.()L"FY_BASE_STRING";", NULL,
			fieldGetUniqueName, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REFLECT_FIELD".isSynthetic.()Z",
	NULL, fieldIsSynthetic, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REFLECT_FIELD".isEnumConstant.()Z",
	NULL, fieldIsEnumconstant, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REFLECT_FIELD".getModifiers.()I",
	NULL, fieldGetModifiers, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getDeclaringClass.()L"FY_BASE_CLASS";", NULL,
			fieldGetDeclaringClass, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getType.()L"FY_BASE_CLASS";", NULL, fieldGetType,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getObject0.(L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
	NULL, fieldGetObject, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getBoolean0.(L"FY_BASE_OBJECT";)Z", NULL, fieldGetBoolean,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getByte0.(L"FY_BASE_OBJECT";)B", NULL, fieldGetByte,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getShort0.(L"FY_BASE_OBJECT";)S", NULL, fieldGetShort,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getChar0.(L"FY_BASE_OBJECT";)C", NULL, fieldGetChar,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getInt0.(L"FY_BASE_OBJECT";)I", NULL, fieldGetInt,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getFloat0.(L"FY_BASE_OBJECT";)F", NULL, fieldGetFloat,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getLong0.(L"FY_BASE_OBJECT";)J", NULL, fieldGetLong,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".getDouble0.(L"FY_BASE_OBJECT";)D", NULL, fieldGetDouble,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setObject.(L"FY_BASE_OBJECT";L"FY_BASE_OBJECT";)V",
	NULL, fieldSetObject, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setBoolean.(L"FY_BASE_OBJECT";Z)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setByte.(L"FY_BASE_OBJECT";B)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setShort.(L"FY_BASE_OBJECT";S)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setChar.(L"FY_BASE_OBJECT";C)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setInt.(L"FY_BASE_OBJECT";I)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setFloat.(L"FY_BASE_OBJECT";F)V", NULL, fieldSetPrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setLong.(L"FY_BASE_OBJECT";J)V", NULL, fieldSetWidePrim,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_FIELD".setDouble.(L"FY_BASE_OBJECT";D)V", NULL, fieldSetWidePrim,
			exception);
	FYEH();
}
static fy_int classPrivateGetDeclaredMethods(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz, *clazzArrayObject;
	fy_uint methodCount;
	fy_uint ret;
	fy_int i, j;

	clazzArrayObject = fy_vmLookupClass(context, context->sArrayObject,
			exception);
	FYEH()0;
	clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue, exception);
	FYEH()0;
	methodCount = clazz->methodCount;
	ret = fy_heapAllocateArray(context, clazzArrayObject, methodCount,
			exception);
	FYEH()0;
	i = 0;
	for (j = 0; j < methodCount; j++) {
		fy_heapPutArrayHandle(context, ret, i++,
				fy_vmGetMethodObjHandle(context, clazz->methods[j], exception),
				exception);
		FYEH()0;
	}
	fy_threadReturnInt(args, ret);
	return ops;
}
static fy_int classPrivateGetDeclaredFields(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz, *clazzArrayObject;
	fy_uint fieldCount;
	fy_uint ret;
	fy_int i, j;

	clazzArrayObject = fy_vmLookupClass(context, context->sArrayObject,
			exception);
	FYEH()0;
	clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue, exception);
	FYEH()0;
	fieldCount = clazz->fieldCount;
	ret = fy_heapAllocateArray(context, clazzArrayObject, fieldCount,
			exception);
	FYEH()0;
	i = 0;
	for (j = 0; j < fieldCount; j++) {
		fy_heapPutArrayHandle(context, ret, i++,
				fy_vmGetFieldObjHandle(context, clazz->fields[j], exception),
				exception);
		FYEH()0;
	}
	fy_threadReturnInt(args, ret);
	return ops;
}
static fy_int classGetComponentType(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	if (clazz->type == array_class) {
		fy_threadReturnInt(args,
				fy_vmGetClassObjHandle(context, clazz->ci.arr.contentClass,
						exception));
	} else {
		fy_threadReturnInt(args, 0);
	}
	return ops;
}

static fy_int classForName(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_uint nameHandle = args[0].uvalue;
	/*
	 fy_boolean initialize = args[1];
	 */
	fy_str str;
	fy_class *clazz;
	str.content = NULL;
	fy_strInit(context->memblocks, &str, 64, exception);
	FYEH()0;

	fy_heapGetString(context, nameHandle, &str, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return 0;
	}
	fy_strReplaceOne(&str, '.', '/');
	clazz = fy_vmLookupClass(context, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	ops = fy_threadClinit(context, thread, clazz, args + argsCount, ops,
			exception);
	if (ops < 0) {
		return 0;
	}
	FYEH()0;
	fy_threadReturnInt(args,
			fy_vmGetClassObjHandle(context, clazz, exception));
	return ops;
}

static fy_int classNewInstanceO(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz;
	fy_method *invoke;
	fy_str str;
	fy_uint handle;
	clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue, exception);
	FYEH()0;
	if (clazz->type != object_class) {
		fy_fault(exception, FY_EXCEPTION_RT, "Class is not an object class!");
		return 0;
	}

	handle = fy_heapAllocate(context, clazz, exception);
	FYEH()0;

	fy_threadReturnInt(args++, handle);
	fy_threadReturnInt(args++, handle);

	str.content = NULL;
	fy_strInitWithUTF8(context->memblocks, &str, "."FY_METHOD_INIT".()V",
			exception);
	FYEH()0;
	invoke = fy_vmLookupMethodVirtual(context, clazz, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	FYEH()0;
	return fy_threadInvokeSpecial(context, thread,
			fy_threadCurrentFrame(context, thread), invoke, args, ops,
			exception);
}

static fy_int classIsInstance(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	fy_class *objClazz =
	fy_heapGetObject(context,args[1].uvalue)->object_data->clazz;
	FYEH()0;
	fy_threadReturnInt(args,
			fy_classCanCastTo(context, objClazz, clazz, TRUE) ? 1 : 0);
	return ops;
}

static fy_int classIsAssignableFrom(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	fy_class *targetClazz = fy_vmGetClassFromClassObject(context,
			args[1].uvalue, exception);
	FYEH()0;
	fy_threadReturnInt(args,
			fy_classCanCastTo(context, targetClazz, clazz, TRUE) ? 1 : 0);
	return ops;
}

static fy_int classIsInterface(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	fy_threadReturnInt(args, (clazz->accessFlags & FY_ACC_INTERFACE) ? 1 : 0);
	return ops;
}

static fy_int classIsArray(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	fy_threadReturnInt(args, clazz->type == array_class ? 1 : 0);
	return ops;
}

static fy_int classIsPrimitive(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	fy_threadReturnInt(args, clazz->type == primitive_class ? 1 : 0);
	return ops;
}

static fy_int classGetSuperclass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	clazz = clazz->s.super;
	fy_threadReturnInt(args,
			clazz == NULL ?
					0 : fy_vmGetClassObjHandle(context, clazz, exception));
	return ops;
}

static fy_int classGetInterfaces(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	fy_class *classOfClass;
	fy_uint ret, i, imax, tmp;
	FYEH()0;
	classOfClass = fy_vmLookupClass(context, context->sArrayClass, exception);
	FYEH()0;
	imax = clazz->interfacesCount;
	ret = fy_heapAllocateArray(context, classOfClass, imax, exception);
	FYEH()0;
	for (i = 0; i < imax; i++) {
		tmp = fy_vmGetClassObjHandle(context, clazz->interfaces[i], exception);
		fy_heapPutArrayHandle(context, ret, i, tmp, exception);
		FYEH()0;
	}
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int classGetNativeName(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	fy_uint ret;
	FYEH()0;
	ret = fy_heapLiteral(context, clazz->className, exception);
	fy_threadReturnInt(args, ret);
	return ops;
}

static fy_int classGetModifiers(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	fy_threadReturnInt(args, clazz->accessFlags);
	return ops;
}

static fy_int classIsEnum(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	FYEH()0;
	fy_threadReturnInt(args, clazz->accessFlags & FY_ACC_ENUM ? 1 : 0);
	return ops;
}

static void registerClasses(fy_context *context, fy_exception *exception) {
	/*Class*/
	fy_vmRegisterNativeHandler(context, FY_BASE_CLASS".isEnum.()Z", NULL,
			classIsEnum, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_CLASS".getModifiers.()I", NULL,
			classGetModifiers, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getNativeName.()L"FY_BASE_STRING";", NULL,
			classGetNativeName, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getPrivateDeclaredMethods0.()[L"FY_BASE_OBJECT";",
	NULL, classPrivateGetDeclaredMethods, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getPrivateDeclaredFields0.()[L"FY_BASE_OBJECT";",
	NULL, classPrivateGetDeclaredFields, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getComponentType.()L"FY_BASE_CLASS";", NULL,
			classGetComponentType, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".forName0.(L"FY_BASE_STRING";Z)L"FY_BASE_CLASS";",
	NULL, classForName, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".newInstance0.()L"FY_BASE_OBJECT";", NULL, classNewInstanceO,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".isInstance.(L"FY_BASE_OBJECT";)Z", NULL, classIsInstance,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".isAssignableFrom.(L"FY_BASE_CLASS";)Z", NULL,
			classIsAssignableFrom, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_CLASS".isInterface.()Z", NULL,
			classIsInterface, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_CLASS".isArray.()Z", NULL,
			classIsArray, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_CLASS".isPrimitive.()Z", NULL,
			classIsPrimitive, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getSuperclass.()L"FY_BASE_CLASS";", NULL, classGetSuperclass,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_CLASS".getInterfaces.()[L"FY_BASE_CLASS";", NULL,
			classGetInterfaces, exception);
	FYEH();

}

/**********************References***********************/
static fy_int refRegister(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_hashMapIPut(context->memblocks, context->references, args[0].uvalue,
			args[1].uvalue, exception);
	return ops;
}

static fy_int refClear(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_hashMapIRemove(context->memblocks, context->references, args[0].uvalue);
	return ops;
}

static fy_int refGet(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_stack_item *args, fy_int argsCount, fy_int ops,
		fy_exception *exception) {
	fy_threadReturnInt(args,
			fy_hashMapIGet(context->memblocks, context->references,
					args[0].uvalue));
	return ops;
}

static void registerReference(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context, FY_REF".register.(L"FY_BASE_OBJECT";)V",
	NULL, refRegister, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REF".clear0.()V", NULL, refClear,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_REF".get0.()L"FY_BASE_OBJECT";",
	NULL, refGet, exception);
	FYEH();
}

static fy_int arrayNewInstance(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0].uvalue,
			exception);
	fy_uint len;
	fy_int sizes[1024];
	fy_int i;
	fy_str name[1];
	fy_class *targetClass;
	FYEH()0;
	if (clazz->type == object_class) {
		ops = fy_threadClinit(context, thread, clazz, args + argsCount, ops,
				exception);
		if (ops < 0) {
			return 0;
		}
		FYEH()0;
	}
	len = fy_heapArrayLength(context, args[1].uvalue, exception);
	FYEH()0;
	for (i = 0; i < len; i++) {
		sizes[i] = fy_heapGetArrayInt(context, args[1].uvalue, i, exception);
		FYEH()0;
	}
	name->content = NULL;
	switch (clazz->type) {
	case object_class: {
		fy_strInit(context->memblocks, name, clazz->className->length + 2 + len,
				exception);
		FYEH()0;
		for (i = 0; i < len; i++) {
			fy_strAppendUTF8(context->memblocks, name, "[", 1, exception);
		}
		fy_strAppendUTF8(context->memblocks, name, "L", 1, exception);
		fy_strAppend(context->memblocks, name, clazz->className, exception);
		fy_strAppendUTF8(context->memblocks, name, ";", 1, exception);
		targetClass = fy_vmLookupClass(context, name, exception);
		fy_strDestroy(context->memblocks, name);
		FYEH()0;
		break;
		case primitive_class:
		fy_strInit(context->memblocks, name, clazz->className->length + len,
				exception);
		FYEH()0;
		for (i = 0; i < len; i++) {
			fy_strAppendUTF8(context->memblocks, name, "[", 1, exception);
		}
		fy_strAppendChar(context->memblocks, name, clazz->ci.prm.pType,
				exception);
		targetClass = fy_vmLookupClass(context, name, exception);
		fy_strDestroy(context->memblocks, name);
		FYEH()0;
		break;
	}
	case array_class: {
		fy_strInit(context->memblocks, name, clazz->className->length + len,
				exception);
		FYEH()0;
		for (i = 0; i < len; i++) {
			fy_strAppendUTF8(context->memblocks, name, "[", 1, exception);
		}
		fy_strAppend(context->memblocks, name, clazz->className, exception);
		targetClass = fy_vmLookupClass(context, name, exception);
		fy_strDestroy(context->memblocks, name);
		FYEH()0;
		break;
	}
	default: {
		fy_fault(exception, "", "Illegal class type %d", clazz->type);
		return 0;
	}
	}

	fy_threadReturnInt(args,
			fy_heapMultiArray(context, targetClass, len, sizes, exception));
	return ops;
}

static fy_int proxyDefineClassImpl(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_stack_item *args,
		fy_int argsCount, fy_int ops, fy_exception *exception) {
	fy_int classNameHandle = args[1].ivalue;
	fy_int dataHandle = args[2].ivalue;
	fy_int offset = args[3].ivalue;
	fy_int length = args[4].ivalue;
	fy_byte *buf;
	fy_str str[1];
	fy_class *clazz;
	fy_int ret;
	/*fy_int len = fy_heapArrayLength(context, dataHandle, exception);*/
	FYEH()0;
	buf = fy_heapGetArrayBytes(context, dataHandle, exception);
	FYEH()0;
	str->content = NULL;
	fy_strInit(context->memblocks, str, 128, exception);
	FYEH()0;
	fy_heapGetString(context, classNameHandle, str, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, str);
		return 0;
	}
	fy_clDefineClass(context, str, buf + offset, length, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, str);
		return 0;
	}
	clazz = fy_vmLookupClass(context, str, exception);
	fy_strDestroy(context->memblocks, str);
	FYEH()0;
	ret = fy_vmGetClassObjHandle(context, clazz, exception);
	FYEH()0;
	fy_threadReturnInt(args, ret);
	return ops;
}

void fy_coreRegisterCoreHandlers(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/SystemInputStream.read0.()I", NULL,
			SISRead0, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_DOUBLE".longBitsToDouble.(J)D",
	NULL, doubleLongBitsToDouble, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context, FY_BASE_FLOAT".intBitsToFloat.(I)F",
	NULL, floatIntBitsToFloat, exception);
	FYEH();

	/*String*/
	fy_vmRegisterNativeHandler(context,
	FY_BASE_STRING".intern.()L"FY_BASE_STRING";", NULL, StringIntern,
			exception);
	FYEH();

	/*Runtime*/
	fy_vmRegisterNativeHandler(context, FY_BASE_RUNTIME".freeMemory.()J", NULL,
			RuntimeFreeMemory, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_RUNTIME".totalMemory.()J", NULL,
			RuntimeTotalMemory, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_RUNTIME".maxMemory.()J", NULL,
			RuntimeMaxMemory, exception);
	FYEH();
	/*System*/
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".setIn0.(L"FY_IO_INPUTSTREAM";)V", NULL, SystemSetIn,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".setOut0.(L"FY_IO_PRINTSTREAM";)V", NULL, SystemSetOut,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".setErr0.(L"FY_IO_PRINTSTREAM";)V", NULL, SystemSetErr,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_SYSTEM".setProperty0.(L"FY_BASE_STRING";L"FY_BASE_STRING";)L"FY_BASE_STRING";",
			NULL, SystemSetProperty, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".getProperty0.(L"FY_BASE_STRING";)L"FY_BASE_STRING";",
	NULL, SystemGetProperty, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".arraycopy.(L"FY_BASE_OBJECT";IL"FY_BASE_OBJECT";II)V",
	NULL, SystemArrayCopy, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_SYSTEM".currentTimeMillis.()J",
	NULL, SystemTimeMS, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_SYSTEM".nanoTime.()J", NULL,
			SystemTimeNS, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_SYSTEM".identityHashCode.(L"FY_BASE_OBJECT";)I", NULL,
			SystemIdentityHashCode, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_SYSTEM".gc.()V", NULL, SystemGC,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_SYSTEM".exit.(I)V", NULL,
			SystemExit, exception);
	FYEH();

	registerClasses(context, exception);
	FYEH();

	/*Object*/
	fy_vmRegisterNativeHandler(context,
	FY_BASE_OBJECT".clone.()L"FY_BASE_OBJECT";", NULL, ObjectClone, exception);
	fy_vmRegisterNativeHandler(context,
	FY_BASE_OBJECT".getClass.()L"FY_BASE_CLASS";", NULL, ObjectGetClass,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_OBJECT".wait.(J)V", NULL,
			ObjectWait, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_OBJECT".notify.()V", NULL,
			ObjectNotify, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_OBJECT".notifyAll.()V", NULL,
			ObjectNotifyAll, exception);
	FYEH();

	/*Thread*/
	fy_vmRegisterNativeHandler(context,
	FY_BASE_THREAD".currentThread.()L"FY_BASE_THREAD";", NULL,
			ThreadCurrentThread, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".setPriority0.(I)V",
	NULL, ThreadSetPriority, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".isAlive.()Z", NULL,
			ThreadIsAlive, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".start0.()V", NULL,
			ThreadStart, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".interrupt0.()V", NULL,
			ThreadInterrupt, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".isInterrupted.(Z)Z",
	NULL, ThreadInterrupted, exception);
	FYEH();

	/*FiScEVM*/
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.save.()V", NULL, FiScEVMSave,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.storeParamsToArray.(I[III)V",
			NULL, FiScEVMStoreParams, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.logOut0.(IL"FY_BASE_STRING";)V",
			NULL, VMLogOut, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.throwOut.(L"FY_BASE_THROWABLE";L"FY_BASE_STRING";)V",
			NULL, VMThrowOut, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.exit.(I)V", NULL, VMExit,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.decode.(L"FY_BASE_STRING";[BII)[C",
			NULL, VMDecode, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.encode.(L"FY_BASE_STRING";[CII)[B",
			NULL, VMEncode, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"java/lang/Double.doubleToRawLongBits.(D)J", NULL, VMGetDoubleRaw,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"java/lang/Float.floatToRawIntBits.(F)I", NULL, VMGetFloatRaw,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.stringToDouble.(L"FY_BASE_STRING";)D",
			NULL, VMStringToDouble, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.doubleToString.(D)L"FY_BASE_STRING";",
			NULL, VMDoubleToString, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.stringToFloat.(L"FY_BASE_STRING";)F",
			NULL, VMStringToFloat, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.floatToString.(F)L"FY_BASE_STRING";",
			NULL, VMFloatToString, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.breakpoint.()V", NULL,
			VMBreakpoint, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/SystemOutputStream.write0.(IL"FY_BASE_STRING";)V",
			NULL, SOSWrite, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
	FY_BASE_THROWABLE".fillInStackTrace0.()V", NULL, throwableFillInStackTrace,
			exception);
	FYEH();
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".sleep.(J)V", NULL,
			ThreadSleep, exception);
	fy_vmRegisterNativeHandler(context, FY_BASE_THREAD".yield.()V", NULL,
			ThreadYield, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
	FY_BASE_FINALIZER".getFinalizee.()[L"FY_BASE_OBJECT";", NULL,
			finalizerGetFinalizee, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
	FY_BASE_FINALIZER".getReferencesToEnqueue.()[L"FY_REF";", NULL,
			finalizerGetReferencesToEnqueue, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
	FY_REFLECT_ARRAY".newInstance.(L"FY_BASE_CLASS";[I)L"FY_BASE_OBJECT";",
	NULL, arrayNewInstance, exception);
	FYEH();

	registerMethod(context, exception);
	FYEH();

	registerConstructor(context, exception);
	FYEH();

	registerFields(context, exception);
	FYEH();

	registerReference(context, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASSLOADER".defineClassImpl.(L"FY_BASE_STRING";[BII)L"FY_BASE_CLASS";",
			NULL, proxyDefineClassImpl, exception);
	FYEH();

}
