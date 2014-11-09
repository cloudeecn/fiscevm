/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part offiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along withfiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fisceprt.h"

/*
 char dying_message[4096];
 */
static long int blocks = 0;

#ifdef __GNUC__
__attribute__((malloc))
#endif
FY_ATTR_EXPORT void *fy_allocate(fy_uint size, fy_exception *exception) {
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

FY_ATTR_EXPORT void fy_free(void *target) {
	blocks--;
	free(target);
}

FY_ATTR_EXPORT long int fy_getAllocated() {
	return blocks;
}

FY_ATTR_EXPORT void fy_fault(fy_exception *exception, const char *clazz,
		const char *format, ...) {
	va_list arg_ptr;
	int i = 0;
#ifdef EMSCRIPTEN
	char msg[512];
#endif
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
		puts("============\n");
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
			puts("Fatal error occored:\n");
			puts(exception->exceptionDesc);
			putchar('\n');
			i++;
			i++;
			exit(-1);
		}
	}
}

FY_ATTR_EXPORT int fy_breakpoint() {
	int i = 1;
	i++;
	if (i == 2) {
		/*Please place breakpoint here*/
		i++;
	}
	return i;
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

FY_ATTR_EXPORT fy_long fy_doubleToLong(fy_double value) {
	union fy_dtol d;
	d.d = value;
	return d.l;
}
FY_ATTR_EXPORT fy_double fy_longToDouble(fy_long value) {
	union fy_dtol d;
	d.l = value;
	return d.d;
}
FY_ATTR_EXPORT fy_int fy_floatToInt(fy_float value) {
	union fy_itof d;
	d.f = value;
	return d.i;
}
FY_ATTR_EXPORT fy_float fy_intToFloat(fy_int value) {
	union fy_itof d;
	d.i = value;
	return d.f;
}

FY_ATTR_EXPORT fy_boolean fy_isnand(fy_double d) {
	return d != d;
}
FY_ATTR_EXPORT fy_boolean fy_isnanf(fy_float f) {
	return f != f;
}

FY_ATTR_EXPORT void fy_portInit(fy_port *data) {
	data->initTimeInMillSec = (fy_long) time(NULL) * 1000;
#if defined(FY_PRT_WIN32)
	QueryPerformanceFrequency(&(data->lpFreq));
	QueryPerformanceCounter(&(data->lpPerfCountBegin));
	data->perfIdv = 1000000000.0 / data->lpFreq.QuadPart;
#elif defined(FY_PRT_POSIX)
	gettimeofday(&(data->tvBeginTime), NULL);
#endif
}
FY_ATTR_EXPORT void fy_portDestroy(fy_port *data) {
	/*No code need yet*/
}
FY_ATTR_EXPORT fy_long fy_portTimeMillSec(fy_port *pd) {
#if defined(FY_PRT_WIN32)
	fy_long timeDelta;
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	timeDelta = lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart;
	timeDelta = timeDelta * 1000 / pd->lpFreq.QuadPart;
	return timeDelta + pd->initTimeInMillSec;
#elif defined(FY_PRT_POSIX)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fy_long) tv.tv_sec) * 1000 + ((fy_long) tv.tv_usec) / 1000;
#else
#error Unsupported platform
#endif
}
FY_ATTR_EXPORT fy_long fy_portTimeNanoSec(fy_port *pd) {
#if defined(FY_PRT_WIN32)
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	return (fy_long) ((lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart)
			* pd->perfIdv);
#elif defined(FY_PRT_POSIX)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fy_long) tv.tv_sec) * 1000000000 + ((fy_long) tv.tv_usec) * 1000;
#else
#error Unsupported platform
#endif
}

FY_ATTR_EXPORT fy_boolean fy_portValidate(){
    if(sizeof(fy_boolean)!=4){
        return FALSE;
    }
    if(sizeof(fy_ubyte)!=1){
        return FALSE;
    }
    if(sizeof(fy_byte)!=1){
        return FALSE;
    }
    if(sizeof(fy_char)!=2){
        return FALSE;
    }
    if(sizeof(fy_short)!=2){
        return FALSE;
    }
    if(sizeof(fy_uint)!=4){
        return FALSE;
    }
    if(sizeof(fy_int)!=4){
        return FALSE;
    }
    if(sizeof(fy_float)!=4){
        return FALSE;
    }
    if(sizeof(fy_ulong)!=8){
        return FALSE;
    }
    if(sizeof(fy_long)!=8){
        return FALSE;
    }
    if(sizeof(fy_double)!=8){
        return FALSE;
    }
    return TRUE;
}