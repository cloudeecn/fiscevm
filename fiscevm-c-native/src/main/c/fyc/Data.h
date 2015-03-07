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

#ifndef FY_DATA_H
#define	FY_DATA_H

#include "../fiscestu.h"
#include "../fisceprt.h"

#ifdef	__cplusplus
extern "C" {
#endif

fy_ubyte fy_dataRead(fy_context *context, fy_inputStream *is, fy_exception *exception);
fy_char fy_dataRead2(fy_context *context, fy_inputStream *is, fy_exception *exception);
fy_uint fy_dataRead4(fy_context *context, fy_inputStream *is, fy_exception *exception);
fy_ulong fy_dataRead8(fy_context *context, fy_inputStream *is, fy_exception *exception);
void fy_dataReadBlock(fy_context *context, FY_ATTR_RESTRICT fy_inputStream* is,
		FY_ATTR_RESTRICT void* buffer, fy_int size, fy_exception *exception);
void fy_dataSkip(fy_context *context, fy_inputStream *is, int size,
		fy_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif	/* FY_DATA_H */
