
# axl.gl

A simple and multiplatform C++ game library.

## Version and supported platforms

- **Latest version**: 0.20.37 alpha
- **Supported platforms**: Windows

## Requirements and Dependencies

- **Requires**
  - A windows or linux desktop platform.
  - [CMake]("https://cmake.org/download"). *"An open-source, cross-platform family of tools designed to build, test and package software."*
  - *cmake*, *gcc* and *c++* executable directories must be added to the `PATH` environment variable.
- **Depends on**
  - *{ `axl.glfl` } -- The C++ OpenGL function loader library.*
  - *The standard C library.*
  - *OpenGL library*: { `opengl32` } on windows and { `GL` } on linux.
  - *Platform specific libraries:* { `gdi32`, `user32` } on windows, { `X11` } on linux.

## Building Guide

- Clone this repository: `git clone https://github.com/defalt8/axl.gl.git`
- Initialize CMake build directory.
  - `cd axl.gl`
  - `mkdir out`
  - `mkdir out/build`
  - `cd out/build`
  - `cmake -S ../../`
- Configure the cmake cache. *Default values are listed first.*
  - `cmake . -DCMAKE_BUILD_TYPE=Debug` -- `Debug|Release|MinSizeRel|RelWithDebInfo`
  - `cmake . -DAXLGL_TYPE=STATIC` -- `STATIC|SHARED`
  - `cmake . -BUILD_TESTS=ON` -- `OFF|ON`
- Build
  - `cmake --build .`
- Running Tests
  - Tests are build when both `BUILD_TESTS` and `AXLGL_BUILD_TESTS` are set to `ON`.
  - Tests are build in the directory `${AXLGL_RUNTIME_DIR}` by default, where all shared libraries are build as well.
  - Some generators add `${CMAKE_BUILD_TYPE}` as suffix to build paths. So, you should check for that.
  - `out/build/bin/axl.gl.lib` -- Basic library tests
- Installing library
  - By default:
    - Headers will be installed to `${CMAKE_INSTALL_PREFIX}/include`
    - Library archives will be installed to `${CMAKE_INSTALL_PREFIX}/lib`
    - Shared libraries will be installed to `${CMAKE_INSTALL_PREFIX}/bin`
  - `cmake . -DCMAKE_INSTALL_PREFIX=<path_to_install>`
  - `cmake --install .`

### Linking the library in a CMake project

- Configuring the project:
  - `set( AXLGL_BUILD_TESTS OFF )`
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

- [axl.gl/platform.h]("include/axl.gl/platform.h") - Defines PLATFORM and ABI macros based on the building platform.
- [axl.gl/lib.hpp]("include/axl.gl/lib.hpp") - Library base header. Declares VERSION and BUILD.
- [include/axl.gl/Display.hpp]("include/axl.gl/Display.hpp") - Display class.
- [include/axl.gl/Application.hpp]("include/axl.gl/Application.hpp") - Application functions.
- [include/axl.gl/View.hpp]("include/axl.gl/View.hpp") - View class.
- [include/axl.gl/Context.hpp]("include/axl.gl/Context.hpp") - OpenGL Context class.
- [include/axl.gl/ContextObject.hpp]("include/axl.gl/ContextObject.hpp") - OpenGL context dependent class.
- [include/axl.gl/gfx/texture/Texture.hpp]("include/axl.gl/gfx/texture/Texture.hpp") - Base texture abstract class.
- [include/axl.gl/gfx/texture/Texture1D.hpp]("include/axl.gl/gfx/texture/Texture1D.hpp") - 1D texture derived class.
- [include/axl.gl/gfx/texture/Texture2D.hpp]("include/axl.gl/gfx/texture/Texture2D.hpp") - 2D texture derived class.
- [include/axl.gl/gfx/texture/Texture3D.hpp]("include/axl.gl/gfx/texture/Texture3D.hpp") - 3D texture derived class.
- [include/axl.gl/gfx/textures.hpp]("include/axl.gl/gfx/textures.hpp") - Includes all headers under gfx/texture.
- [include/axl.gl/gfx/buffer/FrameBuffer.hpp]("include/axl.gl/gfx/buffer/FrameBuffer.hpp") - An OpenGL FrameBuffer implementation class.
- [include/axl.gl/gfx/buffer/RenderBuffer.hpp]("include/axl.gl/gfx/buffer/RenderBuffer.hpp") - An OpenGL RenderBuffer implementation class.
- [include/axl.gl/input/KeyCodes.hpp]("include/axl.gl/input/KeyCodes.hpp") - Key code enumerations and functions.
- [include/axl.gl/input/Keyboard.hpp]("include/axl.gl/input/Keyboard.hpp") - static Keyboard class methods and event listener.
- [include/axl.gl/input/Mouse.hpp]("include/axl.gl/input/Mouse.hpp") - static Mouse class methods and event listener.
- [include/axl.gl/input/Touch.hpp]("include/axl.gl/input/Touch.hpp") - static Touch class methods and event listener.
- [include/axl.gl/input/Key.hpp]("include/axl.gl/input/Key.hpp") - Key class to handle key press and other key events.
