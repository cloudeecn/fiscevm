#
# Copyright 2010-2015 Yuxuan Huang. All rights reserved.
#
# This file is part of fiscevm.
#
# fiscevm is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# fiscevm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with fiscevm.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required (VERSION 2.6)
project (libfisce)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include(CheckCCompilerFlag)

include_directories(main/include main/include-public main/include-dev)
#include_directories("${PROJECT_BINARY_DIR}")

SET(SRC_LIST_UTILS
    main/utils/Portable.c
    main/utils/MemMan.c
    main/utils/Utf8.c
    main/utils/String.c
    main/utils/BitSet.c
    main/utils/HashMap.c
    main/utils/HashMapI.c
    main/utils/ArrList.c
    main/utils/LnkList.c
    main/utils/Debug.c
)

SET(SRC_LIST_LIB
    main/vmcore/Class.c
    main/vmcore/ClassLoader.c
    main/vmcore/CoreHandlers.c
    main/vmcore/MathHandlers.c
    main/vmcore/Debug.c
    main/vmcore/Data.c
    main/vmcore/Heap.c
    main/vmcore/Instructions.c
    main/vmcore/Preverifier.c
    main/vmcore/Thread.c
    main/vmcore/ThreadManager.c
    main/vmcore/VMContext.c
    main/vmcore/ExpDev.c
    main/vmcore/FiScE.c
    main/vmcore/FileInputStream.c
    main/vmcore/BinarySaver.c
	main/vmcore/DataLoader.c
	main/vmcore/BAIS.c
	main/vmcore/RIS.c
    main/vmcore/Engine.c
)

SET(SRC_LIST_JNI
	main/vmcore/JNIService.c
)

SET(SRC_LIST_TEST
	test/vmcore/Test.c
)

set(BUILD_TEST OFF CACHE BOOL "Should generate test binary")
set(BUILD_LIB ON CACHE BOOL "Should generate shared libraries(libfisce.so / libfisce64.so)")
set(BUILD_JNI ON CACHE BOOL "Should generate JNI libraries(libfyjni.so / libfyjni64.so)") 

set(ENABLE_32 ON CACHE BOOL "Should build 32bit binaries")
set(ENABLE_64 ON CACHE BOOL "Should build 32bit binaries")

set(OPTIMIZE_LTO ON CACHE BOOL "Should build 32bit binaries")
set(OPTIMIZE_BTO ON CACHE BOOL "Should use branch optimization (ie. -fbranch-target-load-optimize2 -fbtr-bb-exclusive)")
set(OPTIMIZE_LEVEL "-Ofast" CACHE STRING "Optimize level when using release mode, -O3 -Os -Ofast")
SET_PROPERTY(CACHE OPTIMIZE_LEVEL PROPERTY STRINGS "-O3" "-Os" "-Ofast")

set(DEBUG_VERBOSE OFF CACHE BOOL "The binary built will print all opcodes executed. (Very slow!) For debug only")

set(DISPATCH_MODE 2 CACHE STRING "0 for traditional switch-based dispatch, 2 for threading dispatch (supports GCC only)")
SET_PROPERTY(CACHE DISPATCH_MODE PROPERTY STRINGS 0 2) 

set(REPL_MIN 5000 CACHE STRING "How many instructions will be proceed before engine will decide an instruction' replica target")

set(THREADING_SCHEME 10 CACHE STRING "Threading dispatch mode, usable: 1 3 4 5 6 8 9 10. Please try the best on your machine")

set(ENGINE_COUNT 1 CACHE STRING "How many engine function generated")

set(ANSY_ONLY OFF CACHE BOOLEAN "Only use ansi c")


if(EMSCRIPTEN)
    message("Using emscripten's default configuration...")
    set(BUILD_JNI OFF)
    
    set(ENABLE_32 ON)
    set(ENABLE_64 OFF)
    
    set(HAS_MATH OFF)
    
    set(OPTIMIZE_LTO ON)
    set(OPTIMIZE_BTO OFF)
