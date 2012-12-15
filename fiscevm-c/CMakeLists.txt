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
  "src/main/inc/fiscecon.h.in"
  "${PROJECT_BINARY_DIR}/fiscecon.h"
  )

include_directories("src/main/inc")
include_directories("${PROJECT_BINARY_DIR}")

option (ENABLE_LTO "Use link-time optimization" ON)
option (ENABLE_DEBUG "Debug mode" ON)
option (ENABLE_LIGHT_DEBUG "Light debug mode" ON)
option (ENABLE_VERBOSE "Verbose output" ON)

ADD_LIBRARY(fisce STATIC ${SRC_LIST_LIB})
ADD_EXECUTABLE(fisce-test ${SRC_LIST_TEST})
ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})

set(FY_COPT "")
set(FY_LOPT "")
if(ENABLE_DEBUG)
	set(FY_COPT "${FY_COPT} -g3 -O0")
	set(FY_LOPT "${FY_LOPT} -O0")
	set(FY_CDEF ${FY_CDEF} FY_DEBUG)
elseif(ENABLE_LIGHT_DEBUG)
	set(FY_COPT "${FY_COPT} -g3 -O2")
	set(FY_LOPT "${FY_LOPT} -O2")
else(ENABLE_DEBUG)
	set(FY_COPT "${FY_COPT} -O3")
endif(ENABLE_DEBUG)

if(ENABLE_VERBOSE)
	set(FY_CDEF ${FY_CDEF} FY_VERBOSE)
endif(ENABLE_VERBOSE)

if(ENABLE_LTO)
	set(FY_COPT "${FY_COPT} -flto")
	set(FY_LOPT "${FY_LOPT} -flto")
endif(ENABLE_LTO)

set(FY_CDEF_STATIC ${FY_CDEF} FY_STATIC)
set(FY_CDEF_EXPORT ${FY_CDEF} FY_EXPORT)

set_property(TARGET fisce fisce-test
	APPEND PROPERTY
		COMPILE_DEFINITIONS ${FY_CDEF_STATIC}
)

set_property(TARGET fyjni
	APPEND PROPERTY
		COMPILE_DEFINITIONS ${FY_CDEF_EXPORT}
)

if(WIN32)
	set(FY_LOPT "${FY_LOPT} -Wl,--kill-at")
endif(WIN32)

IF(UNIX)
	TARGET_LINK_LIBRARIES(fisce-test m)
	TARGET_LINK_LIBRARIES(fyjni m)
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(fisce-test fisce)
set_target_properties(fisce fisce-test fyjni
    PROPERTIES
	COMPILE_FLAGS "${FY_COPT}"
	LINK_FLAGS "${FY_LOPT}"
)
#TARGET_LINK_LIBRARIES(fyjni fisce)



#INSTALL(TARGETS fisce
#        RUNTIME DESTINATION bin)
