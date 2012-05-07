
include_HEADERS= \
	../inc/fisce.h \
	../inc/fiscecon.h \
	../inc/fiscedev.h \
	../inc/fisceprt.h \
	../inc/fiscestu.h
	
fyincdir = @includedir@/fy_util
fyinc_HEADERS = \
	../inc/fy_util/LnkList.h \
	../inc/fy_util/MemMan.h \
	../inc/fy_util/String.h \
	../inc/fy_util/HashMap.h \
	../inc/fy_util/HashMapI.h \
	../inc/fy_util/ArrList.h \
	../inc/fy_util/Debug.h \
	../inc/fy_util/Utf8.h


#		roxml-internal.c roxml-parse-engine.c roxml.c 
FY_SRC = \
		Debug.c \
        HashMap.c \
        HashMapI.c \
        ArrList.c \
        LnkList.c \
        MemMan.c \
        Portable.c \
        String.c \
        BitSet.c \
        Utf8.c \
        Class.c \
        ClassLoader.c \
        CoreHandlers.c \
        MathHandlers.c \
        Data.c \
        Heap.c \
        Instructions.c \
        Preverifier.c \
        Thread.c \
        ThreadManager.c \
        VMContext.c \
        ExpDev.c \
        FiScE.c \
        FileInputStream.c \
        BinarySaver.c \
	DataLoader.c

if FY_BUILD_TEST
noinst_LTLIBRARIES = libfytst.la
libfytst_la_SOURCES = $(FY_SRC)
noinst_PROGRAMS = utiltest fiscetest
utiltest_SOURCES = TestUtil.c
utiltest_LDADD = libfytst.la
fiscetest_SOURCES =  Test.c
fiscetest_LDADD = libfytst.la
else
lib_LTLIBRARIES=libfisce.la libfyjni.la
libfisce_la_SOURCES = $(FY_SRC)

libfisce_la_LDFLAGS = -fwhole-program -no-undefined 
libfyjni_la_SOURCES = $(FY_SRC) FisceService.c 
if FY_IS_WIN32
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined -Wl,--kill-at
else
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined
endif

endif
