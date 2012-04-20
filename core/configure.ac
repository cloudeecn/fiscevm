#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(libfisce, 0.9.0, cloudeecn@gmail.com)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE()
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([inc/fiscecon.h])
AC_CONFIG_SRCDIR(src/VMContext.c)


AC_ARG_ENABLE(lto,
  [AS_HELP_STRING([--enable-lto],
  [enable link time optimization, will greatly increase speed of accessing heap [default=no]])],
  [fy_do_lto=$enableval],
  [fy_do_lto="no"])
AC_ARG_ENABLE(debug,
  [AS_HELP_STRING([--enable-debug],[enable debugging [default=no]])],
  [fy_do_debug=$enableval],
  [fy_do_debug="no"])
AC_ARG_ENABLE(light-debug,
  [AS_HELP_STRING([--enable-light-debug],[enable light debugging(enable debug but disable FY_DEBUG macro) [default=no]])],
  [fy_do_ldebug=$enableval],
  [fy_do_ldebug="no"])
OPTIMIZE_LEVEL="-O3 -fomit-frame-pointer"
LD_OPTIMIZE_LEVEL="-O3"

if test x"$fy_do_ldebug" = xyes ; then
	echo "++++++++++ Enabling light debug mode compilation."
	OPTIMIZE_LEVEL="-O2 -g3"
	LD_OPTIMIZE_LEVEL=""
fi

if test x"$fy_do_debug" = xyes ; then
	echo "++++++++++ Enabling debug mode compilation."
	OPTIMIZE_LEVEL="-g3 -O0"
	LD_OPTIMIZE_LEVEL="-O0"
	CFLAGS="$CFLAGS -DFY_DEBUG"
fi
CFLAGS="$CFLAGS $OPTIMIZE_LEVEL"
LDFLAGS="$LDFLAGS $LD_OPTIMIZE_LEVEL"

if test x"$fy_do_lto" = xyes ; then
	echo "++++++++++ Enabling link time optimization."
	CFLAGS="$CFLAGS -flto"
else
	echo "---------- Disabling link time optimization."
fi

AC_ARG_ENABLE(verbose,
  [AS_HELP_STRING([--enable-verbose],[enable verbose output for byte code executed [default=no]])],
  [fy_do_verbose=$enableval],
  [fy_do_verbose="no"])
if test x"$fy_do_verbose" = xyes ; then
	echo "++++++++++ Enabling verbose mode compilation."
	CFLAGS="$CFLAGS -DFY_VERBOSE"
else
	echo "---------- Disabling verbose mode compilation."
fi




AC_ARG_ENABLE(sse2,
  [AS_HELP_STRING([--enable-sse2],
  [enable use sse2 instructions [default=no]])],
  [fy_do_sse2=$enableval],
  [fy_do_sse2="no"])
if test x"$fy_do_sse2" = xyes ; then
	echo "++++++++++ Enabling sse2."
	CFLAGS="$CFLAGS -msse2"
else
	echo "---------- Disabling sse2."
fi

AC_ARG_ENABLE(sse3,
  [AS_HELP_STRING([--enable-sse3],
  [enable use sse3 instructions [default=no]])],
  [fy_do_sse3=$enableval],
  [fy_do_sse3="no"])
if test x"$fy_do_sse3" = xyes ; then
	echo "++++++++++ Enabling sse3."
	CFLAGS="$CFLAGS -msse3"
else
	echo "---------- Disabling sse3."
fi

AC_ARG_ENABLE(test,
  [AS_HELP_STRING([--enable-test],
  [Build fiscetest & utiltest insteadof libs [default=no]])],
  [fy_do_test=$enableval],
  [fy_do_test="no"])
if test x"$fy_do_test" = xyes ; then
	echo "========== Build test."
	CFLAGS="$CFLAGS -DFY_STATIC"
else
	echo "========== Build libraries."
	CFLAGS="$CFLAGS -DFY_EXPORT"
fi
AM_CONDITIONAL([FY_BUILD_TEST],[test x$fy_do_test = xyes])

dnl CFLAGS="$CFLAGS -pipe"


AC_LANG_PUSH(C)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#if !defined (_WIN32) && !defined (__WIN32__) && !defined (WIN32)
# error "not win32"
#endif
]])],[fy_is_win32=true],[fy_is_win32=false])
AC_LANG_POP()

AC_MSG_CHECKING([is win32])
if test x$fy_is_win32 = xtrue ; then
	AC_MSG_RESULT([yes])
	echo "++++++++++ Adding win32 dll flags."
else
	AC_MSG_RESULT([no])
	echo "---------- Disabling win32 dll flags."
fi
AM_CONDITIONAL([FY_IS_WIN32],[test x$fy_is_win32 = xtrue])

LT_INIT(win32-dll)
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL

AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([m],[floor])

# Checks for header files.
AC_CHECK_HEADERS([math.h float.h memory.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
dnl AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset])

#params
CFLAGS="$CFLAGS -Wall"


AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
