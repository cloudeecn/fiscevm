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

#include "fyc/Resource.h"
fy_data *fy_resourceAllocateData(fy_context *context, const char *name,
		fy_exception *exception) {
	FILE *fp;
	fy_data *data;
	size_t length;
	data = fy_mmAllocate(context->memblocks, sizeof(fy_data), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fp = fopen(name, "rb");
	if (fp == NULL) {
		fy_fault(exception, NULL, "Can't get source %s", name);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	rewind(fp);
	data->data = fy_mmAllocate(context->memblocks, length, exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	data->size = length;
	fread(data->data, length, 1, fp);
	fclose(fp);
	return data;
}

void fy_resourceReleaseData(fy_context *context, fy_data *data) {
	fy_mmFree(context->memblocks, data->data);
	fy_mmFree(context->memblocks, data);
}
