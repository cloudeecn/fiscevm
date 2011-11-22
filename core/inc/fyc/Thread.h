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

#ifndef FY_THREAD_H_
#define FY_THREAD_H_

#include <math.h>
#include <float.h>
#include "Portable.h"
#include "Structs.h"
#include "Debug.h"
#include "ThreadManager.h"



#ifdef	__cplusplus
extern "C" {
#endif

FY_EXPORT void fy_nativeReturnInt(fy_VMContext *context,fy_thread *thread,jint value);

FY_EXPORT void fy_nativeReturnHandle(fy_VMContext *context,fy_thread *thread,jint value);

FY_EXPORT void fy_nativeReturnLong(fy_VMContext *context,fy_thread *thread,jlong value);

#define fy_nativeReturnFloat(C,T,V) fy_nativeReturnInt(C,T,fy_floatToInt(V))

#define fy_nativeReturnDouble(C,T,V) fy_nativeReturnLong(C,T,fy_doubleToLong(V))


#ifdef	__cplusplus
}
#endif
#endif /* FY_HEAP_H_ */
