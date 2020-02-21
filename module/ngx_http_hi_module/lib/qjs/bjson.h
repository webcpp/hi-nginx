#ifndef BJSON_H
#define BJSON_H

#include <stdio.h>
#include <stdlib.h>

#include "quickjs.h"

JSModuleDef* js_init_module_bjson(JSContext* ctx, const char* module_name);

#endif // !BJSON_H
