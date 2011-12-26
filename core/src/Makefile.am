FYSOURCES = \
	Debug.c \
	HashMap.c \
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
	Resource.c \
	Thread.c \
	ThreadManager.c \
	VMContext.c \
	ExpDev.c \
	FiScE.c
lib_LTLIBRARIES=libfisce.la
libfisce_la_SOURCES = $(FYSOURCES)
libfisce_la_CFLAGS = $(AM_CFLAGS)
libfisce_la_LDFLAGS = -no-undefined

noinst_PROGRAMS = utiltest fiscetest
utiltest_SOURCES = $(FYSOURCES) \
	TestUtil.c
utiltest_LDADD  = -lcunit

fiscetest_SOURCES = $(FYSOURCES) \
	Test.c
fiscetest_LDADD  = -lcunit