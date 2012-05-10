
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
		../src/Debug.c \
        ../src/HashMap.c \
        ../src/HashMapI.c \
        ../src/ArrList.c \
        ../src/LnkList.c \
        ../src/MemMan.c \
        ../src/Portable.c \
        ../src/String.c \
        ../src/BitSet.c \
        ../src/Utf8.c \
        ../src/Class.c \
        ../src/ClassLoader.c \
        ../src/CoreHandlers.c \
        ../src/MathHandlers.c \
        ../src/Data.c \
        ../src/Heap.c \
        ../src/Instructions.c \
        ../src/Preverifier.c \
        ../src/Thread.c \
        ../src/ThreadManager.c \
        ../src/VMContext.c \
        ../src/ExpDev.c \
        ../src/FiScE.c \
        ../src/FileInputStream.c \
        ../src/BinarySaver.c \
		../src/DataLoader.c

if FY_BUILD_TEST
noinst_LTLIBRARIES = libfytst.la
libfytst_la_SOURCES = $(FY_SRC)
noinst_PROGRAMS = utiltest fiscetest
utiltest_SOURCES = ../src/TestUtil.c
utiltest_LDADD = libfytst.la
fiscetest_SOURCES =  ../src/Test.c
fiscetest_LDADD = libfytst.la
else
lib_LTLIBRARIES=libfisce.la libfyjni.la
libfisce_la_SOURCES = $(FY_SRC)

libfisce_la_LDFLAGS = -fwhole-program -no-undefined 
libfyjni_la_SOURCES = $(FY_SRC) ../src/FisceService.c 
if FY_IS_WIN32
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined -Wl,--kill-at
else
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined
endif

endif
