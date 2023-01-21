# SyriusCore
## About
SyriusCore is a library that is used in the Syrius Engine. It contains abstractions for the window manager and graphics
APIs. It does so by providing generic interface for a window and specifies a custom graphics API. Per platform and 
graphics API, there is a specific implementation of the library. The library also comes with a GUI library, which is
ImGui. The ImGui backends are all implemented in the SyriusCore library. 

### Platforms
* Windows
* Linux (In development)

### Graphics APIs
* OpenGL
* Direct3D11
* Vulkan (In development)
* Direct3D12 (planned)

## Installation
### Requirements
* A c++17 compiler
* A CMake build system
* A CMake version >= 3.16

## Build
Just use cmake to build the library. In the cmake file, a macro is defined that can be toggled to switch between
debug and release builds. If you want the release build, remove this line from the cmake script
```cmake
add_compile_definitions(SR_CORE_DEBUG)
```
IMPORTANT: all DLLs in the cmake output are necessary! This includes:
* syrius_core.dll
* vulkan-1.dll

## Usage
### General
All objects in the library are created using the factory pattern. This means that you never create an object directly.
Instead, call the appropriate factory method to create the object. The factory method will return a pointer to the object.
Managing the lifetime of the object is up to the user. The user can either use a smart pointer or manually delete the object.

All Objects are constructed based on a description. The description is a struct that contains all the information needed
to create the object. The description is passed to the factory method as a parameter.

### Window
#### Window creation
The window class is the main class of the library. It is used to create a window and to handle events. The window
is also responsible for maintaining a context. The context is sort of like a bridge between the window and the
graphics processor. A window is created using the following code:
```cpp
// Create a window description
WindowDesc wDesc;
wDesc.xPos = 200;
wDesc.yPos = 200;
wDesc.width = 1280;
wDesc.height = 720;
wDesc.title = "My Title";

// Create the window object
auto window = createWindow(wDesc);
```
#### Managing the window
Each window is handled by some loop to check if the window is still open. In this loop, the user can poll events and
if a corresponding context is available, swap the buffers. The following code shows how to do this:
```cpp
// check if the user has closed the window
while (window->isOpen()){
    // Poll events
    window->pollEvents();
    while (window->hasEvent()){ // Check if there are any events
        
        auto event = window->getEvent();    // get the first event
        if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
            window->close();
        }
        else if (event.type == Syrius::SR_EVENT_WINDOW_RESIZED){
            context->onResize(event.windowWidth, event.windowHeight);
        }
    }
    // swap buffers
    context->swapBuffers();
}
```

#### Draw GUIs in the window
The window class also provides a method to draw GUIs in the window. The GUIs are drawn using the ImGui library. Note that
a valid context must be created in order to draw using ImGui. The context object is also needed in order to clear the
backbuffer where ImGui has drawn to. The following code shows how to draw a GUI in the window:
```cpp
// Create a context
auto context = window->createContext(Syrius::SR_API_D3D11);
window->createImGuiContext();
```
Then, in the loop, make sure the following functions are called:
```cpp
window->onImGuiBegin();

// test window
ImGui::Begin("Info");

ImGui::End();

window->onImGuiEnd();
```
### Context
#### Context creation
The context object fully abstracts away platform/API specific code. It provides functions to issue different GPU commands
and to create resources in video memory. The context is created using the following code:
```cpp
// Create a context using the D3D11 API
auto context = window->createContext(Syrius::SR_API_D3D11);
```
#### drawing something
Only a few objects are needed in order to draw something to the screen. The necessary objects are:
* Vertex Description
* Shader
* Vertex Buffer
* Vertex Array

#### framebuffers
The context object also provides functions to create framebuffers. A framebuffer is a collection of textures that can be
read or written to. This includes color attachments, depth buffers and stencil buffers. Note that before ANY texture can bes
used to read from, the corresponding framebuffer must be unbound, and vice versa.

