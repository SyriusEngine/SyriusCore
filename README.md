# SyriusCore
## About
SyriusCore is a library that abstracts a window manager and a wrapper for different graphics APIs. It is designed to be 
used in games and other applications that require a window and a graphics context. It is written in C++ and uses CMake 
as a build system. It is currently in development and is ready for use. It is currently only tested on Windows 10 and
Linux (Ubuntu 22.04). No support for MacOS is planned at the moment. Both OpenGL and DirectX 11 are supported. Vulkan
support is planned in the future.

The library is designed to be relatively easy to use. It should be easy to manage a window and draw some basic stuff to 
the screen, BUT advanced features must still be possible to access if wanted. This way, the library can be used for
simple applications, but also for more advanced applications. Beginner programmers can use the library to get started
with graphics programming, while more experienced programmers can use the library to get started with their own
graphics engine.

The library also comes with ImGui support. ImGui is a library that allows you to create a GUI for your application. It
is very easy to use and is very useful for debugging and for creating a simple GUI for your application. Both OpenGL
and DirectX 11 are supported. 

## Supported platforms
### OS
* Windows: Currently tested on Windows 10 and Windows 11.
* Linux: tested on Ubuntu 22.04.

### Graphics APIs
* OpenGL
* Direct3D11
* Vulkan (In development)
* Direct3D12 (planned)

### Notes
* To create a D3D11 Debug context on windows 11, the Graphics Tools feature must be installed. This is not installed by
  default on windows 11. To install it, go to "Turn Windows features on or off" in the control panel and check the 
  "Graphics Tools" box. After that, restart your computer. If you only want to use OpenGL or are running in release mode,
  you don't need to install this feature.

## Installation
### Requirements
* A C++17 compiler, such as MSVC 2019 or GCC 9.3.0. (MSVC is currently broken)
* A CMake build system
* A CMake version >= 3.16

### Build
Building the project is handled by CMake. To build the project, run the following commands in the root directory of the
project:
```
mkdir build
cd build
cmake ..
cmake .
```
This will build the DEBUG version of the library. To build the RELEASE version, remove the following line from the
CMakeLists.txt file:
```
add_compile_definitions(SR_CORE_DEBUG)
```
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
// Create a context description
ContextDesc cDesc;
cDesc.api = Syrius::SR_API_D3D11; // or SR_API_OPENGL
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
