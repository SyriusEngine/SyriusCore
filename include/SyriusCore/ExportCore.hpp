#pragma once

#include <SyriusUtils/SyriusUtils.hpp>

#if defined(SR_PLATFORM_WIN64)
    #if defined(SR_CORE_IMPORT)
        #define SR_CORE_API __declspec(dllimport)
    #else
        #if defined(SR_CORE_EXPORT)
            #define SR_CORE_API __declspec(dllexport)
        #else
            #define SR_CORE_API __declspec(dllexport)
        #endif
    #endif
#else
    #define SR_CORE_API

#endif