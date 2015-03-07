#!/bin/bash
$(dirname "$0")/vmgen.sh
cmake -DENABLE_JNI=OFF -DENABLE_LIBS=OFF -DENABLE_TEST=ON -DCMAKE_BUILD_TYPE=$1 -DENABLE_VERBOSE=OFF -DCMAKE_C_FLAGS="-DFY_DISPATCH_MODE=2 -DREPL_MIN=5000 -DTHREADING_SCHEME=3 -DFY_ENGINE_COUNT=1 -fbranch-target-load-optimize2 -fbtr-bb-exclusive" $(dirname "$0")
make

