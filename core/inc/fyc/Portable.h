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

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include<memory.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#ifndef NULL
#define NULL 0
#endif

/*#ifndef DEBUG
 #define DEBUG
 #endif
 */
/*We should define it on platforms which doesn't support __PRETTY_FUNCTION__*/
#if 0
#define __PRETTY_FUNCTION__ __FILE__
#endif

#ifdef	__cplusplus
extern "C" {
#endif

typedef unsigned int juint;
typedef unsigned short jchar;
typedef int jint;
typedef short jshort;
typedef char jbyte;
typedef unsigned char jubyte;
typedef float jfloat;
typedef double jdouble;
typedef jint jboolean;

#ifdef _MSC_VER
#else
#define strcpy_s(T,TS,S) strncpy(T,S,TS)
#define sprintf_s snprintf
#endif

#ifdef _MSC_VER
typedef unsigned __int64 julong;
typedef __int64 jlong;
#else
typedef unsigned long long julong;
typedef long long jlong;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define fy_I2TOL(I1,I2) ((jlong)(((julong)(I1)<<32) | ((juint)(I2))))
#define fy_I2TOUL(I1,I2) ((julong)(((julong)(I1)<<32) | ((juint)(I2))))
#define fy_B2TOI(B1,B2) ((((juint)(B1))<<8)|((juint)(B2)))
#define fy_B4TOI(B1,B2,B3,B4) ((((juint)(B1))<<24)|(((juint)(B2))<<16)|(((juint)(B3))<<8)|((juint)(B4)))
#define fy_HOFL(L) ((jint)(L>>32))
#define fy_LOFL(L) ((jint)(L))

jlong fy_doubleToLong(jdouble value);
jdouble fy_longToDouble(jlong value);
jint fy_floatToInt(jfloat value);
jfloat fy_intToFloat(jint value);

jboolean fy_isnand(jdouble d);
jboolean fy_isnanf(jfloat f);

void *vm_allocate(juint size);
void vm_free(void *target);
void vm_die(char *msg, ...);
void vm_dummy();
long int vm_getAllocated();

#ifdef	__cplusplus
}
#endif
#endif /* HARDWARE_H_ */
