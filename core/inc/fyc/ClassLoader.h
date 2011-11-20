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

#ifndef FY_CLASSLOADER_H
#define	FY_CLASSLOADER_H

#include "Portable.h"
#include "Structs.h"
#include "VMContext.h"
#include "String.h"
#include "Class.h"
#include "Data.h"
#include "Resource.h"
#include "Debug.h"

#define fy_SIZE_SHIFT_BYTE 0 /*1 bytes*/
#define fy_SIZE_SHIFT_INT 2 /*4 bytes*/
#define fy_SIZE_SHIFT_LONG 3 /*8 bytes*/

#ifdef	__cplusplus
extern "C" {
#endif

fy_str *fy_clGetConstantString(fy_VMContext *context, fy_class *clazz,
		jchar idx);
fy_class *fy_clLoadclass(fy_VMContext *context, fy_str *name,
		fy_exception *exception);
void fy_clPhase2(fy_VMContext *context, fy_class *clazz,
		fy_exception *exception);

fy_data *fy_clOpenResource(fy_VMContext *context, fy_str *name);
void fy_clCloseResource(fy_VMContext *context, fy_data *data);

#ifdef	__cplusplus
}
#endif

#endif	/* FY_CLASSLOADER_H */

