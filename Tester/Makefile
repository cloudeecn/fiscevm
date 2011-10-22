RM := rm -rf

all: gretelvm-debugtest gretelvm-benchtest

dependents:
	-cd ../BaseLibs/Debug && $(MAKE) clean all

Test.o: src/Test.c
	@echo 'Building file: $<'
	gcc  -I"../BaseLibs/inc" -O3 -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Test.debug.o: src/Test.c
	@echo 'Building file: $<'
	gcc  -D_DEBUG -I"../BaseLibs/inc" -O0 -g3 -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Tool invocations
gretelvm-benchtest: Test.o ../BaseLibs/libfisce.a
	@echo 'Building target: $@'
	gcc -L"../BaseLibs" -o "libfisce-benchtest" ./Test.o -lcunit -lfisce -lm
	@echo 'Finished building target: $@'

gretelvm-debugtest: Test.debug.o ../BaseLibs/libfisce-debug.a
	@echo 'Building target: $@'
	gcc -L"../BaseLibs" -o "libfisce-debugtest" ./Test.debug.o -lcunit -lfisce-debug -lm
	@echo 'Finished building target: $@'
	@echo ' '
	
clean:
	-$(RM) *.o *.d libfisce-debugtest libfisce-benchtest
	-@echo ' '

.PHONY: all clean dependents
