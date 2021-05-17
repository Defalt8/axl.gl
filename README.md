
# axl.gl

A simple and multiplatform C++ game library.

## Version and supported platforms

- **Latest version**: 0.42.125 alpha
- **Supported platforms**: Windows
  - It is possible to implement this library for linux systems using the X11 library, and for android using the EGL library.
  - Other platforms will be implemented once the library is complete enough to be a game library.

## Requirements and Dependencies

- **Requires**
  - Visual Studio 2017 or higher on windows.
  - [CMake](https://cmake.org/download). *"An open-source, cross-platform family of tools designed to build, test and package software."*
- **Depends on**
  - *{ `axl.glfl` } -- A local C++ OpenGL function loader library.*
  - *{ `axl.util` } -- A local C++ data structures and utilities library.*
  - *{ `axl.math` } -- A local C++ maths library.*
  - **These libraries { `axl.glfl`, `axl.util`, `axl.math` } need to be presented to this CMake project. By default in the same directory.**
  - *[Freetype2](https://www.freetype.org/) library*.
  - *OpenGL library*: { `opengl32` } on windows and { `GL` } on linux.
  - *Platform specific libraries:* { `gdi32`, `user32` } on windows, { `X11` } on linux.

## Building Guide

- Clone this repositories in the same directory:
  - `git clone https://github.com/defalt8/axl.gl.git`
  - `git clone https://github.com/defalt8/axl.glfl.git`
  - `git clone https://github.com/defalt8/axl.util.git`
  - `git clone https://github.com/defalt8/axl.math.git`
- Place the [Freetype2](https://www.freetype.org/) **built** static library in the directory `lib` under the same directory.
- Initialize CMake build directory.
  - `cd axl.gl`
  - `mkdir out`
  - `mkdir out/build`
  - `cd out/build`
  - `cmake -S ../../`
- Configure the cmake cache. *Default values are listed first.*
  - `cmake . -DCMAKE_BUILD_TYPE=Debug` -- `Debug|Release|MinSizeRel|RelWithDebInfo`
  - `cmake . -DAXLGL_TYPE=STATIC` -- `STATIC|SHARED`
  - `cmake . -BUILD_TESTS=${AXLGL_STANDALONE}` -- `TRUE|FALSE` -- Tests are built if the library is not being included from another CMake project.
- Build
  - `cmake --build .`
- Running Tests
  - Tests are built in the directory `${AXLGL_RUNTIME_DIR}` by default, where all shared libraries are build as well.
  - Some generators add `${CMAKE_BUILD_TYPE}` as suffix to build paths. So, you should check for that.
  - Run `out/build/bin/axl.gl.lib` for basic library tests
- Installing library
  - By default:
    - Headers will be installed to `${CMAKE_INSTALL_PREFIX}/include`
    - Library archives will be installed to `${CMAKE_INSTALL_PREFIX}/lib`
    - Shared libraries will be installed to `${CMAKE_INSTALL_PREFIX}/bin`
  - `cmake . -DCMAKE_INSTALL_PREFIX=<path_to_install>`
  - `cmake --install .`

### Linking the library in a CMake project

- Configuring the project: *These are options you can set to configure the included library.*
  - `set( AXLGL_ARCHIVE_DIR "${PROJECT_BINARY_DIR}/lib" )`
  - `set( AXLGL_RUNTIME_DIR "${PROJECT_BINARY_DIR}/bin" )`
  - `set( AXLGL_INSTALL_HEADERS OFF CACHE BOOL )`
  - `set( AXLGL_INSTALL_ARCHIVES OFF CACHE BOOL )`
  - `set( AXLGL_INSTALL_RUNTIMES ON CACHE BOOL )`
- Including the project:
  - `set( AXLGL_RUNTIME_DIR "<your_runtime_dir>" )` -- Especially for linking the shared library.
  - `add_subdirectory( <axl.gl_path> [local_build_path] )` -- `axl.gl_path` is the path to this cmake project. `local_build_path` is your local build path and it is *\*optional*.
- Linking to a target:
  - `target_link_libraries( <target> <PUBLIC|PRIVATE> axl.gl )`

### Linking the built library

- First you need to build the library (STATIC or SHARED).
- Then you need to install the library to the path you desire.
- The static library base name is `axl.gls` or `axl.glsd` for Debug build type.
- The shared library base name is `axl.gl` or `axl.gld` for Debug build type.
- ***Make sure to define the macro `AXLGL_SHARED` if you are linking to the shared library.***
- Add the include and library archive directories, you installed to, to the compiler's include and link search directories.

## Library outline

### Header files

- [include/axl.gl/everything.hpp](/include/axl.gl/everything.hpp) - Includes every header in the library.
- [include/axl.gl/lib.hpp](/include/axl.gl/lib.hpp) - Library base header. Declares VERSION and BUILD.
- [include/axl.gl/platform.h](/include/axl.gl/platform.h) - Defines PLATFORM and ABI macros based on the building platform.
- [include/axl.gl/Display.hpp](/include/axl.gl/Display.hpp) - Display class.
- [include/axl.gl/Application.hpp](/include/axl.gl/Application.hpp) - Application functions.
- [include/axl.gl/View.hpp](/include/axl.gl/View.hpp) - View class.
- [include/axl.gl/Context.hpp](/include/axl.gl/Context.hpp) - OpenGL Context class.
- [include/axl.gl/ContextObject.hpp](/include/axl.gl/ContextObject.hpp) - OpenGL ContextObject class.
- [include/axl.gl/gfx.hpp](/include/axl.gl/gfx.hpp) - Includes all headers under axl.gl/gfx and its subdirectories.

#### Input

- [include/axl.gl/input/KeyCodes.hpp](/include/axl.gl/input/KeyCodes.hpp) - Key code enumerations and functions.
- [include/axl.gl/input/Keyboard.hpp](/include/axl.gl/input/Keyboard.hpp) - static Keyboard class methods and event listener.
- [include/axl.gl/input/Mouse.hpp](/include/axl.gl/input/Mouse.hpp) - static Mouse class methods and event listener.
- [include/axl.gl/input/Touch.hpp](/include/axl.gl/input/Touch.hpp) - static Touch class methods and event listener.
- [include/axl.gl/input/Key.hpp](/include/axl.gl/input/Key.hpp) - Key class to handle key press and other key events.
- [include/axl.gl/inputs.hpp](/include/axl.gl/inputs.hpp) - Includes all headers under axl.gl/input.

#### Projection

- [include/axl.gl/projection/Projection.hpp](/include/axl.gl/projection/Projection.hpp) - Base projection class
- [include/axl.gl/projection/Orthographic.hpp](/include/axl.gl/projection/Orthographic.hpp) - Derived projection class.
- [include/axl.gl/projection/Perspective.hpp](/include/axl.gl/projection/Perspective.hpp) - Derived projection class.
- [include/axl.gl/projections.hpp](/include/axl.gl/projections.hpp) - Includes all headers under axl.gl/projection.

#### GFX/Camera

- [include/axl.gl/camera/Camera3D.hpp](/include/axl.gl/camera/Camera3D.hpp) - A 3D Camera class.
- [include/axl.gl/cameras.hpp](/include/axl.gl/cameras.hpp) - Includes all headers under axl.gl/camera.

#### GFX/Texture

- [include/axl.gl/gfx/texture/Texture.hpp](/include/axl.gl/gfx/texture/Texture.hpp) - Base texture abstract class.
- [include/axl.gl/gfx/texture/Texture1D.hpp](/include/axl.gl/gfx/texture/Texture1D.hpp) - Derived 1D texture class.
- [include/axl.gl/gfx/texture/Texture2D.hpp](/include/axl.gl/gfx/texture/Texture2D.hpp) - Derived 2D texture class.
- [include/axl.gl/gfx/texture/Texture3D.hpp](/include/axl.gl/gfx/texture/Texture3D.hpp) - Derived 3D texture class.
- [include/axl.gl/gfx/textures.hpp](/include/axl.gl/gfx/textures.hpp) - Includes all headers under axl.gl/gfx/texture.

#### GFX/Buffer

- [include/axl.gl/gfx/buffer/FrameBuffer.hpp](/include/axl.gl/gfx/buffer/FrameBuffer.hpp) - OpenGL FrameBuffer class.
- [include/axl.gl/gfx/buffer/RenderBuffer.hpp](/include/axl.gl/gfx/buffer/RenderBuffer.hpp) - OpenGL RenderBuffer class.
- [include/axl.gl/gfx/buffers.hpp](/include/axl.gl/gfx/buffers.hpp) - Includes all headers under axl.gl/gfx/texture.

#### GFX/Shader

- [include/axl.gl/gfx/shader/Shader.hpp](/include/axl.gl/gfx/shader/Shader.hpp) - OpenGL shader class.
- [include/axl.gl/gfx/shader/Program.hpp](/include/axl.gl/gfx/shader/Program.hpp) - OpenGL program class.
- [include/axl.gl/gfx/shaders.hpp](/include/axl.gl/gfx/shaders.hpp) - Includes all headers under axl.gl/gfx/shader.

#### GFX/Text

- [include/axl.gl/gfx/text/Font.hpp](/include/axl.gl/gfx/text/Font.hpp) - Font class. Texture atlas implementation. Requires Freetype2 library.
- [include/axl.gl/gfx/text/Text.hpp](/include/axl.gl/gfx/text/Text.hpp) - Text class.
- [include/axl.gl/gfx/text.hpp](/include/axl.gl/gfx/text.hpp) - Includes all headers under axl.gl/gfx/text.

#### GFX/UI

- [include/axl.gl/gfx/UI/Component.hpp](/include/axl.gl/gfx/UI/Component.hpp) - The base UI component abstract class.
- [include/axl.gl/gfx/UI/Container.hpp](/include/axl.gl/gfx/UI/Container.hpp) - A `Component` derived class used to hold and organize UI elements.
- [include/axl.gl/gfx/UI/Element.hpp](/include/axl.gl/gfx/UI/Element.hpp) - An abstract `Component` derived class to be used as a base for all UI elements like buttons, text views, etc...
- [include/axl.gl/gfx/UI/Layout.hpp](/include/axl.gl/gfx/UI/Layout.hpp) - An abstarct class to be implemented by different UI components organizing layouts like `Linear layout`, `Grid layout`, etc...

#### GFX/UI/layouts

- [include/axl.gl/gfx/UI/layouts/Linear.hpp](/include/axl.gl/gfx/UI/layouts/Linear.hpp) - A `Layout` derived class used to organize components in a linear fashion either horizontally or vertically.
