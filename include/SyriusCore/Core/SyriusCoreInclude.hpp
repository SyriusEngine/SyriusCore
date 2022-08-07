#pragma once

#include "PlatformDetection.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <deque>
#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <map>

namespace Syrius{

    typedef enum SR_SUPPORTED_API{
        SR_API_NONE     = 0x00,
        SR_API_OPENGL   = 0x01,
        SR_API_VULKAN   = 0x10,

#if defined(SR_PLATFORM_WIN64)
        SR_API_D3D11    = 0x08,
        SR_API_D3D12    = 0x09,
#endif

    } SR_SUPPORTED_API;

    typedef enum SR_PHYSICAL_DEVICE_TYPE {
        SR_PHYSICAL_DEVICE_NONE             = 0x00,
        SR_PHYSICAL_DEVICE_INTEGRATED_GPU   = 0x01,
        SR_PHYSICAL_DEVICE_DISCRETE_GPU     = 0x02,
        SR_PHYSICAL_DEVICE_VIRTUAL_GPU      = 0x03,
        SR_PHYSICAL_DEVICE_CPU              = 0x04,
    } SR_PHYSICAL_DEVICE_TYPE;

    typedef uint8_t uint8;
    typedef int8_t int8;
    typedef uint16_t uint16;
    typedef int16_t int16;
    typedef uint32_t uint32;
    typedef int32_t int32;
    typedef uint64_t uint64;
    typedef int64_t int64;
    typedef unsigned char ubyte;
    typedef char byte;

    typedef size_t Size;
    typedef uint64 Index;
    typedef uint64 Time;

    inline Time getTimeSinceEpoch(){
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    inline Time getTimeSinceEpochMilli(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

#if defined(SR_DEBUG_MODE)
    const static bool s_SyriusDebugMode = true;
#else
    const static bool s_SyriusDebugMode = false;
#endif

}

