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
#ifndef _FY_FISCEPRT_H
#define _FY_FISCEPRT_H

#include "fiscepub.h"

#if defined(_WIN32)
# include <windows.h>
# define FY_PRT_WIN32 1
#elif defined(_POSIX_VERSION) || defined(_POSIX_C_SOURCE) || defined(_DARWIN_FEATURE_UNIX_CONFORMANCE) \
	|| defined(__linux) || defined(__unix) || defined(__posix) || defined(__APPLE__)
# include <sys/time.h>
# include <errno.h>
# define FY_PRT_POSIX 1
#else
# error "Unsupported system"
#endif

#include<time.h>
#include<memory.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

/*
 GCC attributes for optimize
 */
#ifdef __GNUC__
# ifndef likely
#  define likely(X) __builtin_expect(!!(X), 1)
# endif
# ifndef unlikely
#  define unlikely(X) __builtin_expect(!!(X), 0)
# endif
# define FY_NO_INLINE __attribute__((noinline))
# define FY_PREFETCH(ADDR) __builtin_prefetch(ADDR)
# if ((__GNUC__==2 && defined(__GNUC_MINOR__) && __GNUC_MINOR__>=7)||(__GNUC__>2))
#  define MAYBE_UNUSED __attribute__((unused))
# else
#  define MAYBE_UNUSED
# endif
# if ((__GNUC__==4 && defined(__GNUC_MINOR__) && __GNUC_MINOR__>=3)||(__GNUC__>4))
#  define FY_HOT __attribute__((hot))
# else
#  define FY_HOT
# endif
#else
# ifndef likely
#  define likely(X) (X)
# endif
# ifndef unlikely
#  define unlikely(X) (X)
# endif
# define FY_NO_INLINE
# define FY_PREFETCH(ADDR)
# define MAYBE_UNUSED
# define FY_HOT
# define FY_PREFETCH
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/*We should define it on platforms which doesn't support __PRETTY_FUNCTION__*/
#if 0
#define __PRETTY_FUNCTION__ __FILE__
#endif

