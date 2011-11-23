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

#include "fisceprt.h"
#include <stdio.h>
#include <string.h>
/*
 char dying_message[4096];
 */
static long int blocks = 0;

#ifdef __GNUC__
__attribute__((malloc))
#endif
_FY_EXPORT void *fy_allocate(fy_uint size) {
	void *ret;
	blocks++;
	ret = malloc(size);
	if (ret != NULL) {
		memset(ret, 0, size);
	} else {
		fy_fault("Out of memory!");
	}
	return ret;
}

_FY_EXPORT void fy_free(void *target) {
	blocks--;
	free(target);
}

_FY_EXPORT long int fy_getAllocated() {
	return blocks;
}
static void fy_fault_break() {
	int i = 0;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
}
_FY_EXPORT void fy_fault(char *format, ...) {
	/*Put breakpoint here for exception handle!*/
	int i = 0;
	va_list arg_ptr;
	printf("%s", "Fatal error happened!\n");

	va_start(arg_ptr, format);
	vfprintf(stdout, format, arg_ptr);
	va_end(arg_ptr);
	printf("============\n");
	i++;
	i++;
	i++;
	i++;
	fy_fault_break();
	exit(-1);
}

void vm_dummy() {

}

union fy_dtol {
	fy_double d;
	jlong l;
};

union fy_itof {
	fy_float f;
	fy_int i;
};

_FY_EXPORT jlong fy_doubleToLong(fy_double value) {
	union fy_dtol d;
	d.d = value;
	return d.l;
}
_FY_EXPORT fy_double fy_longToDouble(jlong value) {
	union fy_dtol d;
	d.l = value;
	return d.d;
}
_FY_EXPORT fy_int fy_floatToInt(fy_float value) {
	union fy_itof d;
	d.f = value;
	return d.i;
}
_FY_EXPORT fy_float fy_intToFloat(fy_int value) {
	union fy_itof d;
	d.i = value;
	return d.f;
}

_FY_EXPORT fy_boolean fy_isnand(fy_double d) {
	return d != d;
}
_FY_EXPORT fy_boolean fy_isnanf(fy_float f) {
	return f != f;
}
