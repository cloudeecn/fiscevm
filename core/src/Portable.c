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

/*
 char dying_message[4096];
 */
static long int blocks = 0;

#ifdef __GNUC__
__attribute__((malloc))
#endif
_FY_EXPORT void *fy_allocate(fy_uint size, fy_exception *exception) {
	void *ret;
	blocks++;
	ret = malloc(size);
	if (ret != NULL) {
		memset(ret, 0, size);
	} else {
		fy_fault(exception, NULL, "Out of memory!");
		return NULL;
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

_FY_EXPORT void fy_fault(fy_exception *exception, const char *clazz,
		const char *format, ...) {
	va_list arg_ptr;
	int i = 0;
	if (clazz == NULL) {
		clazz = "";
	}
	if (i == 0) {
		i++;
	}
	if (exception == NULL) {
		puts("Fatal error happened\n");
		puts(clazz);
		va_start(arg_ptr, format);
		vfprintf(stdout, format, arg_ptr);
		va_end(arg_ptr);
		printf("============\n");
		i++;
		i++;
		i++;
		i++;
		exit(-1);
	} else {
		exception->exceptionType = exception_normal;
		strcpy(exception->exceptionName, clazz);
		va_start(arg_ptr, format);
		vsprintf_s(exception->exceptionDesc, sizeof(exception->exceptionDesc),
				format, arg_ptr);
		va_end(arg_ptr);
		if (clazz[0] == 0) {
			printf("Fatal error occored: ");
			va_start(arg_ptr, format);
			vprintf(format, arg_ptr);
			va_end(arg_ptr);
		}
	}
}

void vm_dummy() {

}

union fy_dtol {
	fy_double d;
	fy_long l;
};

union fy_itof {
	fy_float f;
	fy_int i;
};

_FY_EXPORT fy_long fy_doubleToLong(fy_double value) {
	union fy_dtol d;
	d.d = value;
	return d.l;
}
_FY_EXPORT fy_double fy_longToDouble(fy_long value) {
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

void fy_portInit(fy_port *data) {
	data->initTimeInMillSec = (fy_long) time(NULL) * 1000;
#if defined(_WIN32)
	QueryPerformanceFrequency(&(data->lpFreq));
	QueryPerformanceCounter(&(data->lpPerfCountBegin));
	data->perfIdv = 1000000000.0 / data->lpFreq.QuadPart;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	gettimeofday(&(data->tvBeginTime), NULL);
#endif
}
void fy_portDestroy(fy_port *data) {
	/*No code need yet*/
}
fy_long fy_portTimeMillSec(fy_port *pd) {
#if defined(_WIN32)
	fy_long timeDelta;
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	timeDelta = lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart;
	timeDelta = timeDelta * 1000 / pd->lpFreq.QuadPart;
	return timeDelta + pd->initTimeInMillSec;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fy_long) tv.tv_sec) * 1000 + ((fy_long) tv.tv_usec) / 1000;
#endif
	return 0;
}
fy_long fy_portTimeNanoSec(fy_port *pd) {
#if defined(_WIN32)
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	return (fy_long) ((lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart)
			* pd->perfIdv);
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fy_long) tv.tv_sec) * 1000000000 + ((fy_long) tv.tv_usec) * 1000;
#endif
	return 0;
}
