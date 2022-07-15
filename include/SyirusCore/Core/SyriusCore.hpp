#pragma once

#include "SyriusCoreInclude.hpp"

namespace Syrius {

    void SR_API syriusInit();

    void SR_API syriusTerminate();

    Time SR_API getStartupTime();

    Time SR_API getElapsedTimeSinceStart();

}

