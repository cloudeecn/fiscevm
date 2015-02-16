/*  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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

#ifndef MAIN_INCLUDE_FYC_INPUTSTREAM_H_
#define MAIN_INCLUDE_FYC_INPUTSTREAM_H_

#include "fisce.h"
#include "fyc/VMContext.h"


#ifdef	__cplusplus
extern "C" {
#endif

typedef struct fy_context fy_context;
typedef struct fy_inputStream fy_inputStream;

struct fy_inputStream {
	void* data;
	fy_int (*isRead)(struct fy_context *context, struct fy_inputStream *is,
			fy_exception *exception);
	fy_int (*isReadBlock)(struct fy_context *context, struct fy_inputStream *is,
			void *target, fy_int size, fy_exception *exception);
	fy_int (*isSkip)(struct fy_context *context, struct fy_inputStream *is,
			fy_int size, fy_exception *exception);
	void (*isClose)(struct fy_context *context, struct fy_inputStream *is,
			fy_exception *exception);
};
#ifdef	__cplusplus
|
#endif

#endif /* MAIN_INCLUDE_FYC_INPUTSTREAM_H_ */
