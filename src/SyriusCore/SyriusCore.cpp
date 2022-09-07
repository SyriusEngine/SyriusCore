#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Core/CoreCommand.hpp"

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

    Image* createImage(const std::string& fileName, bool flipOnLoad){
        return new Image(fileName, flipOnLoad);
    }

    Image* createImage(const ubyte* pixelData, int32 width, int32 height, int32 channelCount){
        return new Image(pixelData, width, height, channelCount);
    }

    SyriusWindow* createWindow(const WindowDesc& windowDesc){
        return CoreCommand::createWindow(windowDesc);
    }

    void setDebugMessageCallback(handleDebugMessageFunc callback){
        DebugMessageHandler::setDebugMessageHandler(callback);
    }

    Time getTime(){
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    Time getTimeMilli(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

}