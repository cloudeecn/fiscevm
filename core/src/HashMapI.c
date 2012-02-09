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
#include "fy_util/HashMapI.h"

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

static fy_hashMapIEntry *getBucket(fy_memblock *mem, fy_hashMapI *this,
		fy_int key) {
	fy_hashMapIEntry *entry;

	if ((entry = this->buckets[key % this->bucketsSizeM1]) != NULL) {
		do {
			if (entry->key == key) {
				return entry;
			}
		} while ((entry = entry->next) != NULL);
	}
	return NULL;
}

FY_ATTR_EXPORT void fy_hashMapIInit(fy_memblock *mem, fy_hashMapI *this,
		fy_uint initFact, fy_uint loadFactor, fy_int nullValue,
		fy_exception *exception) {
	this->loadFactor = loadFactor;
	this->bucketsFact = initFact;
	this->nullValue = nullValue;
	this->bucketsSizeM1 = (1 << initFact) - 1;
	this->buckets = fy_mmAllocate(mem, sizeof(fy_hashMapIEntry*) << initFact,
			exception);
	this->size = 0;
}
FY_ATTR_EXPORT void fy_hashMapIInitSimple(fy_memblock *mem, fy_hashMapI *this,
		fy_int nullValue, fy_exception *exception) {
	fy_hashMapIInit(mem, this, 16, 12, nullValue, exception);
}
FY_ATTR_EXPORT fy_int fy_hashMapIPut(fy_memblock *mem, fy_hashMapI *this,
		int key, int value, fy_exception *exception) {
	fy_hashMapIEntry *entry;
	fy_hashMapIEntry *tmp;
	int pos;
	fy_int ret = this->nullValue;
	entry = getBucket(mem, this, key);
	if (entry == NULL) {
		entry = fy_mmAllocate(mem, sizeof(fy_hashMapIEntry), exception);
		FYEH()this->nullValue;
		FYEH()this->nullValue;
		entry->key = key;
		entry->value = value;

		if ((this->size + 1) * 16 > this->loadFactor << this->bucketsFact) {
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

FY_ATTR_EXPORT fy_int fy_hashMapIGet(fy_memblock *mem, fy_hashMapI *this,
		fy_int key) {
	fy_hashMapIEntry *entry = getBucket(mem, this, key);
	return entry == NULL ? this->nullValue : entry->value;
}

FY_ATTR_EXPORT void fy_hashMapIEachValue(fy_memblock *mem, fy_hashMapI *map,
		void(*fn)(fy_int key, fy_int value, fy_int nullValue, void *addition),
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
