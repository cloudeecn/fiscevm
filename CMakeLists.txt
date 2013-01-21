cmake_minimum_required (VERSION 2.6)
project (fiscevm-c-native)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(SRC_LIST_LIB
	src/main/c/Debug.c
    src/main/c/HashMap.c
    src/main/c/HashMapI.c
    src/main/c/ArrList.c
    src/main/c/LnkList.c
    src/main/c/MemMan.c
    src/main/c/Portable.c
    src/main/c/String.c
    src/main/c/BitSet.c
    src/main/c/Utf8.c
    src/main/c/Class.c
    src/main/c/ClassLoader.c
    src/main/c/CoreHandlers.c
    src/main/c/MathHandlers.c
    src/main/c/Data.c
    src/main/c/Heap.c
    src/main/c/Instructions.c
    src/main/c/Preverifier.c
    src/main/c/Thread.c
    src/main/c/ThreadManager.c
    src/main/c/VMContext.c
    src/main/c/ExpDev.c
    src/main/c/FiScE.c
    src/main/c/FileInputStream.c
    src/main/c/BinarySaver.c
	src/main/c/DataLoader.c
	src/main/c/BAIS.c
)
SET(SRC_LIST_JNI
	src/main/c/FisceService.c
)
SET(SRC_LIST_TEST
	src/main/c/Test.c
)

configure_file (
	"src/main/c/fiscecon.h.in"
	"${PROJECT_BINARY_DIR}/fiscecon.h"
)

include_directories("src/main/c")
include_directories("${PROJECT_BINARY_DIR}")

option (ENABLE_VERBOSE "Verbose output" OFF)
option (ENABLE_TEST "Generate test binary" OFF)
option (ENABLE_LIBS "Generate libraries" ON)

if(ANDROID)
    set(HAS_MATH ON)
    set(HAS_LTO ON)
else(ANDROID)
    try_compile(HAS_M32 ${PROJECT_BINARY_DIR}/detectors/params32 ${PROJECT_SOURCE_DIR}/src/main/c/detectors/params
    	params-test
    	CMAKE_FLAGS "-DTEST_PARAM=-m32"
    )
    
    try_compile(HAS_M64 ${PROJECT_BINARY_DIR}/detectors/params64 ${PROJECT_SOURCE_DIR}/src/main/c/detectors/params
    	params-test
    	CMAKE_FLAGS "-DTEST_PARAM=-m64"
    )
    
    try_compile(HAS_MATH ${PROJECT_BINARY_DIR}/detectors/paramsm ${PROJECT_SOURCE_DIR}/src/main/c/detectors/params
        params-test
        CMAKE_FLAGS "-DTEST_PARAM=-lm"
    )
    
    try_compile(HAS_LTO ${PROJECT_BINARY_DIR}/detectors/paramslto ${PROJECT_SOURCE_DIR}/src/main/c/detectors/params
        params-test
        CMAKE_FLAGS "-DTEST_PARAM=-flto"
    )
        
    try_compile(HAS_WIN32 ${PROJECT_BINARY_DIR}/detectors/win32 ${PROJECT_SOURCE_DIR}/src/main/c/detectors/win32
        win32-test
    )

endif(ANDROID)

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
        ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce m)
	        TARGET_LINK_LIBRARIES(fyjni m)
        endif(HAS_MATH)
        set_property(TARGET fisce fyjni
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fyjni
            PROPERTIES
		        LINK_FLAGS "${W32_ADD_FLAGS} -Wl,--no-undefined"
        )
    endif(NOT (HAS_M32 OR HAS_M64))
    
    if(HAS_M32)
        message("Will generate 32bit binary")
        ADD_LIBRARY(fisce MODULE ${SRC_LIST_LIB})
        ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce m)
	        TARGET_LINK_LIBRARIES(fyjni m)
        endif(HAS_MATH)
        set_property(TARGET fisce fyjni
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce
            PROPERTIES
    		    COMPILE_FLAGS "-m32"
		        LINK_FLAGS "-m32 -Wl,--no-undefined"
        )
        set_target_properties(fyjni
            PROPERTIES
		        COMPILE_FLAGS "-m32"
		        LINK_FLAGS "-m32 ${W32_ADD_FLAGS} -Wl,--no-undefined"
        )
    endif(HAS_M32)
    
    if(HAS_M64)
        message("Will generate 64bit binary")
        ADD_LIBRARY(fisce64 MODULE ${SRC_LIST_LIB})
        ADD_LIBRARY(fyjni64 MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})
        if(HAS_MATH)
    	    TARGET_LINK_LIBRARIES(fisce64 m)
	        TARGET_LINK_LIBRARIES(fyjni64 m)
        endif(HAS_MATH)
        set_property(TARGET fisce64 fyjni64
        	APPEND PROPERTY
    		    COMPILE_DEFINITIONS FY_EXPORT
        )
        set_target_properties(fisce64
            PROPERTIES
    		    COMPILE_FLAGS "-m64"
		        LINK_FLAGS "-m64 -Wl,--no-undefined"
        )
        set_target_properties(fyjni64
            PROPERTIES
		        COMPILE_FLAGS "-m64"
		        LINK_FLAGS "-m64 ${W32_ADD_FLAGS} -Wl,--no-undefined"
        )
    endif(HAS_M64)
endif(ENABLE_LIBS)
