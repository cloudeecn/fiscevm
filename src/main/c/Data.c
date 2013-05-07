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

#include "fyc/Data.h"

/*****************public*********************/
fy_ubyte fy_dataRead(fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fy_int ret = is->isRead(context, is, exception);
	FYEH()0;
	if (ret < 0) {
		fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
		return 0;
	}
	return ret;
}

fy_char fy_dataRead2(fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	int i;
	fy_char ret = 0;
	fy_int value;
	for (i = 0; i < 2; i++) {
		value = is->isRead(context, is, exception);
		FYEH()0;
		if (value < 0) {
			fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
			return 0;
		}
		ret = (ret << 8) + value;
	}
	return ret;
}

fy_uint fy_dataRead4(fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	int i;
	fy_uint ret = 0;
	fy_int value;
	for (i = 0; i < 4; i++) {
		value = is->isRead(context, is, exception);
		FYEH()0;
		if (value < 0) {
			fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
			return 0;
		}
		ret = (ret << 8) + value;
	}
	return ret;
}

fy_ulong fy_dataRead8(fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	int i;
	fy_ulong ret = 0;
	fy_int value;
	for (i = 0; i < 8; i++) {
		value = is->isRead(context, is, exception);
		FYEH()0;
		if (value < 0) {
			fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
			return 0;
		}
		ret = (ret << 8) + value;
	}
	return ret;
}
void fy_dataReadBlock(fy_context *context, FY_ATTR_RESTRICT fy_inputStream* is,
		FY_ATTR_RESTRICT void* buffer, fy_int size, fy_exception *exception) {
	fy_int read;
	fy_int pos = 0;
	while ((read = is->isReadBlock(context, is, (fy_byte*) buffer + pos, size,
			exception)) >= 0 && size > 0) {
		FYEH();
		size -= read;
		pos += read;
	}
	if (size != 0) {
		fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
		return;
	}
}
void fy_dataSkip(fy_context *context, fy_inputStream *is, int size,
		fy_exception *exception) {
	fy_int read;
	while ((read = is->isSkip(context, is, size, exception)) >= 0 && size > 0) {
		FYEH();
		size -= read;
	}
	FYEH();
	if (size != 0) {
		fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
		return;
	}
}

