#include "fiscedev.h"

#include "fyc/Thread.h"
_FY_EXPORT void fy_nativeReturnInt(fy_context *context, fy_thread *thread,
		fy_int value) {
	return fy_threadReturnInt(context, thread, value);
}

_FY_EXPORT void fy_nativeReturnHandle(fy_context *context, fy_thread *thread,
		fy_int value) {
	return fy_threadReturnHandle(context, thread, value);
}

_FY_EXPORT void fy_nativeReturnLong(fy_context *context, fy_thread *thread,
		jlong value) {
	return fy_threadReturnLong(context, thread, value);
}
