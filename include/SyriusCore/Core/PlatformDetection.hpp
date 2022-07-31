#pragma once

#if defined(SR_DEBUG_MODE)

    #define SR_DEBUG_BREAK() __debugbreak()
#endif

// identify operating system and toolchain
#if defined(WIN64) || defined(_WIN64) || defined(WIN32)
    #define SR_PLATFORM_WIN64

#elif defined(__linux__)
    #define SR_PLATFORM_LINUX

#else
    #error "Platform not supported"
#endif


#if defined(_MSC_VER)
    #define SR_COMPILER_MSVC

    // This is some bs right here but it works
    #define and &&
    #define or ||

#elif defined(__MINGW64__)
    #define SR_COMPILER_MINGW_W64

#elif defined(__MINGW32__)
    #define SR_COMPILER_MINGW_W64

#elif defined(__GNUC__)
    #define SR_COMPILER_GCC

#else
    #error "Compiler is not supported"
#endif

// this is just for consistency
#define SR_FILE __FILE__
#define SR_LINE __LINE__
#define SR_FUNC __FUNCTION__

// portable dll import/export macros
#if defined(SR_PLATFORM_WIN64)
    #if defined(SR_IMPORT_DLL)
        #define SR_API __declspec(dllimport)
    #else
        #define SR_API __declspec(dllexport)
    #endif

#else
    #define SR_API

#endif




