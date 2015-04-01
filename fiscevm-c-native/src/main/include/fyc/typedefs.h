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

#ifndef MAIN_INCLUDE_DEV_FYC_TYPEDEFS_H_
#define MAIN_INCLUDE_DEV_FYC_TYPEDEFS_H_

typedef struct fy_nh fy_nh;

typedef struct fy_engine fy_engine;
typedef struct fy_invoke fy_invoke;
typedef struct fy_instruction fy_instruction;
typedef struct fy_instruction_extra fy_instruction_extra;
typedef struct fy_thread fy_thread;
typedef struct fy_frame fy_frame;

typedef struct fy_switch_lookup fy_switch_lookup;
typedef struct fy_switch_table fy_switch_table;


typedef struct ConstantClass ConstantClass;
typedef struct ConstantFieldRef ConstantFieldRef;
typedef struct ConstantMethodRef ConstantMethodRef;
typedef struct ConstantStringInfo ConstantStringInfo;
typedef struct ConstantIntegerFloatInfo ConstantIntegerFloatInfo;
typedef struct ConstantLongDoubleInfo ConstantLongDoubleInfo;
typedef struct ConstantNameAndTypeInfo ConstantNameAndTypeInfo;
typedef struct ConstantUtf8Info ConstantUtf8Info;

typedef struct fy_lineNumber fy_lineNumber;
typedef struct fy_exceptionHandler fy_exceptionHandler;
typedef struct fy_stack_map_table fy_stack_map_table;

typedef struct fy_context fy_context;
typedef struct fy_class fy_class;
typedef struct fy_method fy_method;
typedef struct fy_field fy_field;

typedef struct fy_object fy_object;
typedef struct fy_object_data fy_object_data;
typedef struct fy_inputStream fy_inputStream;

typedef union stringInfo stringInfo;
typedef union classInfo classInfo;


typedef fisce_int (*fy_nhFunction)(
		fy_context *context,
		fy_thread *thread,
		void *data,
		fisce_stack_item *args,
		fisce_int argsCount,
		fisce_int ops,
		fisce_exception *exception
);

#endif /* MAIN_INCLUDE_DEV_FYC_TYPEDEFS_H_ */
