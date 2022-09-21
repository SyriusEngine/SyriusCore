#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Core/CoreCommand.hpp"

namespace Syrius {

    void syriusCoreInit(){
        CoreCommand::init();
    }

    void syriusCoreTerminate(){
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
        return getTimeSinceEpoch();
    }

    Time getTimeMilli(){
        return getTimeSinceEpochMilli();
    }

    template<>
    SR_API float getRandom(float min, float max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_API uint64 getRandom(uint64 min, uint64 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint64> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_API int64 getRandom(int64 min, int64 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<int64> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_API uint32 getRandom(uint32 min, uint32 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint32> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_API int32 getRandom(int32 min, int32 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<int32> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_API uint16 getRandom(uint16 min, uint16 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint16> dist(min, max);
        return dist(gen);
    }

}