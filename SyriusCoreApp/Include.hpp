#pragma once

#include <iostream>

#include "../include/SyriusCore/SyriusCore.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm.hpp"
#include "../glm/ext.hpp"
#include "../glm/gtx/quaternion.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "EasyIni.hpp"

using namespace Syrius;

template<typename T>
using UP = std::unique_ptr<T>;

// RCP = Reference Counting Pointer => because i'm lazy and do not want to usage std::shared_pointer every time
template<typename T>
using RCP = std::shared_ptr<T>;

template<typename T, typename... Args>
inline UP<T> createUP(Args&&... args){
    return std::make_unique<T>(args...);
}

template<typename T, typename... Args>
inline RCP<T> createRCP(Args&&... args){
    return std::make_shared<T>(args...);
}
