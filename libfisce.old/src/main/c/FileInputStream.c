/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fyc/FileInputStream.h"
#include <stdio.h>
#include <errno.h>

typedef struct {
	fy_boolean closed;
	FILE *fp;
} fisData;

static fy_int isRead(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fisData *data = is->data;
	int ret = fgetc(data->fp);
#if EOF != -1
	if (ret == EOF) {
		return -1;
	}
#endif
	return ret;
}

static fy_int isReadBlock(struct fy_context *context, fy_inputStream *is,
		void *target, fy_int size, fy_exception *exception) {
	fisData *data = is->data;
	fy_int read = fread(target, 1, size, data->fp);
	fy_int err;
	if (read == 0) {
		err = ferror(data->fp);
		if (err) {
			fy_fault(exception, FY_EXCEPTION_IO, "Error no: %"FY_PRINT32"d.",
					err);
			FYEH()0;
		} else {
			return -1;
		}
	}
	return read;
}

static fy_int isSkip(struct fy_context *context, fy_inputStream *is,
		fy_int size, fy_exception *exception) {
	fisData *data = is->data;
	if (data->closed) {
		fy_fault(exception, FY_EXCEPTION_IO, "closed.");
		FYEH() 0;
	}
	if (fseek(data->fp, size, SEEK_CUR)) {
		fy_fault(exception, FY_EXCEPTION_IO, "%s", strerror(errno));
		return 0;
	} else {
		return size;
	}
}

static void isClose(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fisData *data = is->data;
	if (fclose(data->fp)) {
		fy_fault(exception, FY_EXCEPTION_IO, "%s", strerror(errno));
	}
}

static fy_inputStream* isOpen(struct fy_context *context, const char *name,
		fy_exception *exception) {
	FILE *fp;
	fy_inputStream *ret = fy_mmAllocate(context->memblocks,
			sizeof(fy_inputStream), exception);
	fy_memblock *block = context->memblocks;
	fy_str localName;
	char targetName[1024];
	fisData *data;
	FYEH()NULL;
	localName.content = NULL;
	fy_strInit(block, &localName, 256, exception);
	if (exception->exceptionType != exception_none) {
		fy_mmFree(context->memblocks, ret);
		return NULL;
	}

	if (name[0] == '/') {
		fy_strAppendUTF8(block, &localName, "runtime", -1, exception);
	} else {
		fy_strAppendUTF8(block, &localName, "runtime/", -1, exception);
	}

	if (exception->exceptionType != exception_none) {
		fy_strDestroy(block, &localName);
		fy_mmFree(context->memblocks, ret);
		return NULL;
	}
	fy_strAppendUTF8(block, &localName, name, -1, exception);
	if (exception->exceptionType != exception_none) {
		fy_strDestroy(block, &localName);
		fy_mmFree(context->memblocks, ret);
		return NULL;
	}
	/*
	 fy_strAppendUTF8(block, &localName, ".class", 6, exception);
	 if (exception->exceptionType != exception_none) {
	 fy_strDestroy(block, &localName);
	 return NULL ;
	 }
	 */
	fy_strSPrint(targetName, sizeof(targetName), &localName);
	fy_strDestroy(block, &localName);

	fp = fopen(targetName, "rb");
	if (fp == NULL) {
		fy_mmFree(context->memblocks, ret);
		return NULL;
	}

	data = fy_mmAllocate(context->memblocks, sizeof(fisData), exception);
	if (exception->exceptionType != exception_none) {
		fy_mmFree(context->memblocks, ret);
		fclose(fp);
		return NULL;
	}
	data->closed = FALSE;
	data->fp = fp;
	ret->data = data;
	ret->isClose = isClose;
	ret->isRead = isRead;
	ret->isReadBlock = isReadBlock;
	ret->isSkip = isSkip;
	return ret;
}

void fy_fisInitInputStream(fy_context *context) {
	context->isOpen = isOpen;
}