typedef struct fy_port {
	fisce_long initTimeInMillSec;
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
#  if defined(DLL_EXPORT) || defined(FY_EXPORT)
#   define FY_ATTR_EXPORT __attribute__((dllexport))
#  elif defined(FY_STATIC)
#    define FY_ATTR_EXPORT
#  else
#    define FY_ATTR_EXPORT __attribute__((dllimport))
#  endif
# elif defined(_MSC_VER)
#  if defined(DLL_EXPORT) || defined(FY_EXPORT)
#   define FY_ATTR_EXPORT __declspec(dllexport)
#  elif defined(FY_STATIC)
#   define FY_ATTR_EXPORT
#  else
#    define FY_ATTR_EXPORT __declspec((dllimport))
#  endif
# endif
#elif defined(EMSCRIPTEN)
# define FY_ATTR_EXPORT
#else
# define FY_ATTR_EXPORT
#endif

#if __STDC_VERSION__ >= 199901L
# define _C99
# define FY_ATTR_RESTRICT restrict
#else
# define FY_ATTR_RESTRICT
#endif


#if defined(EMSCRIPTEN)
# define FY_VLS(TYPE,X) TYPE X[1]
#elif defined(_C99)
# define FY_VLS(TYPE,X) TYPE X[]
#elif defined(__GNUC__) && !defined(FY_STRICT_DECLARATION)
# define FY_VLS(TYPE,X) TYPE X[0]
#else
# define FY_VLS(TYPE,X) TYPE X[1]
#endif

#if defined(__GNUC__) && !defined(FY_STRICT_DECLARATION)
# define FY_ENUM_BITS_SUPPORTED
#endif

#if (defined(_C99))
# define FY_INLINE inline
#else
# define FY_INLINE
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

#define FY_PDIFF(TYPE, B, A) ((fisce_int)(((size_t)B - (size_t)A)/sizeof(TYPE)))
#define fy_I2TOL(I1,I2) ((fisce_long)(((fisce_ulong)(fisce_uint)(I1)<<32) | ((fisce_ulong)(fisce_uint)(I2))))
#define fy_I2TOUL(I1,I2) ((fisce_ulong)(((fisce_ulong)(fisce_uint)(I1)<<32) | ((fisce_ulong)(fisce_uint)(I2))))
#define fy_B2TOUI(B1,B2) ((((fisce_uint)(fisce_ubyte)(B1))<<8)|((fisce_uint)(fisce_ubyte)(B2)))
#define fy_B2TOI(B1,B2) ((fisce_short)((((fisce_uint)(fisce_ubyte)(B1))<<8)|((fisce_uint)(fisce_ubyte)(B2))))
#define fy_B4TOI(B1,B2,B3,B4) ((((fisce_uint)(fisce_ubyte)(B1))<<24)|(((fisce_uint)(fisce_ubyte)(B2))<<16)|(((fisce_uint)(fisce_ubyte)(B3))<<8)|((fisce_uint)(fisce_ubyte)(B4)))
#define fy_HOFL(L) ((fisce_int)(L>>32))
#define fy_LOFL(L) ((fisce_int)(L))

FY_ATTR_EXPORT void *fy_allocate(fisce_uint size, fisce_exception *exception);
FY_ATTR_EXPORT void fy_free(void *target);
FY_ATTR_EXPORT fisce_exception *fy_fault(fisce_exception *exception, const char *clazz,
		const char *msg, ...);
FY_ATTR_EXPORT int fy_breakpoint();
FY_ATTR_EXPORT long int fy_getAllocated();

FY_ATTR_EXPORT void fy_portInit(fy_port *pd);
FY_ATTR_EXPORT void fy_portDestroy(fy_port *pd);
FY_ATTR_EXPORT fisce_long fy_portTimeMillSec(fy_port *pd);
FY_ATTR_EXPORT fisce_long fy_portTimeNanoSec(fy_port *pd);
    
FY_ATTR_EXPORT fisce_boolean fy_portValidate();

#define fisce_stack_item2iarray(spp) ((int*)(void*)(spp))
#define fisce_stack_item2farray(spp) ((float*)(void*)(spp))

#define FYEH() if(unlikely((exception)!=NULL&&(exception)->exceptionType!=exception_none)) return
#define FYEX(HANDLER) if(unlikely((exception)!=NULL&&(exception)->exceptionType!=exception_none)) {HANDLER;}
#define FYEG(X) if(unlikely((exception)!=NULL&&(exception)->exceptionType!=exception_none)) goto X

MAYBE_UNUSED FY_INLINE static fisce_int fy_mini(fisce_int x,fisce_int y){
	return y ^ ((x ^ y) & -(x < y));
}

MAYBE_UNUSED FY_INLINE static fisce_int fy_maxi(fisce_int x,fisce_int y){
	return x ^ ((x ^ y) & -(x < y));
}

union fy_dtol {
	fisce_double d;
	fisce_long l;
};

union fy_itof {
	fisce_float f;
	fisce_int i;
};

MAYBE_UNUSED FY_INLINE static  fisce_long fisce_doubleToLong(fisce_double value) {
	union fy_dtol d;
	d.d = value;
	return d.l;
}
MAYBE_UNUSED FY_INLINE static  fisce_double fisce_longToDouble(fisce_long value) {
	union fy_dtol d;
	d.l = value;
	return d.d;
}
MAYBE_UNUSED FY_INLINE static  fisce_int fisce_floatToInt(fisce_float value) {
	union fy_itof d;
	d.f = value;
	return d.i;
}
MAYBE_UNUSED FY_INLINE static  fisce_float fisce_intToFloat(fisce_int value) {
	union fy_itof d;
	d.i = value;
	return d.f;
}

MAYBE_UNUSED FY_INLINE static  fisce_boolean fy_isnand(fisce_double d) {
	return d != d;
}
MAYBE_UNUSED FY_INLINE static  fisce_boolean fy_isnanf(fisce_float f) {
	return f != f;
}

#ifdef	__cplusplus
}
#endif

#endif