elseif(ANDROID)
	set(HAS_MATH ON)
else()
	CHECK_C_COMPILER_FLAG("-m32" HAS_M32)
	if(NOT HAS_M32)
		message("Disable explicit 32bit build as -m32 is not supported")
		set(ENABLE_32 OFF)
	endif()
	CHECK_C_COMPILER_FLAG("-m64" HAS_M64)
	if(NOT HAS_M64)
		message("Disable explicit 64bit build as -m64 is not supported")
		set(ENABLE_64 OFF)
	endif()
	
	CHECK_C_COMPILER_FLAG("-lm" HAS_MATH)
	
	CHECK_C_COMPILER_FLAG("-flto" HAS_LTO)
	if(NOT HAS_LTO)
		message("Disable lto as -flto is not supported")
		set(OPTIMIZE_LTO OFF)
	endif()
	
	CHECK_C_COMPILER_FLAG("-fbranch-target-load-optimize2 -fbtr-bb-exclusive" HAS_BTO)
	if(NOT HAS_BTO)
		message("Disable branch target load optimization as flags is not supported")
		set(OPTIMIZE_BTO OFF)
	endif()
endif()

message("Result: following options enabled:")

if(OPTIMIZE_BTO)
	message("Branch target load optimize")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fbranch-target-load-optimize2 -fbtr-bb-exclusive")
	set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} -fbranch-target-load-optimize2 -fbtr-bb-exclusive")
endif()

if(OPTIMIZE_LTO)
	message("Link time optimize")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
	set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} -flto")
endif()

message("DISPATCH_MODE=${DISPATCH_MODE}")
message("REPL_MIN=${REPL_MIN}")
message("THREADING_SCHEME=${THREADING_SCHEME}")
message("ENGINE_COUNT=${ENGINE_COUNT}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFY_DISPATCH_MODE=${DISPATCH_MODE} -DREPL_MIN=${REPL_MIN} -DTHREADING_SCHEME=${THREADING_SCHEME} -DENGINE_COUNT=${ENGINE_COUNT}")

set(CMAKE_C_FLAGS_DEBUG "-DFY_DEBUG -g3")
set(CMAKE_C_FLAGS_RELEASE "${OPTIMIZE_LEVEL} -g0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og")

set(CMAKE_LINK_FLAGS_DEBUG "-g3")
set(CMAKE_LINK_FLAGS_RELEASE "${OPTIMIZE_LEVEL} -g0")
set(CMAKE_LINK_FLAGS_RELWITHDEBINFO "-Og")

IF(ANSI_ONLY)
message("Use pedantic ANSI C89 with long long support")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wno-long-long -Werror -DFY_STRICT_DECLARATION")
ENDIF(ANSI_ONLY)

IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(ENABLE_VERBOSE)
	message("Verbose output enabled")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFY_VERBOSE")
ENDIF(ENABLE_VERBOSE)

set(JNI_ADD_FLAGS "")

if(WIN32)
#######Win32
    message("###Win32###")
    set(JNI_ADD_FLAGS "-Wl,--kill-at")
    set(CUSTOM_JNI_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
#######/Win32
elseif(APPLE)
#######Apple
	message("###Apple###")
	set(CUSTOM_JNI_EXT ".jnilib")
#######/Apple
elseif(UNIX)
#######Unix
	message("###*nix###")
#######/Unix
else()
#######Other
	message("###Other###")
#######/Other
endif()


if(HAS_MATH)
    message("Using external math library")
endif(HAS_MATH)

if(BUILD_TEST)
	if((NOT ENABLE_32) AND (NOT ENABLE_64))
        message("Will generate default test binary")
        ADD_EXECUTABLE(fisce-test ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_TEST})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fisce-test m)
        endif()
        set_property(TARGET fisce-test
            APPEND PROPERTY
                COMPILE_DEFINITIONS FY_STATIC
        )
    endif()
    
    if(ENABLE_32)
        message("Will generate 32bit test binary")
        ADD_EXECUTABLE(fisce-test ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_TEST})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce-test m)
        endif()
        set_property(TARGET fisce-test
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_STATIC
        )
        set_target_properties(fisce-test
            PROPERTIES
    		    COMPILE_FLAGS "-m32"
		        LINK_FLAGS "-m32"
        )
    endif()
    
    if(ENABLE_64)
        message("Will generate 64bit test binary")
        ADD_EXECUTABLE(fisce-test64 ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_TEST})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce-test64 m)
        endif()
        set_property(TARGET fisce-test64
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce-test64
            PROPERTIES
    		    COMPILE_FLAGS "-m64"
		        LINK_FLAGS "-m64"
        )
    endif()
