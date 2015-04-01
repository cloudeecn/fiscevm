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

#include "fyc/BinarySaver.h"
#include "fyc/Heap.h"
#include "fyc/DataLoader.h"
#include "fyc/Constants.h"
#include <stdio.h>

#if 0
Binary save data format, all data in little endian
int 11BF15CE
int F15CE002
int classCount
{
	int F15CE003
	int classid
	int handle
	int clinited
	int nameLength
	char[] name
	int staticSize
	int[] staticArea
}
int F15CE004
int methodCount
{
	int F15CE005
	int methodId
	int handle
	int nameLength
	char[] name
}
int F15CE006
int fieldCount
{
	int F15CE007
	int fieldId
	int handle
	int nameLength
	char[] name
}
int F15CE009
int nextHandle
int objectCount
{
	int F15CE00A
	int handle
	int classId
	int posInHeap
	int gen
	int finalizeStatus
	int monitorOwner
	int mouitorCount
	int len
	int dataSize
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
	int priority
	int daemon
	int destroyPending
	int interrupted
	int nextWakeUpTimeLow
	int nextWakeUpTimeHigh
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

static void writeInt(FILE *fp, fisce_uint value, fisce_exception *exception) {
	int i;
	for (i = 4; i > 0; i--) {
		if (fputc(value, fp) == EOF) {
			fy_fault(exception, FY_EXCEPTION_IO, "Can't write to save file");
			return;
		}
		value >>= 8;
	}
}

static void writeChar(FILE *fp, fisce_char value, fisce_exception *exception) {
	int i;
	for (i = 2; i > 0; i--) {
		if (fputc(value, fp) == EOF) {
			fy_fault(exception, FY_EXCEPTION_IO, "Can't write to save file");
			return;
		}
		value >>= 8;
	}
}

static void* saveBegin(struct fy_context *context, fisce_exception *exception) {/*TODO*/
	FILE *fp = fopen(context->saveloadParam, "wb");
	if (fp == NULL) {
		fy_fault(exception, FY_EXCEPTION_IO, "Can't open %s for save.",
				context->saveloadParam);
		return NULL;
	}
	writeInt(fp, 0x11BF15CE, exception);
	FYEH()NULL;
	return fp;
}
static void savePrepareClass(struct fy_context *context, void *saver,
		fisce_uint classCount, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE002, exception);
	writeInt(fp, classCount, exception);

}
static void saveClass(struct fy_context *context, void *saver, fisce_uint classId,
		fisce_uint handle, fisce_int clinited, fy_str *name, fisce_uint staticSize,
		fisce_int *staticArea, fisce_exception *exception) {
	FILE *fp = saver;
	fisce_int i, imax;
	writeInt(fp, 0xF15CE003, exception);
	writeInt(fp, classId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, clinited, exception);
	writeInt(fp, imax = name->length, exception);
	for (i = 0; i < imax; i++) {
		writeChar(fp, fy_strGet(name, i), exception);
	}
	writeInt(fp, imax = staticSize, exception);
	for (i = 0; i < imax; i++) {
		writeInt(fp, staticArea[i], exception);
	}
}
static void saveEndClass(struct fy_context *context, void *saver,
		fisce_exception *exception) {
	/*NONEED*/
}
static void savePrepareMethod(struct fy_context *context, void *saver,
		fisce_uint methodCount, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE004, exception);
	writeInt(fp, methodCount, exception);
}
static void saveMethod(struct fy_context *context, void *saver,
		fisce_uint methodId, fisce_uint handle, fy_str *uniqueName,
		fisce_exception *exception) {
	fisce_int i, imax;
	FILE *fp = saver;
	writeInt(fp, 0xF15CE005, exception);
	writeInt(fp, methodId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, imax = uniqueName->length, exception);
	for (i = 0; i < imax; i++) {
		writeChar(fp, fy_strGet(uniqueName, i), exception);
	}
}
static void saveEndMethod(struct fy_context *context, void *saver,
		fisce_exception *exception) {
	/*NONEED*/
}
static void savePrepareField(struct fy_context *context, void *saver,
		fisce_uint fieldCount, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE006, exception);
	writeInt(fp, fieldCount, exception);
}
static void saveField(struct fy_context *context, void *saver, fisce_uint fieldId,
		fisce_uint handle, fy_str *uniqueName, fisce_exception *exception) {
	fisce_int i, imax;
	FILE *fp = saver;
	writeInt(fp, 0xF15CE007, exception);
	writeInt(fp, fieldId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, imax = uniqueName->length, exception);
	for (i = 0; i < imax; i++) {
		writeChar(fp, fy_strGet(uniqueName, i), exception);
	}
}
static void saveEndField(struct fy_context *context, void *saver,
		fisce_exception *exception) {/*TODO*/
}
static void savePrepareObjects(struct fy_context *context, void *saver,
		fisce_uint nextHandle, fisce_uint objectCount, fisce_exception *exception) {/*TODO*/
	FILE *fp = saver;
	writeInt(fp, 0xF15CE009, exception);
	writeInt(fp, nextHandle, exception);
	writeInt(fp, objectCount, exception);
}
static void saveObject(struct fy_context *context, void *saver, fisce_uint handle,
		fisce_uint classId, fisce_int posInHeap, fisce_int gen, fisce_int finalizeStatus,
		fisce_uint monitorOwner, fisce_uint monitorCount, fisce_uint length,
		fisce_uint dataSize, fisce_uint *data, fisce_exception *exception) {
	FILE *fp = saver;
	fisce_uint i;
	writeInt(fp, 0xF15CE00A, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, classId, exception);
	writeInt(fp, posInHeap, exception);
	writeInt(fp, gen, exception);
	writeInt(fp, finalizeStatus, exception);
	writeInt(fp, monitorOwner, exception);
	writeInt(fp, monitorCount, exception);
	writeInt(fp, length, exception);
	writeInt(fp, dataSize, exception);
	for (i = 0; i < dataSize; i++) {
		writeInt(fp, data[i], exception);
	}

}
static void saveEndObject(struct fy_context *context, void *saver,
		fisce_exception *exception) {/*NONEED*/
}
static void saveLiterals(struct fy_context *context, void *saver, fisce_uint count,
		fisce_uint *handles, fisce_exception *exception) {
	FILE *fp = saver;
	int i;
	writeInt(fp, 0xF15CE008, exception);
	writeInt(fp, count, exception);
	for (i = 0; i < count; i++) {
		writeInt(fp, handles[i], exception);
	}
}
static void saveFinalizes(struct fy_context *context, void *saver,
		fisce_uint count, fisce_uint *handles, fisce_exception *exception) {
	FILE *fp = saver;
	int i;
	writeInt(fp, 0xF15CE00B, exception);
	writeInt(fp, count, exception);
	for (i = 0; i < count; i++) {
		writeInt(fp, handles[i], exception);
	}
}
static void savePrepareThreads(struct fy_context *context, void *saver,
		fisce_uint threadsCount, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE00C, exception);
	writeInt(fp, threadsCount, exception);
}
static void saveThread(struct fy_context *context, void *saver,
		fisce_uint threadId, fisce_uint handle, fisce_int priority, fisce_uint daemon,
		fisce_uint destroyPending, fisce_uint interrupted, fisce_long nextWakeupTime,
		fisce_uint pendingLockCount, fisce_uint waitForLockId,
		fisce_uint waitForNotifyId, fisce_uint stackSize, fisce_stack_item *stack,
		fisce_exception *exception) {
	FILE *fp = saver;
	int i;
	writeInt(fp, 0xF15CE00D, exception);
	writeInt(fp, threadId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, priority, exception);
	writeInt(fp, daemon, exception);
	writeInt(fp, destroyPending, exception);
	writeInt(fp, interrupted, exception);
	writeInt(fp, fy_LOFL(nextWakeupTime), exception);
	writeInt(fp, fy_HOFL(nextWakeupTime), exception);
	writeInt(fp, pendingLockCount, exception);
	writeInt(fp, waitForLockId, exception);
	writeInt(fp, waitForNotifyId, exception);
	writeInt(fp, stackSize, exception);
	for (i = 0; i < stackSize; i++) {
		writeInt(fp, stack[i].ivalue, exception);
	}
}
static void savePrepareFrame(struct fy_context *context, void *saver,
		fisce_uint count, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, count, exception);
}
static void saveFrame(struct fy_context *context, void *saver, fisce_uint methodId,
		fisce_uint sb, fisce_uint lpc, fisce_int pcofs, fisce_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xf15ce00e, exception);
	writeInt(fp, methodId, exception);
	writeInt(fp, sb, exception);
	writeInt(fp, lpc, exception);
	writeInt(fp, pcofs, exception);
}
static void saveEndFrame(struct fy_context *context, void *saver,
		fisce_exception *exception) {
}
static void saveEndThread(struct fy_context *context, void *saver,
		fisce_exception *exception) {
}
static void saveEnd(struct fy_context *context, void *saver,
		fisce_exception *exception) {
	fclose(saver);
}
static fisce_uint readInt(FILE *fp, fisce_exception *exception) {
	int i, read;
	fisce_uint ret = 0;
	for (i = 0; i < 4; i++) {
		read = fgetc(fp);
		if (read == EOF) {
			if (ferror(fp)) {
				fy_fault(exception, FY_EXCEPTION_IO,
						"IO exception when reading %d", ferror(fp));
			} else if (feof(fp)) {
				fy_fault(exception, FY_EXCEPTION_IO, "File end when reading");
			} else {
				fy_fault(exception, FY_EXCEPTION_IO,
						"Unknown error when reading");
			}
		}
		ret += read << (i << 3);/*read<<(1*8)*/
	}
	return ret;
}

