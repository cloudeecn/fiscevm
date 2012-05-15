#!/bin/sh
./configure --enable-test --disable-shared $@ && make clean all && make/fiscetest

