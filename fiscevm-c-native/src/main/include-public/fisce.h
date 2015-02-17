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

#ifndef MAIN_INCLUDE_PUBLIC_FISCE_H_
#define MAIN_INCLUDE_PUBLIC_FISCE_H_

#include "fy_util/Portable.h"
#include "fyc/typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

union fy_stack_item {
	fy_uint uvalue;
	fy_int ivalue;
	fy_float fvalue;
};


struct fy_nativeCall {
	char *methodName;
	fy_uint paramCount;
	fy_stack_item *params;
};

typedef enum fy_messageType {
	/*message_continue = 0, In thread*/
	message_none = 1, /*Thread Only*/
	message_thread_dead = 2, /*Thread Only*/
	message_invoke_native = 3,/*Thread And TM pass thread*/
	/*message_exception = 4, Thread And TM pass thread*/
	message_sleep = 5, /*TM Only*/
	message_vm_dead = 6
	/*TM Only*/
}fy_messageType;

struct fy_message {
	fy_messageType messageType;
	fy_int threadId;
	/*We care more about stability than some hundreds bytes of memory*/
	struct {
		fy_nativeCall call;
		fy_long sleepTime;
	}body;

};

FY_ATTR_EXPORT fy_context *fisceAllocateContext();

FY_ATTR_EXPORT void fisceInitContext(fy_context *context,
		fy_exception *exception);

FY_ATTR_EXPORT void fisceDestroyContext(fy_context *context);

FY_ATTR_EXPORT void fisceBootFromMain(fy_context *context, const char *name,
		fy_exception *exception);

FY_ATTR_EXPORT void fisceBootFromData(fy_context *context,
                                      fy_exception *exception);

FY_ATTR_EXPORT void fisceSave(fy_context *context,
                                  fy_exception *exception);

FY_ATTR_EXPORT void fisceRun(fy_context *context, fy_message *message,
		fy_exception *exception);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_PUBLIC_FISCE_H_ */
