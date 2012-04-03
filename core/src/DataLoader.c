/**
 *  Copyright 2010-2012 Yuxuan Huang. All rights reserved.
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

#include "fyc/DataLoader.h"
#include "fyc/VMContext.h"
#include "fyc/Preverifier.h"
typedef struct ClassTemp {
	fy_str name[1];
	fy_uint handle;
	fy_int clinited;
	fy_int *staticArea;
} ClassTemp;

typedef struct MethodTemp {
	fy_uint handle;
} MethodTemp;

typedef struct FieldTemp {
	fy_uint handle;
} FieldTemp;

typedef struct Loader {
	fy_uint classCount;
	ClassTemp *classTemp;
	fy_uint methodCount;
	MethodTemp *methodTemp;
	fy_uint fieldCount;
	FieldTemp *fieldTemp;
} Loader;

void* fy_loadBegin(struct fy_context *context, fy_exception *exception) {
	/*TODO validate context's initialization*/
	return fy_mmAllocate(context->memblocks, sizeof(Loader), exception);
}
void fy_loadPrepareClass(struct fy_context *context, void *loader_,
		fy_uint classCount, fy_exception *exception) {
	Loader *loader = loader_;
	loader->classCount = classCount;
	loader->classTemp = fy_mmAllocate(context->memblocks,
			(classCount + 1) * sizeof(ClassTemp), exception);
}
void fy_loadClass(struct fy_context *context, void *loader_, fy_uint classId,
		fy_uint handle, fy_int clinited, fy_str *name, fy_uint staticSize,
		fy_int *staticArea, fy_exception *exception) {
	Loader *loader = loader_;
	ClassTemp *ct = loader->classTemp + classId;
	fy_uint *pCid = fy_mmAllocate(context->memblocks, sizeof(fy_uint),
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
			staticSize * sizeof(fy_int), exception);
	memcpy(ct->staticArea, staticArea, staticSize * sizeof(fy_int));
	FYEH();
}
void fy_loadEndClass(struct fy_context *context, void *loader_,
		fy_exception *exception) {
}
void fy_loadPrepareMethod(struct fy_context *context, void *loader_,
		fy_uint methodCount, fy_exception *exception) {
	Loader *loader = loader_;
	loader->methodCount = methodCount;
	loader->methodTemp = fy_mmAllocate(context->memblocks,
			methodCount * sizeof(MethodTemp), exception);
}
void fy_loadMethod(struct fy_context *context, void *loader_, fy_uint methodId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception) {
	Loader *loader = loader_;
	MethodTemp *mt = loader->methodTemp + methodId;
	fy_uint *pMid = fy_mmAllocate(context->memblocks, sizeof(fy_uint),
			exception);
	FYEH();
	mt->handle = handle;
	*pMid = methodId;
	fy_hashMapPut(context->memblocks, context->mapMethodNameToId, uniqueName,
			pMid, exception);
}
void fy_loadEndMethod(struct fy_context *context, void *loader_,
		fy_exception *exception) {
}
void fy_loadPrepareField(struct fy_context *context, void *loader_,
		fy_uint fieldCount, fy_exception *exception) {
	Loader *loader = loader_;
	loader->fieldCount = fieldCount;
	loader->fieldTemp = fy_mmAllocate(context->memblocks,
			fieldCount * sizeof(MethodTemp), exception);
}
void fy_loadField(struct fy_context *context, void *loader_, fy_uint fieldId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception) {
	Loader *loader = loader_;
	FieldTemp *ft = loader->fieldTemp + fieldId;
	fy_uint *pFid = fy_mmAllocate(context->memblocks, sizeof(fy_uint),
			exception);
	FYEH();
	*pFid = fieldId;
	fy_hashMapPut(context->memblocks, context->mapFieldNameToId, uniqueName,
			pFid, exception);
	ft->handle = handle;
}
void fy_loadEndField(struct fy_context *context, void *loader_,
		fy_exception *exception) {
	/*We do the class loading here!*/
	Loader *loader = loader_;
	fy_class *clazz;
	fy_method *method;
	fy_field *field;
	ClassTemp *ct;
	fy_uint i, imax;
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
				clazz->staticSize * sizeof(fy_uint));
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
		fy_uint nextHandle, fy_uint objectCount, fy_exception *exception) {
	context->nextHandle = nextHandle;
}
void fy_loadObject(struct fy_context *context, void *loader_, fy_uint handle,
		fy_uint classId, fy_int posInHeap, fy_int gen, fy_int finalizeStatus,
		fy_uint monitorOwner, fy_uint monitorCount, fy_uint attachedId,
		fy_uint length, fy_uint dataLength, fy_uint *data,
		fy_exception *exception) {
	fy_object *object = context->objects + handle;
	fy_class *clazz = context->classes[classId];
	if (clazz == NULL) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Can't find class with id=%d", classId);
		return;
	}
