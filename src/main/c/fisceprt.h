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
# include <windows.h>
# define FY_PRT_WIN32 1
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE) \
	|| defined(__linux) || defined(__unix) || defined(__posix)
# include <sys/time.h>
# include <errno.h>
# define FY_PRT_POSIX 1
#else
# error "Unsupported system"
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
#define FYEH() fy_exceptionCheckAndReturn(exception)
typedef struct fy_port {
	fy_long initTimeInMillSec;
#if defined(FY_PRT_WIN32)
	LARGE_INTEGER lpFreq;
	LARGE_INTEGER lpPerfCountBegin;
	double perfIdv;
#elif defined(FY_PRT_POSIX)
	struct timeval tvBeginTime;
#endif
} fy_port;
#ifdef _MSC_VER
#else
# define strcpy_s(T,TS,S) strncpy(T,S,TS)
# define sprintf_s snprintf
# define vsprintf_s vsnprintf
#endif
#if defined(FY_PRT_WIN32)
# if defined(__GNUC__)
#  if defined(FY_EXPORT) && defined(DLL_EXPORT)
#   define FY_ATTR_EXPORT __attribute__((externally_visible)) __attribute__((dllexport))
#  elif defined(FY_EXPORT) && !defined(DLL_EXPORT)
#   define FY_ATTR_EXPORT __attribute__((externally_visible))
#  elif !defined(FY_EXPORT) && defined(FY_STATIC)
#    define FY_ATTR_EXPORT
#  else
#    define FY_ATTR_EXPORT __attribute__((dllimport))
#  endif
# elif defined(_MSC_VER)
#  if defined(DLL_EXPORT)
#   define FY_ATTR_EXPORT __declspec(dllexport)
#  else
#   ifdef FY_STATIC
#    define FY_ATTR_EXPORT
#   else
#    define FY_ATTR_EXPORT __attribute__((dllimport))
#   endif
#  endif
# endif
#elif defined(__GNUC__)
# if defined(FY_EXPORT)
#  define FY_ATTR_EXPORT __attribute__((externally_visible))
# else
#  define FY_ATTR_EXPORT
# endif
#else
# define FY_ATTR_EXPORT
#endif
#if __STDC_VERSION__ >= 199901L
# define _C99
# define FY_ATTR_RESTRICT /*restrict*/
#else
# define FY_ATTR_RESTRICT
#endif
#if defined(_C99) || defined(__GNUC__)
# define FY_LATE_DECLARATION
#endif
#if defined(_C99)
# define FY_VLS(TYPE,X) TYPE X[]
#elif defined(__GNUC__)
# define FY_VLS(TYPE,X) TYPE X[0]
#else
# define FY_VLS(TYPE,X) TYPE X[1]
#endif
#if 1
#define FY_GOTO
#endif
#if 1
#define FY_FASTCALL __fastcall
#else
#define FY_FASTCALL
#endif
#ifdef FY_DEBUG
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
#if defined(_WIN32)
# define FY_PRINT32 "I32"
#else
# define FY_PRINT32 ""
#endif

#ifdef AAAA
# define likely(x) __builtin_expect(!!(x),1)
# define unlikely(x) __builtin_expect((x),0)
#else
# define likely(x) x
# define unlikely(x) x
#endif

#define fy_I2TOL(I1,I2) ((fy_long)(((fy_ulong)(fy_uint)(I1)<<32) | ((fy_ulong)(fy_uint)(I2))))
#define fy_I2TOUL(I1,I2) ((fy_ulong)(((fy_ulong)(fy_uint)(I1)<<32) | ((fy_ulong)(fy_uint)(I2))))
#define fy_B2TOUI(B1,B2) ((((fy_uint)(fy_ubyte)(B1))<<8)|((fy_uint)(fy_ubyte)(B2)))
#define fy_B2TOI(B1,B2) ((fy_short)((((fy_uint)(fy_ubyte)(B1))<<8)|((fy_uint)(fy_ubyte)(B2))))
#define fy_B4TOI(B1,B2,B3,B4) ((((fy_uint)(fy_ubyte)(B1))<<24)|(((fy_uint)(fy_ubyte)(B2))<<16)|(((fy_uint)(fy_ubyte)(B3))<<8)|((fy_uint)(fy_ubyte)(B4)))
#define fy_HOFL(L) ((fy_int)(L>>32))
#define fy_LOFL(L) ((fy_int)(L))
FY_ATTR_EXPORT fy_long fy_doubleToLong(fy_double value);
FY_ATTR_EXPORT fy_double fy_longToDouble(fy_long value);
FY_ATTR_EXPORT fy_int fy_floatToInt(fy_float value);
FY_ATTR_EXPORT fy_float fy_intToFloat(fy_int value);
FY_ATTR_EXPORT fy_boolean fy_isnand(fy_double d);
FY_ATTR_EXPORT fy_boolean fy_isnanf(fy_float f);

FY_ATTR_EXPORT void *fy_allocate(fy_uint size, fy_exception *exception);
FY_ATTR_EXPORT void fy_free(void *target);
FY_ATTR_EXPORT void fy_fault(fy_exception *exception, const char *clazz,
		const char *msg, ...);
FY_ATTR_EXPORT int fy_breakpoint();
FY_ATTR_EXPORT long int fy_getAllocated();

FY_ATTR_EXPORT void fy_portInit(fy_port *pd);
FY_ATTR_EXPORT void fy_portDestroy(fy_port *pd);
FY_ATTR_EXPORT fy_long fy_portTimeMillSec(fy_port *pd);
FY_ATTR_EXPORT fy_long fy_portTimeNanoSec(fy_port *pd);

#ifdef	__cplusplus
}
#endif

#endif
