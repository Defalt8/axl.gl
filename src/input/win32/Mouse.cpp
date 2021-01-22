#include "common.hpp"
#include <axl.gl/lib.hpp>
#include <axl.math/vec/Vec2i.hpp>
#include <axl.gl/input/Mouse.hpp>

namespace axl {
namespace gl {
namespace input {

bool Mouse::isPresent()
{
	UINT num_devices = 0;
	RAWINPUTDEVICELIST *input_devices;
	if (GetRawInputDeviceList(nullptr, &num_devices, sizeof(RAWINPUTDEVICELIST)) != 0 || num_devices == 0)
		return false;
	input_devices = new  RAWINPUTDEVICELIST[num_devices];
	if (GetRawInputDeviceList(input_devices, &num_devices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
	{
		delete[] input_devices;
		return false;
	}
	for(UINT i = 0; i < num_devices; ++i)
	{
		if(input_devices[i].dwType == RIM_TYPEMOUSE)
			return true;
	}
	return false;
}

bool Mouse::isButtonDown(MouseButton button)
{
	int v_key;
	switch(button)
	{
		case MBTN_LEFT: v_key = VK_LBUTTON; break;
		case MBTN_RIGHT: v_key = VK_RBUTTON; break;
		case MBTN_MIDDLE: v_key = VK_MBUTTON; break;
		case MBTN_EXTRA: v_key = VK_XBUTTON1; break;
		default:
			return false;
	}
	return 0 != GetAsyncKeyState(v_key);
}

bool Mouse::setCursorPosition(int x, int y)
{
	return SetCursorPos(x, y) != FALSE;
}

bool Mouse::setCursorPosition(const axl::math::Vec2i& cur_pos)
{
	return SetCursorPos(cur_pos.x, cur_pos.y) != FALSE;
}

////////////////////
////
///  Mouse::Listener
//

struct MouseHookData
{
	HHOOK handle;
};

LRESULT CALLBACK HookMouseProc(int ncode, WPARAM wparam, LPARAM lparam)
{
	if(ncode == HC_ACTION && lparam)
	{
		switch(wparam)
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouse)
						Mouse::Listener::onMouse(axl::gl::input::MBTN_LEFT, data.pt.x, data.pt.y, (wparam == WM_LBUTTONDOWN));
				}
				return 0;
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouse)
						Mouse::Listener::onMouse(axl::gl::input::MBTN_RIGHT, data.pt.x, data.pt.y, (wparam == WM_RBUTTONDOWN));
				}
				return 0;
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouse)
						Mouse::Listener::onMouse(axl::gl::input::MBTN_MIDDLE, data.pt.x, data.pt.y, (wparam == WM_MBUTTONDOWN));
				}
				return 0;
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouse)
						Mouse::Listener::onMouse(axl::gl::input::MBTN_EXTRA, data.pt.x, data.pt.y, (wparam == WM_XBUTTONDOWN));
				}
				return 0;
			case WM_MOUSEMOVE:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouseMove)
						Mouse::Listener::onMouseMove(data.pt.x, data.pt.y);
				}
				return 0;
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
				{
					MSLLHOOKSTRUCT data = *((MSLLHOOKSTRUCT*)lparam);
					if(Mouse::Listener::onMouseWheel)
						Mouse::Listener::onMouseWheel((wparam == WM_MOUSEWHEEL), (int)GET_WHEEL_DELTA_WPARAM((data.mouseData)), data.pt.x, data.pt.y);
				}
				return 0;
			default: return 0;
		}
	}
	return CallNextHookEx(((MouseHookData*)Mouse::Listener::reserved)->handle, ncode, wparam, lparam);
}

bool Mouse::Listener::init()
{
	if(Mouse::Listener::isValid())
		return true;
	m_reserved = (void*)new MouseHookData();
	if(m_reserved)
	{
		((MouseHookData*)Mouse::Listener::m_reserved)->handle = SetWindowsHookExW(WH_MOUSE_LL, HookMouseProc, NULL, 0);
		return NULL != ((MouseHookData*)Mouse::Listener::m_reserved)->handle;
	}
	return false;
}

void Mouse::Listener::release()
{
	if(Mouse::Listener::isValid())
	{
		UnhookWindowsHookEx(((MouseHookData*)Mouse::Listener::m_reserved)->handle);
		delete (MouseHookData*)m_reserved;
	}
	m_reserved = (MouseHookData*)0;
}

bool Mouse::Listener::isValid()
{
	return m_reserved && ((MouseHookData*)m_reserved)->handle;
}

void (*Mouse::Listener::onMouse)(MouseButton button, int x, int y, bool is_down) = 0;
void (*Mouse::Listener::onMouseMove)(int x, int y) = 0;
void (*Mouse::Listener::onMouseWheel)(bool is_vertical, int delta, int x, int y) = 0;
void* Mouse::Listener::m_reserved = (void*)0;
const void*const Mouse::Listener::reserved = Mouse::Listener::m_reserved;

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl