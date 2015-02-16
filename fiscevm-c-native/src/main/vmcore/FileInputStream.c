/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 *fiscevmis free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 *fiscevmis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

/* pedantic: Every C code and header with structure definition in FiScE
 * should include following two headers at very first*/
#include "fy_util/Portable.h"
#include "fyc/Config.h"

#include "fyc/FileInputStream.h"
#include "fyc/Constants.h"
#include "fyc/InputStream.h"
#include <stdio.h>
#include <errno.h>

typedef struct {
	fy_boolean closed;
	FILE *fp;
} fisData;

static fy_int isRead(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fisData *data = is->data;
	int ret = fgetc(data->fp);
#if EOF != -1
	if (ret == EOF) {
		return -1;
	}
#endif
	return ret;
}

static fy_int isReadBlock(struct fy_context *context, fy_inputStream *is,
		void *target, fy_int size, fy_exception *exception) {
	fisData *data = is->data;
	fy_int read = (fy_int)fread(target, 1, size, data->fp);
	fy_int err;
	if (read == 0) {
		err = ferror(data->fp);
		if (err) {
			fy_fault(exception, FY_EXCEPTION_IO, "Error no: %"FY_PRINT32"d.",
					err);
			FYEH()0;
		} else {
			return -1;
		}
	}
	return read;
}

static fy_int isSkip(struct fy_context *context, fy_inputStream *is,
		fy_int size, fy_exception *exception) {
	fisData *data = is->data;
	if (data->closed) {
		fy_fault(exception, FY_EXCEPTION_IO, "closed.");
		FYEH()0;
	}
	if (fseek(data->fp, size, SEEK_CUR)) {
		fy_fault(exception, FY_EXCEPTION_IO, "%s", strerror(errno));
		return 0;
	} else {
		return size;
	}
}

static void isClose(struct fy_context *context, fy_inputStream *is,
		fy_exception *exception) {
	fisData *data = is->data;
	if (fclose(data->fp)) {
		fy_fault(exception, FY_EXCEPTION_IO, "%s", strerror(errno));
	}
}

static fy_inputStream* isOpen(struct fy_context *context, const char *name,
		fy_exception *exception) {
	FILE *fp = NULL;
	fy_inputStream *ret;
	char targetName[1024];
    const char *const *baseNames = context->isParam;
	fisData *data;
	fy_int baseLen;
	fy_boolean baseSlash, nameSlash;
    const char* baseName;
    fy_int i = 0;

    while((baseName=baseNames[i++])!=NULL){
        targetName[0] = 0;
        baseLen = (fy_int)strlen(baseName);
        if (baseName == NULL || (baseLen = (fy_int)strlen(baseName)) == 0) {
            baseLen = 1;
            baseName = ".";
        }
        
        baseSlash = baseName[baseLen - 1] == '/';
        nameSlash = name[0] == '/';
        
        if (baseSlash && nameSlash) {
            strncat(targetName, baseName, sizeof(targetName) - strlen(targetName) - 1);
            strncat(targetName, name + 1, sizeof(targetName) - strlen(targetName) - 1);
        } else if ((!baseSlash) && (!nameSlash)) {
            strncat(targetName, baseName, sizeof(targetName) - strlen(targetName) - 1);
            strncat(targetName, "/", sizeof(targetName) - strlen(targetName) - 1);
            strncat(targetName, name, sizeof(targetName) - strlen(targetName) - 1);
        } else {
            strncat(targetName, baseName, sizeof(targetName) - strlen(targetName) - 1);
            strncat(targetName, name, sizeof(targetName) - strlen(targetName) - 1);
        }
        fp = fopen(targetName, "rb");
        if(fp != NULL){
            break;
        }
    }
	
	if (fp != NULL) {
		ret = fy_mmAllocate(context->memblocks, sizeof(fy_inputStream),
				exception);
		if (exception->exceptionType != exception_none) {
			fclose(fp);
			return NULL;
		}
		data = fy_mmAllocate(context->memblocks, sizeof(fisData), exception);
		if (exception->exceptionType != exception_none) {
			fy_mmFree(context->memblocks, ret);
			fclose(fp);
			return NULL;
		}
		data->closed = FALSE;
		data->fp = fp;
		ret->data = data;
		ret->isClose = isClose;
		ret->isRead = isRead;
		ret->isReadBlock = isReadBlock;
		ret->isSkip = isSkip;
	} else {
		ret = NULL;
	}
	return ret;
}

void fy_fisInitInputStream(fy_context *context, fy_exception *exception) {
	const char defaultPath[] = "runtime";
	char *path;
	context->isOpen = isOpen;
	path = fy_mmAllocate(context->memblocks, sizeof(defaultPath), exception);
	FYEH();
	memcpy(path, defaultPath, sizeof(defaultPath));
	context->isParam = path;
}
