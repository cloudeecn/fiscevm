/*
 * Stack.h
 *
 *  Created on: Dec 6, 2011
 *      Author: cloudee
 */

#ifndef STACK_H_
#define STACK_H_

#include "MemMan.h"

typedef struct fy_stack {
	fy_int length;
	fy_int maxLength;
	size_t entrySize;
	void *data;
} fy_stack;

void fy_stackInit(fy_memblock *block, fy_stack *stack, size_t entrySize,
		fy_int entryCount, fy_exception *exception);

void fy_stackDestroy(fy_memblock *block, fy_stack *stack);

void fy_stackPush(fy_memblock *block, fy_stack *stack, const void *data,
		fy_exception *exception);

void *fy_stackPop(fy_memblock *block, fy_stack *stack, void *storage);

#endif /* STACK_H_ */
