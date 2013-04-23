/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fy_util/HashMapI.h"
#define FY_MEM_LOG_TO_CONTEXT 0
#if FY_MEM_LOG_TO_CONTEXT
# include "fiscestu.h"
#endif

typedef struct fy_hashMapIEntry {
	fy_int key;
	fy_int value;
	struct fy_hashMapIEntry *next;
} fy_hashMapIEntry;

static void expandBuckets(fy_memblock *mem, fy_hashMapI *this,
		fy_uint targetFact, fy_exception *exception) {
	int i, imax;
	fy_hashMapIEntry *entry;
	fy_hashMapIEntry *tmp;
	int newPos;
	fy_hashMapIEntry **newEntries;
	fy_int targetSizeM1 = (1 << targetFact) - 1;
#if FY_MEM_LOG_TO_CONTEXT
	fy_context *context = mem->gcContext;
	context->logDVarLn(context, "HashMapI.expandBuckets %p to %"FY_PRINT32"d",
			this, targetSizeM1);
#endif
	if (targetFact <= this->bucketsFact) {
		return;
	} else {
		newEntries = fy_mmAllocate(mem, sizeof(fy_hashMapIEntry*) << targetFact,
				exception);
		FYEH();
		for (i = 0, imax = 1 << this->bucketsFact; i < imax; i++) {
			if ((entry = this->buckets[i]) != NULL) {

				do {
					newPos = entry->key & targetSizeM1;
					if ((tmp = newEntries[newPos]) != NULL) {
						while (tmp->next != NULL) {
							tmp = tmp->next;
						}
						tmp->next = entry;
						tmp = entry;
						entry = entry->next;
						tmp->next = NULL;
					} else {
						newEntries[newPos] = entry;
						tmp = entry;
						entry = entry->next;
						tmp->next = NULL;
					}
				} while (entry != NULL);
			}
		}
		fy_mmFree(mem, this->buckets);
		this->buckets = (void**) newEntries;
		this->bucketsFact = targetFact;
		this->bucketsSizeM1 = targetSizeM1;
	}

}

/*
 *
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...entry is null");
 #endif
 return this->nullValue;
 }
 do {
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...entry == %p", entry);
 #endif
 if (entry->key == key) {
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...key matched");
 #endif
 if (last == NULL) {
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...last == NULL, entry == %p",
 entry);
 #endif
 this->buckets[key & this->bucketsSizeM1] = entry->next;
 } else {
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...last == %p, entry == %p", last,
 entry);
 #endif
 last->next = entry->next;
 }
 ret = entry->value;
 if (!this->perm) {
 fy_mmFree(mem, entry);
 }
 this->size--;
 return ret;
 }
 last = entry;
 #if FY_MEM_LOG_TO_CONTEXT
 context->logDVarLn(context, "...entry->next=%p", entry->next);
 #endif
 */
static fy_hashMapIEntry *getBucket(fy_memblock *mem, fy_hashMapI *this,
		fy_int key) {
	fy_hashMapIEntry *entry;
#if FY_MEM_LOG_TO_CONTEXT
	fy_context *context = mem->gcContext;
	if (this == context->references)
		context->logDVarLn(context,
				"HashMapI getBucket %p[%"FY_PRINT32"d]@%"FY_PRINT32"d", this,
				key, key & this->bucketsSizeM1);
#endif
	if ((entry = this->buckets[key & this->bucketsSizeM1]) != NULL) {
		do {
#if FY_MEM_LOG_TO_CONTEXT
			if (this == context->references)
				context->logDVarLn(context,
						"...entry == %p key == %"FY_PRINT32"d", entry,
						entry->key);
#endif
			if (entry->key == key) {
#if FY_MEM_LOG_TO_CONTEXT
				if (this == context->references)
					context->logDVarLn(context, "...matched", entry,
							entry->key);
#endif
				return entry;
			}
#if FY_MEM_LOG_TO_CONTEXT
			if (this == context->references)
				context->logDVarLn(context, "...entry->next=%p", entry->next);
#endif
		} while ((entry = entry->next) != NULL);
	} else {
#if FY_MEM_LOG_TO_CONTEXT
		if (this == context->references)
			context->logDVarLn(context, "...entry is null");
#endif
	}

	return NULL;
}

