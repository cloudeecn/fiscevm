gcc -shared -flto  -Wall -fwhole-program -O3 -Wl,-no-undefined -o libfisce.so src/*.o -lm

