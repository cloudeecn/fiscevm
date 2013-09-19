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

#include "fyc/BinarySaver.h"
#include "fyc/Heap.h"
#include "fyc/DataLoader.h"
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

static void callForSave(struct fy_context *context, fy_exception *exception) {
	fy_vmSave(context, exception);
}
static void* saveBegin(struct fy_context *context, fy_exception *exception) {/*TODO*/
	FILE *fp = fopen("save.dat", "wb");
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
		writeChar(fp, fy_strGet(name,i), exception);
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
		writeChar(fp, fy_strGet(uniqueName,i), exception);
	}
}
static void saveEndMethod(struct fy_context *context, void *saver,
		fy_exception *exception) {
	/*NONEED*/
}
static void savePrepareField(struct fy_context *context, void *saver,
		fy_uint fieldCount, fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE006, exception);
	writeInt(fp, fieldCount, exception);
}
static void saveField(struct fy_context *context, void *saver, fy_uint fieldId,
		fy_uint handle, fy_str *uniqueName, fy_exception *exception) {
	fy_int i, imax;
	FILE *fp = saver;
	writeInt(fp, 0xF15CE007, exception);
	writeInt(fp, fieldId, exception);
	writeInt(fp, handle, exception);
	writeInt(fp, imax = uniqueName->length, exception);
	for (i = 0; i < imax; i++) {
		writeChar(fp, fy_strGet(uniqueName,i), exception);
	}
}
static void saveEndField(struct fy_context *context, void *saver,
		fy_exception *exception) {/*TODO*/
}
static void savePrepareObjects(struct fy_context *context, void *saver,
		fy_uint nextHandle, fy_uint objectCount, fy_exception *exception) {/*TODO*/
	FILE *fp = saver;
	writeInt(fp, 0xF15CE009, exception);
	writeInt(fp, nextHandle, exception);
	writeInt(fp, objectCount, exception);
}
static void saveObject(struct fy_context *context, void *saver, fy_uint handle,
		fy_uint classId, fy_int posInHeap, fy_int gen, fy_int finalizeStatus,
		fy_uint monitorOwner, fy_uint monitorCount, fy_uint length,
		fy_uint dataSize, fy_uint *data, fy_exception *exception) {
	FILE *fp = saver;
	fy_uint i;
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
		fy_exception *exception) {/*NONEED*/
}
static void saveLiterals(struct fy_context *context, void *saver, fy_uint count,
		fy_uint *handles, fy_exception *exception) {
	FILE *fp = saver;
	int i;
	writeInt(fp, 0xF15CE008, exception);
	writeInt(fp, count, exception);
	for (i = 0; i < count; i++) {
		writeInt(fp, handles[i], exception);
	}
}
static void saveFinalizes(struct fy_context *context, void *saver,
		fy_uint count, fy_uint *handles, fy_exception *exception) {
	FILE *fp = saver;
	int i;
	writeInt(fp, 0xF15CE00B, exception);
	writeInt(fp, count, exception);
	for (i = 0; i < count; i++) {
		writeInt(fp, handles[i], exception);
	}
}
static void savePrepareThreads(struct fy_context *context, void *saver,
		fy_uint threadsCount, fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xF15CE00C, exception);
	writeInt(fp, threadsCount, exception);
}
static void saveThread(struct fy_context *context, void *saver,
		fy_uint threadId, fy_uint handle, fy_int priority, fy_uint daemon,
		fy_uint destroyPending, fy_uint interrupted, fy_long nextWakeupTime,
		fy_uint pendingLockCount, fy_uint waitForLockId,
		fy_uint waitForNotifyId, fy_uint stackSize, fy_uint *stack,
		fy_uint *typeStack, fy_exception *exception) {
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
		writeInt(fp, stack[i], exception);
	}
	for (i = 0; i < (stackSize + 31) / 32; i++) {
		writeInt(fp, typeStack[i], exception);
	}
}
static void savePrepareFrame(struct fy_context *context, void *saver,
		fy_uint count, fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, count, exception);
}
static void saveFrame(struct fy_context *context, void *saver, fy_uint methodId,
		fy_uint sb, fy_uint sp, fy_uint pc, fy_uint lpc,
		fy_exception *exception) {
	FILE *fp = saver;
	writeInt(fp, 0xf15ce00e, exception);
	writeInt(fp, methodId, exception);
	writeInt(fp, sb, exception);
	writeInt(fp, sp, exception);
	writeInt(fp, pc, exception);
	writeInt(fp, lpc, exception);
}
static void saveEndFrame(struct fy_context *context, void *saver,
		fy_exception *exception) {
}
static void saveEndThread(struct fy_context *context, void *saver,
		fy_exception *exception) {
}
static void saveEnd(struct fy_context *context, void *saver,
		fy_exception *exception) {
	fclose(saver);
}
static fy_uint readInt(FILE *fp, fy_exception *exception) {
	int i, read;
	fy_uint ret = 0;
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

static fy_char readChar(FILE *fp, fy_exception *exception) {
	int i, read;
	fy_char ret = 0;
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
static void readIntBlock(fy_context *context, FILE *fp, fy_int *bufsize,
		fy_int **buf, fy_int size, fy_exception *exception) {
	int i;
	fy_uint read;
	if (*bufsize == 0) {
		*bufsize = 1024;
		*buf = fy_mmAllocate(context->memblocks, *bufsize * sizeof(fy_int),
				exception);
		FYEH();
	}
	if (*bufsize < size) {
		while (*bufsize < size) {
			*bufsize *= 2;
		}
		fy_mmFree(context->memblocks, buf);
		fy_mmAllocate(context->memblocks, *bufsize * sizeof(fy_int), exception);
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

static void readUIntBlock(fy_context *context, FILE *fp, fy_int *bufsize,
		fy_uint **buf, fy_int size, fy_exception *exception) {
	int i;
	fy_uint read;
	if (*bufsize == 0) {
		*bufsize = 1024;
		*buf = fy_mmAllocate(context->memblocks, *bufsize * sizeof(fy_int),
				exception);
		FYEH();
	}
	if (*bufsize < size) {
		while (*bufsize < size) {
			*bufsize *= 2;
		}
		fy_mmFree(context->memblocks, buf);
		fy_mmAllocate(context->memblocks, *bufsize * sizeof(fy_int), exception);
		FYEH();
	}
	for (i = 0; i < size; i++) {
		read = readInt(fp, exception);
		FYEH();
		(*buf)[i] = read;
		FYEH();
	}
}

static void readString(fy_context *context, FILE *fp, fy_str *str, fy_int size,
		fy_exception *exception) {
	int i;
	fy_char read;
	fy_strEnsureSize(context->memblocks, str, size + str->length, exception);
	for (i = 0; i < size; i++) {
		read = readChar(fp, exception);
		FYEH();
		fy_strAppendChar(context->memblocks, str, read, exception);
		FYEH();
	}
}

#define FY_ASSERTF(VALUE) if(readInt(fp,exception)!=VALUE){FYEH();fy_fault(exception,FY_EXCEPTION_IO,"Data mismatch");return;}

static void loadData(struct fy_context *context, fy_exception *exception) {
	FILE *fp = fopen("save.dat", "rb");
	void *loader;
	fy_uint i, j;
	fy_uint classCount;
	fy_str strbuf[1];
	fy_int uintbufSize = 0;
	fy_uint *uintbuf;
	fy_int uintbufSize2 = 0;
	fy_uint *uintbuf2;

	fy_uint nameSize;
	fy_uint id;
	fy_uint handle;

	fy_uint clinited;
	fy_uint staticSize;
	/*
	 fy_uint *staticArea;
	 */
	fy_uint methodCount;
	fy_uint fieldCount;

	fy_uint nextHandle;
	fy_uint objectCount;

	fy_uint classId;
	fy_int posInHeap;
	fy_int gen;
	fy_int finalizeStatus;
	fy_int monitorOwner;
	fy_int monitorCount;
	fy_int len;
	fy_int dataSize;

	fy_int literalCount;

	fy_int finalizeCount;

	fy_int threadCount;

	fy_int daemon;
	fy_int priority;
	fy_int destroyPending;
	fy_int interrupted;
	fy_int nextWakeUpTimeL;
	fy_int nextWakeUpTimeH;
	fy_int pendingLockCount;
	fy_int waitForLockId;
	fy_int waitForNotifyId;
	fy_int stackSize;

	fy_int frameCount;
	fy_int methodId;
	fy_int sb;
	fy_int sp;
	fy_int pc;
	fy_int lpc;

	fy_thread *thread;

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
		readUIntBlock(context, fp, &uintbufSize2, &uintbuf2,
				(stackSize + 31) / 32, exception);
		FYEH();
		thread = fy_loadThread(context, loader, id, handle, priority, daemon,
				destroyPending, interrupted,
				fy_I2TOL(nextWakeUpTimeH,nextWakeUpTimeL), pendingLockCount,
				waitForLockId, waitForNotifyId, stackSize, uintbuf, uintbuf2,
				exception);
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
			sp = readInt(fp, exception);
			FYEH();
			pc = readInt(fp, exception);
			FYEH();
			lpc = readInt(fp, exception);
			FYEH();
			fy_loadFrame(context, loader, thread, methodId, sb, sp, pc, lpc,
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
	context->loadData = loadData;
}
