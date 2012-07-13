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
)
SET(SRC_LIST_JNI
		FisceService.c
)
SET(SRC_LIST_TEST
		Test.c
)

ADD_LIBRARY(fisce STATIC ${SRC_LIST_LIB})
ADD_EXECUTABLE(fisce-test Test.c)
ADD_LIBRARY(fyjni MODULE FisceService.c)

include_directories(../inc)

set_target_properties(fisce fisce-test
    PROPERTIES
	COMPILE_DEFINITIONS "FY_STATIC"
)

set_target_properties(fyjni
    PROPERTIES
	COMPILE_DEFINITIONS "FY_EXPORT"
)

TARGET_LINK_LIBRARIES(fisce-test fisce)
TARGET_LINK_LIBRARIES(fyjni fisce)

#INSTALL(TARGETS fisce
#        RUNTIME DESTINATION bin)