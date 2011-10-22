RM := rm -rf
USER_OBJS :=
LIBS :=
OBJS += \
Class.o \
ClassLoader.o \
CoreHandlers.o \
Data.o \
Debug.o \
Global.o \
HashMap.o \
Heap.o \
HeapAccessers.o \
LinkedList.o \
Portable.o \
Resource.o \
String.o \
Thread.o \
Utf8.o \
VMContext.o \
VMPortable.o

D_OBJS += \
Class.debug.o \
ClassLoader.debug.o \
CoreHandlers.debug.o \
Data.debug.o \
Debug.debug.o \
Global.debug.o \
HashMap.debug.o \
Heap.debug.o \
HeapAccessers.debug.o \
LinkedList.debug.o \
Portable.debug.o \
Resource.debug.o \
String.debug.o \
Thread.debug.o \
Utf8.debug.o \
VMContext.debug.o \
VMPortable.debug.o

all: libfisce.a libfisce-debug.a

# Each subdirectory must supply rules for building sources it contributes
%.debug.o: src/%.c
	@echo 'Building file: $<'
	gcc -I"inc/" -D_DEBUG -O0 -g3 -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: src/%.c
	@echo 'Building file: $<'
	gcc -I"inc/" -O3 -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Add inputs and outputs from these tool invocations to the build variables 

# Tool invocations
libfisce.a: $(OBJS) 
	@echo 'Building target: $@'
	ar -r  "libfisce.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libfisce-debug.a: $(D_OBJS)
	@echo 'Building target: $@'
	ar -r  "libfisce-debug.a" $(D_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *.o *.d *.a *.so *.dll
	-@echo ' '

