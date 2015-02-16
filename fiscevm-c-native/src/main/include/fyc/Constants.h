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
#ifndef MAIN_INCLUDE_FYC_CONSTANTS_H_
#define MAIN_INCLUDE_FYC_CONSTANTS_H_

/*Access flags*/
#define FY_ACC_ABSTRACT 1024
#define FY_ACC_FINAL 16
#define FY_ACC_INTERFACE 512
#define FY_ACC_NATIVE 256
#define FY_ACC_PRIVATE 2
#define FY_ACC_PROTECTED 4
#define FY_ACC_PUBLIC 1
#define FY_ACC_STATIC 8
#define FY_ACC_STRICT 2048
#define FY_ACC_SUPER 32
#define FY_ACC_SYNCHRONIZED 32
#define FY_ACC_TRANSIENT 128
#define FY_ACC_VOLATILE 64
#define FY_ACC_VARARGS 128
#define FY_ACC_BRIDGE 64
/*Extended access flags*/
#define FY_ACC_SYNTHETIC	0x00001000
#define FY_ACC_ANNOTATION	0x00002000
#define FY_ACC_ENUM			0x00004000
#define FY_ACC_SOFT_REF		0x00008000
#define FY_ACC_WEAK_REF		0x00010000
#define FY_ACC_PHANTOM_REF	0x00020000
#define FY_ACC_CONSTRUCTOR	0x00100000
#define FY_ACC_CLINIT		0x00200000
#define FY_ACC_VERIFIED		0x80000000

#define FY_TM_STATE_NEW  0
#define FY_TM_STATE_BOOT_PENDING  1
#define FY_TM_STATE_STOP  2
#define FY_TM_STATE_RUN_PENDING  3
#define FY_TM_STATE_RUNNING  4
#define FY_TM_STATE_STOP_PENDING  5
#define FY_TM_STATE_DEAD_PENDING  6
#define FY_TM_STATE_DEAD  7

#define FY_CLINIT_DONE 0
#define FY_CLINIT_WAIT -1
#define FY_CLINIT_INVOKE 1

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


#endif /* MAIN_INCLUDE_FYC_CONSTANTS_H_ */
