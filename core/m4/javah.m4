# ===========================================================================
#       http://www.gnu.org/software/autoconf-archive/ax_prog_javah.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_PROG_JAVAH
#
# DESCRIPTION
#
#   AX_PROG_JAVAH tests the availability of the javah header generator and
#   looks for the jni.h header file. If available, JAVAH is set to the full
#   path of javah and CPPFLAGS is updated accordingly.
#
# LICENSE
#
#   Copyright (c) 2008 Luc Maisonobe <luc@spaceroots.org>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 5

AU_ALIAS([AC_PROG_JAVAH], [AX_PROG_JAVAH])
AC_DEFUN([AX_PROG_JAVAH],[
ac_jni_file=`find $JAVA_HOME/../ -name "jni.h"`
if test x"$ac_jni_file" != x; then
ac_jni_dir=`AS_DIRNAME(["$ac_jni_file"])`

changequote(, )dnl
ac_machdep=`echo $build_os | sed 's,[-0-9].*,,' | sed 's,cygwin,win32,'`
changequote([, ])dnl

CFLAGS="$CFLAGS -DHAVE_JNI_H -I$ac_jni_dir -I$ac_jni_dir/$ac_machdep"
fi
])
