#pragma once

#include <vector>
#include <string>
#include <deque>
#include <cstdint>

#include "../ExportCore.hpp"

#define SR_DEFAULT_WIDTH 1280
#define SR_DEFAULT_HEIGHT 720

typedef enum SR_SUPPORTED_API {
    SR_API_NONE = 0x00,
    SR_API_OPENGL = 0x01,
    SR_API_VULKAN = 0x10,
    SR_API_D3D11 = 0x08,
    SR_API_D3D12 = 0x09,
} SR_SUPPORTED_API;


namespace Syrius {

    /**
     * @brief This function will return a string containing the name of the symbolic constant encoding the API.
     * @param api Symbolic API constant
     * @return string containing the name of the API
     */
    std::string SR_CORE_API getAPIName(SR_SUPPORTED_API api);
}

