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

#ifndef FY_THREADMANAGER_H_
#define FY_THREADMANAGER_H_

#include "fisce.h"
#include "fyc/VMContext.h"
#include "fyc/Thread.h"

#ifdef	__cplusplus
extern "C" {
#endif
fy_int fy_tmMonitorEnter(fy_context *context, fy_thread *thread,
		fy_uint monitorId, fy_int ops);
fy_int fy_tmMonitorExit(fy_context *context, fy_thread *thread, fy_uint monitorId, fy_int ops,
		fy_exception *exception);
void fy_tmSleep(fy_context *context, fy_thread *thread, fy_long time);
void fy_tmInterrupt(fy_context *context, fy_uint targetHandle,
		fy_exception *exception);
fy_boolean fy_tmIsInterrupted(fy_context *context, fy_uint targetHandle,
		fy_boolean clear, fy_exception *exception);
fy_int fy_tmWait(fy_context *context, fy_thread *thread, fy_int monitorId,
		fy_long time, fy_int ops, fy_exception *exception);
void fy_tmNotify(fy_context *context, fy_thread *thread, fy_int monitorId,
		fy_boolean all, fy_exception *exception);
fy_boolean fy_tmIsAlive(fy_context *context, fy_uint threadHandle,
		fy_exception *exception);
void fy_tmDestroyThread(fy_thread *thread);
void fy_tmBootFromMain(fy_context *context, fy_class *clazz,
		fy_exception *exception);
void fy_tmPushThread(fy_context *context, fy_uint threadHandle,
		fy_exception *exception);
void fy_tmRun(fy_context *context, fy_message *message, fy_exception *exception);

#ifdef	__cplusplus
}
#endif
#endif /* FY_THREADMANAGER_H_ */

