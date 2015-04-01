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
#ifndef FY_BAIS_H_
#define FY_BAIS_H_

#include "fy_util/Portable.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

FY_ATTR_EXPORT fy_inputStream *fy_baisOpenByteArrayInputStream(
		fy_context *context, void *buffer, fisce_int bufferLen,
		fisce_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif /* ARRLIST_H_ */
