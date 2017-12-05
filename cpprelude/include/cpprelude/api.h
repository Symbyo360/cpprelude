#pragma once

#include "cpprelude/defines.h"

#ifdef OS_WINDOWS
#ifdef CPPR_DLL
    #define API_CPPR __declspec(dllexport)
#else
	#define API_CPPR __declspec(dllimport)
#endif
#endif

#ifdef OS_LINUX
    #define API_CPPR
#endif