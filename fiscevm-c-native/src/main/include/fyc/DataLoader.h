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

#ifndef DATALOADER_H_
#define DATALOADER_H_

#include "fy_util/Portable.h"

#include "fy_util/String.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif
void* fy_loadBegin(struct fy_context *context, fisce_exception *exception);
void fy_loadPrepareClass(struct fy_context *context, void *loader_,
		fisce_uint classCount, fisce_exception *exception);
void fy_loadClass(struct fy_context *context, void *loader_, fisce_uint classId,
		fisce_uint handle, fisce_int clinited, fy_str *name, fisce_uint staticSize,
		fisce_uint *staticArea, fisce_exception *exception);
void fy_loadEndClass(struct fy_context *context, void *loader_,
		fisce_exception *exception);
void fy_loadPrepareMethod(struct fy_context *context, void *loader_,
		fisce_uint methodCount, fisce_exception *exception);
void fy_loadMethod(struct fy_context *context, void *loader_, fisce_uint methodId,
		fisce_uint handle, fy_str *uniqueName, fisce_exception *exception);
void fy_loadEndMethod(struct fy_context *context, void *loader_,
		fisce_exception *exception);
void fy_loadPrepareField(struct fy_context *context, void *loader_,
		fisce_uint fieldCount, fisce_exception *exception);
void fy_loadField(struct fy_context *context, void *loader_, fisce_uint fieldId,
		fisce_uint handle, fy_str *uniqueName, fisce_exception *exception);
void fy_loadEndField(struct fy_context *context, void *loader_,
		fisce_exception *exception);
void fy_loadPrepareObjects(struct fy_context *context, void *loader_,
		fisce_uint nextHandle, fisce_uint objectCount, fisce_exception *exception);
void fy_loadObject(struct fy_context *context, void *loader_, fisce_uint handle,
		fisce_uint classId, fisce_int posInHeap, fisce_int gen, fisce_int finalizeStatus,
		fisce_uint monitorOwner, fisce_uint monitorCount,
		fisce_uint length, fisce_uint dataLength, fisce_uint *data,
		fisce_exception *exception);
void fy_loadEndObject(struct fy_context *context, void *loader_,
		fisce_exception *exception);
void fy_loadLiterals(struct fy_context *context, void *loader_, fisce_uint count,
		fisce_uint *handles, fisce_exception *exception);
void fy_loadFinalizes(struct fy_context *context, void *loader_, fisce_uint count,
		fisce_uint *handles, fisce_exception *exception);
void fy_loadPrepareThreads(struct fy_context *context, void *loader_,
		fisce_uint threadsCount, fisce_exception *exception);
fy_thread *fy_loadThread(struct fy_context *context, void *loader_,
		fisce_uint threadId, fisce_uint handle, fisce_int priority, fisce_uint daemon,
		fisce_uint destroyPending, fisce_uint interrupted, fisce_long nextWakeupTime,
		fisce_uint pendingLockCount, fisce_uint waitForLockId,
		fisce_uint waitForNotifyId, fisce_uint stackSize, fisce_uint *stack,
		fisce_exception *exception);
void fy_loadPrepareFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fisce_uint count, fisce_exception *exception);
void fy_loadFrame(struct fy_context *context, void *loader_, fy_thread *thread,
		fisce_uint methodId, fisce_uint sb, fisce_uint lpc, fisce_int pcofs,
		fisce_exception *exception);
void fy_loadEndFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fisce_exception *exception);
void fy_loadEndThread(struct fy_context *context, void *loader_,
		fisce_exception *exception);
void fy_loadEnd(struct fy_context *context, void *loader_,
		fisce_exception *exception);
#ifdef	__cplusplus
}
#endif

#endif /* DATALOADER_H_ */
