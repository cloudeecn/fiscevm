/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fy_util/HashMap.h"

typedef struct fy_hashMapEntry {
	struct fy_str *key;
	fy_uint keyHash;
	void *value;
	struct fy_hashMapEntry *next;
} fy_hashMapEntry;

static void expandBuckets(fy_memblock *mem, fy_hashMap *this,
		fy_uint targetSize, fy_exception *exception) {
	int i, imax;
	fy_hashMapEntry *entry;
	fy_hashMapEntry *tmp;
	int newPos;
	fy_hashMapEntry **newEntries;
	if (targetSize <= this->bucketsCount) {
		return;
	} else {
		newEntries = fy_mmAllocate(mem, sizeof(fy_hashMapEntry*) * targetSize,
				exception);
		fy_exceptionCheckAndReturn(exception);
		for (i = 0, imax = this->bucketsCount; i < imax; i++) {
			if ((entry = this->buckets[i]) != NULL) {

				do {
					newPos = entry->keyHash % targetSize;
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
		this->bucketsCount = targetSize;
	}

}

static fy_hashMapEntry *getBucket(fy_memblock *mem, fy_hashMap *this,
		fy_str *key) {
	fy_uint hashCode;
	fy_hashMapEntry *entry;

	hashCode = fy_strHash(key);

	if ((entry = this->buckets[hashCode % this->bucketsCount]) != NULL) {
		do {
			if (fy_strCmp(entry->key, key) == 0) {
				return entry;
			}
		} while ((entry = entry->next) != NULL);
	}
	return NULL;
}

FY_ATTR_EXPORT void fy_hashMapInit(fy_memblock *mem, fy_hashMap *this, fy_uint initSize,
		fy_uint loadFactor, fy_exception *exception) {
	this->loadFactor = loadFactor;
	this->bucketsCount = initSize;
	this->buckets = fy_mmAllocate(mem, sizeof(fy_hashMapEntry*) * initSize,
			exception);
	this->size = 0;
}
FY_ATTR_EXPORT void fy_hashMapInitSimple(fy_memblock *mem, fy_hashMap *this,
		fy_exception *exception) {
	fy_hashMapInit(mem, this, 16, 12, exception);
}
FY_ATTR_EXPORT void *fy_hashMapPut(fy_memblock *mem, fy_hashMap *this, fy_str *key,
		void *value, fy_exception *exception) {
	fy_hashMapEntry *entry;
	fy_hashMapEntry *tmp;
	fy_str *keyClone;
	int pos;
	void *ret = NULL;
	entry = getBucket(mem, this, key);
	if (entry == NULL) {
		entry = fy_mmAllocate(mem, sizeof(fy_hashMapEntry), exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		keyClone = fy_strCreateClone(mem, key, exception);
		fy_exceptionCheckAndReturn(exception)NULL;
		entry->key = keyClone;
		entry->keyHash = fy_strHash(keyClone);
		entry->value = value;

		if ((this->size + 1) * 16 > this->bucketsCount * this->loadFactor) {
			expandBuckets(mem, this, this->bucketsCount << 1, exception);
			fy_exceptionCheckAndReturn(exception)NULL;
		}

		pos = entry->keyHash % this->bucketsCount;

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

FY_ATTR_EXPORT void *fy_hashMapPutUtf8(fy_memblock *mem, fy_hashMap *this, const char *keyUtf8,
		void *value, fy_exception *exception) {
	fy_str *key;
	void *ret;

	key = fy_mmAllocate(mem, sizeof(fy_str), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strInit(mem, key, fy_utf8SizeS(keyUtf8, -1), exception);
	fy_exceptionCheckAndReturn(exception)NULL;
	fy_strAppendUTF8(mem, key, keyUtf8, -1, exception);
	fy_exceptionCheckAndReturn(exception)NULL;

	ret = fy_hashMapPut(mem, this, key, value, exception);
	fy_exceptionCheckAndReturn(exception)NULL;

	fy_strDestroy(mem, key);
	fy_mmFree(mem, key);
	return ret;
}

FY_ATTR_EXPORT void* fy_hashMapGet(fy_memblock *mem, fy_hashMap *this, fy_str *key) {
	fy_hashMapEntry *entry = getBucket(mem, this, key);
	return entry == NULL ? NULL : entry->value;
}

FY_ATTR_EXPORT void fy_hashMapEachValue(fy_memblock *mem, fy_hashMap *map,
		void(*fn)(fy_str *key, void *value, void *addition), void *addition) {
	fy_uint i, imax, j, jmax;
	fy_hashMapEntry *entry;
	imax = map->bucketsCount;
	for (i = 0; i < imax; i++) {
		entry = (fy_hashMapEntry*) (map->buckets[i]);
		while (entry != NULL) {
			fn(entry->key, entry->value, addition);
			entry = entry->next;
		}
	}
}

FY_ATTR_EXPORT void fy_hashMapDestroy(fy_memblock *mem, fy_hashMap *this) {
	int i, imax;
	fy_hashMapEntry *entry, *tmp;
	for (i = 0, imax = this->bucketsCount; i < imax; i++) {
		entry = this->buckets[i];
		while (entry != NULL) {
			fy_strDestroy(mem, entry->key);
			fy_mmFree(mem, entry->key);
			tmp = entry;
			entry = entry->next;
			fy_mmFree(mem, tmp);
		}
	}
	fy_mmFree(mem, this->buckets);
}
