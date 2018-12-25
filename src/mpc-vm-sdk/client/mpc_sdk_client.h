#pragma once

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(PLATONMPCVM_SDK_STATIC)
#    define PLATONMPCVM_SDK_DLL_API 
#  else
#    if defined(PLATONMPCVM_SDK_EXPORTS)
#      define PLATONMPCVM_SDK_DLL_API __declspec(dllexport)
#    else
#      define PLATONMPCVM_SDK_DLL_API __declspec(dllimport)
#    endif
#  endif
#else
#  define PLATONMPCVM_SDK_DLL_API 
#endif
