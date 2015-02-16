#
# Copyright 2010-2013 Yuxuan Huang. All rights reserved.
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
project (fiscevm-c-native)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(main/include main/include-public main/include-dev)

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
    main/vmcore/Engine.c
    main/vmcore/ThreadManager.c
    main/vmcore/VMContext.c
    main/vmcore/ExpDev.c
    main/vmcore/FiScE.c
    main/vmcore/FileInputStream.c
    main/vmcore/BinarySaver.c
	main/vmcore/DataLoader.c
	main/vmcore/BAIS.c
	main/vmcore/RIS.c
)
SET(SRC_LIST_JNI
	main/vmcore/JNIService.c
)
SET(SRC_LIST_TEST
	test/vmcore/Test.c
)

include_directories("${PROJECT_BINARY_DIR}")

option (ENABLE_VERBOSE "Verbose output" OFF)
option (ENABLE_TEST "Generate test binary" OFF)
option (ENABLE_LIBS "Generate shared libraries" ON)
option (ENABLE_JNI "Enable fyjni build" ON)
option (ENABLE_32 "Enable 32bit" ON)
option (ENABLE_64 "Enable 64bit" ON)
option (ENABLE_IC "Enable instructions count" OFF)
option (ENABLE_LTO "Enable Link time optimization" ON)

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
    endif(ANDROID)
endif(EMSCRIPTEN)

if(HAS_LTO)
    if(ENABLE_LTO)
    message("Link time optimization supported")
    set(FLAG_LTO "-flto")
    endif(ENABLE_LTO)
endif(HAS_LTO)
        
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DFY_DEBUG -g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Ofast ${FLAG_LTO}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -g3 -O2 ${FLAG_LTO}")

set(CMAKE_LINK_FLAGS_DEBUG "${CMAKE_LINK_FLAGS}")
set(CMAKE_LINK_FLAGS_RELEASE "${CMAKE_LINK_FLAGS} -Ofast ${FLAG_LTO}")
set(CMAKE_LINK_FLAGS_RELWITHDEBINFO "${CMAKE_LINK_FLAGS} -O2 ${FLAG_LTO}")

#set(NO_UNDEFINED "-Wl,--no-undefined")

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(ENABLE_VERBOSE)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFY_VERBOSE")
ENDIF(ENABLE_VERBOSE)

IF(ENABLE_IC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFY_INSTRUCTION_COUNT")
ENDIF(ENABLE_IC)

set(JNI_ADD_FLAGS "")

if(WIN32)
#######Win32
    message("###Win32###")
    set(JNI_ADD_FLAGS "-Wl,--kill-at")
    set(CUSTOM_JNI_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
#######/Win32
else(WIN32)
	if(APPLE)
#######Apple
	message("###Apple###")
	set(CUSTOM_JNI_EXT ".jnilib")
#######/Apple
	else(APPLE)
		if(UNIX)
#######Unix
	message("###*nix###")
#######/Unix
		else(UNIX)
#######Other
	message("###Othere###")
#######/Other
		endif(UNIX)
	endif(APPLE)
endif(WIN32)


if(HAS_MATH)
    message("Using external math library")
endif(HAS_MATH)

if(ENABLE_TEST)
    message("Will generate test binary")
    ADD_EXECUTABLE(fisce-test ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_TEST})
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
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fisce m)
        endif(HAS_MATH)
        set_property(TARGET fisce
            APPEND PROPERTY
                COMPILE_DEFINITIONS FY_EXPORT
        )
    endif(NOT (HAS_M32 OR HAS_M64))
    
    if(HAS_M32)
        message("Will generate 32bit binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
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
		        LINK_FLAGS "-m32 ${NO_UNDEFINED}"
        )
    endif(HAS_M32)
    
    if(HAS_M64)
        message("Will generate 64bit binary")
        ADD_LIBRARY(fisce64 MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB})
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
		        LINK_FLAGS "-m64 ${NO_UNDEFINED}"
        )
    endif(HAS_M64)
endif(ENABLE_LIBS)

if(ENABLE_JNI)
	if(NOT (HAS_M32 OR HAS_M64))
        message("Will generate default binary with jni")
        ADD_LIBRARY(fyjni MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
        if(HAS_MATH)
            TARGET_LINK_LIBRARIES(fyjni m)
        endif(HAS_MATH)
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
        endif(DEFINED CUSTOM_JNI_EXT)
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif(DEFINED CUSTOM_JNI_PREFIX)
	endif(NOT (HAS_M32 OR HAS_M64))
    
	if(HAS_M32)
		message("Will generate 32bit binary with jni")
		ADD_LIBRARY(fyjni MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
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
				LINK_FLAGS "-m32 ${JNI_ADD_FLAGS} ${NO_UNDEFINED}"
		)
		if(DEFINED CUSTOM_JNI_EXT)
     		set_target_properties(fyjni
            	PROPERTIES
               		SUFFIX "${CUSTOM_JNI_EXT}"
        	)
        endif(DEFINED CUSTOM_JNI_EXT)
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif(DEFINED CUSTOM_JNI_PREFIX)
	endif(HAS_M32)
    
	if(HAS_M64)
		message("Will generate 64bit binary with jni")
		ADD_LIBRARY(fyjni64 MODULE ${SRC_LIST_UTILS} ${SRC_LIST_LIB} ${SRC_LIST_JNI})
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
				LINK_FLAGS "-m64 ${JNI_ADD_FLAGS} ${NO_UNDEFINED}"
		)
        if(DEFINED CUSTOM_JNI_EXT)
     		set_target_properties(fyjni64
            	PROPERTIES
               		SUFFIX "${CUSTOM_JNI_EXT}"
        	)
        endif(DEFINED CUSTOM_JNI_EXT)
        
        if(DEFINED CUSTOM_JNI_PREFIX)
     		set_target_properties(fyjni64
            	PROPERTIES
               		PREFIX "${CUSTOM_JNI_PREFIX}"
        	)
        endif(DEFINED CUSTOM_JNI_PREFIX)
	endif(HAS_M64)    
endif(ENABLE_JNI)
