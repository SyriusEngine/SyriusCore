#include "../../../include/SyirusCore/Core/SyriusCore.hpp"
#include "CoreCommand.hpp"

namespace Syrius {

    void syriusInit(){
        CoreCommand::init();
    }

    void syriusTerminate(){
        CoreCommand::terminate();
    }

    Time getStartupTime(){
        return CoreCommand::getStartupTime();
    }

    Time getElapsedTimeSinceStart(){
        return CoreCommand::getElapsedTimeSinceStart();
    }

}