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

#include "fyc/BAIS.h"
#include "fyc/InputStream.h"
typedef struct baisData {
	fisce_byte *buf;
	fisce_int pos;
	fisce_int len;
} baisData;

static fisce_int isRead(struct fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	baisData *bd = is->data;
	if (bd->pos == bd->len) {
		return -1;
	}
	return bd->buf[bd->pos++] & 0xff;
}

static fisce_int isReadBlock(struct fy_context *context, fy_inputStream *is,
		void *target, fisce_int size, fisce_exception *exception) {
	baisData *bd = is->data;
	if (bd->pos == bd->len) {
		return -1;
	}
	if (bd->len - bd->pos < size) {
		size = bd->len - bd->pos;
	}
	memcpy(target, bd->buf + bd->pos, size);
	bd->pos += size;
	return size;
}

static fisce_int isSkip(struct fy_context *context, fy_inputStream *is,
		fisce_int size, fisce_exception *exception) {
	baisData *bd = is->data;
	if (bd->len - bd->pos < size) {
		return 0;
	} else {
		bd->pos += size;
		return size;
	}
}

static void isClose(struct fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	fy_mmFree(context->memblocks, is->data);
}

FY_ATTR_EXPORT fy_inputStream *fy_baisOpenByteArrayInputStream(
		fy_context *context, void *buffer, fisce_int bufferLen,
		fisce_exception *exception) {
	baisData *bd;
	fy_inputStream *is;
	if (buffer == NULL) {
		return NULL;
	}
	bd = fy_mmAllocate(context->memblocks, sizeof(baisData), exception);
	FYEH()NULL;
	bd->buf = buffer;
	bd->len = bufferLen;
	bd->pos = 0;
	is = fy_mmAllocate(context->memblocks, sizeof(fy_inputStream), exception);
	if (exception->exceptionType != exception_none) {
		fy_mmFree(context->memblocks, bd);
		return NULL;
	}
	is->data = bd;
	is->isClose = isClose;
	is->isRead = isRead;
	is->isReadBlock = isReadBlock;
	is->isSkip = isSkip;
	return is;
}
