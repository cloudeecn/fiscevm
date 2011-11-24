/*
 * MemMan.h
 *
 *  Created on: Nov 24, 2011
 *      Author: cloudee
 */

#ifndef FY_MEMMAN_H_
#define FY_MEMMAN_H_

#include "../fisceprt.h"

typedef struct fy_memblock {
	void *first;
	void *last;
	fy_int blocks;
} fy_memblock;

void fy_mmInit(fy_memblock *block);
void fy_mmDestroy(fy_memblock *block);

void* fy_mmAllocate(fy_memblock *block, int size);
void fy_mmFree(fy_memblock *block, void *address);

#endif /* MEMMAN_H_ */
