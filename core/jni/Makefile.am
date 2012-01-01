lib_LTLIBRARIES = libfyjni.la
libfyjni_la_SOURCES = FisceService.c
if FY_IS_WIN32
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined -Wl,--kill-at
else
libfyjni_la_LDFLAGS = -fwhole-program -no-undefined
endif
libfyjni_la_LIBADD = ../src/libfisce.la