/**
 *  Copyright 2010 Yuxuan Huang. All rights reserved.
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
#include "fyc/BAIS.h"
typedef struct baisData {
	fy_byte *buf;
	fy_int pos;
	fy_int len;
} baisData;

static fy_int isRead(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	baisData *bd = is->data;
	if (bd->pos == bd->len) {
		return -1;
	}
	return bd->buf[bd->pos++] & 0xff;
}

static fy_int isReadBlock(struct fy_context *context, fy_inputStream *is,
		void *target, fy_int size, fy_exception *exception) {
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

static fy_int isSkip(struct fy_context *context, fy_inputStream *is,
		fy_int size, fy_exception *exception) {
	baisData *bd = is->data;
	if (bd->len - bd->pos < size) {
		return 0;
	} else {
		bd->pos += size;
		return size;
	}
}

static void isClose(struct fy_context *context, fy_inputStream *is) {
	fy_mmFree(context->memblocks, is->data);
}

FY_ATTR_EXPORT fy_inputStream *fy_baisOpenByteArrayInputStream(fy_context *context,
		void *buffer, fy_int bufferLen, fy_exception *exception) {
	baisData *bd;
	fy_inputStream *is;
	if (buffer == NULL ) {
		return NULL ;
	}
	bd = fy_mmAllocate(context->memblocks, sizeof(baisData), exception);
	FYEH()NULL;
	bd->buf = buffer;
	bd->len = bufferLen;
	bd->pos = 0;
	is = fy_mmAllocate(context->memblocks, sizeof(fy_inputStream), exception);
	if (exception->exceptionType != exception_none) {
		fy_mmFree(context->memblocks, bd);
		return NULL ;
	}
	is->data = bd;
	is->isClose = isClose;
	is->isRead = isRead;
	is->isReadBlock = isReadBlock;
	is->isSkip = isSkip;
	return is;
}
