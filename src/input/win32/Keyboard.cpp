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
		case VK_CANCEL: return KeyCode::KEY_CANCEL;
		case VK_BACK: return KeyCode::KEY_BACK;
		case VK_TAB: return KeyCode::KEY_TAB;
		case VK_CLEAR: return KeyCode::KEY_CLEAR;
		case VK_RETURN: return KeyCode::KEY_RETURN;
		case VK_SHIFT: return KeyCode::KEY_SHIFT;
		case VK_LSHIFT: return KeyCode::KEY_LSHIFT;
		case VK_RSHIFT: return KeyCode::KEY_RSHIFT;
		case VK_CONTROL: return KeyCode::KEY_CONTROL;
		case VK_LCONTROL: return KeyCode::KEY_LCONTROL;
		case VK_RCONTROL: return KeyCode::KEY_RCONTROL;
		case VK_MENU: return KeyCode::KEY_ALT;
		case VK_LMENU: return KeyCode::KEY_LALT;
		case VK_RMENU: return KeyCode::KEY_RALT;
		case VK_LWIN: return KeyCode::KEY_LCMD;
		case VK_RWIN: return KeyCode::KEY_RCMD;
		case VK_PAUSE: return KeyCode::KEY_PAUSE;
		case VK_CAPITAL: return KeyCode::KEY_CAPITAL;
		case VK_ESCAPE: return KeyCode::KEY_ESCAPE;
		case VK_SPACE: return KeyCode::KEY_SPACE;
		case VK_PRIOR: return KeyCode::KEY_PAGEUP;
		case VK_NEXT: return KeyCode::KEY_PAGEDOWN;
		case VK_END: return KeyCode::KEY_END;
		case VK_HOME: return KeyCode::KEY_HOME;
		case VK_LEFT: return KeyCode::KEY_LEFT;
		case VK_UP: return KeyCode::KEY_UP;
		case VK_RIGHT: return KeyCode::KEY_RIGHT;
		case VK_DOWN: return KeyCode::KEY_DOWN;
		case VK_SELECT: return KeyCode::KEY_SELECT;
		case VK_EXECUTE: return KeyCode::KEY_EXEC;
		case VK_SNAPSHOT: return KeyCode::KEY_SNAPSHOT;
		case VK_INSERT: return KeyCode::KEY_INSERT;
		case VK_DELETE: return KeyCode::KEY_DELETE;
		case VK_HELP: return KeyCode::KEY_HELP;
		case 0x30: return KeyCode::KEY_0;
		case 0x31: return KeyCode::KEY_1;
		case 0x32: return KeyCode::KEY_2;
		case 0x33: return KeyCode::KEY_3;
		case 0x34: return KeyCode::KEY_4;
		case 0x35: return KeyCode::KEY_5;
		case 0x36: return KeyCode::KEY_6;
		case 0x37: return KeyCode::KEY_7;
		case 0x38: return KeyCode::KEY_8;
		case 0x39: return KeyCode::KEY_9;
		case 0x41: return KeyCode::KEY_A;
		case 0x42: return KeyCode::KEY_B;
		case 0x43: return KeyCode::KEY_C;
		case 0x44: return KeyCode::KEY_D;
		case 0x45: return KeyCode::KEY_E;
		case 0x46: return KeyCode::KEY_F;
		case 0x47: return KeyCode::KEY_G;
		case 0x48: return KeyCode::KEY_H;
		case 0x49: return KeyCode::KEY_I;
		case 0x4A: return KeyCode::KEY_J;
		case 0x4B: return KeyCode::KEY_K;
		case 0x4C: return KeyCode::KEY_L;
		case 0x4D: return KeyCode::KEY_M;
		case 0x4E: return KeyCode::KEY_N;
		case 0x4F: return KeyCode::KEY_O;
		case 0x50: return KeyCode::KEY_P;
		case 0x51: return KeyCode::KEY_Q;
		case 0x52: return KeyCode::KEY_R;
		case 0x53: return KeyCode::KEY_S;
		case 0x54: return KeyCode::KEY_T;
		case 0x55: return KeyCode::KEY_U;
		case 0x56: return KeyCode::KEY_V;
		case 0x57: return KeyCode::KEY_W;
		case 0x58: return KeyCode::KEY_X;
		case 0x59: return KeyCode::KEY_Y;
		case 0x5A: return KeyCode::KEY_Z;
		case VK_NUMPAD0: return KeyCode::KEY_NUMPAD0;
		case VK_NUMPAD1: return KeyCode::KEY_NUMPAD1;
		case VK_NUMPAD2: return KeyCode::KEY_NUMPAD2;
		case VK_NUMPAD3: return KeyCode::KEY_NUMPAD3;
		case VK_NUMPAD4: return KeyCode::KEY_NUMPAD4;
		case VK_NUMPAD5: return KeyCode::KEY_NUMPAD5;
		case VK_NUMPAD6: return KeyCode::KEY_NUMPAD6;
		case VK_NUMPAD7: return KeyCode::KEY_NUMPAD7;
		case VK_NUMPAD8: return KeyCode::KEY_NUMPAD8;
		case VK_NUMPAD9: return KeyCode::KEY_NUMPAD9;
		case VK_MULTIPLY: return KeyCode::KEY_MULTIPLY;
		case VK_ADD: return KeyCode::KEY_ADD;
		case VK_SEPARATOR: return KeyCode::KEY_SEPARATOR;
		case VK_SUBTRACT: return KeyCode::KEY_SUBTRACT;
		case VK_DECIMAL: return KeyCode::KEY_DECIMAL;
		case VK_DIVIDE: return KeyCode::KEY_DIVIDE;
		case VK_F1: return KeyCode::KEY_F1;
		case VK_F2: return KeyCode::KEY_F2;
		case VK_F3: return KeyCode::KEY_F3;
		case VK_F4: return KeyCode::KEY_F4;
		case VK_F5: return KeyCode::KEY_F5;
		case VK_F6: return KeyCode::KEY_F6;
		case VK_F7: return KeyCode::KEY_F7;
		case VK_F8: return KeyCode::KEY_F8;
		case VK_F9: return KeyCode::KEY_F9;
		case VK_F10: return KeyCode::KEY_F10;
		case VK_F11: return KeyCode::KEY_F11;
		case VK_F12: return KeyCode::KEY_F12;
		case VK_F13: return KeyCode::KEY_F13;
		case VK_F14: return KeyCode::KEY_F14;
		case VK_F15: return KeyCode::KEY_F15;
		case VK_F16: return KeyCode::KEY_F16;
		case VK_F17: return KeyCode::KEY_F17;
		case VK_F18: return KeyCode::KEY_F18;
		case VK_F19: return KeyCode::KEY_F19;
		case VK_F20: return KeyCode::KEY_F20;
		case VK_F21: return KeyCode::KEY_F21;
		case VK_F22: return KeyCode::KEY_F22;
		case VK_F23: return KeyCode::KEY_F23;
		case VK_F24: return KeyCode::KEY_F24;
		case VK_NUMLOCK: return KeyCode::KEY_NUMLOCK;
		case VK_SCROLL: return KeyCode::KEY_SCROLL;
		case VK_PRINT: return KeyCode::KEY_PRINT;
		case VK_APPS: return KeyCode::KEY_APPS;
		case VK_SLEEP: return KeyCode::KEY_SLEEP;
		case VK_VOLUME_MUTE: return KeyCode::KEY_VOLUME_MUTE;
		case VK_VOLUME_DOWN: return KeyCode::KEY_VOLUME_DOWN;
		case VK_VOLUME_UP: return KeyCode::KEY_VOLUME_UP;
		case VK_MEDIA_NEXT_TRACK: return KeyCode::KEY_MEDIA_NEXT_TRACK;
		case VK_MEDIA_PREV_TRACK: return KeyCode::KEY_MEDIA_PREV_TRACK;
		case VK_MEDIA_STOP: return KeyCode::KEY_MEDIA_STOP;
		case VK_MEDIA_PLAY_PAUSE: return KeyCode::KEY_MEDIA_PLAY_PAUSE;
		case VK_LAUNCH_MAIL: return KeyCode::KEY_LAUNCH_MAIL;
		case VK_LAUNCH_MEDIA_SELECT: return KeyCode::KEY_LAUNCH_MEDIA_SELECT;
		case VK_LAUNCH_APP1: return KeyCode::KEY_LAUNCH_APP1;
		case VK_LAUNCH_APP2: return KeyCode::KEY_LAUNCH_APP2;
		default:
			const UINT k_char = MapVirtualKeyW(platform_key_code, MAPVK_VK_TO_CHAR);
			switch(k_char)
			{
				case '`': return KeyCode::KEY_TILDE;
				case '-': return KeyCode::KEY_MINUS;
				case '=': return KeyCode::KEY_EQUALS;
				case '[': return KeyCode::KEY_OPENBRACE;
				case ']': return KeyCode::KEY_CLOSEBRACE;
				case '\\': return KeyCode::KEY_BACKSLASH;
				case '/': return KeyCode::KEY_FORWARDSLASH;
				case ';': return KeyCode::KEY_SEMICOLON;
				case '\'': return KeyCode::KEY_QUOTE;
				case ',': return KeyCode::KEY_COMMA;
				case '.': return KeyCode::KEY_PERIOD;
				default: break;
			}
	}
	return KeyCode::KEY_UNKNOWN;
}

