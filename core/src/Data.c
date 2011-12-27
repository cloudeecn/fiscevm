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

#include "fyc/Data.h"

/*****************public*********************/
fy_ubyte fy_dataRead(fy_context *context, void *is, fy_exception *exception) {
	fy_int ret = context->isRead(context, is, exception);
	fy_exceptionCheckAndReturn(exception)0;
	if (ret < 0) {
		fy_fault(exception, FY_EXCEPTION_IO, "Buffer overflow");
		return 0;
	}
	return ret;
}

fy_char fy_dataRead2(fy_context *context, void *is, fy_exception *exception) {
	int i;
	fy_char ret = 0;
	fy_int value;
	for(i=0;i<2;i++){

	}
	if (data->size < 2) {
		fy_fault(NULL, NULL, "Buffer overflow!");
	}
	data->size -= 2;
	for (i = 0; i < 2; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

fy_uint fy_dataRead4(fy_context *context, void *is, fy_exception *exception) {
	int i;
	fy_uint ret = 0;
	if (data->size < 4) {
		fy_fault(NULL, NULL, "Buffer overflow!");
	}
	data->size -= 4;
	for (i = 0; i < 4; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

fy_ulong fy_dataRead8(fy_context *context, void *is, fy_exception *exception) {
	int i;
	fy_ulong ret = 0;
	if (data->size < 8) {
		fy_fault(NULL, NULL, "Buffer overflow!");
	}
	data->size -= 8;
	for (i = 0; i < 8; i++) {
		ret = (ret << 8) + (unsigned char) *(data->data++);
	}
	return ret;
}

void fy_dataSkip(fy_context *context, void *is, int size,
		fy_exception *exception) {
	if (data->size < size) {
		fy_fault(NULL, NULL, "Buffer overflow!");
	}
	data->size -= size;
	data->data += size;
}

