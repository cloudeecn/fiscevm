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

#include "fyc/Data.h"
#include "fyc/Constants.h"
#include "fyc/InputStream.h"

/*****************public*********************/
fisce_ubyte fy_dataRead(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	fisce_int ret = is->isRead(context, is, exception);
	FYEH()0;
	if (ret < 0) {
		fy_fault(exception, FY_EXCEPTION_IO, "Unexpected end of file");
		return 0;
	}
	return ret;
}

fisce_char fy_dataRead2(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	int i;
	fisce_char ret = 0;
	fisce_int value;
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

fisce_uint fy_dataRead4(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	int i;
	fisce_uint ret = 0;
	fisce_int value;
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

fisce_ulong fy_dataRead8(fy_context *context, fy_inputStream *is,
		fisce_exception *exception) {
	int i;
	fisce_ulong ret = 0;
	fisce_int value;
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
void fy_dataReadBlock(fy_context *context, fy_inputStream* is,
        void* buffer, fisce_int size, fisce_exception *exception) {
	fisce_int read;
	fisce_int pos = 0;
	while ((read = is->isReadBlock(context, is, (fisce_byte*) buffer + pos, size,
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
		fisce_exception *exception) {
	fisce_int read;
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

