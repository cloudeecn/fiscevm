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

option (ENABLE_DEBUG "Debug mode" OFF)
option (ENABLE_LIGHT_DEBUG "Light debug mode" OFF)
option (ENABLE_LTO "Use link-time optimization" ON)
option (ENABLE_VERBOSE "Verbose output" OFF)

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
elseif(ENABLE_LTO)
	set(FY_COPT "${FY_LOPT} -flto")
	set(FY_LOPT "${FY_LOPT} -O3 -flto")
else(ENABLE_DEBUG)
	set(FY_COPT "${FY_COPT} -O3")
	set(FY_LOPT "${FY_LOPT} -O3")
endif(ENABLE_DEBUG)

if(ENABLE_VERBOSE)
	set(FY_CDEF ${FY_CDEF} FY_VERBOSE)
endif(ENABLE_VERBOSE)

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

set(FY_COPT "${FY_COPT} -Wall")

set_target_properties(fisce fisce-test fyjni
    PROPERTIES
	COMPILE_FLAGS "${FY_COPT}"
	LINK_FLAGS "${FY_LOPT}"
)