FY_ATTR_EXPORT void fy_hashMapIInit(fy_memblock *mem, fy_hashMapI *this,
		fy_uint initFact, fy_uint loadFactor, fy_int nullValue,
		fy_exception *exception) {
#if FY_MEM_LOG_TO_CONTEXT
	fy_int i;
	fy_context *context = mem->gcContext;
#endif
	this->perm = FALSE;
	this->loadFactor = loadFactor;
	this->bucketsFact = initFact;
	this->nullValue = nullValue;
	this->bucketsSizeM1 = (1 << initFact) - 1;
	this->buckets = fy_mmAllocate(mem, sizeof(fy_hashMapIEntry*) << initFact,
			exception);
#if FY_MEM_LOG_TO_CONTEXT
	context->logEVarLn(context,
			"HashMapI.init %p.buckets[%"FY_PRINT32"d bytes] == %p", this,
			sizeof(fy_hashMapIEntry*) << initFact, this->buckets);
	for (i = 0; i < (1 << initFact); i++) {
		if (this->buckets[i] != NULL) {
			context->logEVarLn(context,
					"HashMapI.init %p.buckets[%"FY_PRINT32"d] is not NULL",
					this, i);
		}
	}
#endif
	this->size = 0;
}

FY_ATTR_EXPORT void fy_hashMapIInitPerm(fy_memblock *mem, fy_hashMapI *this,
		fy_uint initFact, fy_int nullValue, fy_exception *exception) {
#if FY_MEM_LOG_TO_CONTEXT
	fy_int i;
	fy_context *context = mem->gcContext;
#endif
	this->perm = TRUE;
	this->bucketsFact = initFact;
	this->nullValue = nullValue;
	this->bucketsSizeM1 = (1 << initFact) - 1;
	this->buckets = fy_mmAllocatePerm(mem,
			sizeof(fy_hashMapIEntry*) << initFact, exception);
	this->size = 0;
#if FY_MEM_LOG_TO_CONTEXT
	/*
	 context->logEVarLn(context,
	 "HashMapI.initPerm %p.buckets[%"FY_PRINT32"d bytes] == %p", this,
	 sizeof(fy_hashMapIEntry*) << initFact, this->buckets);
	 */
	for (i = 0; i < (1 << initFact); i++) {
		if (this->buckets[i] != NULL) {
			context->logEVarLn(context,
					"HashMapI.init %p.buckets[%"FY_PRINT32"d] is not NULL",
					this, i);
		}
	}
#endif
}

FY_ATTR_EXPORT fy_int fy_hashMapIPut(fy_memblock *mem, fy_hashMapI *this,
		fy_int key, fy_int value, fy_exception *exception) {
	fy_hashMapIEntry *entry;
	fy_hashMapIEntry *tmp;
	int pos;
	fy_int ret = this->nullValue;
#if FY_MEM_LOG_TO_CONTEXT
	fy_context *context = mem->gcContext;
	if (this == context->references)
		context->logDVarLn(context,
				"HashMapI put %p[%"FY_PRINT32"d]@%"FY_PRINT32"d <<< %"FY_PRINT32"d",
				this, key, key & this->bucketsSizeM1, value);
#endif

	entry = getBucket(mem, this, key);
	if (entry == NULL) {
		if (this->perm) {
			entry = fy_mmAllocatePerm(mem, sizeof(fy_hashMapIEntry), exception);
		} else {
			entry = fy_mmAllocate(mem, sizeof(fy_hashMapIEntry), exception);
		}
		FYEH()this->nullValue;
		FYEH()this->nullValue;
		entry->key = key;
		entry->value = value;

		if (!this->perm
				&& ((this->size + 1) * 16
						> this->loadFactor << this->bucketsFact)) {
			expandBuckets(mem, this, this->bucketsFact + 1, exception);
			FYEH()this->nullValue;
		}

		pos = key & this->bucketsSizeM1;

		tmp = this->buckets[pos];
		if (tmp) {
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = entry;
		} else {
			this->buckets[pos] = entry;
		}
		this->size++;

		return ret;
	} else {
		ret = entry->value;
		entry->value = value;
		return ret;
	}
}

