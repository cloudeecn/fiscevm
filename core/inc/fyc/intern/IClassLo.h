#ifndef FY_ICLASSLOADER_H
#define FY_ICLASSLOADER_H

#include "../ClassLdr.h"

fy_str *fy_clGetConstantString(fy_VMContext *context, fy_class *clazz,
		jchar idx);
fy_class *fy_clLoadclass(fy_VMContext *context, fy_str *name,
		fy_exception *exception);
void fy_clPhase2(fy_VMContext *context, fy_class *clazz,
		fy_exception *exception);

fy_data *fy_clOpenResource(fy_VMContext *context, fy_str *name);
void fy_clCloseResource(fy_VMContext *context, fy_data *data);

#endif
