#include "../../include/SyriusCore/SyriusCore.hpp"
#include "Core/CoreCommand.hpp"

namespace Syrius {

    static CoreCommand* s_Instance = nullptr;

    void syriusCoreInit(){
        s_Instance = new CoreCommand();
    }

    void syriusCoreTerminate(){
        delete s_Instance;
    }

    Time getStartupTime(){
        SR_CORE_PRECONDITION(s_Instance != nullptr, "Syrius Core is not initialized");

        return s_Instance->getStartupTime();
    }

    Time getElapsedTimeSinceStart(){
        SR_CORE_PRECONDITION(s_Instance != nullptr, "Syrius Core is not initialized");

        return s_Instance->getElapsedTimeSinceStart();
    }

    Resource<Image> createImage(const std::string& fileName, bool flipOnLoad){
        return createResource<Image>(fileName, flipOnLoad);
    }

    Resource<Image> createImage(const ubyte* pixelData, int32 width, int32 height, SR_TEXTURE_FORMAT format){
        return createResource<Image>(pixelData, width, height, format);
    }

    Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc){
        SR_CORE_PRECONDITION(s_Instance != nullptr, "Syrius Core is not initialized");

        return s_Instance->createWindow(windowDesc);
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

    Size getAllocatedMemory(){
        return MemoryAllocationTracker::getTotalAllocated();
    }

    Size getFreedMemory(){
        return MemoryAllocationTracker::getTotalFreed();
    }

    Size getMemoryUsage(){
        return MemoryAllocationTracker::getUsage();
    }

    template<>
    SR_CORE_API float getRandom(float min, float max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_CORE_API uint64 getRandom(uint64 min, uint64 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint64> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_CORE_API int64 getRandom(int64 min, int64 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<int64> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_CORE_API uint32 getRandom(uint32 min, uint32 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint32> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_CORE_API int32 getRandom(int32 min, int32 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<int32> dist(min, max);
        return dist(gen);
    }

    template<>
    SR_CORE_API uint16 getRandom(uint16 min, uint16 max){
        SR_CORE_PRECONDITION(min < max, "Min value must be smaller than max value");
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint16> dist(min, max);
        return dist(gen);
    }

}