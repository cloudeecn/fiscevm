/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
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

#include "fyc/DataLoader.h"
#include "fyc/Heap.h"
#include "fyc/Thread.h"
#include "fyc/VMContext.h"
#include "fyc/Preverifier.h"
#include "fyc/Constants.h"
typedef struct ClassTemp {
	fy_str name[1];
	fisce_uint handle;
	fisce_int clinited;
	fisce_int *staticArea;
} ClassTemp;

typedef struct MethodTemp {
	fisce_uint handle;
} MethodTemp;

typedef struct FieldTemp {
	fisce_uint handle;
} FieldTemp;

typedef struct Loader {
	fisce_uint classCount;
	ClassTemp *classTemp;
	fisce_uint methodCount;
	MethodTemp *methodTemp;
	fisce_uint fieldCount;
	FieldTemp *fieldTemp;
} Loader;

void* fy_loadBegin(struct fy_context *context, fisce_exception *exception) {
	/*TODO validate context's initialization*/
	return fy_mmAllocate(context->memblocks, sizeof(Loader), exception);
}
void fy_loadPrepareClass(struct fy_context *context, void *loader_,
		fisce_uint classCount, fisce_exception *exception) {
	Loader *loader = loader_;
	loader->classCount = classCount;
	loader->classTemp = fy_mmAllocate(context->memblocks,
			(classCount + 1) * sizeof(ClassTemp), exception);
	context->classesCount += classCount;
}
void fy_loadClass(struct fy_context *context, void *loader_, fisce_uint classId,
		fisce_uint handle, fisce_int clinited, fy_str *name, fisce_uint staticSize,
		fisce_uint *staticArea, fisce_exception *exception) {
	Loader *loader = loader_;
	ClassTemp *ct = loader->classTemp + classId;
	fisce_uint *pCid = fy_mmAllocate(context->memblocks, sizeof(fisce_uint),
			exception);
	FYEH();
	*pCid = classId;
	fy_hashMapPut(context->memblocks, context->mapClassNameToId, name, pCid,
			exception);
	FYEH();
	fy_strInit(context->memblocks, ct->name, name->length, exception);
	FYEH();
	fy_strAppend(context->memblocks, ct->name, name, exception);
	FYEH();
	ct->handle = handle;
	ct->clinited = clinited;
	ct->staticArea = fy_mmAllocate(context->memblocks,
			staticSize * sizeof(fisce_int), exception);
	memcpy(ct->staticArea, staticArea, staticSize * sizeof(fisce_int));
	FYEH();
}
void fy_loadEndClass(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
}
void fy_loadPrepareMethod(struct fy_context *context, void *loader_,
		fisce_uint methodCount, fisce_exception *exception) {
	Loader *loader = loader_;
	loader->methodCount = methodCount;
	loader->methodTemp = fy_mmAllocate(context->memblocks,
			methodCount * sizeof(MethodTemp), exception);
	context->methodsCount += methodCount;
}
void fy_loadMethod(struct fy_context *context, void *loader_, fisce_uint methodId,
		fisce_uint handle, fy_str *uniqueName, fisce_exception *exception) {
	Loader *loader = loader_;
	MethodTemp *mt = loader->methodTemp + methodId;
	fisce_uint *pMid = fy_mmAllocate(context->memblocks, sizeof(fisce_uint),
			exception);
	FYEH();
	mt->handle = handle;
	*pMid = methodId;
	fy_hashMapPut(context->memblocks, context->mapMethodNameToId, uniqueName,
			pMid, exception);
}
void fy_loadEndMethod(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
}
void fy_loadPrepareField(struct fy_context *context, void *loader_,
		fisce_uint fieldCount, fisce_exception *exception) {
	Loader *loader = loader_;
	loader->fieldCount = fieldCount;
	loader->fieldTemp = fy_mmAllocate(context->memblocks,
			fieldCount * sizeof(MethodTemp), exception);
	context->fieldsCount += fieldCount;
}
void fy_loadField(struct fy_context *context, void *loader_, fisce_uint fieldId,
		fisce_uint handle, fy_str *uniqueName, fisce_exception *exception) {
	Loader *loader = loader_;
	FieldTemp *ft = loader->fieldTemp + fieldId;
	fisce_uint *pFid = fy_mmAllocate(context->memblocks, sizeof(fisce_uint),
			exception);
	FYEH();
	*pFid = fieldId;
	fy_hashMapPut(context->memblocks, context->mapFieldNameToId, uniqueName,
			pFid, exception);
	ft->handle = handle;
}
void fy_loadEndField(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
	/*We do the class loading here!*/
	Loader *loader = loader_;
	fy_class *clazz;
	fy_method *method;
	fy_field *field;
	ClassTemp *ct;
	fisce_uint i, imax;
	char msg[256];
	imax = loader->classCount;
	for (i = 1; i <= imax; i++) {
		ct = loader->classTemp + i;
		clazz = fy_vmLoadClass(context, ct->name, exception);
		if (clazz == NULL) {
			fy_strSPrint(msg, sizeof(msg), ct->name);
			fy_fault(exception, FY_EXCEPTION_VM, "Failed loading class %s",
					msg);
			return;
		}
		if (ct->handle > 0) {
			fy_hashMapIPut(context->memblocks, context->classObjIds,
					clazz->classId, ct->handle, exception);
			FYEH();
		}
		clazz->clinitThreadId = ct->clinited;
		memcpy(clazz->staticArea, ct->staticArea,
				clazz->staticSize * sizeof(fisce_uint));
		fy_strDestroy(context->memblocks, ct->name);
		fy_mmFree(context->memblocks, ct->staticArea);
	}
	fy_mmFree(context->memblocks, loader->classTemp);
	imax = loader->methodCount;
	for (i = 0; i < imax; i++) {
		method = context->methods[i];
		if (method == NULL) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Method %d not loaded.", i);
			return;
		}
		method->method_id = i;
		/*TODO set handle*/
	}
	fy_mmFree(context->memblocks, loader->methodTemp);
	imax = loader->fieldCount;
	for (i = 0; i < imax; i++) {
		field = context->fields[i];
		if (field == NULL) {
			fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
					"Field %d not loaded.", i);
			return;
		}
		field->field_id = i;
		/*TODO set handle*/
	}
	fy_mmFree(context->memblocks, loader->fieldTemp);

}
void fy_loadPrepareObjects(struct fy_context *context, void *loader_,
		fisce_uint nextHandle, fisce_uint objectCount, fisce_exception *exception) {
	context->nextHandle = nextHandle;
}
void fy_loadObject(struct fy_context *context, void *loader_, fisce_uint handle,
		fisce_uint classId, fisce_int posInHeap, fisce_int gen, fisce_int finalizeStatus,
		fisce_uint monitorOwner, fisce_uint monitorCount, fisce_uint multiUsageData,
		fisce_uint dataLength, fisce_uint *data, fisce_exception *exception) {
	fy_object *object = context->objects + handle;
	fy_class *clazz = context->classes[classId];
	if (clazz == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Can't find class with id=%d", classId);
		return;
	}
#ifdef FY_VERBOSE
	context->logDVar(context, "AllocateDirect object[%"FY_PRINT32"d] class=",
			handle);
	context->logDStr(context, clazz->className);
	context->logDVar(context, "\n");
#endif
	fy_heapAllocateDirect(context, dataLength, clazz, multiUsageData, handle,
			posInHeap, exception);
	if (object->object_data->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Object allocate error!", classId);
		return;
	}
	object->object_data->position = posInHeap;
	object->object_data->gen = gen;
	object->object_data->finalizeStatus = finalizeStatus;
	object->object_data->monitorOwnerId = monitorOwner;
	object->object_data->monitorOwnerTimes = monitorCount;
	object->object_data->m.multiUsageData = multiUsageData;
	memcpy(object->object_data->data, data, dataLength * sizeof(fisce_uint));
}
void fy_loadEndObject(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
}
void fy_loadLiterals(struct fy_context *context, void *loader_, fisce_uint count,
		fisce_uint *handles, fisce_exception *exception) {
	fisce_uint i;
	fy_str str[1];
	fisce_uint handle;
	fisce_uint *pInt;
	str->content = NULL;
	fy_strInit(context->memblocks, str, 256, exception);
	for (i = 0; i < count; i++) {
		str->length = 0;
		handle = handles[i];
		fy_heapGetString(context, handle, str, exception);
		pInt = fy_mmAllocate(context->memblocks, sizeof(fisce_uint), exception);
		FYEH();
		*pInt = handle;
		fy_hashMapPut(context->memblocks, context->literals, str, pInt,
				exception);
		FYEH();
	}
}
void fy_loadFinalizes(struct fy_context *context, void *loader_, fisce_uint count,
		fisce_uint *handles, fisce_exception *exception) {
	fisce_uint i;
	for (i = 0; i < count; i++) {
		fy_arrayListAdd(context->memblocks, context->toFinalize, handles + i,
				exception);
	}
}
void fy_loadPrepareThreads(struct fy_context *context, void *loader_,
		fisce_uint threadsCount, fisce_exception *exception) {
}
fy_thread *fy_loadThread(struct fy_context *context, void *loader_,
		fisce_uint threadId, fisce_uint handle, fisce_int priority, fisce_uint daemon,
		fisce_uint destroyPending, fisce_uint interrupted, fisce_long nextWakeupTime,
		fisce_uint pendingLockCount, fisce_uint waitForLockId,
		fisce_uint waitForNotifyId, fisce_uint stackSize, fisce_uint *stack,
		fisce_exception *exception) {
	fy_thread *thread = context->threads[threadId] = fy_mmAllocate(
			context->memblocks, sizeof(fy_thread), exception);
	FYEH()NULL;
	fy_arrayListAdd(context->memblocks, context->runningThreads, &thread,
			exception);
	FYEH()NULL;
	thread->threadId = threadId;
	thread->handle = handle;
	thread->priority = priority;
	thread->daemon = daemon;
	thread->destroyPending = destroyPending;
	thread->interrupted = interrupted;
	thread->nextWakeTime = nextWakeupTime;
	thread->pendingLockCount = pendingLockCount;
	thread->waitForLockId = waitForLockId;
	thread->waitForNotifyId = waitForNotifyId;
	memcpy(thread->stack, stack, stackSize * sizeof(fisce_uint));
	return thread;
}
void fy_loadPrepareFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fisce_uint count, fisce_exception *exception) {
}
void fy_loadFrame(struct fy_context *context, void *loader_, fy_thread *thread,
		fisce_uint methodId, fisce_uint sb, fisce_uint lpc, fisce_int pcofs,
		fisce_exception *exception) {
	fy_frame *frame = FY_GET_FRAME(thread,thread->frameCount++);
	frame->method = context->methods[methodId];
	if (!(frame->method->access_flags & FY_ACC_VERIFIED)) {
		fy_preverify(context, frame->method, exception);
	}
	frame->baseSpp = thread->stack + sb;
	frame->lpc = lpc;
	frame->pcofs = pcofs;
#ifdef FY_STRICT_CHECK
	frame->size = frame->method->max_locals + frame->method->max_stack;
	frame->localCount = frame->method->max_locals;
	frame->codeSize = frame->method->codeLength;
#endif
}
void fy_loadEndFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fisce_exception *exception) {
}
void fy_loadEndThread(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
}
void fy_loadEnd(struct fy_context *context, void *loader_,
		fisce_exception *exception) {
	fy_mmFree(context->memblocks, loader_);
}
