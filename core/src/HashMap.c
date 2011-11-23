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
#include "fyc/HashMap.h"

static void expandBuckets(fy_context *context, fy_hashMap *this,
		fy_uint targetSize) {
	int i, imax;
	fy_hashMapEntry *entry;
	fy_hashMapEntry *tmp;
	int newPos;
	fy_hashMapEntry **newEntries;
	if (targetSize <= this->bucketsCount) {
		return;
	} else {
		newEntries = fy_vmAllocate(context,
				sizeof(fy_hashMapEntry*) * targetSize);
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
		fy_vmFree(context, this->buckets);
		this->buckets = newEntries;
		this->bucketsCount = targetSize;
	}

}

static fy_hashMapEntry *getBucket(fy_context *context, fy_hashMap *this,
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

void fy_hashMapInit(fy_context *context, fy_hashMap *this, fy_uint initSize,
		fy_uint loadFactor) {
	this->loadFactor = loadFactor;
	this->bucketsCount = initSize;
	this->buckets = fy_vmAllocate(context, sizeof(fy_hashMapEntry*) * initSize);
	this->size = 0;
}
void fy_hashMapInitSimple(fy_context *context, fy_hashMap *this) {
	fy_hashMapInit(context, this, 16, 12);
}
void *fy_hashMapPut(fy_context *context, fy_hashMap *this, fy_str *key,
		void *value) {
	fy_hashMapEntry *entry;
	fy_hashMapEntry *tmp;
	fy_str *keyClone;
	int pos;
	void *ret = NULL;
	entry = getBucket(context, this, key);
	if (entry == NULL) {
		entry = fy_vmAllocate(context, sizeof(fy_hashMapEntry));
		keyClone = fy_strAllocateClone(context,key);
		entry->key = keyClone;
		entry->keyHash = fy_strHash(keyClone);
		entry->value = value;

		if ((this->size + 1) * 16 > this->bucketsCount * this->loadFactor) {
			expandBuckets(context, this, this->bucketsCount << 1);
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

void *fy_hashMapPutUtf8(fy_context *context, fy_hashMap *this,
		const char *keyUtf8, void *value) {
	fy_str *key;
	void *ret;

	key = fy_allocate(sizeof(fy_str));
	fy_strInit(context, key, fy_utf8SizeS(keyUtf8, -1));
	fy_strAppendUTF8(context, key, keyUtf8, -1);

	ret = fy_hashMapPut(context, this, key, value);

	fy_strDestroy(context, key);
	fy_free(key);
	return ret;
}

void* fy_hashMapGet(fy_context *context, fy_hashMap *this, fy_str *key) {
	fy_hashMapEntry *entry = getBucket(context, this, key);
	return entry == NULL ? NULL : entry->value;
}

void fy_hashMapDestroy(fy_context *context, fy_hashMap *this) {
	int i, imax;
	fy_hashMapEntry *entry, *tmp;
	for (i = 0, imax = this->bucketsCount; i < imax; i++) {
		entry = this->buckets[i];
		while (entry != NULL) {
			fy_strDestroy(context, entry->key);
			fy_vmFree(context, entry->key);
			tmp = entry;
			entry = entry->next;
			fy_vmFree(context, tmp);
		}
	}
	fy_vmFree(context, this->buckets);
}