static fisce_char readChar(FILE *fp, fisce_exception *exception) {
	int i, read;
	fisce_char ret = 0;
	for (i = 0; i < 2; i++) {
		read = fgetc(fp);
		if (read == EOF) {
			fy_fault(exception, FY_EXCEPTION_IO, "IO exception when reading");
		}
		ret += read << (i << 3);/*read<<(1*8)*/
	}
	return ret;
}

#if 0
static void readIntBlock(fy_context *context, FILE *fp, fisce_int *bufsize,
		fisce_int **buf, fisce_int size, fisce_exception *exception) {
	int i;
	fisce_uint read;
	if (*bufsize == 0) {
		*bufsize = 1024;
		*buf = fy_mmAllocate(context->memblocks, *bufsize * sizeof(fisce_int),
				exception);
		FYEH();
	}
	if (*bufsize < size) {
		while (*bufsize < size) {
			*bufsize *= 2;
		}
		fy_mmFree(context->memblocks, buf);
		fy_mmAllocate(context->memblocks, *bufsize * sizeof(fisce_int), exception);
		FYEH();
	}
	for (i = 0; i < size; i++) {
		read = readInt(fp, exception);
		FYEH();
		(*buf)[i] = read;
		FYEH();
	}
}
#endif

static void readUIntBlock(fy_context *context, FILE *fp, fisce_int *bufsize,
		fisce_uint **buf, fisce_int size, fisce_exception *exception) {
	int i;
	fisce_uint read;
	if (*bufsize == 0) {
		*bufsize = 1024;
		*buf = fy_mmAllocate(context->memblocks, *bufsize * sizeof(fisce_int),
				exception);
		FYEH();
	}
	if (*bufsize < size) {
		while (*bufsize < size) {
			*bufsize *= 2;
		}
		fy_mmFree(context->memblocks, buf);
		fy_mmAllocate(context->memblocks, *bufsize * sizeof(fisce_int), exception);
		FYEH();
	}
	for (i = 0; i < size; i++) {
		read = readInt(fp, exception);
		FYEH();
		(*buf)[i] = read;
		FYEH();
	}
}

