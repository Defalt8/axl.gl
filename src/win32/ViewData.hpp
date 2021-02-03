#pragma once
#include "common.hpp"

namespace axl {
namespace gl {

typedef struct
{
	HINSTANCE hinst;
	HWND hwnd;
	HDC hdc;
	HICON hicon_small;
	HICON hicon_big;
	HCURSOR hcursor;
	HCURSOR hcursor_custom;
	RECT prev_rect;
	DWORD style;
	bool is_recreating;
	bool from_message;
} ViewData;

} // namespace axl.gl
} // namespace axl