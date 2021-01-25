#pragma once
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(x) ((SHORT)LOWORD(x))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(y) ((SHORT)HIWORD(y))
#endif
