cmake_minimum_required(VERSION 3.21)
project(SyriusCore)

set(CMAKE_CXX_STANDARD 17)

set(SYRIUS_CORE_SRC
        )

add_library(syrius_core SHARED)

add_executable(SyriusCore main.cpp)
