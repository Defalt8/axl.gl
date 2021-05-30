#include "common.hpp"
#include <axl.gl/lib.hpp>
#include <axl.gl/input/KeyCodes.hpp>
#include <axl.gl/input/Keyboard.hpp>

namespace axl {
namespace gl {
namespace input {

bool Keyboard::isPresent()
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
		if(input_devices[i].dwType == RIM_TYPEKEYBOARD)
			return true;
	}
	return false;
}

bool Keyboard::isKeyDown(KeyCode key_code)
{
	return 0 != GetAsyncKeyState(mapKeyCode(key_code));
}

KeyCode Keyboard::mapPlatformKeyCode(int platform_key_code)
{
	switch (platform_key_code)
	{
		case VK_CANCEL: return KEY_CANCEL;
		case VK_BACK: return KEY_BACK;
		case VK_TAB: return KEY_TAB;
		case VK_CLEAR: return KEY_CLEAR;
		case VK_RETURN: return KEY_RETURN;
		case VK_SHIFT: return KEY_SHIFT;
		case VK_LSHIFT: return KEY_LSHIFT;
		case VK_RSHIFT: return KEY_RSHIFT;
		case VK_CONTROL: return KEY_CONTROL;
		case VK_LCONTROL: return KEY_LCONTROL;
		case VK_RCONTROL: return KEY_RCONTROL;
		case VK_MENU: return KEY_ALT;
		case VK_LMENU: return KEY_LALT;
		case VK_RMENU: return KEY_RALT;
		case VK_LWIN: return KEY_LCMD;
		case VK_RWIN: return KEY_RCMD;
		case VK_PAUSE: return KEY_PAUSE;
		case VK_CAPITAL: return KEY_CAPITAL;
		case VK_ESCAPE: return KEY_ESCAPE;
		case VK_SPACE: return KEY_SPACE;
		case VK_PRIOR: return KEY_PAGEUP;
		case VK_NEXT: return KEY_PAGEDOWN;
		case VK_END: return KEY_END;
		case VK_HOME: return KEY_HOME;
		case VK_LEFT: return KEY_LEFT;
		case VK_UP: return KEY_UP;
		case VK_RIGHT: return KEY_RIGHT;
		case VK_DOWN: return KEY_DOWN;
		case VK_SELECT: return KEY_SELECT;
		case VK_EXECUTE: return KEY_EXEC;
		case VK_SNAPSHOT: return KEY_SNAPSHOT;
		case VK_INSERT: return KEY_INSERT;
		case VK_DELETE: return KEY_DELETE;
		case VK_HELP: return KEY_HELP;
		case 0x30: return KEY_0;
		case 0x31: return KEY_1;
		case 0x32: return KEY_2;
		case 0x33: return KEY_3;
		case 0x34: return KEY_4;
		case 0x35: return KEY_5;
		case 0x36: return KEY_6;
		case 0x37: return KEY_7;
		case 0x38: return KEY_8;
		case 0x39: return KEY_9;
		case 0x41: return KEY_A;
		case 0x42: return KEY_B;
		case 0x43: return KEY_C;
		case 0x44: return KEY_D;
		case 0x45: return KEY_E;
		case 0x46: return KEY_F;
		case 0x47: return KEY_G;
		case 0x48: return KEY_H;
		case 0x49: return KEY_I;
		case 0x4A: return KEY_J;
		case 0x4B: return KEY_K;
		case 0x4C: return KEY_L;
		case 0x4D: return KEY_M;
		case 0x4E: return KEY_N;
		case 0x4F: return KEY_O;
		case 0x50: return KEY_P;
		case 0x51: return KEY_Q;
		case 0x52: return KEY_R;
		case 0x53: return KEY_S;
		case 0x54: return KEY_T;
		case 0x55: return KEY_U;
		case 0x56: return KEY_V;
		case 0x57: return KEY_W;
		case 0x58: return KEY_X;
		case 0x59: return KEY_Y;
		case 0x5A: return KEY_Z;
		case VK_NUMPAD0: return KEY_NUMPAD0;
		case VK_NUMPAD1: return KEY_NUMPAD1;
		case VK_NUMPAD2: return KEY_NUMPAD2;
		case VK_NUMPAD3: return KEY_NUMPAD3;
		case VK_NUMPAD4: return KEY_NUMPAD4;
		case VK_NUMPAD5: return KEY_NUMPAD5;
		case VK_NUMPAD6: return KEY_NUMPAD6;
		case VK_NUMPAD7: return KEY_NUMPAD7;
		case VK_NUMPAD8: return KEY_NUMPAD8;
		case VK_NUMPAD9: return KEY_NUMPAD9;
		case VK_MULTIPLY: return KEY_MULTIPLY;
		case VK_ADD: return KEY_ADD;
		case VK_SEPARATOR: return KEY_SEPARATOR;
		case VK_SUBTRACT: return KEY_SUBTRACT;
		case VK_DECIMAL: return KEY_DECIMAL;
		case VK_DIVIDE: return KEY_DIVIDE;
		case VK_F1: return KEY_F1;
		case VK_F2: return KEY_F2;
		case VK_F3: return KEY_F3;
		case VK_F4: return KEY_F4;
		case VK_F5: return KEY_F5;
		case VK_F6: return KEY_F6;
		case VK_F7: return KEY_F7;
		case VK_F8: return KEY_F8;
		case VK_F9: return KEY_F9;
		case VK_F10: return KEY_F10;
		case VK_F11: return KEY_F11;
		case VK_F12: return KEY_F12;
		case VK_F13: return KEY_F13;
		case VK_F14: return KEY_F14;
		case VK_F15: return KEY_F15;
		case VK_F16: return KEY_F16;
		case VK_F17: return KEY_F17;
		case VK_F18: return KEY_F18;
		case VK_F19: return KEY_F19;
		case VK_F20: return KEY_F20;
		case VK_F21: return KEY_F21;
		case VK_F22: return KEY_F22;
		case VK_F23: return KEY_F23;
		case VK_F24: return KEY_F24;
		case VK_NUMLOCK: return KEY_NUMLOCK;
		case VK_SCROLL: return KEY_SCROLL;
		case VK_PRINT: return KEY_PRINT;
		case VK_APPS: return KEY_APPS;
		case VK_SLEEP: return KEY_SLEEP;
		case VK_VOLUME_MUTE: return KEY_VOLUME_MUTE;
		case VK_VOLUME_DOWN: return KEY_VOLUME_DOWN;
		case VK_VOLUME_UP: return KEY_VOLUME_UP;
		case VK_MEDIA_NEXT_TRACK: return KEY_MEDIA_NEXT_TRACK;
		case VK_MEDIA_PREV_TRACK: return KEY_MEDIA_PREV_TRACK;
		case VK_MEDIA_STOP: return KEY_MEDIA_STOP;
		case VK_MEDIA_PLAY_PAUSE: return KEY_MEDIA_PLAY_PAUSE;
		case VK_LAUNCH_MAIL: return KEY_LAUNCH_MAIL;
		case VK_LAUNCH_MEDIA_SELECT: return KEY_LAUNCH_MEDIA_SELECT;
		case VK_LAUNCH_APP1: return KEY_LAUNCH_APP1;
		case VK_LAUNCH_APP2: return KEY_LAUNCH_APP2;
		default:
			const UINT k_char = MapVirtualKeyW(platform_key_code, MAPVK_VK_TO_CHAR);
			switch(k_char)
			{
				case '`': return KEY_TILDE;
				case '-': return KEY_MINUS;
				case '=': return KEY_EQUALS;
				case '[': return KEY_OPENBRACE;
				case ']': return KEY_CLOSEBRACE;
				case '\\': return KEY_BACKSLASH;
				case '/': return KEY_FORWARDSLASH;
				case ';': return KEY_SEMICOLON;
				case '\'': return KEY_QUOTE;
				case ',': return KEY_COMMA;
				case '.': return KEY_PERIOD;
				default: break;
			}
	}
	return KEY_UNKNOWN;
}

int Keyboard::mapKeyCode(KeyCode key_code)
{
	switch (key_code)
	{
		case KEY_CANCEL: return VK_CANCEL;
		case KEY_BACK: return VK_BACK;
		case KEY_TAB: return VK_TAB;
		case KEY_CLEAR: return VK_CLEAR;
		case KEY_RETURN: return VK_RETURN;
		case KEY_SHIFT: return VK_SHIFT;
		case KEY_LSHIFT: return VK_LSHIFT;
		case KEY_RSHIFT: return VK_RSHIFT;
		case KEY_CONTROL: return VK_CONTROL;
		case KEY_LCONTROL: return VK_LCONTROL;
		case KEY_RCONTROL: return VK_RCONTROL;
		case KEY_ALT: return VK_MENU;
		case KEY_LALT: return VK_LMENU;
		case KEY_RALT: return VK_RMENU;
		case KEY_CMD: return VK_LWIN;
		case KEY_LCMD: return VK_LWIN;
		case KEY_RCMD: return VK_RWIN;
		case KEY_PAUSE: return VK_PAUSE;
		case KEY_CAPITAL: return VK_CAPITAL;
		case KEY_ESCAPE: return VK_ESCAPE;
		case KEY_SPACE: return VK_SPACE;
		case KEY_PAGEUP: return VK_PRIOR;
		case KEY_PAGEDOWN: return VK_NEXT;
		case KEY_END: return VK_END;
		case KEY_HOME: return VK_HOME;
		case KEY_LEFT: return VK_LEFT;
		case KEY_UP: return VK_UP;
		case KEY_RIGHT: return VK_RIGHT;
		case KEY_DOWN: return VK_DOWN;
		case KEY_SELECT: return VK_SELECT;
		case KEY_EXEC: return VK_EXECUTE;
		case KEY_SNAPSHOT: return VK_SNAPSHOT;
		case KEY_INSERT: return VK_INSERT;
		case KEY_DELETE: return VK_DELETE;
		case KEY_HELP: return VK_HELP;
		case KEY_0: return 0x30;
		case KEY_1: return 0x31;
		case KEY_2: return 0x32;
		case KEY_3: return 0x33;
		case KEY_4: return 0x34;
		case KEY_5: return 0x35;
		case KEY_6: return 0x36;
		case KEY_7: return 0x37;
		case KEY_8: return 0x38;
		case KEY_9: return 0x39;
		case KEY_A: return 0x41;
		case KEY_B: return 0x42;
		case KEY_C: return 0x43;
		case KEY_D: return 0x44;
		case KEY_E: return 0x45;
		case KEY_F: return 0x46;
		case KEY_G: return 0x47;
		case KEY_H: return 0x48;
		case KEY_I: return 0x49;
		case KEY_J: return 0x4A;
		case KEY_K: return 0x4B;
		case KEY_L: return 0x4C;
		case KEY_M: return 0x4D;
		case KEY_N: return 0x4E;
		case KEY_O: return 0x4F;
		case KEY_P: return 0x50;
		case KEY_Q: return 0x51;
		case KEY_R: return 0x52;
		case KEY_S: return 0x53;
		case KEY_T: return 0x54;
		case KEY_U: return 0x55;
		case KEY_V: return 0x56;
		case KEY_W: return 0x57;
		case KEY_X: return 0x58;
		case KEY_Y: return 0x59;
		case KEY_Z: return 0x5A;
		case KEY_NUMPAD0: return VK_NUMPAD0;
		case KEY_NUMPAD1: return VK_NUMPAD1;
		case KEY_NUMPAD2: return VK_NUMPAD2;
		case KEY_NUMPAD3: return VK_NUMPAD3;
		case KEY_NUMPAD4: return VK_NUMPAD4;
		case KEY_NUMPAD5: return VK_NUMPAD5;
		case KEY_NUMPAD6: return VK_NUMPAD6;
		case KEY_NUMPAD7: return VK_NUMPAD7;
		case KEY_NUMPAD8: return VK_NUMPAD8;
		case KEY_NUMPAD9: return VK_NUMPAD9;
		case KEY_MULTIPLY: return VK_MULTIPLY;
		case KEY_ADD: return VK_ADD;
		case KEY_SEPARATOR: return VK_SEPARATOR;
		case KEY_SUBTRACT: return VK_SUBTRACT;
		case KEY_DECIMAL: return VK_DECIMAL;
		case KEY_DIVIDE: return VK_DIVIDE;
		case KEY_F1: return VK_F1;
		case KEY_F2: return VK_F2;
		case KEY_F3: return VK_F3;
		case KEY_F4: return VK_F4;
		case KEY_F5: return VK_F5;
		case KEY_F6: return VK_F6;
		case KEY_F7: return VK_F7;
		case KEY_F8: return VK_F8;
		case KEY_F9: return VK_F9;
		case KEY_F10: return VK_F10;
		case KEY_F11: return VK_F11;
		case KEY_F12: return VK_F12;
		case KEY_F13: return VK_F13;
		case KEY_F14: return VK_F14;
		case KEY_F15: return VK_F15;
		case KEY_F16: return VK_F16;
		case KEY_F17: return VK_F17;
		case KEY_F18: return VK_F18;
		case KEY_F19: return VK_F19;
		case KEY_F20: return VK_F20;
		case KEY_F21: return VK_F21;
		case KEY_F22: return VK_F22;
		case KEY_F23: return VK_F23;
		case KEY_F24: return VK_F24;
		case KEY_NUMLOCK: return VK_NUMLOCK;
		case KEY_SCROLL: return VK_SCROLL;
		case KEY_PRINT: return VK_PRINT;
		case KEY_APPS: return VK_APPS;
		case KEY_SLEEP: return VK_SLEEP;
		case KEY_VOLUME_MUTE: return VK_VOLUME_MUTE;
		case KEY_VOLUME_DOWN: return VK_VOLUME_DOWN;
		case KEY_VOLUME_UP: return VK_VOLUME_UP;
		case KEY_MEDIA_NEXT_TRACK: return VK_MEDIA_NEXT_TRACK;
		case KEY_MEDIA_PREV_TRACK: return VK_MEDIA_PREV_TRACK;
		case KEY_MEDIA_STOP: return VK_MEDIA_STOP;
		case KEY_MEDIA_PLAY_PAUSE: return VK_MEDIA_PLAY_PAUSE;
		case KEY_LAUNCH_MAIL: return VK_LAUNCH_MAIL;
		case KEY_LAUNCH_MEDIA_SELECT: return VK_LAUNCH_MEDIA_SELECT;
		case KEY_LAUNCH_APP1: return VK_LAUNCH_APP1;
		case KEY_LAUNCH_APP2: return VK_LAUNCH_APP2;
		case KEY_TILDE: return '`';
		case KEY_MINUS: return '-';
		case KEY_EQUALS: return '=';
		case KEY_OPENBRACE: return '[';
		case KEY_CLOSEBRACE: return ']';
		case KEY_BACKSLASH: return '\\';
		case KEY_FORWARDSLASH: return '/';
		case KEY_SEMICOLON: return ';';
		case KEY_QUOTE: return '\'';
		case KEY_COMMA: return ',';
		case KEY_PERIOD: return '.';
	}
	return (int)key_code;
}

////////////////////
////
///  Keyboard::Listener
//

struct KeyboardHookData
{
	HHOOK handle;
};


LRESULT CALLBACK HookKeyboardProc(int ncode, WPARAM wparam, LPARAM lparam)
{
	
	if(ncode == HC_ACTION && lparam)
	{
		KBDLLHOOKSTRUCT data = *((KBDLLHOOKSTRUCT*)lparam);
		switch(wparam)
		{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYUP:
				if(Keyboard::Listener::onKey)
				{
					bool is_down = (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN);
					switch(data.vkCode)
					{
						case VK_LCONTROL:
						case VK_RCONTROL:
							Keyboard::Listener::onKey(axl::gl::input::KeyCode::KEY_CONTROL, is_down);
							break;
						case VK_LSHIFT:
						case VK_RSHIFT:
							Keyboard::Listener::onKey(axl::gl::input::KeyCode::KEY_SHIFT, is_down);
							break;
						case VK_LMENU:
						case VK_RMENU:
							Keyboard::Listener::onKey(axl::gl::input::KeyCode::KEY_ALT, is_down);
							break;
						case VK_LWIN:
						case VK_RWIN:
							Keyboard::Listener::onKey(axl::gl::input::KeyCode::KEY_CMD, is_down);
							break;
					}
					Keyboard::Listener::onKey(input::Keyboard::mapPlatformKeyCode((int)data.vkCode), is_down);
				}
				return 0;
			default: return 0;
		}
	}
	return CallNextHookEx(((KeyboardHookData*)Keyboard::Listener::reserved)->handle, ncode, wparam, lparam);
}

bool Keyboard::Listener::init()
{
	if(Keyboard::Listener::isValid())
		return true;
	m_reserved = (void*)new KeyboardHookData();
	if(m_reserved)
	{
		((KeyboardHookData*)Keyboard::Listener::m_reserved)->handle = SetWindowsHookExW(WH_KEYBOARD_LL, HookKeyboardProc, NULL, 0);
		return NULL != ((KeyboardHookData*)Keyboard::Listener::m_reserved)->handle;
	}
	return false;
}

void Keyboard::Listener::release()
{
	if(Keyboard::Listener::isValid())
	{
		UnhookWindowsHookEx(((KeyboardHookData*)Keyboard::Listener::m_reserved)->handle);
		delete (KeyboardHookData*)m_reserved;
	}
	m_reserved = (KeyboardHookData*)0;
}

bool Keyboard::Listener::isValid()
{
	return m_reserved && ((KeyboardHookData*)m_reserved)->handle;
}

void (*Keyboard::Listener::onKey)(KeyCode key_code, bool is_down) = 0;
void* Keyboard::Listener::m_reserved = (void*)0;
const void*const Keyboard::Listener::reserved = Keyboard::Listener::m_reserved;

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl