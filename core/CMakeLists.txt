cmake_minimum_required (VERSION 2.6)
project (fisce)

SET(SRC_LIST_LIB
		src/Debug.c
        src/HashMap.c
        src/HashMapI.c
        src/ArrList.c
        src/LnkList.c
        src/MemMan.c
        src/Portable.c
        src/String.c
        src/BitSet.c
        src/Utf8.c
        src/Class.c
        src/ClassLoader.c
        src/CoreHandlers.c
        src/MathHandlers.c
        src/Data.c
        src/Heap.c
        src/Instructions.c
        src/Preverifier.c
        src/Thread.c
        src/ThreadManager.c
        src/VMContext.c
        src/ExpDev.c
        src/FiScE.c
        src/FileInputStream.c
        src/BinarySaver.c
		src/DataLoader.c
)
SET(SRC_LIST_JNI
		src/FisceService.c
)
SET(SRC_LIST_TEST
		src/Test.c
)

ADD_LIBRARY(fisce STATIC ${SRC_LIST_LIB})
ADD_EXECUTABLE(fisce-test ${SRC_LIST_TEST})
ADD_LIBRARY(fyjni MODULE ${SRC_LIST_LIB} ${SRC_LIST_JNI})

include_directories(inc)

set_target_properties(fisce fisce-test
    PROPERTIES
	COMPILE_DEFINITIONS "FY_STATIC"
)

set_target_properties(fyjni
    PROPERTIES
	COMPILE_DEFINITIONS "FY_EXPORT"
)

if(WIN32)
set_target_properties(fyjni
    PROPERTIES
	LINK_FLAGS "-Wl,--kill-at"
)
endif()

IF(UNIX)
  TARGET_LINK_LIBRARIES(fisce-test m)
  TARGET_LINK_LIBRARIES(fyjni m)
ENDIF(UNIX)

TARGET_LINK_LIBRARIES(fisce-test fisce)
#TARGET_LINK_LIBRARIES(fyjni fisce)



#INSTALL(TARGETS fisce
#        RUNTIME DESTINATION bin)
