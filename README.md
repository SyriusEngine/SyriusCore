# SyriusCore
## About
SyriusCore is a library that is used in the Syrius Engine. It contains abstractions for the window manager and graphics
APIs. It does so by providing generic interface for a window and specifies a custom graphics API. Per platform and 
graphics API, there is a specific implementation of the library.

### Platforms
* Windows
* Linux (In development)

### Graphics APIs
* OpenGL
* Vulkan (In development)
* Direct3D11 (In development)
* Direct3D12 (planned)

## Installation
### Requirements
* A c++17 compiler
* A CMake build system
* A CMake version >= 3.21

## Build
Just use cmake to build the library. In the cmake file, a macro is defined that can be toggled to switch between
debug and release builds. If you want the release build, remove this line from the cmake script
```cmake
add_compile_definitions(SR_DEBUG_MODE)
```
IMPORTANT: all DLLs in the cmake output are necessary! This includes:
* syrius_core.dll
* vulkan-1.dll
