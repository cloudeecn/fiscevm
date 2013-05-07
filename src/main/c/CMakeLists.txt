#
# Copyright 2010-2013 Yuxuan Huang. All rights reserved.
#
# This file is part of libfisce.
#
# libfisce is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# libfisce is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required (VERSION 2.6)
project (fiscevm-c-native)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(SRC_LIST_LIB
	Debug.c
    HashMap.c
    HashMapI.c
    ArrList.c
    LnkList.c
    MemMan.c
    Portable.c
    String.c
    BitSet.c
    Utf8.c
    Class.c
    ClassLoader.c
    CoreHandlers.c
    MathHandlers.c
    Data.c
    Heap.c
    Instructions.c
    Preverifier.c
    Thread.c
    ThreadManager.c
    VMContext.c
    ExpDev.c
    FiScE.c
    FileInputStream.c
    BinarySaver.c
	DataLoader.c
	BAIS.c
	RIS.c
)
SET(SRC_LIST_JNI
	FisceService.c
)
SET(SRC_LIST_TEST
	Test.c
)

configure_file (
	"fiscecon.h.in"
	"${PROJECT_BINARY_DIR}/fiscecon.h"
)

#include_directories("src/main/inc")
include_directories("${PROJECT_BINARY_DIR}")

option (ENABLE_VERBOSE "Verbose output" OFF)
option (ENABLE_TEST "Generate test binary" OFF)
option (ENABLE_LIBS "Generate libraries" ON)
option (ENABLE_32 "Enable 32bit" ON)
option (ENABLE_64 "Enable 64bit" ON)
option (ENABLE_JNI "Enable fyjni build" ON)
if(EMSCRIPTEN)
    message("Using emscripten default configure...")
    set(ENABLE_32 ON)
    set(ENABLE_64 OFF)
    set(ENABLE_JNI OFF)
    set(HAS_M32 ON)
    set(HAS_MATH OFF)
    set(HAS_WIN32 OFF)
    set(HAS_LTO ON)
else(EMSCRIPTEN)
if(ANDROID)
    set(HAS_MATH ON)
    set(HAS_LTO ON)
else(ANDROID)
    if(ENABLE_32)
    try_compile(HAS_M32 ${PROJECT_BINARY_DIR}/detectors/params32 ${PROJECT_SOURCE_DIR}/detectors/params
    	params-test
    	CMAKE_FLAGS "-DTEST_PARAM=-m32"
    )
    endif(ENABLE_32)
    if(ENABLE_64)
    try_compile(HAS_M64 ${PROJECT_BINARY_DIR}/detectors/params64 ${PROJECT_SOURCE_DIR}/detectors/params
    	params-test
    	CMAKE_FLAGS "-DTEST_PARAM=-m64"
    )
    endif(ENABLE_64)
    try_compile(HAS_MATH ${PROJECT_BINARY_DIR}/detectors/paramsm ${PROJECT_SOURCE_DIR}/detectors/params
        params-test
        CMAKE_FLAGS "-DTEST_PARAM=-lm"
    )
    
    try_compile(HAS_LTO ${PROJECT_BINARY_DIR}/detectors/paramslto ${PROJECT_SOURCE_DIR}/detectors/params
        params-test
        CMAKE_FLAGS "-DTEST_PARAM=-flto"
    )
        
    try_compile(HAS_WIN32 ${PROJECT_BINARY_DIR}/detectors/win32 ${PROJECT_SOURCE_DIR}/detectors/win32
        win32-test
    )
    endif(ANDROID)
endif(EMSCRIPTEN)

if(HAS_LTO)
    message("Link time optimization supported")
    set(FLAG_LTO "-flto")
endif(HAS_LTO)
        
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFY_DEBUG -g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 ${FLAG_LTO}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g3 -O2 ${FLAG_LTO}")

