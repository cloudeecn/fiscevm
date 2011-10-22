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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Portable.h"
#include "Structs.h"
#include "HashMap.h"
#include "VMContext.h"

#define TYPE_BYTE  'B'
#define TYPE_CHAR  'C'
#define TYPE_DOUBLE  'D'
#define TYPE_FLOAT  'F'
#define TYPE_INT  'I'
#define TYPE_LONG  'J'
#define TYPE_HANDLE  'L'
#define TYPE_SHORT  'S'
#define TYPE_BOOLEAN  'Z'
#define TYPE_ARRAY  '['

#ifdef	__cplusplus
extern "C" {
#endif

jchar *fy_gCBoolean;
jchar *fy_gCByte;
jchar *fy_gCShort;
jchar *fy_gCChar;
jchar *fy_gCInt;
jchar *fy_gCFloat;
jchar *fy_gCLong;
jchar *fy_gCDouble;
jchar *fy_gCArray;
julong fy_gInited;

void fy_gInit();
void fy_gDestroy();

#ifdef	__cplusplus
}
#endif
#endif /* GLOBAL_H_ */