int Keyboard::mapKeyCode(KeyCode key_code)
{
	switch (key_code)
	{
		case KeyCode::KEY_CANCEL: return VK_CANCEL;
		case KeyCode::KEY_BACK: return VK_BACK;
		case KeyCode::KEY_TAB: return VK_TAB;
		case KeyCode::KEY_CLEAR: return VK_CLEAR;
		case KeyCode::KEY_RETURN: return VK_RETURN;
		case KeyCode::KEY_SHIFT: return VK_SHIFT;
		case KeyCode::KEY_LSHIFT: return VK_LSHIFT;
		case KeyCode::KEY_RSHIFT: return VK_RSHIFT;
		case KeyCode::KEY_CONTROL: return VK_CONTROL;
		case KeyCode::KEY_LCONTROL: return VK_LCONTROL;
		case KeyCode::KEY_RCONTROL: return VK_RCONTROL;
		case KeyCode::KEY_ALT: return VK_MENU;
		case KeyCode::KEY_LALT: return VK_LMENU;
		case KeyCode::KEY_RALT: return VK_RMENU;
		case KeyCode::KEY_CMD: return VK_LWIN;
		case KeyCode::KEY_LCMD: return VK_LWIN;
		case KeyCode::KEY_RCMD: return VK_RWIN;
		case KeyCode::KEY_PAUSE: return VK_PAUSE;
		case KeyCode::KEY_CAPITAL: return VK_CAPITAL;
		case KeyCode::KEY_ESCAPE: return VK_ESCAPE;
		case KeyCode::KEY_SPACE: return VK_SPACE;
		case KeyCode::KEY_PAGEUP: return VK_PRIOR;
		case KeyCode::KEY_PAGEDOWN: return VK_NEXT;
		case KeyCode::KEY_END: return VK_END;
		case KeyCode::KEY_HOME: return VK_HOME;
		case KeyCode::KEY_LEFT: return VK_LEFT;
		case KeyCode::KEY_UP: return VK_UP;
		case KeyCode::KEY_RIGHT: return VK_RIGHT;
		case KeyCode::KEY_DOWN: return VK_DOWN;
		case KeyCode::KEY_SELECT: return VK_SELECT;
		case KeyCode::KEY_EXEC: return VK_EXECUTE;
		case KeyCode::KEY_SNAPSHOT: return VK_SNAPSHOT;
		case KeyCode::KEY_INSERT: return VK_INSERT;
		case KeyCode::KEY_DELETE: return VK_DELETE;
		case KeyCode::KEY_HELP: return VK_HELP;
		case KeyCode::KEY_0: return 0x30;
		case KeyCode::KEY_1: return 0x31;
		case KeyCode::KEY_2: return 0x32;
		case KeyCode::KEY_3: return 0x33;
		case KeyCode::KEY_4: return 0x34;
		case KeyCode::KEY_5: return 0x35;
		case KeyCode::KEY_6: return 0x36;
		case KeyCode::KEY_7: return 0x37;
		case KeyCode::KEY_8: return 0x38;
		case KeyCode::KEY_9: return 0x39;
		case KeyCode::KEY_A: return 0x41;
		case KeyCode::KEY_B: return 0x42;
		case KeyCode::KEY_C: return 0x43;
		case KeyCode::KEY_D: return 0x44;
		case KeyCode::KEY_E: return 0x45;
		case KeyCode::KEY_F: return 0x46;
		case KeyCode::KEY_G: return 0x47;
		case KeyCode::KEY_H: return 0x48;
		case KeyCode::KEY_I: return 0x49;
		case KeyCode::KEY_J: return 0x4A;
		case KeyCode::KEY_K: return 0x4B;
		case KeyCode::KEY_L: return 0x4C;
		case KeyCode::KEY_M: return 0x4D;
		case KeyCode::KEY_N: return 0x4E;
		case KeyCode::KEY_O: return 0x4F;
		case KeyCode::KEY_P: return 0x50;
		case KeyCode::KEY_Q: return 0x51;
		case KeyCode::KEY_R: return 0x52;
		case KeyCode::KEY_S: return 0x53;
		case KeyCode::KEY_T: return 0x54;
		case KeyCode::KEY_U: return 0x55;
		case KeyCode::KEY_V: return 0x56;
		case KeyCode::KEY_W: return 0x57;
		case KeyCode::KEY_X: return 0x58;
		case KeyCode::KEY_Y: return 0x59;
		case KeyCode::KEY_Z: return 0x5A;
		case KeyCode::KEY_NUMPAD0: return VK_NUMPAD0;
		case KeyCode::KEY_NUMPAD1: return VK_NUMPAD1;
		case KeyCode::KEY_NUMPAD2: return VK_NUMPAD2;
		case KeyCode::KEY_NUMPAD3: return VK_NUMPAD3;
		case KeyCode::KEY_NUMPAD4: return VK_NUMPAD4;
		case KeyCode::KEY_NUMPAD5: return VK_NUMPAD5;
		case KeyCode::KEY_NUMPAD6: return VK_NUMPAD6;
		case KeyCode::KEY_NUMPAD7: return VK_NUMPAD7;
		case KeyCode::KEY_NUMPAD8: return VK_NUMPAD8;
		case KeyCode::KEY_NUMPAD9: return VK_NUMPAD9;
		case KeyCode::KEY_MULTIPLY: return VK_MULTIPLY;
		case KeyCode::KEY_ADD: return VK_ADD;
		case KeyCode::KEY_SEPARATOR: return VK_SEPARATOR;
		case KeyCode::KEY_SUBTRACT: return VK_SUBTRACT;
		case KeyCode::KEY_DECIMAL: return VK_DECIMAL;
		case KeyCode::KEY_DIVIDE: return VK_DIVIDE;
		case KeyCode::KEY_F1: return VK_F1;
		case KeyCode::KEY_F2: return VK_F2;
		case KeyCode::KEY_F3: return VK_F3;
		case KeyCode::KEY_F4: return VK_F4;
		case KeyCode::KEY_F5: return VK_F5;
		case KeyCode::KEY_F6: return VK_F6;
		case KeyCode::KEY_F7: return VK_F7;
		case KeyCode::KEY_F8: return VK_F8;
		case KeyCode::KEY_F9: return VK_F9;
		case KeyCode::KEY_F10: return VK_F10;
		case KeyCode::KEY_F11: return VK_F11;
		case KeyCode::KEY_F12: return VK_F12;
		case KeyCode::KEY_F13: return VK_F13;
		case KeyCode::KEY_F14: return VK_F14;
		case KeyCode::KEY_F15: return VK_F15;
		case KeyCode::KEY_F16: return VK_F16;
		case KeyCode::KEY_F17: return VK_F17;
		case KeyCode::KEY_F18: return VK_F18;
		case KeyCode::KEY_F19: return VK_F19;
		case KeyCode::KEY_F20: return VK_F20;
		case KeyCode::KEY_F21: return VK_F21;
		case KeyCode::KEY_F22: return VK_F22;
		case KeyCode::KEY_F23: return VK_F23;
		case KeyCode::KEY_F24: return VK_F24;
		case KeyCode::KEY_NUMLOCK: return VK_NUMLOCK;
		case KeyCode::KEY_SCROLL: return VK_SCROLL;
		case KeyCode::KEY_PRINT: return VK_PRINT;
		case KeyCode::KEY_APPS: return VK_APPS;
		case KeyCode::KEY_SLEEP: return VK_SLEEP;
		case KeyCode::KEY_VOLUME_MUTE: return VK_VOLUME_MUTE;
		case KeyCode::KEY_VOLUME_DOWN: return VK_VOLUME_DOWN;
		case KeyCode::KEY_VOLUME_UP: return VK_VOLUME_UP;
		case KeyCode::KEY_MEDIA_NEXT_TRACK: return VK_MEDIA_NEXT_TRACK;
		case KeyCode::KEY_MEDIA_PREV_TRACK: return VK_MEDIA_PREV_TRACK;
		case KeyCode::KEY_MEDIA_STOP: return VK_MEDIA_STOP;
		case KeyCode::KEY_MEDIA_PLAY_PAUSE: return VK_MEDIA_PLAY_PAUSE;
		case KeyCode::KEY_LAUNCH_MAIL: return VK_LAUNCH_MAIL;
		case KeyCode::KEY_LAUNCH_MEDIA_SELECT: return VK_LAUNCH_MEDIA_SELECT;
		case KeyCode::KEY_LAUNCH_APP1: return VK_LAUNCH_APP1;
		case KeyCode::KEY_LAUNCH_APP2: return VK_LAUNCH_APP2;
		case KeyCode::KEY_TILDE: return '`';
		case KeyCode::KEY_MINUS: return '-';
		case KeyCode::KEY_EQUALS: return '=';
		case KeyCode::KEY_OPENBRACE: return '[';
		case KeyCode::KEY_CLOSEBRACE: return ']';
		case KeyCode::KEY_BACKSLASH: return '\\';
		case KeyCode::KEY_FORWARDSLASH: return '/';
		case KeyCode::KEY_SEMICOLON: return ';';
		case KeyCode::KEY_QUOTE: return '\'';
		case KeyCode::KEY_COMMA: return ',';
		case KeyCode::KEY_PERIOD: return '.';
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