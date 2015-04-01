/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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

#include "fy_util/Portable.h"

/*
 char dying_message[4096];
 */
static long int blocks = 0;

#ifdef __GNUC__
__attribute__((malloc))
#endif
FY_ATTR_EXPORT void *fy_allocate(fisce_uint size, fisce_exception *exception) {
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

FY_ATTR_EXPORT fisce_exception* fy_fault(fisce_exception *exception, const char *clazz,
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
	return exception;
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

FY_ATTR_EXPORT void fy_portInit(fy_port *data) {
	data->initTimeInMillSec = (fisce_long) time(NULL) * 1000;
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
FY_ATTR_EXPORT fisce_long fy_portTimeMillSec(fy_port *pd) {
#if defined(FY_PRT_WIN32)
	fisce_long timeDelta;
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	timeDelta = lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart;
	timeDelta = timeDelta * 1000 / pd->lpFreq.QuadPart;
	return timeDelta + pd->initTimeInMillSec;
#elif defined(FY_PRT_POSIX)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fisce_long) tv.tv_sec) * 1000 + ((fisce_long) tv.tv_usec) / 1000;
#else
#error Unsupported platform
#endif
}
FY_ATTR_EXPORT fisce_long fy_portTimeNanoSec(fy_port *pd) {
#if defined(FY_PRT_WIN32)
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	return (fisce_long) ((lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart)
			* pd->perfIdv);
#elif defined(FY_PRT_POSIX)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((fisce_long) tv.tv_sec) * 1000000000 + ((fisce_long) tv.tv_usec) * 1000;
#else
#error Unsupported platform
#endif
}

FY_ATTR_EXPORT fisce_boolean fy_portValidate(){
    if(sizeof(fisce_boolean)!=4){
        return FALSE;
    }
    if(sizeof(fisce_ubyte)!=1){
        return FALSE;
    }
    if(sizeof(fisce_byte)!=1){
        return FALSE;
    }
    if(sizeof(fisce_char)!=2){
        return FALSE;
    }
    if(sizeof(fisce_short)!=2){
        return FALSE;
    }
    if(sizeof(fisce_uint)!=4){
        return FALSE;
    }
    if(sizeof(fisce_int)!=4){
        return FALSE;
    }
    if(sizeof(fisce_float)!=4){
        return FALSE;
    }
    if(sizeof(fisce_ulong)!=8){
        return FALSE;
    }
    if(sizeof(fisce_long)!=8){
        return FALSE;
    }
    if(sizeof(fisce_double)!=8){
        return FALSE;
    }
    return TRUE;
}
