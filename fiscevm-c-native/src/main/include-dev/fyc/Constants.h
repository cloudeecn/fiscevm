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

/*Prm types*/
#define FY_TYPE_BYTE  'B'
#define FY_TYPE_CHAR  'C'
#define FY_TYPE_DOUBLE  'D'
#define FY_TYPE_FLOAT  'F'
#define FY_TYPE_LONG  'J'
#define FY_TYPE_SHORT  'S'
#define FY_TYPE_BOOLEAN  'Z'
#define FY_TYPE_ARRAY  '['
/*Below are shared by thread and context*/
#define FY_TYPE_INT  'I'
#define FY_TYPE_HANDLE  'L'
/*Below are used only by thread*/
#define FY_TYPE_WIDE  'W'
#define FY_TYPE_RETURN  'R'
#define FY_TYPE_WIDE2  '_'
#define FY_TYPE_UNKNOWN  'X'
#define FY_TYPE_VOID  'V'

/*Methods*/
#define FY_METHOD_INIT "<init>"
#define FY_METHOD_CLINIT "<clinit>"
#define FY_METHODF_MAIN ".main.([L"FY_BASE_STRING";)V"
#define FY_METHODF_RUN ".run.()V"
#define FY_METHODF_FINALIZE ".finalize.()V"
#define FY_FIELDF_PRIORITY ".priority.I"
#define FY_FIELDF_NAME ".name.[C"
#define FY_FIELDF_DAEMON ".daemon.Z"
#define FY_ATT_CODE "Code"
#define FY_ATT_LINENUM "LineNumberTable"
#define FY_ATT_STACK_MAP_TABLE "StackMapTable"
#define FY_ATT_SYNTH "Synthetic"
#define FY_ATT_SOURCE_FILE "SourceFile"
#define FY_ATT_CONSTANT_VALIE "ConstantValue"

/*Core classes*/
#define FY_BASE_VM "com/cirnoworks/fisce/privat/FiScEVM"
#define FY_BASE_STRING "java/lang/String"
#define FY_BASE_ENUM "java/lang/Enum"
#define FY_BASE_ANNOTATION "java/lang/annotation/Annotation"
#define FY_BASE_STRING_BUILDER "java/lang/StringBuilder"
#define FY_BASE_OBJECT "java/lang/Object"
#define FY_BASE_CLASS "java/lang/Class"
#define FY_BASE_CLASSLOADER "java/lang/ClassLoader"
#define FY_BASE_THROWABLE "java/lang/Throwable"
#define FY_BASE_THREAD "java/lang/Thread"
#define FY_BASE_SYSTEM "java/lang/System"
#define FY_BASE_RUNTIME "java/lang/Runtime"
#define FY_BASE_BOOLEAN "java/lang/Boolean"
#define FY_BASE_BYTE "java/lang/Byte"
#define FY_BASE_CHAR "java/lang/Character"
#define FY_BASE_SHORT "java/lang/Short"
#define FY_BASE_INT "java/lang/Integer"
#define FY_BASE_LONG "java/lang/Long"
#define FY_BASE_FLOAT "java/lang/Float"
#define FY_BASE_DOUBLE "java/lang/Double"
#define FY_BASE_MATH "java/lang/Math"
#define FY_BASE_FINALIZER "java/lang/Finalizer"
#define FY_BASE_RUNTIME "java/lang/Runtime"

#define FY_IO_INPUTSTREAM "java/io/InputStream"
#define FY_IO_PRINTSTREAM "java/io/PrintStream"

#define FY_REFLECT_ARRAY 		"java/lang/reflect/Array"
#define FY_REFLECT_CONSTRUCTOR 	"java/lang/reflect/Constructor"
#define FY_REFLECT_FIELD 		"java/lang/reflect/Field"
#define FY_REFLECT_METHOD 		"java/lang/reflect/Method"
#define FY_REFLECT_PROXY 		"java/lang/reflect/Proxy"

#define FY_REF_SOFT			"java/lang/ref/SoftReference"
#define FY_REF_WEAK			"java/lang/ref/WeakReference"
#define FY_REF_PHANTOM		"java/lang/ref/PhantomReference"
#define FY_REF				"java/lang/ref/Reference"

#define FY_BASE_STACKTHREADELEMENT "java/lang/StackTraceElement"

