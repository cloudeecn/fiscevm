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

#include "fyc/CoreHandlers.h"
#include "fiscedev.h"

static void FiScEVMSave(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	context->callForSave(context, exception);
}

static void FiScEVMStoreParams(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
//We don't have a frame for native function currently, so the current frame is the caller frame.
	fy_frame *frame = thread->frames + (thread->frameCount - 1);
	fy_int begin = args[0];
	fy_int targetHandle = args[1];
	fy_int targetPos = args[2];
	fy_int count = args[3];
	fy_int *targetData = (fy_int*) fy_heapGetArrayBytes(context, targetHandle,
			exception);
	if (fy_heapArrayLength(context, targetHandle, exception)
			< count + targetPos) {
		fy_fault(exception, FY_EXCEPTION_IOOB,
				"Target array is too small to fit param size");
		return;
	}
	if (frame->method->max_locals < count + begin) {
		fy_fault(exception, FY_EXCEPTION_IOOB,
				"Current method doesn't have %d params.", count);
		return;
	}
	memcpy(targetData + targetPos, thread->stack + frame->sb + begin,
			count * sizeof(fy_int));
}

static void SystemGC(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_heapGC(context, exception);
}

static void SystemExit(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	context->exitCode = args[0];
}

static void StringIntern(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_str str;
	fy_uint ret;

	str.content = NULL;
	fy_strInit(context->memblocks, &str, 512, exception);
	FYEH();
	fy_heapGetString(context, args[0], &str, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return;
	}
	ret = fy_heapLiteral(context, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	fy_nativeReturnHandle(context, thread, ret);
}

static void floatIntBitsToFloat(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_nativeReturnInt(context, thread, args[0]);
}

static void SystemArrayCopy(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_int srcHandle = args[0];
	fy_int srcPos = args[1];
	fy_int dstHandle = args[2];
	fy_int dstPos = args[3];
	fy_int len = args[4];
	fy_heapArrayCopy(context, srcHandle, srcPos, dstHandle, dstPos, len,
			exception);
}

static void SystemTimeMS(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_nativeReturnLong(context, thread, fy_portTimeMillSec(context->port));
}

static void SystemTimeNS(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_nativeReturnLong(context, thread, fy_portTimeNanoSec(context->port));
}

static void SystemIdentityHashCode(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_nativeReturnInt(context, thread, args[0]);
}

static void ObjectGetClass(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_class *clazz = fy_heapGetClassOfObject(context, args[0]);
	fy_nativeReturnHandle(context, thread,
			fy_vmGetClassObjHandle(context, clazz, exception));
}
static void ObjectClone(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_int ret = fy_heapClone(context, args[0], exception);
	FYEH();
	fy_nativeReturnHandle(context, thread, ret);
}
static void ObjectWait(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_uint monitorId = args[0];
	fy_uint high = args[1];
	fy_uint low = args[2];
	fy_long time = fy_I2TOL(high,low);
	fy_tmWait(context, thread, monitorId, time, exception);
}

static void ObjectNotify(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_uint monitorId = args[0];
	fy_tmNotify(context, thread, monitorId, FALSE, exception);
}

static void ObjectNotifyAll(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_uint monitorId = args[0];
	fy_tmNotify(context, thread, monitorId, TRUE, exception);
}

static void ClassGetComponentType(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_heapGetClassOfObject(context, args[0]);
	if (clazz->type == arr) {
		fy_nativeReturnHandle(context, thread,
				fy_vmGetClassObjHandle(context, clazz->ci.arr.contentClass,
						exception));
	} else {
		fy_nativeReturnHandle(context, thread, 0);
	}
}

static void ClassInvokeMethod(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_uint classHandle = args[0];
	fy_uint methodNameHandle = args[1];
	/*fy_boolean isStatic = args[2];*/
	fy_uint paramsHandle = args[3];
	fy_class *clazz = fy_vmGetClassFromClassObject(context, classHandle,
			exception);
	fy_str methodName;
	FYEH();
	fy_method *method;
	fy_byte *paramTypes;
	fy_uint count, i;
	fy_frame *currentFrame = thread->frames + (thread->frameCount - 1);
	fy_uint sp = currentFrame->sp;

	methodName.content = NULL;
	fy_strInit(context->memblocks, &methodName, 64, exception);
	FYEH();

	fy_strAppendUTF8(context->memblocks, &methodName, ".", 1, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &methodName);
		return;
	}

	fy_heapGetString(context, methodNameHandle, &methodName, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &methodName);
		return;
	}

	method = fy_vmLookupMethodVirtual(context, clazz, &methodName, exception);
	fy_strDestroy(context->memblocks, &methodName);
	FYEH();
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_ITE, "Method not found!");
		return;
	}
	if (method->returnType != FY_TYPE_HANDLE) {
		fy_fault(exception, FY_EXCEPTION_ITE, "Return type not Object!");
		return;
	}
	paramTypes = method->paramTypes;
	count = paramsHandle == 0 ?
			0 : fy_heapArrayLength(context, paramsHandle, exception);
	FYEH();
	if (count != method->paramCount) {
		fy_fault(exception, FY_EXCEPTION_ITE, "param count not match!");
		return;
	}
	for (i = 0; i < count; i++) {
		thread->stack[sp + i] = fy_heapGetArrayInt(context, paramsHandle, i,
				exception);
		FYEH();
		thread->typeStack[sp + i] = FY_TYPE_HANDLE;
		if (paramTypes[i] != FY_TYPE_HANDLE) {
			fy_fault(exception, FY_EXCEPTION_ITE, "param type not all handle");
			break;
		}
	}
	fy_threadPushMethod(context, thread, method, NULL, exception);
	FYEH();

}

