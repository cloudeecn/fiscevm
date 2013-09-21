/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
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
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATALOADER_H_
#define DATALOADER_H_

#include "../fiscestu.h"
#ifdef	__cplusplus
extern "C" {
#endif
void* fy_loadBegin(struct fy_context *context, fy_exception *exception);
void fy_loadPrepareClass(struct fy_context *context, void *loader_,
		fy_uint classCount, fy_exception *exception);
void fy_loadClass(struct fy_context *context, void *loader_, fy_uint classId,
		fy_uint handle, fy_int clinited, fy_str *name, fy_uint staticSize,
		fy_uint *staticArea, fy_exception *exception);
void fy_loadEndClass(struct fy_context *context, void *loader_,
		fy_exception *exception);
void fy_loadPrepareMethod(struct fy_context *context, void *loader_,
		fy_uint methodCount, fy_exception *exception);
void fy_loadMethod(struct fy_context *context, void *loader_, fy_uint methodId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception);
void fy_loadEndMethod(struct fy_context *context, void *loader_,
		fy_exception *exception);
void fy_loadPrepareField(struct fy_context *context, void *loader_,
		fy_uint fieldCount, fy_exception *exception);
void fy_loadField(struct fy_context *context, void *loader_, fy_uint fieldId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception);
void fy_loadEndField(struct fy_context *context, void *loader_,
		fy_exception *exception);
void fy_loadPrepareObjects(struct fy_context *context, void *loader_,
		fy_uint nextHandle, fy_uint objectCount, fy_exception *exception);
void fy_loadObject(struct fy_context *context, void *loader_, fy_uint handle,
		fy_uint classId, fy_int posInHeap, fy_int gen, fy_int finalizeStatus,
		fy_uint monitorOwner, fy_uint monitorCount,
		fy_uint length, fy_uint dataLength, fy_uint *data,
		fy_exception *exception);
void fy_loadEndObject(struct fy_context *context, void *loader_,
		fy_exception *exception);
void fy_loadLiterals(struct fy_context *context, void *loader_, fy_uint count,
		fy_uint *handles, fy_exception *exception);
void fy_loadFinalizes(struct fy_context *context, void *loader_, fy_uint count,
		fy_uint *handles, fy_exception *exception);
void fy_loadPrepareThreads(struct fy_context *context, void *loader_,
		fy_uint threadsCount, fy_exception *exception);
fy_thread *fy_loadThread(struct fy_context *context, void *loader_,
		fy_uint threadId, fy_uint handle, fy_int priority, fy_uint daemon,
		fy_uint destroyPending, fy_uint interrupted, fy_long nextWakeupTime,
		fy_uint pendingLockCount, fy_uint waitForLockId,
		fy_uint waitForNotifyId, fy_uint stackSize, fy_uint *stack,
		fy_uint *typeStack, fy_exception *exception);
void fy_loadPrepareFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fy_uint count, fy_exception *exception);
void fy_loadFrame(struct fy_context *context, void *loader_, fy_thread *thread,
		fy_uint methodId, fy_uint sb, fy_uint sp, fy_uint pc, fy_uint lpc,
		fy_exception *exception);
void fy_loadEndFrame(struct fy_context *context, void *loader_,
		fy_thread *thread, fy_exception *exception);
void fy_loadEndThread(struct fy_context *context, void *loader_,
		fy_exception *exception);
void fy_loadEnd(struct fy_context *context, void *loader_,
		fy_exception *exception);
#ifdef	__cplusplus
}
#endif

#endif /* DATALOADER_H_ */
