#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(libfisce, 0.9.0, cloudeecn@gmail.com)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE()
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([inc/fiscecon.h])
AC_CONFIG_SRCDIR(src/VMContext.c)

AC_ARG_ENABLE(debug,
  [AS_HELP_STRING([--enable-debug],[enable debugging [default=no]])],
  [fy_do_debug=$enableval],
  [fy_do_debug="no"])
if test x"$fy_do_debug" = xyes ; then
	echo "++++++++++ Enabling debug mode compilation."
	CFLAGS="$CFLAGS -D_DEBUG -g3 -O0"
	ENABLE_DEBUG="TRUE"
else
	echo "---------- Disabling debug mode compilation."
	CFLAGS="$CFLAGS -O3 -fomit-frame-pointer"
	ENABLE_DEBUG="FALSE"
fi

AC_ARG_ENABLE(verbose,
  [AS_HELP_STRING([--enable-verbose],[enable verbose output for byte code executed [default=no]])],
  [fy_do_verbose=$enableval],
  [fy_do_verbose="no"])
if test x"$fy_do_verbose" = xyes ; then
	echo "++++++++++ Enabling verbose mode compilation."
	CFLAGS="$CFLAGS -DFY_VERBOSE"
	ENABLE_VERBOSE="TRUE"
else
	echo "---------- Disabling verbose mode compilation."
	ENABLE_VERBOSe="FALSE"
fi

AC_ARG_ENABLE(lto,
  [AS_HELP_STRING([--enable-lto],
  [enable link time optimization, will greatly increase speed of accessing heap [default=no]])],
  [fy_do_lto=$enableval],
  [fy_do_lto="no"])
if test x"$fy_do_lto" = xyes ; then
	echo "++++++++++ Enabling link time optimization."
	CFLAGS="$CFLAGS -flto"
	ENABLE_LTO="TRUE"
else
	echo "---------- Disabling link time optimization."
	ENABLE_LTO="FALSE"
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

CFLAGS="$CFLAGS -pipe"

AC_MSG_CHECKING([is win32])
AC_LANG_PUSH(C)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#if !defined (_WIN32) && !defined (__WIN32__) && !defined (WIN32)
# error "not win32"
#endif
]])],[fy_is_win32=true],[fy_is_win32=false])
AC_LANG_POP()

if test x$fy_is_win32 = xtrue ; then
	echo "++++++++++ Adding win32 dll flags."
else
	echo "---------- Disabling win32 dll flags."
fi
AM_CONDITIONAL([FY_IS_WIN32],[test x$fy_is_win32 = xtrue])

LT_INIT(win32-dll)
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
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


AC_CONFIG_FILES(Makefile src/Makefile jni/Makefile)
AC_OUTPUT
