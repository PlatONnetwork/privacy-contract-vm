#pragma once

#ifdef __cplusplus
#  ifndef EXTERN_C_BEG
#    define EXTERN_C_BEG extern "C" {
#    define EXTERN_C_END }
#  endif
#else
#define EXTERN_C_BEG
#define EXTERN_C_END
#endif

#define MPC_IGNORE_EXCEPTIONS(X) try { X; } catch (...) {}

#include "mpc_ns_define.h"

