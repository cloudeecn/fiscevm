#include "fiscedev.h"

#include "fyc/Thread.h"
FY_EXPORT void fy_nativeReturnInt(fy_VMContext *context, fy_thread *thread,
		jint value) {
	return fy_threadReturnInt(context, thread, value);
}

FY_EXPORT void fy_nativeReturnHandle(fy_VMContext *context, fy_thread *thread,
		jint value) {
	return fy_threadReturnHandle(context, thread, value);
}

FY_EXPORT void fy_nativeReturnLong(fy_VMContext *context, fy_thread *thread,
		jlong value) {
	return fy_threadReturnLong(context, thread, value);
}
