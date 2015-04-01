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

#ifndef FYC_DEBUG_H_
#define FYC_DEBUG_H_

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fisce.h"
#include "fy_util/String.h"
#include "fy_util/Debug.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef FY_DEBUG

#define   DLOG   \
		context->logDVar(context, "%s(%s:%d):D: ",__FUNCTION__,   __FILE__,   __LINE__); \
        context->logDVarLn
#define   ILOG   \
		context->logIVar(context, "%s(%s:%d):I: ",__FUNCTION__,   __FILE__,   __LINE__); \
        context->logIVarLn
#define   WLOG   \
		context->logWVar(context, "%s(%s:%d):W: ",__FUNCTION__,   __FILE__,   __LINE__); \
        context->logWVarLn
#define   ELOG   \
		context->logEVar(context, "%s(%s:%d):E: ",__FUNCTION__,   __FILE__,   __LINE__); \
        context->logEVarLn
#else

#define DLOG context->logDVarLn
#define ILOG context->logIVarLn
#define WLOG context->logWVarLn
#define ELOG context->logEVarLn

#endif

FY_ATTR_EXPORT void fy_debugInit(fy_context *context);

#ifdef	__cplusplus
}
#endif
#endif /* FYC_DEBUG_H_ */
