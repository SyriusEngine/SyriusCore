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

    Image* createImage(const std::vector<ubyte>& pixelData, int32 width, int32 height, int32 channelCount){
        return new Image(pixelData, width, height, channelCount);
    }

    SyriusWindow* createWindow(const WindowDesc& windowDesc){
        return CoreCommand::createWindow(windowDesc);
    }

}