#ifdef FY_VERBOSE
	printf("AllocateDirect object[%"FY_PRINT32"d] class=", handle);
	fy_strPrint(clazz->className);
	printf("\n");
#endif
	fy_heapAllocateDirect(context, dataLength, clazz, length, handle, posInHeap,
			exception);
	if (object->clazz != clazz) {
		fy_fault(exception, FY_EXCEPTION_INCOMPAT_CHANGE,
				"Object allocate error!", classId);
		return;
	}
	object->position = posInHeap;
	object->gen = gen;
	object->finalizeStatus = finalizeStatus;
	object->object_data->monitorOwnerId = monitorOwner;
	object->object_data->monitorOwnerTimes = monitorCount;
	object->object_data->attachedId = attachedId;
	object->object_data->length = length;
	memcpy(object->object_data->data, data, dataLength * sizeof(fy_uint));
}
void fy_loadEndObject(struct fy_context *context, void *loader_,
		fy_exception *exception) {
}
void fy_loadLiterals(struct fy_context *context, void *loader_, fy_uint count,
		fy_uint *handles, fy_exception *exception) {
	fy_uint i;
	fy_str str[1];
	fy_uint handle;
	fy_uint *pInt;
	str->content = NULL;
	fy_strInit(context->memblocks, str, 256, exception);
	for (i = 0; i < count; i++) {
		str->length = 0;
		handle = handles[i];
		fy_heapGetString(context, handle, str, exception);
		pInt = fy_mmAllocate(context->memblocks, sizeof(fy_uint), exception);
		FYEH();
		*pInt = handle;
		fy_hashMapPut(context->memblocks, context->literals, str, pInt,
				exception);
		FYEH();
	}
}
void fy_loadFinalizes(struct fy_context *context, void *loader_, fy_uint count,
		fy_uint *handles, fy_exception *exception) {
	fy_uint i;
	for (i = 0; i < count; i++) {
		fy_arrayListAdd(context->memblocks, context->toFinalize, handles + i,
				exception);
	}
}
void fy_loadPrepareThreads(struct fy_context *context, void *loader_,
		fy_uint threadsCount, fy_exception *exception) {
}
fy_thread *fy_loadThread(struct fy_context *context, void *loader_,
		fy_uint threadId, fy_uint handle, fy_int priority, fy_uint daemon,
		fy_uint destroyPending, fy_uint interrupted, fy_long nextWakeupTime,
		fy_uint pendingLockCount, fy_uint waitForLockId,
		fy_uint waitForNotifyId, fy_uint stackSize, fy_uint *stack,
		fy_uint *typeStack, fy_exception *exception) {
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
	memcpy(thread->stack, stack, stackSize * sizeof(fy_uint));
	memcpy(thread->typeStack, typeStack,
			(stackSize + 31) / 32 * sizeof(fy_uint));
	return thread;
}
void fy_loadPrepareFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fy_uint count, fy_exception *exception) {
}
void fy_loadFrame(struct fy_context *context, void *loader_, fy_thread *thread,
		fy_uint methodId, fy_uint sb, fy_uint sp, fy_uint pc, fy_uint lpc,
		fy_exception *exception) {
	fy_frame *frame = FY_GET_FRAME(thread,thread->frameCount++);
	frame->methodId = methodId;
	frame->method = context->methods[methodId];
	if (!(frame->method->access_flags & FY_ACC_VERIFIED)) {
		fy_preverify(context, frame->method, exception);
	}
	frame->instructions = frame->method->instructions;
	frame->sb = sb;
	frame->sp = sp;
	frame->pc = pc;
	frame->lpc = lpc;
#ifdef FY_STRICT_CHECK
	frame->size = frame->method->max_locals + frame->method->max_stack;
	frame->localCount = frame->method->max_locals;
	frame->codeSize = frame->method->codeLength;
#endif
}
void fy_loadEndFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fy_exception *exception) {
}
void fy_loadEndThread(struct fy_context *context, void *loader_,
		fy_exception *exception) {
}
void fy_loadEnd(struct fy_context *context, void *loader_,
		fy_exception *exception) {
	fy_mmFree(context->memblocks, loader_);
}
