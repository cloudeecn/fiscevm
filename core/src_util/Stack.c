#include "fy_util/Stack.h"

void fy_stackInit(fy_memblock *block, fy_stack *stack, size_t entrySize,
		fy_int entryCount, fy_exception *exception) {
	stack->length = 0;
	stack->maxLength = entryCount;
	stack->entrySize = entrySize;
	stack->data = fy_mmAllocate(block, entrySize * entryCount, exception);
}

void fy_stackDestroy(fy_memblock *block, fy_stack *stack) {
	fy_mmFree(block, stack->data);
}

static void ensureCap(fy_memblock *block, fy_stack *stack, fy_int cap,
		fy_exception *exception) {
	fy_int targetLength;
	void *data;
	if (cap > stack->maxLength) {
		targetLength = stack->maxLength;
		while (cap > targetLength) {
			targetLength <<= 1;
		}
		data = fy_mmAllocate(block, targetLength * stack->entrySize, exception);
		fy_exceptionCheckAndReturn(exception);

		memcpy(data, stack->data, stack->length * stack->entrySize);
		fy_mmFree(block, stack->data);
		stack->data = data;
		stack->maxLength = targetLength;
	}
}

void fy_stackPush(fy_memblock *block, fy_stack *stack, const void *data,
		fy_exception *exception) {
	ensureCap(block, stack, stack->length + 1, exception);
	fy_exceptionCheckAndReturn(exception);

	memcpy((fy_byte*) stack->data + stack->length * stack->entrySize, data,
			stack->entrySize);
	stack->length++;
}

void *fy_stackPop(fy_memblock *block, fy_stack *stack, void *storage) {
	if (stack->length == 0) {
		return NULL;
	} else {
		if (storage) {
			memcpy(
					storage,
					(fy_byte*) stack->data
							+ (--stack->length) * stack->entrySize,
					stack->entrySize);
			return storage;
		} else {
			return (fy_byte*) stack->data + (--stack->length) * stack->entrySize;
		}
	}
}