static void readString(fy_context *context, FILE *fp, fy_str *str, fisce_int size,
		fisce_exception *exception) {
	int i;
	fisce_char read;
	fy_strEnsureSize(context->memblocks, str, size + str->length, exception);
	for (i = 0; i < size; i++) {
		read = readChar(fp, exception);
		FYEH();
		fy_strAppendChar(context->memblocks, str, read, exception);
		FYEH();
	}
}

#define FY_ASSERTF(VALUE) if(readInt(fp,exception)!=VALUE){FYEH();fy_fault(exception,FY_EXCEPTION_IO,"Data mismatch");return;}

static void loadData(struct fy_context *context, fisce_exception *exception) {
	FILE *fp = fopen(context->saveloadParam, "rb");
	void *loader;
	fisce_uint i, j;
	fisce_uint classCount;
	fy_str strbuf[1];
	fisce_int uintbufSize = 0;
	fisce_uint *uintbuf;
	fisce_uint *uintbuf2 = NULL;

	fisce_uint nameSize;
	fisce_uint id;
	fisce_uint handle;

	fisce_uint clinited;
	fisce_uint staticSize;
	/*
	 fisce_uint *staticArea;
	 */
	fisce_uint methodCount;
	fisce_uint fieldCount;

	fisce_uint nextHandle;
	fisce_uint objectCount;

	fisce_uint classId;
	fisce_int posInHeap;
	fisce_int gen;
	fisce_int finalizeStatus;
	fisce_int monitorOwner;
	fisce_int monitorCount;
	fisce_int len;
	fisce_int dataSize;

	fisce_int literalCount;

	fisce_int finalizeCount;

	fisce_int threadCount;

	fisce_int daemon;
	fisce_int priority;
	fisce_int destroyPending;
	fisce_int interrupted;
	fisce_int nextWakeUpTimeL;
	fisce_int nextWakeUpTimeH;
	fisce_int pendingLockCount;
	fisce_int waitForLockId;
	fisce_int waitForNotifyId;
	fisce_int stackSize;

	fisce_int frameCount;
	fisce_int methodId;
	fisce_int sb;
	fisce_int lpc;
	fisce_int pcofs;

	fy_thread *thread;

	if (fp == NULL) {
		fy_fault(exception, FY_EXCEPTION_IO, "Can't load %s for load.",
				context->saveloadParam);
	}

	if (fp == NULL) {
		fy_fault(exception, FY_EXCEPTION_IO, "Can't open save.dat for read.");
		return;
	}
	FY_ASSERTF(0x11BF15CE);
	loader = fy_loadBegin(context, exception);
	FY_ASSERTF(0xF15CE002);
	classCount = readInt(fp, exception);
	FYEH();
	fy_loadPrepareClass(context, loader, classCount, exception);
	strbuf->content = NULL;
	fy_strInit(context->memblocks, strbuf, 1024, exception);
	for (i = 1; i <= classCount; i++) {
		FY_ASSERTF(0xF15CE003);
		id = readInt(fp, exception);
		FYEH();
		handle = readInt(fp, exception);
		FYEH();
		clinited = readInt(fp, exception);
		FYEH();
		nameSize = readInt(fp, exception);
		FYEH();
		fy_strClear(strbuf);
		strbuf->length = 0;
		readString(context, fp, strbuf, nameSize, exception);
		FYEH();
		staticSize = readInt(fp, exception);
		readUIntBlock(context, fp, &uintbufSize, &uintbuf, staticSize,
				exception);
		fy_loadClass(context, loader, id, handle, clinited, strbuf, staticSize,
				uintbuf, exception);
	}
	fy_loadEndClass(context, loader, exception);

	FY_ASSERTF(0xF15CE004);
	methodCount = readInt(fp, exception);
	FYEH();
	fy_loadPrepareMethod(context, loader, methodCount, exception);
	for (i = 0; i < methodCount; i++) {
		FY_ASSERTF(0xF15CE005);
		id = readInt(fp, exception);
		FYEH();
		handle = readInt(fp, exception);
		FYEH();
		nameSize = readInt(fp, exception);
		FYEH();
		fy_strClear(strbuf);
		readString(context, fp, strbuf, nameSize, exception);
		FYEH();
		fy_loadMethod(context, loader, id, handle, strbuf, exception);
	}
	fy_loadEndMethod(context, loader, exception);

	FY_ASSERTF(0xF15CE006);
	fieldCount = readInt(fp, exception);
	FYEH();
	fy_loadPrepareField(context, loader, fieldCount, exception);
	for (i = 0; i < fieldCount; i++) {
		FY_ASSERTF(0xF15CE007);
		id = readInt(fp, exception);
		FYEH();
		handle = readInt(fp, exception);
		FYEH();
		nameSize = readInt(fp, exception);
		FYEH();
		fy_strClear(strbuf);
		readString(context, fp, strbuf, nameSize, exception);
		FYEH();
		fy_loadField(context, loader, id, handle, strbuf, exception);
	}
	fy_loadEndField(context, loader, exception);

	FY_ASSERTF(0xF15CE009);
	nextHandle = readInt(fp, exception);
	FYEH();
	objectCount = readInt(fp, exception);
	FYEH();
	fy_loadPrepareObjects(context, loader, nextHandle, objectCount, exception);
	FYEH();
	for (i = 0; i < objectCount; i++) {
		FY_ASSERTF(0xF15CE00A);
		handle = readInt(fp, exception);
		FYEH();
		classId = readInt(fp, exception);
		FYEH();
		posInHeap = readInt(fp, exception);
		FYEH();
		gen = readInt(fp, exception);
		FYEH();
		finalizeStatus = readInt(fp, exception);
		FYEH();
		monitorOwner = readInt(fp, exception);
		FYEH();
		monitorCount = readInt(fp, exception);
		FYEH();
		len = readInt(fp, exception);
		FYEH();
		dataSize = readInt(fp, exception);
		FYEH();
		readUIntBlock(context, fp, &uintbufSize, &uintbuf, dataSize, exception);
		FYEH();
		fy_loadObject(context, loader, handle, classId, posInHeap, gen,
				finalizeStatus, monitorOwner, monitorCount, len, dataSize,
				uintbuf, exception);
		FYEH();
	}
	fy_loadEndObject(context, loader, exception);

	FY_ASSERTF(0xF15CE008);
	literalCount = readInt(fp, exception);
	readUIntBlock(context, fp, &uintbufSize, &uintbuf, literalCount, exception);
	FYEH();
	fy_loadLiterals(context, loader, literalCount, uintbuf, exception);
	FYEH();

	FY_ASSERTF(0xF15CE00B);
	finalizeCount = readInt(fp, exception);
	readUIntBlock(context, fp, &uintbufSize, &uintbuf, finalizeCount,
			exception);
	FYEH();
	fy_loadFinalizes(context, loader, finalizeCount, uintbuf, exception);
	FYEH();

	FY_ASSERTF(0xF15CE00C);
	threadCount = readInt(fp, exception);
	FYEH();
	fy_loadPrepareThreads(context, loader, threadCount, exception);
	for (i = 0; i < threadCount; i++) {
		FY_ASSERTF(0xF15CE00D);
		id = readInt(fp, exception);
		FYEH();
		handle = readInt(fp, exception);
		FYEH();
		priority = readInt(fp, exception);
		FYEH();
		daemon = readInt(fp, exception);
		FYEH();
		destroyPending = readInt(fp, exception);
		FYEH();
		interrupted = readInt(fp, exception);
		FYEH();
		nextWakeUpTimeL = readInt(fp, exception);
		FYEH();
		nextWakeUpTimeH = readInt(fp, exception);
		FYEH();
		pendingLockCount = readInt(fp, exception);
		FYEH();
		waitForLockId = readInt(fp, exception);
		FYEH();
		waitForNotifyId = readInt(fp, exception);
		FYEH();
		stackSize = readInt(fp, exception);
		FYEH();
		readUIntBlock(context, fp, &uintbufSize, &uintbuf, stackSize,
				exception);
		FYEH();
		thread = fy_loadThread(context, loader, id, handle, priority, daemon,
				destroyPending, interrupted,
				fy_I2TOL(nextWakeUpTimeH, nextWakeUpTimeL), pendingLockCount,
				waitForLockId, waitForNotifyId, stackSize, uintbuf, exception);
		FYEH();
		frameCount = readInt(fp, exception);
		FYEH();
		fy_loadPrepareFrame(context, loader, thread, frameCount, exception);
		FYEH();
		for (j = 0; j < frameCount; j++) {
			FY_ASSERTF(0xF15CE00E);
			methodId = readInt(fp, exception);
			FYEH();
			sb = readInt(fp, exception);
			FYEH();
			lpc = readInt(fp, exception);
			FYEH();
			pcofs = readInt(fp, exception);
			FYEH();
			fy_loadFrame(context, loader, thread, methodId, sb, lpc, pcofs,
					exception);
			FYEH();
		}
		fy_loadEndFrame(context, loader, thread, exception);
		FYEH();
	}
	fy_loadEndThread(context, loader, exception);
	FYEH();
	fy_loadEnd(context, loader, exception);
	FYEH();

	fy_strDestroy(context->memblocks, strbuf);
	if (uintbuf != NULL) {
		fy_mmFree(context->memblocks, uintbuf);
	}
	if (uintbuf2 != NULL) {
		fy_mmFree(context->memblocks, uintbuf2);
	}

	fclose(fp);
}
void fy_bsRegisterBinarySaver(fy_context *context) {
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
	context->loadData = loadData;
}