static void ThreadCurrentThread(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_nativeReturnHandle(context, thread, thread->handle);
}

static void ThreadSetPriority(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_thread *target;
	fy_object *obj = context->objects + args[0];
	target = context->threads[obj->attachedId];
	if (target != NULL) {
		target->priority = args[1];
	}
}

static void ThreadIsAlive(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_nativeReturnInt(context, thread,
			fy_tmIsAlive(context, args[0], exception) ? 1 : 0);
}

static void ThreadInterrupt(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_tmInterrupt(context, args[0], exception);
}

static void ThreadInterrupted(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_boolean ret;
	ret = fy_tmIsInterrupted(context, args[0], args[1], exception);
	fy_nativeReturnInt(context, thread, ret ? 1 : 0);
}

static void ThreadStart(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_tmPushThread(context, args[0], exception);
}

static void ThreadSleep(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_tmSleep(context, thread, fy_I2TOL(args[0],args[1]));
}

static void ThreadYield(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	thread->yield = TRUE;
}

static void VMDebugOut(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_str str[1];
	fy_memblock *block = context->memblocks;
	fy_vmLookupClass(context, context->sString, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	str->content = NULL;
	fy_strInit(block, str, 128, exception);
	FYEH();
	fy_heapGetString(context, args[0], str, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	printf("VMDebugOut: ");
	fy_strPrint(str);
	printf("\n");
	fy_strDestroy(block, str);
}

static void VMDebugOutI(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %d\n", args[0]);
}

static void VMDebugOutJ(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %"FY_PRINT64"d\n", fy_I2TOL(args[0],args[1]));
}

static void VMDebugOutF(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %f\n", fy_intToFloat(args[0]));
}

static void VMDebugOutD(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	printf("VMDebugOutI: %f\n", fy_longToDouble(fy_I2TOL(args[0],args[1])));
}

static void VMThrowOut(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	/*TODO */
	thread->currentThrowable = args[0];
}

static void VMExit(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	/*TODO */
}

static void SOSWrite(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	putchar(args[1]);
}

static void VMDecode(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_int handleSrc = args[1];
	fy_int ofs = args[2];
	fy_int len = args[3];
	int i, maxi;
	fy_byte *array;
	fy_byte *newArray;
	fy_str str[1];
	fy_int handleRet;
	fy_class *charArray;
	fy_memblock *block = context->memblocks;
	charArray = fy_vmLookupClass(context, context->sArrayChar, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}
	array = fy_heapGetArrayBytes(context, handleSrc, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}

	newArray = /*TEMP*/fy_allocate(len + 1, exception);
	FYEH();
	memcpy(newArray, array + ofs, len);
	newArray[len] = 0;
	str->content = NULL;
	fy_strInitWithUTF8(block, str, (char*) newArray, exception);
	fy_free(newArray);
	FYEH();

	handleRet = fy_heapAllocateArray(context, charArray, maxi = str->length,
			exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		fy_strDestroy(block, str);
		return;
	}
	for (i = 0; i < maxi; i++) {
		fy_heapPutArrayChar(context, handleRet, i, str->content[i], exception);
		if (exception->exceptionType != exception_none) {
			fy_nativeReturnHandle(context, thread, 0);
			fy_strDestroy(block, str);
			return;
		}
	}
	fy_strDestroy(block, str);
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMEncode(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_int handleSrc = args[1];
	fy_int ofs = args[2];
	fy_int len = args[3];
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
		fy_nativeReturnHandle(context, thread, 0);
		return;
	}
	left = len * 3 + 1;
	olen = 0;
	out = outTmp = /*TEMP*/fy_allocate(left, exception);
	FYEH();
	for (i = 0; i < len; i++) {
		ch = fy_heapGetArrayChar(context, handleSrc, i + ofs, exception);
		if (exception->exceptionType != exception_none) {
			fy_nativeReturnHandle(context, thread, 0);
			fy_free(out);
			return;
		}
		read = fy_utf8Write(ch, (char**) &outTmp, &left);
		if (read <= 0) {
			break;
		}
		olen += read;
	}
	handleRet = fy_heapAllocateArray(context, byteClass, olen, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		fy_free(out);
		return;
	}

	outTmp = fy_heapGetArrayBytes(context, handleRet, exception);
	if (exception->exceptionType != exception_none) {
		fy_nativeReturnHandle(context, thread, 0);
		fy_free(out);
		return;
	}

	memcpy(outTmp, out, olen);
	fy_free(out);
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMGetDoubleRaw(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_nativeReturnLong(context, thread,
			((fy_long) args[0] << 32) | ((fy_uint) args[1]));
}

static void VMGetFloatRaw(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_nativeReturnInt(context, thread, args[0]);
}

static void VMStringToDouble(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	char ch[64];
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH();
	fy_double value;
	fy_heapGetString(context, args[0], str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	fy_strDestroy(context->memblocks, str);
	FYEH();
	value = atof(ch);
	fy_nativeReturnLong(context, thread, fy_doubleToLong(value));
}

static void VMDoubleToString(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_long lvalue = ((fy_ulong) args[0] << 32) | ((fy_uint) args[1]);
	fy_double dvalue = fy_longToDouble(lvalue);
	fy_int handleRet;
	char ch[64];
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH();
	sprintf_s(ch, sizeof(ch), "%f", dvalue);
	fy_strAppendUTF8(context->memblocks, str, ch, 64, exception);
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strDestroy(context->memblocks, str);
	FYEH();
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void VMStringToFloat(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	char ch[64];
	fy_double value;
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH();
	fy_heapGetString(context, args[0], str, exception);
	fy_strSPrint(ch, sizeof(ch), str);
	fy_strDestroy(context->memblocks, str);
	FYEH();
	value = atof(ch);
	fy_nativeReturnInt(context, thread, fy_floatToInt((fy_float) value));
}

static void VMFloatToString(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_memblock *block = context->memblocks;
	fy_int value = args[0];
	fy_float fvalue = fy_intToFloat(value);
	fy_int handleRet;
	char ch[64];
	fy_str str[1];
	str->content = NULL;
	fy_strInit(context->memblocks, str, 64, exception);
	FYEH();
	sprintf_s(ch, sizeof(ch), "%f", fvalue);
	fy_strAppendUTF8(block, str, ch, 64, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, str);
		return;
	}
	handleRet = fy_heapMakeString(context, str, exception);
	fy_strDestroy(block, str);
	if (exception->exceptionType != exception_none) {
		return;
	}
	fy_nativeReturnHandle(context, thread, handleRet);
}

static void throwableFillInStackTrace(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_int value = args[0];
	fy_threadFillException(context, thread, value, exception);
}

static void classGetName(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_uint handle;
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	handle = fy_heapMakeString(context, clazz->className, exception);
	if (exception->exceptionType != exception_none) {
		return;
	}
	fy_nativeReturnHandle(context, thread, handle);
}

static void classForName(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_uint nameHandle = args[0];
	fy_boolean initialize = args[1];
	fy_str str;
	fy_class *clazz;
	str.content = NULL;
	fy_strInit(context->memblocks, &str, 64, exception);
	FYEH();

	fy_heapGetString(context, nameHandle, &str, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return;
	}
	fy_strReplaceOne(&str, '.', '/');
	clazz = fy_vmLookupClass(context, &str, exception);
	FYEH();
	fy_nativeReturnHandle(context, thread,
			fy_vmGetClassObjHandle(context, clazz, exception));
}

static void classNewInstanceO(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz;
	fy_frame *currentFrame = thread->frames + thread->frameCount - 1;
	fy_method *invoke;
	fy_str str;
	fy_uint sp;
	fy_uint handle;
	clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	if (clazz->type != obj) {
		fy_fault(exception, FY_EXCEPTION_RT, "Class is not an object class!");
		return;
	}
	handle = fy_heapAllocate(context, clazz, exception);
	FYEH();
	fy_nativeReturnHandle(context, thread, handle);
	sp = currentFrame->sp;
	str.content = NULL;
	fy_strInitWithUTF8(context->memblocks, &str, "."FY_METHOD_INIT".()V",
			exception);
	FYEH();
	invoke = fy_vmLookupMethodVirtual(context, clazz, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	FYEH();
	thread->stack[sp] = handle;
	thread->typeStack[sp] = FY_TYPE_HANDLE;
	fy_threadPushMethod(context, thread, invoke, NULL, exception);
}

/*Make an array of array class for example Object[]->Object[]*/
static void classNewInstanceA(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz;
	clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	if (clazz->type != arr) {
		fy_fault(exception, FY_EXCEPTION_RT, "Class is not an object class!");
	}
	fy_nativeReturnHandle(context, thread,
			fy_heapAllocateArray(context, clazz, args[1], exception));
}

static void vmNewInstance(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_class *clazz;
	fy_frame *currentFrame = thread->frames + thread->frameCount - 1;
	fy_method *invoke;
	fy_str str;
	fy_uint sp;
	fy_uint handle;
	fy_uint paramsArray = args[2];
	fy_uint paramsClassArray = args[1];
	fy_uint paramClassHandle;
	fy_int len, i;
	fy_class *paramClass;
	clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	if (clazz->type != obj) {
		fy_fault(exception, FY_EXCEPTION_RT, "Class is not an object class!");
		return;
	}
	len = fy_heapArrayLength(context, paramsArray, exception);
	FYEH();

	handle = fy_heapAllocate(context, clazz, exception);
	FYEH();
	fy_nativeReturnHandle(context, thread, handle);
	sp = currentFrame->sp;
	str.content = NULL;
	fy_strInitWithUTF8(context->memblocks, &str, "."FY_METHOD_INIT".(",
			exception);
	FYEH();
	for (i = 0; i < len; i++) {
		paramClassHandle = fy_heapGetArrayHandle(context, paramsClassArray, i,
				exception);
		if (exception->exceptionType != exception_none) {
			fy_strDestroy(context->memblocks, &str);
			return;
		}
		paramClass = fy_vmGetClassFromClassObject(context, paramClassHandle,
				exception);
		if (exception->exceptionType != exception_none) {
			fy_strDestroy(context->memblocks, &str);
			return;
		}
		switch (paramClass->type) {
		case obj:
			fy_strAppendUTF8(context->memblocks, &str, "L", 1, exception);
			if (exception->exceptionType != exception_none) {
				fy_strDestroy(context->memblocks, &str);
				return;
			}
			fy_strAppend(context->memblocks, &str, paramClass->className,
					exception);
			if (exception->exceptionType != exception_none) {
				fy_strDestroy(context->memblocks, &str);
				return;
			}
			fy_strAppendUTF8(context->memblocks, &str, ";", 1, exception);
			if (exception->exceptionType != exception_none) {
				fy_strDestroy(context->memblocks, &str);
				return;
			}
			break;
		case arr:
			fy_strAppend(context->memblocks, &str, paramClass->className,
					exception);
			if (exception->exceptionType != exception_none) {
				fy_strDestroy(context->memblocks, &str);
				return;
			}
			break;
		case prm:
			fy_strAppendChar(context->memblocks, &str, paramClass->ci.prm.pType,
					exception);
			if (exception->exceptionType != exception_none) {
				fy_strDestroy(context->memblocks, &str);
				return;
			}
			break;
		default:
			fy_fault(exception, NULL, "Invalid class type %d",
					paramClass->type);
			break;
		}
	}
	fy_strAppendUTF8(context->memblocks, &str, ")V", 2, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return;
	}
	invoke = fy_vmLookupMethodVirtual(context, clazz, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	FYEH();
	thread->stack[sp] = handle;
	thread->typeStack[sp] = FY_TYPE_HANDLE;
	for (i = 0; i < len; i++) {
		paramClassHandle = fy_heapGetArrayHandle(context, paramsArray, i,
				exception);
		FYEH();
		thread->stack[sp + i + 1] = paramClassHandle;
		thread->typeStack[sp + i + 1] = FY_TYPE_HANDLE;
	}
	fy_threadPushMethod(context, thread, invoke, NULL, exception);
}

/*Make an array of any class for example Object->Object[]*/
static void vmNewArray(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_class *clazz;
	fy_str str;
	fy_char *cc;
	clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();

	str.content = NULL;
	fy_strInit(context->memblocks, &str, clazz->className->length + 3,
			exception);
	FYEH();
	switch (clazz->type) {
	case obj:
		fy_strAppendUTF8(context->memblocks, &str, "[L", 2, exception);
		fy_strAppend(context->memblocks, &str, clazz->className, exception);
		fy_strAppendChar(context->memblocks, &str, ';', exception);
		break;
	case arr:
		fy_strAppendUTF8(context->memblocks, &str, "[", 1, exception);
		fy_strAppend(context->memblocks, &str, clazz->className, exception);
		break;
	case prm:
		fy_strAppendChar(context->memblocks, &str, '[', exception);
		cc = fy_hashMapGet(context->memblocks, context->mapPrimitivesRev,
				clazz->className);
		fy_strAppendChar(context->memblocks, &str, *cc, exception);
		break;
	}
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(context->memblocks, &str);
		return;
	}
	clazz = fy_vmLookupClass(context, &str, exception);
	fy_strDestroy(context->memblocks, &str);
	FYEH();
	if (clazz->type != arr) {
		fy_fault(exception, FY_EXCEPTION_RT, "Class is not an object class!");
	}
	fy_nativeReturnHandle(context, thread,
			fy_heapAllocateArray(context, clazz, args[1], exception));
}

static void classIsInstance(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	fy_class *objClazz = fy_heapGetObject(context,args[1])->clazz;
	FYEH();
	fy_nativeReturnInt(context, thread,
			fy_classCanCastTo(context, objClazz, clazz) ? 1 : 0);
}

static void classIsAssignableFrom(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	fy_class *targetClazz = fy_vmGetClassFromClassObject(context, args[1],
			exception);
	FYEH();
	fy_nativeReturnInt(context, thread,
			fy_classCanCastTo(context, clazz, targetClazz) ? 1 : 0);
}

static void classIsInterface(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	fy_nativeReturnInt(context, thread,
			(clazz->accessFlags & FY_ACC_INTERFACE) ? 1 : 0);
}

static void classIsArray(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	fy_nativeReturnInt(context, thread, clazz->type == arr ? 1 : 0);
}

static void classIsPrimitive(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	fy_nativeReturnInt(context, thread, clazz->type == prm ? 1 : 0);
}

static void classGetSuperclass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	FYEH();
	clazz = clazz->super;
	fy_nativeReturnInt(context, thread,
			clazz == NULL ?
					0 : fy_vmGetClassObjHandle(context, clazz, exception));
}

static void classGetInterfaces(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_class *clazz = fy_vmGetClassFromClassObject(context, args[0], exception);
	fy_class *classOfClass;
	fy_uint ret, i, imax, tmp;
	FYEH();
	classOfClass = fy_vmLookupClass(context, context->sClassClass, exception);
	FYEH();
	imax = clazz->interfacesCount;
	ret = fy_heapAllocateArray(context, classOfClass, imax, exception);
	FYEH();
	for (i = 0; i < imax; i++) {
		tmp = fy_vmGetClassObjHandle(context, clazz->interfaces[i], exception);
		fy_heapPutArrayHandle(context, ret, i, tmp, exception);
		FYEH();
	}
	fy_nativeReturnHandle(context, thread, ret);
}

static void finalizerGetFinalizee(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_uint ret;
	fy_int storage, i, len = context->toFinalize->length;
	fy_object *object;

	fy_class *clazz = fy_vmLookupClass(context, context->sArrayObject,
			exception);
	FYEH();

//	printf("#Finalizer %d objects need finalize\n", len);

	ret = fy_heapAllocateArray(context, clazz, len, exception);
	FYEH();

	for (i = 0; i < len; i++) {
		fy_arrayListGet(context->memblocks, context->toFinalize, i, &storage);
//		printf("GET %d need finalize\n", storage);
		fy_heapPutArrayHandle(context, ret, i, storage, exception);
		FYEH();
		object = fy_heapGetObject(context,storage);
		object->finalizeStatus = finalized;
	}
	fy_arrayListClear(context->memblocks, context->toFinalize);
	fy_nativeReturnHandle(context, thread, ret);
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
static void methodIsBridge(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnInt(context, thread, FY_ACC_BRIDGE && method->access_flags);
}

static void methodIsVarArgs(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnInt(context, thread, FY_ACC_VARARGS && method->access_flags);
}

static void methodIsSynthetic(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnInt(context, thread, method->synthetic);
}

static void methodGetDeclaringClass(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnInt(context, thread,
			fy_vmGetClassObjHandle(context, method->owner, exception));
}

static void methodExceptionTypes(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_int i, max, ret, handler;
	fy_exceptionHandler *eh;
	fy_class *handlerClass, *classClass;
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	classClass = fy_vmLookupClass(context, context->sArrayClass, exception);
	FYEH();

	max = method->exception_table_length;
	ret = fy_heapAllocateArray(context, classClass, max, exception);
	FYEH();
	for (i = 0; i < max; i++) {
		eh = method->exception_table + i;
		handlerClass = fy_vmLookupClassFromExceptionHandler(context, eh,
				exception);
		FYEH();
		handler = fy_vmGetClassObjHandle(context, handlerClass, exception);
		FYEH();
		fy_heapPutArrayHandle(context, ret, i, handler, exception);
		FYEH();
	}
	fy_nativeReturnHandle(context, thread, ret);
}

static void methodGetModifiers(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnInt(context, thread, method->access_flags);
}

static void methodGetName(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	fy_int ret;
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	ret = fy_heapLiteral(context, method->name, exception);
	FYEH();
	fy_nativeReturnHandle(context, thread, ret);
}

static void methodGetParameterTypes(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	fy_class *clazz;
	fy_int ret, i, max, handle;
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	clazz = fy_vmLookupClass(context, context->sArrayClass, exception);
	FYEH();
	max = method->parameterTypes->length;
	ret = fy_heapAllocateArray(context, clazz, max, exception);
	for (i = 0; i < max; i++) {
		fy_arrayListGet(context->memblocks, method->parameterTypes, i, &clazz);
		handle = fy_vmGetClassObjHandle(context, clazz, exception);
		FYEH();
		fy_heapPutArrayHandle(context, ret, i, handle, exception);
		FYEH();
	}
	fy_nativeReturnHandle(context, thread, ret);

}

static void methodGetReturnType(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnHandle(context, thread,
			fy_vmGetClassObjHandle(context, method->returnTypeClass,
					exception));
}

static void methodInvoke(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	/*TODO*/
}

static void methodGetUniqueName(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *methodObj = fy_heapGetObject(context,args[0]);
	fy_method *method = context->methods[methodObj->attachedId];
	if (method == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	fy_nativeReturnHandle(context, thread,
			fy_heapLiteral(context, method->uniqueName, exception));

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
			FY_REFLECT_METHOD".getName.()L"FY_BASE_STRING";", NULL,
			methodGetName, exception);
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
			FY_REFLECT_METHOD".invoke.(L"FY_BASE_OBJECT";[L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
			NULL, methodInvoke, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_REFLECT_METHOD".getUniqueName.()L"FY_BASE_STRING";", NULL,
			methodGetUniqueName, exception);
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

static void fieldIsSynthetic(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context,args[0]);
	fy_field *field = context->fields[fieldObj->attachedId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	/*TODO*/
	fy_nativeReturnInt(context, thread, 0);
}

static void fieldIsEnumconstant(struct fy_context *context,
		struct fy_thread *thread, void *data, fy_uint *args, fy_int argsCount,
		fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context,args[0]);
	fy_field *field = context->fields[fieldObj->attachedId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	/*FIXME 谁知道这种事情啊*/
}

static void fieldGet(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context,args[0]);
	fy_field *field = context->fields[fieldObj->attachedId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}
	/*TODO STUD*/
	fy_fault(exception, FY_EXCEPTION_NO_METHOD, "stub");
}

static void field(struct fy_context *context, struct fy_thread *thread,
		void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
	fy_object *fieldObj = fy_heapGetObject(context,args[0]);
	fy_field *field = context->fields[fieldObj->attachedId];
	if (field == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
		FYEH();
	}

}
/*
 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }

 static void field(struct fy_context *context, struct fy_thread *thread,
 void *data, fy_uint *args, fy_int argsCount, fy_exception *exception) {
 fy_object *fieldObj = fy_heapGetObject(context,args[0]);
 fy_field *field = context->fields[fieldObj->attachedId];
 if (field == NULL) {
 fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE, "Method not found!");
 FYEH();
 }

 }
 */

void fy_coreRegisterCoreHandlers(fy_context *context, fy_exception *exception) {
	fy_vmRegisterNativeHandler(context, FY_BASE_FLOAT".intBitsToFloat.(I)F",
			NULL, floatIntBitsToFloat, exception);
	/*vm*/
	fy_vmRegisterNativeHandler(context,
			FY_BASE_VM".newInstance0.(L"FY_BASE_CLASS";I)[L"FY_BASE_OBJECT";",
			NULL, classNewInstanceA, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_VM".newInstance0.(L"FY_BASE_CLASS";[L"FY_BASE_CLASS";[L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
			NULL, vmNewInstance, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_VM".newArray0.(L"FY_BASE_CLASS";I)[L"FY_BASE_OBJECT";",
			NULL, vmNewArray, exception);
	FYEH();
	/*String*/
	fy_vmRegisterNativeHandler(context,
			FY_BASE_STRING".intern.()L"FY_BASE_STRING";", NULL, StringIntern,
			exception);
	FYEH();

	/*System*/
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

	/*Class*/
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".getComponentType.()L"FY_BASE_CLASS";", NULL,
			ClassGetComponentType, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".invokeMethodHandleReturn0.(L"FY_BASE_STRING";Z[L"FY_BASE_OBJECT";)L"FY_BASE_OBJECT";",
			NULL, ClassInvokeMethod, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".forName0.(L"FY_BASE_STRING";Z)L"FY_BASE_CLASS";",
			NULL, classForName, exception);
	FYEH();

	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".newInstance0.()L"FY_BASE_OBJECT";", NULL,
			classNewInstanceO, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".isInstance.(L"FY_BASE_OBJECT";)Z", NULL,
			classIsInstance, exception);
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
			FY_BASE_CLASS".getSuperclass.()L"FY_BASE_CLASS";", NULL,
			classGetSuperclass, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".getInterfaces.()[L"FY_BASE_CLASS";", NULL,
			classGetInterfaces, exception);
	FYEH();

	/*Object*/
	fy_vmRegisterNativeHandler(context,
			FY_BASE_OBJECT".clone.()L"FY_BASE_OBJECT";", NULL, ObjectClone,
			exception);
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
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(L"FY_BASE_STRING";)V",
			NULL, VMDebugOut, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(I)V", NULL,
			VMDebugOutI, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(J)V", NULL,
			VMDebugOutJ, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(F)V", NULL,
			VMDebugOutF, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.debugOut.(D)V", NULL,
			VMDebugOutD, exception);
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
			"com/cirnoworks/fisce/privat/FiScEVM.getDoubleRaw.(D)J", NULL,
			VMGetDoubleRaw, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			"com/cirnoworks/fisce/privat/FiScEVM.getFloatRaw.(F)I", NULL,
			VMGetFloatRaw, exception);
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
			"com/cirnoworks/fisce/privat/SystemOutputStream.write0.(IL"FY_BASE_STRING";)V",
			NULL, SOSWrite, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_THROWABLE".fillInStackTrace0.()V", NULL,
			throwableFillInStackTrace, exception);
	FYEH();
	fy_vmRegisterNativeHandler(context,
			FY_BASE_CLASS".getName0.()L"FY_BASE_STRING";", NULL, classGetName,
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

	registerMethod(context, exception);
	FYEH();

}
