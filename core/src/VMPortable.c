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

#include "fyc/VMPortable.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
#include <sys/time.h>
#endif
#include <time.h>
typedef struct PortableData {
	fy_long initTimeInMillSec;
#if defined(_WIN32)
	LARGE_INTEGER lpFreq;
	LARGE_INTEGER lpPerfCountBegin;
	double perfIdv;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tvBeginTime;
#endif
} PortableData;

void fy_portInit(fy_context *context) {
	PortableData *pd;
	pd = (PortableData*) (context->portableData = fy_allocate(
			sizeof(PortableData)));
	pd->initTimeInMillSec = (fy_long) time(NULL) * 1000;
#if defined(_WIN32)
	QueryPerformanceFrequency(&(pd->lpFreq));
	QueryPerformanceCounter(&(pd->lpPerfCountBegin));
	pd->perfIdv = 1000000000.0 / pd->lpFreq.QuadPart;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	gettimeofday(&(pd->tvBeginTime),NULL);
#endif
}
void fy_portDestroy(fy_context *context) {
	PortableData *pd = (PortableData*) (context->portableData);

	fy_free(context->portableData);
}
fy_long fy_portTimeMillSec(fy_context *context) {
	PortableData *pd = (PortableData*) (context->portableData);
#if defined(_WIN32)
	fy_long timeDelta;
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	timeDelta = lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart;
	timeDelta = timeDelta * 1000 / pd->lpFreq.QuadPart;
	return timeDelta + pd->initTimeInMillSec;
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return ((fy_long)tv.tv_sec)*1000+((fy_long)tv.tv_usec)/1000;
#endif
	return 0;
}
fy_long fy_portTimeNanoSec(fy_context *context) {
	PortableData *pd = (PortableData*) (context->portableData);
#if defined(_WIN32)
	LARGE_INTEGER lPerfCount;
	QueryPerformanceCounter(&lPerfCount);
	return (fy_long) ((lPerfCount.QuadPart - pd->lpPerfCountBegin.QuadPart)
			* pd->perfIdv);
#elif defined(_POSIX_VERSION) || defined(_DARWIN_FEATURE_ONLY_UNIX_CONFORMANCE)
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return ((fy_long)tv.tv_sec)*1000000000+((fy_long)tv.tv_usec)*1000;
#endif
	return 0;
}
