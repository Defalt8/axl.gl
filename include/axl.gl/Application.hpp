#pragma once
#include "lib.hpp"
#include "Display.hpp"

namespace axl {
namespace gl {
namespace Application {

AXLGLAPI bool init();
AXLGLAPI void quit(int quit_code = 0);
AXLGLAPI void exit(int quit_code = 0);
AXLGLAPI bool pollEvent(const Display& display);
AXLGLAPI void pollEvents(const Display& display);
AXLGLAPI void loopEvents(const Display& display);

AXLGLAPI void (*onQuit) (int quit_code);
AXLGLAPI void (*onExit) (int exit_code);
AXLGLAPI const bool& IS_QUITTING;

} // axl::gl::Application
} // axl::gl
} // axl
