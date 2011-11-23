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
#define fy_B2TOUI(B1,B2) ((((fy_uint)(B1))<<8)+((fy_uint)(B2)))
#define fy_B2TOI(B1,B2) ((fy_short)((((fy_uint)(B1))<<8)+((fy_uint)(B2))))
#define fy_B4TOI(B1,B2,B3,B4) ((((fy_uint)(B1))<<24)+(((fy_uint)(B2))<<16)+(((fy_uint)(B3))<<8)+((fy_uint)(B4)))
#define fy_HOFL(L) ((fy_int)(L>>32))
#define fy_LOFL(L) ((fy_int)(L))

_FY_EXPORT jlong fy_doubleToLong(fy_double value);
_FY_EXPORT fy_double fy_longToDouble(jlong value);
_FY_EXPORT fy_int fy_floatToInt(fy_float value);
_FY_EXPORT fy_float fy_intToFloat(fy_int value);

_FY_EXPORT fy_boolean fy_isnand(fy_double d);
_FY_EXPORT fy_boolean fy_isnanf(fy_float f);

_FY_EXPORT void *fy_allocate(fy_uint size);
_FY_EXPORT void fy_free(void *target);
_FY_EXPORT void fy_fault(char *msg, ...);
_FY_EXPORT long int fy_getAllocated();

#ifdef	__cplusplus
}
#endif

#endif
