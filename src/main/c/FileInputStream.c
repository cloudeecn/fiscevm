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
#include "fyc/FileInputStream.h"
#include <stdio.h>

static fy_int isRead(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	int ret = fgetc(is->data);
#if EOF != -1
	if (ret == EOF) {
		return -1;
	}
#endif
	return ret;
}

static fy_int isReadBlock(struct fy_context *context, fy_inputStream *is,
		void *target, fy_int size, fy_exception *exception) {
	return fread(target, 1, size, is->data);
}

static fy_int isSkip(struct fy_context *context, fy_inputStream *is,
		fy_int size, fy_exception *exception) {
	if (fseek(is->data, size, SEEK_CUR)) {
		return 0;
	} else {
		return size;
	}
}

static void isClose(struct fy_context *context, fy_inputStream *is) {
	fclose(is->data);
}

static void* isOpen(struct fy_context *context, const char *name,
		fy_exception *exception) {
	FILE *fp;
	fy_inputStream *ret = fy_mmAllocate(context->memblocks,
			sizeof(fy_inputStream), exception);
	fy_memblock *block = context->memblocks;
	fy_str localName;
	char targetName[1024];
	FYEH()NULL;
	localName.content = NULL;
	fy_strInit(block, &localName, 256, exception);
	if (exception->exceptionType != exception_none) {
		return NULL ;
	}
	fy_strAppendUTF8(block, &localName, "runtime/", -1, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(block, &localName);
		return NULL ;
	}
	fy_strAppendUTF8(block, &localName, name, -1, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(block, &localName);
		return NULL ;
	}
	fy_strAppendUTF8(block, &localName, ".class", 6, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(block, &localName);
		return NULL ;
	}
	fy_strSPrint(targetName, sizeof(targetName), &localName);

	fp = fopen(targetName, "rb");
	fy_strDestroy(block, &localName);
	if (fp == NULL ) {
		fy_mmFree(context->memblocks, ret);
		return NULL ;
	}
	ret->data = fp;
	ret->isClose = isClose;
	ret->isRead = isRead;
	ret->isReadBlock = isReadBlock;
	ret->isSkip = isSkip;
	return ret;
}

void fy_fisInitInputStream(fy_context *context) {
	context->isOpen = isOpen;
}
