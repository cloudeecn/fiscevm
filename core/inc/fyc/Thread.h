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
#include "../fisceprt.h"
#include "../fiscestu.h"
#include "../fiscedev.h"
#include "../fy_util/Debug.h"
#include "ThreadManager.h"

void fy_threadSetCurrentThrowable(fy_context *context, fy_thread *thread,
		fy_int handle, fy_exception *exception);

void fy_threadMonitorEnter(fy_context *context, fy_thread *thread,
		fy_int handle);

void fy_threadMonitorExit(fy_context *context, fy_thread *thread, fy_int handle,
		fy_exception *exception);

void fy_threadDestroy(fy_context *context, fy_thread *thread);

fy_method *fy_threadGetCurrentMethod(fy_context *context, fy_thread *thread);

void fy_threadInitWithRun(fy_context *context, fy_thread *thread, int handle,
		fy_exception *exception);

void fy_threadInitWithData(fy_context *context, fy_thread *thread,
		fy_byte *data, fy_int length, fy_exception *exception);

void fy_threadInitWithMethod(fy_context *context, fy_thread *thread,
		int threadHandle, fy_method *method, fy_exception *exception);

void fy_threadPushMethod(fy_context *context, fy_thread *thread,
		fy_method *invoke, fy_frame **localFrame, fy_exception *exception);

void fy_threadRun(fy_context *context, fy_thread *thread, fy_message *message,
		fy_int ops);

void fy_threadFillException(fy_context *context, fy_thread *thread,
		fy_uint handle, fy_exception *exception);

fy_uint fy_threadPrepareThrowable(fy_context *context, fy_thread *thread,
		_FY_RESTRICT fy_exception *toPrepare,
		_FY_RESTRICT fy_exception *exception);

void fy_threadReturnInt(fy_context *context, fy_thread *thread, fy_int value);

void fy_threadReturnHandle(fy_context *context, fy_thread *thread, fy_int value);

void fy_threadReturnLong(fy_context *context, fy_thread *thread, fy_long value);

void fy_threadScanRef(fy_context *context,fy_thread *thread,fy_uint *mask);

#define fy_threadReturnFloat(C,T,V) fy_threadReturnInt(C,T,fy_floatToInt(V))

#define fy_threadReturnDouble(C,T,V) fy_threadReturnLong(C,T,fy_doubleToLong(V))

#endif /* FY_HEAP_H_ */
