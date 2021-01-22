#include <axl.gl/platform.h>

#if PLATFORM==PLATFORM_WINDOWS
#	include "win32/Application.cpp"
#else
#	error("Unimplemented platform")
#endif