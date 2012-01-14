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
#include "fyc/BinarySaver.h"
#include <stdio.h>

static void callForSave(struct fy_context *context, fy_exception *exception) {/*TODO*/
}
static void* saveBegin(struct fy_context *context, fy_exception *exception) {/*TODO*/
	return NULL;
}
static void savePrepareClass(struct fy_context *context, void *saver,
		fy_uint classCount, fy_exception *exception) {/*TODO*/
}
static void saveClass(struct fy_context *context, void *saver, fy_uint classId,
		fy_uint handle, fy_int clinited, fy_str *name, fy_uint staticSize,
		fy_int *staticArea, fy_exception *exception) {/*TODO*/
}
static void saveEndClass(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void savePrepareMethod(struct fy_context *context, void *saver,
		fy_uint methodCount, fy_exception *exception) {/*TODO*/
}
static void saveMethod(struct fy_context *context, void *saver,
		fy_uint methodId, fy_uint handle, fy_str *uniqueName,
		fy_exception *exception) {/*TODO*/
}
static void saveEndMethod(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void savePrepareField(struct fy_context *context, void *saver,
		fy_uint fieldCount, fy_exception *exception) {/*TODO*/
}
static void saveField(struct fy_context *context, void *saver, fy_uint fieldId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception) {/*TODO*/
}
static void saveEndField(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void savePrepareObjects(struct fy_context *context, void *saver,
		fy_uint nextHandle, fy_uint objectCount, fy_exception *exception) {/*TODO*/
}
static void saveObject(struct fy_context *context, void *saver, fy_uint handle,
		fy_uint classId, fy_int posInHeap, fy_int gen, fy_int finalizeStatus,
		fy_uint monitorOwner, fy_uint monitorCount, fy_uint attachedId,
		fy_uint length, fy_uint *data, fy_exception *exception) {/*TODO*/
}
static void saveEndObject(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void saveLiterals(struct fy_context *context, void *saver, fy_uint count,
		fy_uint *handles, fy_exception *exception) {/*TODO*/
}
static void saveFinalizes(struct fy_context *context, void *saver,
		fy_uint count, fy_uint *handles, fy_exception *exception) {/*TODO*/
}
static void savePrepareThreads(struct fy_context *context, void *saver,
		fy_uint threadsCount, fy_exception *exception) {/*TODO*/
}
static void saveThread(struct fy_context *context, void *saver,
		fy_uint threadId, fy_uint daemon, fy_uint destroyPending,
		fy_uint interrupted, fy_long nextWakeupTime, fy_uint pendingLockCount,
		fy_uint waitForLockId, fy_uint waitForNotifyId, fy_uint stackSize,
		fy_uint *stack, fy_uint *typeStack, fy_uint frameCount,
		fy_exception *exception) {/*TODO*/
}
static void savePrepareFrame(struct fy_context *context, void *saver,
		fy_uint count, fy_exception *exception) {/*TODO*/
}
static void saveFrame(struct fy_context *context, void *saver, fy_uint methodId,
		fy_uint sb, fy_uint sp, fy_uint pc, fy_uint lpc,
		fy_exception *exception) {/*TODO*/
}
static void saveEndFrame(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void saveEndThread(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void saveEnd(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}

void registerBinarySaver(fy_context *context) {
	context->callForSave = callForSave;
	context->saveBegin = saveBegin;
	context->savePrepareClass = savePrepareClass;
	context->saveClass = saveClass;
	context->saveEndClass = saveEndClass;
	context->savePrepareMethod = savePrepareMethod;
	context->saveMethod = saveMethod;
	context->saveEndMethod = saveEndMethod;
	context->savePrepareField = savePrepareField;
	context->saveField = saveField;
	context->saveEndField = saveEndField;
	context->savePrepareObjects = savePrepareObjects;
	context->saveObject = saveObject;
	context->saveEndObject = saveEndObject;
	context->saveLiterals = saveLiterals;
	context->saveFinalizes = saveFinalizes;
	context->savePrepareThreads = savePrepareThreads;
	context->saveThread = saveThread;
	context->savePrepareFrame = savePrepareFrame;
	context->saveFrame = saveFrame;
	context->saveEndFrame = saveEndFrame;
	context->saveEndThread = saveEndThread;
	context->saveEnd = saveEnd;
}
