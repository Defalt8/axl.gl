#include <axl.gl/platform.h>

#if PLATFORM==PLATFORM_WINDOWS
#	include "win32/Touch.cpp"
#else
#	error "Unsupported platform"
#endif