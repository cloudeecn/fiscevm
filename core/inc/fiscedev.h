/*
 * fiscedev.h
 *
 *  Created on: Nov 22, 2011
 *      Author: cloudee
 */

#ifndef FISCEDEV_H_
#define FISCEDEV_H_

#include "fiscestu.h"

#ifdef	__cplusplus
extern "C" {
#endif

_FY_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		jint value);

_FY_EXPORT void fy_nativeReturnHandle(fy_context *context, fy_thread *thread,
		jint value);

_FY_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		jlong value);

#define fy_nativeReturnFloat(C,T,V) fy_nativeReturnInt(C,T,fy_floatToInt(V))

#define fy_nativeReturnDouble(C,T,V) fy_nativeReturnLong(C,T,fy_doubleToLong(V))

#ifdef	__cplusplus
}
#endif

#endif /* FISCEDEV_H_ */
