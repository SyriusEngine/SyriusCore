#include "CoreCommand.hpp"

namespace Syrius{

    Time CoreCommand::m_StartupTime = 0;


    void CoreCommand::init() {

    }

    void CoreCommand::terminate() {

    }

    void CoreCommand::initGlad() {

    }

    void CoreCommand::terminateGlad() {

    }

    void CoreCommand::initPlatformGlad(GlPlatformDesc* glDesc){

    }

    void CoreCommand::terminatePlatformGlad() {

    }

    Time CoreCommand::getStartupTime() {
        return m_StartupTime;
    }
}