FY_ATTR_EXPORT fy_int fy_hashMapIRemove(fy_memblock *mem, fy_hashMapI *this,
		fy_int key) {
	fy_hashMapIEntry *entry;
	fy_hashMapIEntry *last = NULL;
	fy_int ret;
#if FY_MEM_LOG_TO_CONTEXT
	fy_context *context = mem->gcContext;
	context->logDVarLn(context,
			"HashMapI remove %p->%p[%"FY_PRINT32"d]@%"FY_PRINT32"d", this,
			this->buckets, key, key & this->bucketsSizeM1);
#endif

	entry = this->buckets[key & this->bucketsSizeM1];
	if (entry == NULL) {
#if FY_MEM_LOG_TO_CONTEXT
		context->logDVarLn(context, "...entry is null");
#endif
		return this->nullValue;
	}
	do {
#if FY_MEM_LOG_TO_CONTEXT
		context->logDVarLn(context, "...entry == %p", entry);
		context->logDVarLn(context, "...key == %"FY_PRINT32"d", entry->key);
		context->logDVarLn(context, "...value == %"FY_PRINT32"d", entry->value);
#endif
		if (entry->key == key) {
#if FY_MEM_LOG_TO_CONTEXT
			context->logDVarLn(context, "...key matched");
#endif
			if (last == NULL) {
#if FY_MEM_LOG_TO_CONTEXT
				context->logDVarLn(context, "...last == NULL, entry == %p",
						entry);
#endif
				this->buckets[key & this->bucketsSizeM1] = entry->next;
			} else {
#if FY_MEM_LOG_TO_CONTEXT
				context->logDVarLn(context, "...last == %p, entry == %p", last,
						entry);
#endif
				last->next = entry->next;
			}
			ret = entry->value;
			if (!this->perm) {
				fy_mmFree(mem, entry);
			}
			this->size--;
			return ret;
		}
		last = entry;
#if FY_MEM_LOG_TO_CONTEXT
		context->logDVarLn(context, "...entry->next=%p", entry->next);
#endif
	} while ((entry = entry->next) != NULL);
	return this->nullValue;
}

FY_ATTR_EXPORT fy_int fy_hashMapIGet(fy_memblock *mem, fy_hashMapI *this,
		fy_int key) {
	fy_hashMapIEntry *entry = getBucket(mem, this, key);
	return entry == NULL ? this->nullValue : entry->value;
}

FY_ATTR_EXPORT void fy_hashMapIEachValue(fy_memblock *mem, fy_hashMapI *map,
		void (*fn)(fy_int key, fy_int value, fy_int nullValue, void *addition),
		void *addition) {
	fy_uint i, imax;
	fy_hashMapIEntry *entry;
	imax = 1 << map->bucketsFact;
	for (i = 0; i < imax; i++) {
		entry = (fy_hashMapIEntry*) (map->buckets[i]);
		while (entry != NULL) {
			fn(entry->key, entry->value, map->nullValue, addition);
			entry = entry->next;
		}
	}
}

FY_ATTR_EXPORT void fy_hashMapIDestroy(fy_memblock *mem, fy_hashMapI *this) {
	int i, imax;
	fy_hashMapIEntry *entry, *tmp;
#if FY_MEM_LOG_TO_CONTEXT
	fy_context *context = mem->gcContext;
	context->logEVarLn(context, "HashMapI.destroy %p.buckets == %p", this,
			this->buckets);
#endif
	for (i = 0, imax = 1 << this->bucketsFact; i < imax; i++) {
		entry = this->buckets[i];
		while (entry != NULL) {
			tmp = entry;
			entry = entry->next;
			fy_mmFree(mem, tmp);
		}
	}
	fy_mmFree(mem, this->buckets);
}
