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
#ifndef _FY_FISCEPRT_H
#define _FY_FISCEPRT_H

#include "fiscecon.h"
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
#include <sys/time.h>
#include <errno.h>
#endif
#include <time.h>
#include<memory.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

/*We should define it on platforms which doesn't support __PRETTY_FUNCTION__*/
#if 0
#define __PRETTY_FUNCTION__ __FILE__
#endif

typedef unsigned int fy_uint;
typedef unsigned short fy_char;
typedef signed int fy_int;
typedef signed short fy_short;
typedef signed char fy_byte;
typedef unsigned char fy_ubyte;
typedef float fy_float;
typedef double fy_double;
typedef fy_int fy_boolean;

#ifdef _MSC_VER
typedef unsigned __int64 fy_ulong;
typedef __int64 fy_long;
#else
typedef unsigned long long fy_ulong;
typedef long long fy_long;
#endif

typedef struct fy_exception {
	enum exceptionType {
		exception_none = 0, exception_normal
	/*, exception_critical // use fy_fault instead!*/
	} exceptionType;
	char exceptionName[64];
	char exceptionDesc[64];
} fy_exception;
#define fy_exceptionCheckAndReturn(EXCEPTION) if((EXCEPTION)!=NULL&&(EXCEPTION)->exceptionType!=exception_none) return

typedef struct fy_port {
	fy_long initTimeInMillSec;
#if defined(_WIN32)
	LARGE_INTEGER lpFreq;
	LARGE_INTEGER lpPerfCountBegin;
	double perfIdv;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tvBeginTime;
#endif
} fy_port;
#ifdef _MSC_VER
#else
# define strcpy_s(T,TS,S) strncpy(T,S,TS)
# define sprintf_s snprintf
# define vsprintf_s vsnprintf
#endif
#ifdef _MSC_VER
# ifdef _FY_BUILD_LIB
#  define _FY_EXPORT __declspec(dllexport)
# else
#  define _FY_EXPORT __declspec(dllimport)
# endif
#elif defined(__GNUC__)
# ifdef _WIN32
#  define _FY_EXPORT __attribute__((externally_visible)) __attribute__((dllexport))
# else
#  define _FY_EXPORT __attribute__((externally_visible))
# endif
#else
# define _FY_EXPORT
#endif
#if __STDC_VERSION__ >= 199901L
# define _C99
# define _FY_RESTRICT restrict
#else
# define _FY_RESTRICT
#endif
#if defined(_C99) || defined(__GNUC__)
# define _FY_LATE_DECLARATION
#endif
#if defined(_C99)
# define _FY_VLS(TYPE,X) TYPE X[]
#elif defined(__GNUC__)
# define _FY_VLS(TYPE,X) TYPE X[0]
#else
# define _FY_VLS(TYPE,X) TYPE X[1]
#endif
#if 1
#define _FY_GOTO
#endif
#if 1
#define _FY_FASTCALL __fastcall
#else
#define _FY_FASTCALL
#endif
#ifdef _DEBUG
#define FY_STRICT_CHECK
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#if defined(_WIN32)
# define FY_PRINT64 "I64"
#else
# define FY_PRINT64 "ll"
#endif
#define fy_I2TOL(I1,I2) ((fy_long)(((fy_ulong)(I1)<<32) + ((fy_ulong)(I2))))
#define fy_I2TOUL(I1,I2) ((fy_ulong)(((fy_ulong)(I1)<<32) + ((fy_ulong)(I2))))
#define fy_B2TOUI(B1,B2) ((((fy_uint)(B1))<<8)+((fy_uint)(B2)))
#define fy_B2TOI(B1,B2) ((fy_short)((((fy_uint)(B1))<<8)+((fy_uint)(B2))))
#define fy_B4TOI(B1,B2,B3,B4) ((((fy_uint)(B1))<<24)+(((fy_uint)(B2))<<16)+(((fy_uint)(B3))<<8)+((fy_uint)(B4)))
#define fy_HOFL(L) ((fy_int)(L>>32))
#define fy_LOFL(L) ((fy_int)(L))
_FY_EXPORT fy_long fy_doubleToLong(fy_double value);
_FY_EXPORT fy_double fy_longToDouble(fy_long value);
_FY_EXPORT fy_int fy_floatToInt(fy_float value);
_FY_EXPORT fy_float fy_intToFloat(fy_int value);
_FY_EXPORT fy_boolean fy_isnand(fy_double d);
_FY_EXPORT fy_boolean fy_isnanf(fy_float f);

_FY_EXPORT void *fy_allocate(fy_uint size, fy_exception *exception);
_FY_EXPORT void fy_free(void *target);
_FY_EXPORT void fy_fault(fy_exception *exception, const char *clazz,
		const char *msg, ...);
_FY_EXPORT long int fy_getAllocated();

_FY_EXPORT void fy_portInit(fy_port *pd);
_FY_EXPORT void fy_portDestroy(fy_port *pd);
_FY_EXPORT fy_long fy_portTimeMillSec(fy_port *pd);
_FY_EXPORT fy_long fy_portTimeNanoSec(fy_port *pd);

#ifdef	__cplusplus
}
#endif

#endif
