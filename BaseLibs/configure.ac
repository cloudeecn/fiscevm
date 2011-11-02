#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(libfisce, 0.01, cloudeecn@gmail.com)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_ARG_ENABLE(debug,
  [AS_HELP_STRING([--enable-debug],[enable debugging [default=no]])],
  [cu_do_debug=$enableval],
  [cu_do_debug="no"])
if test x"$cu_do_debug" = xyes ; then
	echo "++++++++++ Enabling debug mode compilation."
	CFLAGS="$CFLAGS -D_DEBUG -g3 -O0"
	ENABLE_DEBUG="TRUE"
else
	echo "---------- Disabling debug mode compilation."
	CFLAGS="$CFLAGS -O3"
	ENABLE_DEBUG="FALSE"
fi


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL ye
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([m],[floor])

# Checks for header files.
AC_CHECK_HEADERS([float.h memory.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor gettimeofday memset])

#params
CFLAGS="$CFLAGS -Wall -W -I${PWD}/inc/"


AC_CONFIG_FILES(Makefile src/Makefile inc/Makefile)
AC_OUTPUT
