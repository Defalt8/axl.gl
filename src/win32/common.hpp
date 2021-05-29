#pragma once
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(x) ((SHORT)LOWORD(x))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(y) ((SHORT)HIWORD(y))
#endif
#ifndef DMDO_DEFAULT
#	define DMDO_DEFAULT 0
#endif
#ifndef DMDO_90
#	define DMDO_90 1
#endif
#ifndef DMDO_180
#	define DMDO_180 2
#endif
#ifndef DMDO_270
#	define DMDO_270 3
#endif
#ifndef DM_DISPLAYORIENTATION
#	define DM_DISPLAYORIENTATION 0x80
#endif
