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

#if 0
Binary save data format, all data in little endian
int F15CE001
int F15CE002
int classCount
{
	int F15CE003
	int classid
	int handle
	int clinited
	int nameLength
	int[] name
	int staticSize
	int[] staticArea
}
int F15CE004
int nextMethodId
int methodCount
{
	int F15CE005
	int methodId
	int handle
	int nameLength
	int[] name
}
int F15CE006
int nextFieldId
int fieldCount
{
	int F15CE007
	int fieldId
	int handle
	int nameLength
	int[] name
}
int F15CE009
int nextHandle
int objectCount
{
	int F15CE00A
	int handle
	int classId
	int type
	int posInHeap
	int gen
	int finalizeStatus
	int monitorOwner
	int mouitorCount
	int attachedId
	int dataSize(obj)/Length(arr)
	int[] data
}
int F15CE008
int literalCount
int[] literalHandles
int F15CE00B
int finalizeCount
int[] handles
int F15CE00C
int threadCount
{
	int F15CE00D
	int threadId
	int daemon
	int destroyPending
	int interrupted
	int nextWakeUpTime
	int pendingLockCount
	int waitForLockId
	int waitForNotifyId
	int stackSize
	int[] stack
	int[] typeStack
	int frameCount
	{
		int F15CE00E
		int methodId
		int sb
		int sp
		int pc
		int lpc
	}
}
#endif

static void writeInt(FILE *fp, fy_uint value, fy_exception *exception) {
	int i;
	for (i = 4; i > 0; i--) {
		if (fputc(value, fp) == EOF) {
			fy_fault(exception, FY_EXCEPTION_IO, "Can't write to save.dat");
			return;
		}
		value >>= 8;
	}
}

static void writeChar(FILE *fp, fy_char value, fy_exception *exception) {
	int i;
	for (i = 2; i > 0; i--) {
		if (fputc(value, fp) == EOF) {
			fy_fault(exception, FY_EXCEPTION_IO, "Can't write to save.dat");
			return;
		}
		value >>= 8;
	}
}

static void callForSave(struct fy_context *context, fy_exception *exception) {/*TODO*/
}
static void* saveBegin(struct fy_context *context, fy_exception *exception) {/*TODO*/
	FILE *fp = fopen("save.dat", "w");
	if (fp == NULL) {
		fy_fault(exception, FY_EXCEPTION_IO, "Can't open save.dat for save.");
		return NULL;
	}
	writeInt(fp, 0x11BF15CE, exception);
	FYEH()NULL;
	return fp;
}
static void savePrepareClass(struct fy_context *context, void *saver,
		fy_uint classCount, fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE002, exception);
	writeInt(fp, classCount, exception);

}
static void saveClass(struct fy_context *context, void *saver, fy_uint classId,
		fy_uint handle, fy_int clinited, fy_str *name, fy_uint staticSize,
		fy_int *staticArea, fy_exception *exception) {
	FILE *fp = saver;
	fy_int i, imax;
	writeInt(fp, 0xF15CE003, exception);
	writeInt(fp, classId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, clinited, exception);
	writeInt(fp, imax = name->length, exception);
	for (i = 0; i < imax; i++) {
		writeChar(fp, name->content[i], exception);
	}
	writeInt(fp, imax = staticSize, exception);
	for (i = 0; i < imax; i++) {
		writeInt(fp, staticArea[i], exception);
	}
}
static void saveEndClass(struct fy_context *context, void *saver,
		fy_exception *exception) {
	/*NONEED*/
}
static void savePrepareMethod(struct fy_context *context, void *saver,
		fy_uint methodCount, fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE004, exception);
	writeInt(fp, methodCount, exception);
}
static void saveMethod(struct fy_context *context, void *saver,
		fy_uint methodId, fy_uint handle, fy_str *uniqueName,
		fy_exception *exception) {
	fy_int i, imax;
	FILE *fp = saver;
	writeInt(fp, 0xF15CE005, exception);
	writeInt(fp, methodId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, imax = uniqueName->length, exception);
	for (i = 0; i < imax; i++) {
		writeInt(fp, uniqueName->content[i], exception);
	}
}
static void saveEndMethod(struct fy_context *context, void *saver,
		fy_exception *exception) {
	/*NONEED*/
}
static void savePrepareField(struct fy_context *context, void *saver,
		fy_uint fieldCount, fy_exception *exception) {/*TODO*/
}
static void saveField(struct fy_context *context, void *saver, fy_uint fieldId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception) {
	fy_int i, imax;
	FILE *fp = saver;
	writeInt(fp, 0xF15CE006, exception);
	writeInt(fp, fieldId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, imax = uniqueName->length, exception);
	for (i = 0; i < imax; i++) {
		writeInt(fp, uniqueName->content[i], exception);
	}
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