set(CMAKE_LINK_FLAGS_DEBUG "${CMAKE_LINK_FLAGS_DEBUG}")
set(CMAKE_LINK_FLAGS_RELEASE "${CMAKE_LINK_FLAGS_RELEASE} -O3 ${FLAG_LTO}")
set(CMAKE_LINK_FLAGS_RELWITHDEBINFO "${CMAKE_LINK_FLAGS_RELWITHDEBINFO} -O2     ${FLAG_LTO}")

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(ENABLE_VERBOSE)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFY_VERBOSE")
ENDIF(ENABLE_VERBOSE)

if(HAS_WIN32)
    message("Using win32 params")
    set(W32_ADD_FLAGS "-Wl,--kill-at")
endif(HAS_WIN32)

if(HAS_MATH)
    message("Using external math library")
endif(HAS_MATH)

if(ENABLE_TEST)
    message("Will generate test binary")
    ADD_EXECUTABLE(fisce-test ${SRC_LIST_LIB} ${SRC_LIST_TEST})
    if(HAS_MATH)
    	TARGET_LINK_LIBRARIES(fisce-test m)
    endif(HAS_MATH)
    set_property(TARGET fisce-test
    	APPEND PROPERTY
    		COMPILE_DEFINITIONS FY_STATIC
    )
endif(ENABLE_TEST)

if(ENABLE_LIBS)
    if(NOT (HAS_M32 OR HAS_M64))
        message("Will generate default binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_LIB})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fisce m)
        endif(HAS_MATH)
        set_property(TARGET fisce
            APPEND PROPERTY
                COMPILE_DEFINITIONS FY_EXPORT
        )
        
        if(ENABLE_JNI)
            message("Will generate default binary with jni")
            ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
            if(HAS_MATH)
                TARGET_LINK_LIBRARIES(fyjni m)
            endif(HAS_MATH)
            set_property(TARGET fyjni
                APPEND PROPERTY
                    COMPILE_DEFINITIONS FY_EXPORT
            )
            set_target_properties(fyjni
                PROPERTIES
                   LINK_FLAGS "${W32_ADD_FLAGS} -Wl,--no-undefined"
            )
        endif(ENABLE_JNI)
    endif(NOT (HAS_M32 OR HAS_M64))
    
    if(HAS_M32)
        message("Will generate 32bit binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_LIB})
         if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce m)
        endif(HAS_MATH)
        set_property(TARGET fisce
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce
            PROPERTIES
    		    COMPILE_FLAGS "-m32"
		        LINK_FLAGS "-m32 -Wl,--no-undefined"
        )
        
        if(ENABLE_JNI)
            message("Will generate 32bit binary with jni")
            ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
            if(HAS_MATH)
	            TARGET_LINK_LIBRARIES(fyjni m)
            endif(HAS_MATH)
            set_property(TARGET fyjni
            	APPEND PROPERTY
    		        COMPILE_DEFINITIONS FY_EXPORT
            )
            set_target_properties(fyjni
                PROPERTIES
		            COMPILE_FLAGS "-m32"
		            LINK_FLAGS "-m32 ${W32_ADD_FLAGS} -Wl,--no-undefined"
            )
        endif(ENABLE_JNI)
    endif(HAS_M32)
    
    if(HAS_M64)
        message("Will generate 64bit binary")
        ADD_LIBRARY(fisce64 MODULE ${SRC_LIST_LIB})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce64 m)
        endif(HAS_MATH)
        set_property(TARGET fisce64
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce64
            PROPERTIES
    		    COMPILE_FLAGS "-m64"
		        LINK_FLAGS "-m64 -Wl,--no-undefined"
        )
        
        if(ENABLE_JNI)
            message("Will generate 64brit binary with jni")
            ADD_LIBRARY(fyjni64 MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
            if(HAS_MATH)
    	        TARGET_LINK_LIBRARIES(fyjni64 m)
            endif(HAS_MATH)
            set_property(TARGET fyjni64
            	APPEND PROPERTY
    		        COMPILE_DEFINITIONS FY_EXPORT
            )
            set_target_properties(fyjni64
                PROPERTIES
		            COMPILE_FLAGS "-m64"
		            LINK_FLAGS "-m64 ${W32_ADD_FLAGS} -Wl,--no-undefined"
            )
        endif(ENABLE_JNI)
    endif(HAS_M64)
endif(ENABLE_LIBS)
