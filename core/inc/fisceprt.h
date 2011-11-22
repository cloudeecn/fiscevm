#ifndef _FY_FISCEPRT_H
#define _FY_FISCEPRT_H

#include "fiscecon.h"
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
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

typedef unsigned int juint;
typedef unsigned short jchar;
typedef signed int jint;
typedef signed short jshort;
typedef signed char jbyte;
typedef unsigned char jubyte;
typedef float jfloat;
typedef double jdouble;
typedef jint jboolean;

#ifdef _MSC_VER
#else
# define strcpy_s(T,TS,S) strncpy(T,S,TS)
# define sprintf_s snprintf
#endif

#ifdef _MSC_VER
# define _FY_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
# ifdef _WIN32
#  define _FY_EXPORT __attribute__((externally_visible)) __attribute__((dllexport))
# else
#  define _FY_EXPORT __attribute__((externally_visible))
# endif
#else
# define _FY_EXPORT
#endif

#ifdef _MSC_VER
typedef unsigned __int64 julong;
typedef __int64 jlong;
#else
typedef unsigned long long julong;
typedef long long jlong;
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

#define fy_I2TOL(I1,I2) ((jlong)(((julong)(I1)<<32) + ((julong)(I2))))
#define fy_I2TOUL(I1,I2) ((julong)(((julong)(I1)<<32) + ((julong)(I2))))
#define fy_B2TOUI(B1,B2) ((((juint)(B1))<<8)+((juint)(B2)))
#define fy_B2TOI(B1,B2) ((jshort)((((juint)(B1))<<8)+((juint)(B2))))
#define fy_B4TOI(B1,B2,B3,B4) ((((juint)(B1))<<24)+(((juint)(B2))<<16)+(((juint)(B3))<<8)+((juint)(B4)))
#define fy_HOFL(L) ((jint)(L>>32))
#define fy_LOFL(L) ((jint)(L))

_FY_EXPORT jlong fy_doubleToLong(jdouble value);
_FY_EXPORT jdouble fy_longToDouble(jlong value);
_FY_EXPORT jint fy_floatToInt(jfloat value);
_FY_EXPORT jfloat fy_intToFloat(jint value);

_FY_EXPORT jboolean fy_isnand(jdouble d);
_FY_EXPORT jboolean fy_isnanf(jfloat f);

_FY_EXPORT void *vm_allocate(juint size);
_FY_EXPORT void vm_free(void *target);
_FY_EXPORT void vm_die(char *msg, ...);
_FY_EXPORT long int vm_getAllocated();

#ifdef	__cplusplus
}
#endif

#endif