endif()

if(BUILD_LIBS)
    if(NOT (ENABLE_32 OR ENABLE_64))
        message("Will generate default binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fisce m)
        endif()
        set_property(TARGET fisce
            APPEND PROPERTY
                COMPILE_DEFINITIONS FY_EXPORT
        )
    endif()
    
    if(ENABLE_32)
        message("Will generate 32bit binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce m)
        endif()
        set_property(TARGET fisce
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce
            PROPERTIES
    		    COMPILE_FLAGS "-m32"
		        LINK_FLAGS "-m32 ${NO_UNDEFINED}"
        )
    endif()
    
    if(ENABLE_64)
        message("Will generate 64bit binary")
        ADD_LIBRARY(fisce64 MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce64 m)
        endif()
        set_property(TARGET fisce64
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce64
            PROPERTIES
    		    COMPILE_FLAGS "-m64"
		        LINK_FLAGS "-m64 ${NO_UNDEFINED}"
        )
    endif()
endif()

if(BUILD_JNI)
	if(NOT (ENABLE_32 OR ENABLE_32))
        message("Will generate default binary with jni")
        ADD_LIBRARY(fyjni MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fyjni m)
        endif()
        set_property(TARGET fyjni
            APPEND PROPERTY
                COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fyjni
            PROPERTIES
               LINK_FLAGS "${JNI_ADD_FLAGS} ${NO_UNDEFINED}"
        )
        if(DEFINED CUSTOM_JNI_EXT)
     		set_target_properties(fyjni
            	PROPERTIES
               		SUFFIX "${CUSTOM_JNI_EXT}"
        	)
        endif()
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif()
	endif()
    
	if(ENABLE_32)
		message("Will generate 32bit binary with jni")
		ADD_LIBRARY(fyjni MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
		if(HAS_MATH)
			TARGET_LINK_LIBRARIES(fyjni m)
		endif()
		set_property(TARGET fyjni
			APPEND PROPERTY
				COMPILE_DEFINITIONS FY_EXPORT
		)
		set_target_properties(fyjni
			PROPERTIES
				COMPILE_FLAGS "-m32"
				LINK_FLAGS "-m32 ${JNI_ADD_FLAGS} ${NO_UNDEFINED}"
		)
		if(DEFINED CUSTOM_JNI_EXT)
     		set_target_properties(fyjni
            	PROPERTIES
               		SUFFIX "${CUSTOM_JNI_EXT}"
        	)
        endif()
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif()
	endif()
    
	if(ENABLE_64)
		message("Will generate 64bit binary with jni")
		ADD_LIBRARY(fyjni64 MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
		if(HAS_MATH)
			TARGET_LINK_LIBRARIES(fyjni64 m)
		endif()
		set_property(TARGET fyjni64
			APPEND PROPERTY
				COMPILE_DEFINITIONS FY_EXPORT
		)
		set_target_properties(fyjni64
			PROPERTIES
				COMPILE_FLAGS "-m64"
				LINK_FLAGS "-m64 ${JNI_ADD_FLAGS} ${NO_UNDEFINED}"
		)
        if(DEFINED CUSTOM_JNI_EXT)
     		set_target_properties(fyjni64
            	PROPERTIES
               		SUFFIX "${CUSTOM_JNI_EXT}"
        	)
        endif()
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni64
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif()
	endif()    
endif()