#define FY_BOX_BOOLEAN FY_BASE_BOOLEAN".valueOf.(Z).L"FY_BASE_BOOLEAN";"
#define FY_BOX_BYTE FY_BASE_BYTE".valueOf.(B).L"FY_BASE_BYTE";"
#define FY_BOX_SHORT FY_BASE_SHORT".valueOf.(S).L"FY_BASE_SHORT";"
#define FY_BOX_CHARACTER FY_BASE_CHARACTER".valueOf.(C).L"FY_BASE_CHARACTER";"
#define FY_BOX_INTEGER FY_BASE_INTEGER".valueOf.(I).L"FY_BASE_INTEGER";"
#define FY_BOX_FLOAT FY_BASE_FLOAT".valueOf.(F).L"FY_BASE_FLOAT";"
#define FY_BOX_LONG FY_BASE_LONG".valueOf.(J).L"FY_BASE_LONG";"
#define FY_BOX_DOUBLE FY_BASE_DOUBLE".valueOf.(D).L"FY_BASE_DOUBLE";"

#define FY_UNBOX_BOOLEAN FY_BASE_BOOLEAN".booleanValue.()Z"
#define FY_UNBOX_BYTE FY_BASE_BYTE".byteValue.()B"
#define FY_UNBOX_SHORT FY_BASE_SHORT".shortValue.()S"
#define FY_UNBOX_CHARACTER FY_BASE_CHAR".charValue.()C"
#define FY_UNBOX_INTEGER FY_BASE_INT".intValue.()I"
#define FY_UNBOX_FLOAT FY_BASE_FLOAT".floatValue.()F"
#define FY_UNBOX_LONG FY_BASE_LONG".longValue.()J"
#define FY_UNBOX_DOUBLE FY_BASE_DOUBLE".doubleValue.()D"

#define FY_VALUE_BOOLEAN FY_BASE_BOOLEAN".value.Z"
#define FY_VALUE_BYTE FY_BASE_BYTE".value.B"
#define FY_VALUE_SHORT FY_BASE_SHORT".value.S"
#define FY_VALUE_CHARACTER FY_BASE_CHAR".value.C"
#define FY_VALUE_INTEGER FY_BASE_INT".value.I"
#define FY_VALUE_FLOAT FY_BASE_FLOAT".value.F"
#define FY_VALUE_LONG FY_BASE_LONG".value.J"
#define FY_VALUE_DOUBLE FY_BASE_DOUBLE".value.D"
/*Exceptions*/
#define FY_EXCEPTION_ITE "java/lang/InvocationTargetException"
#define FY_EXCEPTION_MONITOR "java/lang/IllegalMonitorStateException"
#define FY_EXCEPTION_NO_METHOD "java/lang/NoSuchMethodError"
#define FY_EXCEPTION_NPT "java/lang/NullPointerException"
#define FY_EXCEPTION_ARITHMETIC "java/lang/ArithmeticException"
#define FY_EXCEPTION_INCOMPAT_CHANGE "java/lang/IncompatibleClassChangeError"
#define FY_EXCEPTION_VM "java/lang/VirtualMachineError"
#define FY_EXCEPTION_CAST "java/lang/ClassCastException"
#define FY_EXCEPTION_IO "java/io/IOException"
#define FY_EXCEPTION_FNF "java/io/FileNotFoundException"
#define FY_EXCEPTION_RT "java/lang/RuntimeException"
#define FY_EXCEPTION_IOOB "java/lang/IndexOutOfBoundsException"
#define FY_EXCEPTION_AIOOB "java/lang/ArrayIndexOutOfBoundsException"
#define FY_EXCEPTION_STORE "java/lang/ArrayStoreException"
#define FY_EXCEPTION_CLASSNOTFOUND "java/lang/ClassNotFoundException"
#define FY_EXCEPTION_ABSTRACT "java/lang/AbstractMethodError"
#define FY_EXCEPTION_ACCESS "java/lang/IllegalAccessError"
#define FY_EXCEPTION_NASE "java/lang/NegativeArraySizeException"
#define FY_EXCEPTION_INTR "java/lang/InterruptedException"
#define FY_EXCEPTION_IMSE "java/lang/IllegalMonitorStateException"
#define FY_EXCEPTION_ARGU "java/lang/IllegalArgumentException"

#endif /* MAIN_INCLUDE_FYC_CONSTANTS_H_ */
