/**
 *  Copyright 2010-2015 Yuxuan Huang. All rights reserved.
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

#include "fy_util/Portable.h"
#include "fyc/typedefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

fisce_ubyte fy_dataRead(fy_context *context, fy_inputStream *is, fisce_exception *exception);
fisce_char fy_dataRead2(fy_context *context, fy_inputStream *is, fisce_exception *exception);
fisce_uint fy_dataRead4(fy_context *context, fy_inputStream *is, fisce_exception *exception);
fisce_ulong fy_dataRead8(fy_context *context, fy_inputStream *is, fisce_exception *exception);
void fy_dataReadBlock(fy_context *context, fy_inputStream* is,
        void* buffer, fisce_int size, fisce_exception *exception);
void fy_dataSkip(fy_context *context, fy_inputStream *is, int size,
		fisce_exception *exception);

#ifdef	__cplusplus
}
#endif

#endif	/* FY_DATA_H */

