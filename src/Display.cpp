#include <axl.gl/platform.h>
#if PLATFORM==PLATFORM_WINDOWS
#include "win32/Display.cpp"
#else
#	error("Unimplemented platform!")
#endif