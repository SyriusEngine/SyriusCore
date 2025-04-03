# SyriusCore
## About
SyriusCore is the core windows/graphicsAPI wrapper for the Syrius project, but can also be used in other projects.
The library provides an interface to control a window on Windows or UNIX systems. To draw stuff to a created window,
the library provides an interface to communicate with the GPU using OpenGL, Direct3D11 or Vulkan. There are a lot of
very similar libraries out there, this one is designed as a hobby project and to be easy to use within the Syrius 
engine.

## Supported platforms
### OS
* Windows: Currently tested on Windows 10 and Windows 11.
* UNIX: tested on Ubuntu 22.04.

### Graphics APIs
* OpenGL
* Direct3D11
* Vulkan (planned)

## Installation
### Requirements
* A C++17 compiler, such as MSVC 2019 or GCC 9.3.0.
* A CMake build system (version 3.16 or higher)

### Dependencies
Windows:
- Only for creating D3D11 debug devices, the Graphics Tools feature must be installed (not installed by default).
  To install it, go to "Turn Windows features on or off" in the control panel and check the "Graphics Tools" box. 
  After that, restart your computer.

UNIX:
- For X11 support, the following packages must be installed:
  ```
  sudo apt install mesa-common-dev xorg-dev
  ```


### Build
Building the project is handled by CMake. To build the project, run the following commands in the root directory of the
project:
```
mkdir build
cd build
cmake ..
make .
```
Optional CMake variables
- `-DBUILD_APP`: Builds an example application demonstrating the different provided objects.
- `-DBUILD_TESTS`: Builds the tests for the library. The tests are written using GoogleTest.

### Project integration
* All headers found in the include directory are public headers. They can be included in your project. For convenience,
  you can add the include directory to your include path. This way, you can include the headers like this:
  ```cpp
    #include <SyriusCore/SyriusCore.hpp>
    ```
* The header SyriusCore is a convenience header that includes all other headers. You can include it if you want to use
  the whole library.
* The library is built by default as a dynamic library. Other DLLs found in the build directory are also required to run
  the library. To use the library in your project, you can either copy the DLLs to the output directory of your project,
  or you can add the build directory to your PATH environment variable. This way, the DLLs will be found automatically.
  Note: such a DLL is the vulkan DLL.

## Usage
### General
Creating objects should be done by calling the create() function. This function returns a pointer to the created object.
This pointer is a custom smart pointer that is used to manage the lifetime of the object. The pointer is called a
Resource. Some objects can depend on other resources managed internally by the library. if this is the case, a ResourceView
is returned. A resource can create a resource view by calling the createView() function. Using these smart pointers
you don't have to worry about managing the lifetime of the objects. The library will take care of that for you.

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
        if (event.usage == Syrius::SR_EVENT_WINDOW_CLOSED){
            window->close();
        }
        else if (event.usage == Syrius::SR_EVENT_WINDOW_RESIZED){
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
// Create a context description
ContextDesc cDesc;
cDesc.api = SR_API_D3D11; // or SR_API_OPENGL
// Create the context
auto context = window->createContext(Syrius::SR_API_D3D11);
window->createImGuiContext(); // Create the ImGui context (managed internally by the context)
```
Then, in the loop, make sure the following functions are called:
```cpp
window->onImGuiBegin();

// test window
ImGui::Begin("Info");
// additional ImGui code
ImGui::End();

window->onImGuiEnd();
```
