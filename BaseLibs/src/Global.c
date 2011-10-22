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

#include "fyc/Global.h"

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

void fy_gInit() {
	fy_gCBoolean = vm_allocate(sizeof(jchar));
	fy_gCByte = vm_allocate(sizeof(jchar));
	fy_gCShort = vm_allocate(sizeof(jchar));
	fy_gCChar = vm_allocate(sizeof(jchar));
	fy_gCInt = vm_allocate(sizeof(jchar));
	fy_gCFloat = vm_allocate(sizeof(jchar));
	fy_gCLong = vm_allocate(sizeof(jchar));
	fy_gCDouble = vm_allocate(sizeof(jchar));
	fy_gCArray = vm_allocate(sizeof(jchar));
	*fy_gCBoolean = TYPE_BOOLEAN;
	*fy_gCByte = TYPE_BYTE;
	*fy_gCShort = TYPE_SHORT;
	*fy_gCChar = TYPE_CHAR;
	*fy_gCInt = TYPE_INT;
	*fy_gCFloat = TYPE_FLOAT;
	*fy_gCLong = TYPE_LONG;
	*fy_gCDouble = TYPE_DOUBLE;
	*fy_gCArray = TYPE_ARRAY;
	
	fy_gInited = 0x1234567890abcdefl;
}
void fy_gDestroy() {
	fy_gInited = 0;
	vm_free(fy_gCBoolean);
	vm_free(fy_gCByte);
	vm_free(fy_gCShort);
	vm_free(fy_gCChar);
	vm_free(fy_gCInt);
	vm_free(fy_gCFloat);
	vm_free(fy_gCLong);
	vm_free(fy_gCDouble);
	vm_free(fy_gCArray);
}
