#pragma once

#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/miracl_hash.h"
#else
#include "emp-tool/utils/relic_hash.h"
#endif//OT_NP_USE_MIRACL
