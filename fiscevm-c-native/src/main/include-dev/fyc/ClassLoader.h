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

#ifndef FY_CLASSLOADER_H
#define	FY_CLASSLOADER_H

#include "fy_util/Portable.h"
#include "fyc/ClassStruct.h"
#include "fyc/VMContext.h"

#ifdef	__cplusplus
extern "C" {
#endif
void fy_clDefineClass(fy_context *context, fy_str *name, fy_byte *data,
		fy_int dataLen, fy_exception *exception);
fy_str *fy_clGetConstantString(fy_context *context, fy_class *clazz,
		fy_char idx);
fy_class *fy_clLoadclass(fy_context *context, fy_str *name,
		fy_exception *exception);
void fy_clPhase2(fy_context *context, fy_class *clazz, fy_exception *exception);
#ifdef	__cplusplus
}
#endif
#endif	/* FY_CLASSLOADER_H */

