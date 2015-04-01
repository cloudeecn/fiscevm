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

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/VMContext.h"


#ifdef	__cplusplus
extern "C" {
#endif

struct fy_inputStream {
	void* data;
	fisce_int (*isRead)(struct fy_context *context, struct fy_inputStream *is,
			fisce_exception *exception);
	fisce_int (*isReadBlock)(struct fy_context *context, struct fy_inputStream *is,
			void *target, fisce_int size, fisce_exception *exception);
	fisce_int (*isSkip)(struct fy_context *context, struct fy_inputStream *is,
			fisce_int size, fisce_exception *exception);
	void (*isClose)(struct fy_context *context, struct fy_inputStream *is,
			fisce_exception *exception);
};
#ifdef	__cplusplus
|
#endif

#endif /* MAIN_INCLUDE_FYC_INPUTSTREAM_H_ */
