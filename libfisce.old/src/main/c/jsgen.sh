#!/bin/bash
rm *.o
rm test*
emcc -m32 -O0 -g RIS.c Debug.c HashMap.c HashMapI.c ArrList.c LnkList.c MemMan.c Portable.c String.c BitSet.c Utf8.c Class.c ClassLoader.c CoreHandlers.c MathHandlers.c Data.c Heap.c Instructions.c Preverifier.c Thread.c ThreadManager.c VMContext.c ExpDev.c FiScE.c FileInputStream.c BinarySaver.c DataLoader.c BAIS.c Test.c -o test.bc -DFY_DEBUG 
llvm-dis test.bc
python2 ~/develop/emscripten/tools/autodebugger.py test.ll test.ad.ll
emcc test.ad.ll -o test.ad.js --embed-files runtime
emcc test.ll -o test.js --embed-files runtime

