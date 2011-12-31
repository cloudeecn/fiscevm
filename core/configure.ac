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
	CFLAGS="$CFLAGS -O3"
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

LT_INIT(win32-dll)
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([m],[floor])
AC_CHECK_LIB([cunit],[CU_initialize_registry],
  [fy_test=$enableval])
# Checks for header files.
AC_CHECK_HEADERS([math.h float.h memory.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset])

#params
CFLAGS="$CFLAGS -Wall"


AC_CONFIG_FILES(Makefile src/Makefile jni/Makefile)
AC_OUTPUT
