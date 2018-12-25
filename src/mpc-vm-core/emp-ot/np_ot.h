#pragma once

#ifdef OT_NP_USE_MIRACL//use miracl version npot
#include "emp-ot/miracl_np_ot.h"
#include "emp-tool/utils/mir_util.h"
#else
#include "emp-ot/relic_np_ot.h"
#include "emp-tool/utils/relic_ec_utils.h"
#endif//OT_NP_USE_MIRACL

