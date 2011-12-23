#!/bin/sh
ln -s ../core/inc

rm incjni
JAVAH_PATH=`which javah`
if test x$JAVAH_PATH = x; then
        echo "Can't find javah, aborting..."
fi

if [ -h $JAVAH_PATH ]; then
        JAVAH_PATH=`readlink -f $JAVAH_PATH`
        if test x$JAVAH_PATH = x; then
                echo "Can't resolve javah, aborting..."
        fi
fi

JAVAH_PATH=`dirname $JAVAH_PATH`
JAVAH_PATH=`dirname $JAVAH_PATH`

JNI_PATH=`find $JAVAH_PATH -name "jni.h"`
if test x$JNI_PATH = x; then
        echo "Can't find jni.h in $JAVAH_PATH, aborting..."
fi
JNI_PATH=`dirname $JNI_PATH`
ln -s $JNI_PATH incjni

