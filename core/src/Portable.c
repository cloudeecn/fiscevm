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

#include "fyc/Portable.h"
#include <stdio.h>
#include <string.h>
/*
 char dying_message[4096];
 */
static long int blocks = 0;

#ifdef __GNUC__
__attribute__((malloc))
#endif
void *vm_allocate(juint size) {
	void *ret;
	blocks++;
	ret = malloc(size);
	if (ret != NULL) {
		memset(ret, 0, size);
	} else {
		vm_die("Out of memory!");
	}
	return ret;
}

void vm_free(void *target) {
	blocks--;
	free(target);
}

long int vm_getAllocated() {
	return blocks;
}
static void vm_die_break() {
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
void vm_die(char *format, ...) {
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
	vm_die_break();
	exit(-1);
}

void vm_dummy() {

}

union fy_dtol {
	jdouble d;
	jlong l;
};

union fy_itof {
	jfloat f;
	jint i;
};

jlong fy_doubleToLong(jdouble value) {
	union fy_dtol d;
	d.d = value;
	return d.l;
}
jdouble fy_longToDouble(jlong value) {
	union fy_dtol d;
	d.l = value;
	return d.d;
}
jint fy_floatToInt(jfloat value) {
	union fy_itof d;
	d.f = value;
	return d.i;
}
jfloat fy_intToFloat(jint value) {
	union fy_itof d;
	d.i = value;
	return d.f;
}

jboolean fy_isnand(jdouble d) {
	return d != d;
}
jboolean fy_isnanf(jfloat f) {
	return f != f;
}
