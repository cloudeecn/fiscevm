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
#ifndef FY_CLASS_H_
#define FY_CLASS_H_
#include "Portable.h"
#include "Structs.h"
#include "VMContext.h"
#include "String.h"
#include "Debug.h"
#define CONSTANT_Class	 7
#define CONSTANT_Fieldref	 9
#define CONSTANT_Methodref	 10
#define CONSTANT_InterfaceMethodref	 11
#define CONSTANT_String	 8
#define CONSTANT_Integer	 3
#define CONSTANT_Float	 4
#define CONSTANT_Long	 5
#define CONSTANT_Double	 6
#define CONSTANT_NameAndType	 12
#define CONSTANT_Utf8	 1
#ifdef	__cplusplus
extern "C" {
#endif

jboolean fy_classCanCastTo(fy_VMContext *context, fy_class *this,
		fy_class *other);
jboolean fy_classIsSuperClassOf(fy_VMContext *context, fy_class *this,
		fy_class *other);

#ifdef	__cplusplus
}
#endif

#endif /* FY_CLASS_H_ */
