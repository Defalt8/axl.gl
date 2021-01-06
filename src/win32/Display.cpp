#include "common.hpp"
#include <axl.gl/Display.hpp>
#include "DisplayData.hpp"

namespace axl {
namespace gl {

int _display_enum_index = -1;
int _display_enum_count = 0;

BOOL CALLBACK _AXLGL_InfoEnumProc(HMONITOR hmon, HDC hdc, LPRECT lprc, LPARAM lparam)
{
	MONITORINFO moninfo;
	ZeroMemory(&moninfo, sizeof(MONITORINFO));
	moninfo.cbSize = sizeof(MONITORINFO);
	if(0 == GetMonitorInfoW(hmon, &moninfo)) return TRUE;
	RECT rcmon = moninfo.rcMonitor;
	POINT pointmon = {moninfo.rcMonitor.left, moninfo.rcMonitor.top};
	HWND desktop = WindowFromPoint(pointmon);
	if(!desktop) return TRUE;
	hdc = GetDC(desktop);
	DisplayData *display_data = (DisplayData*)lparam;
	static int display_enum_counter = 0;
	if(_display_enum_index < 0 || _display_enum_index >= _display_enum_count) return FALSE;
	{
		if(display_enum_counter == _display_enum_index)
		{
			display_data->hmonitor = hmon;
			display_data->hdc = hdc;
			display_data->rect = rcmon;
			display_data->mil_size_x = GetDeviceCaps(hdc, HORZSIZE);
			display_data->mil_size_y = GetDeviceCaps(hdc, VERTSIZE);
			display_data->color_bits = GetDeviceCaps(hdc, BITSPIXEL);
			display_data->refresh_rate = GetDeviceCaps(hdc, VREFRESH);
			display_data->set = true;
			display_enum_counter = 0;
			_display_enum_index = -1;
			_display_enum_count = 0;
			ReleaseDC(desktop, hdc);
			return FALSE;
		}
	}
	++display_enum_counter;
	ReleaseDC(desktop, hdc);
	return TRUE;
}
Display::Display(int p_index) :
	index(m_index),
	position(m_position),
	size(m_size),
	physical_size(m_physical_size),
	ppmm(m_ppmm),
	ppi(m_ppi),
	m_index(-1),
	m_position(0,0),
	m_size(0,0),
	m_physical_size(0.0f,0.0f),
	m_ppmm(0.0f,0.0f),
	m_ppi(0.0f,0.0f),
	m_reserved(new DisplayData())
{
	if(p_index >= 0 && p_index < Display::count())
		Display::reopen(p_index);
}
Display::~Display()
{
	if(m_reserved)
	{
		delete (DisplayData*)m_reserved;
		m_reserved = (void*)0;
	}
}
bool Display::isOpen() const
{
	return m_reserved && ((DisplayData*)this->m_reserved)->set && m_index >= 0 && m_index < Display::count();
}
bool Display::reopen(int p_index)
{
	if(!m_reserved) return false;
	if(p_index == m_index) return true;
	int display_count = Display::count();
	if(p_index < 0 || p_index >= display_count) return false;
	_display_enum_index = p_index;
	_display_enum_count = display_count;
	EnumDisplayMonitors(NULL, NULL, _AXLGL_InfoEnumProc, (LPARAM)this->m_reserved);
	DisplayData* display_data = (DisplayData*)this->m_reserved;
	if(display_data->set)
	{
		m_index  = p_index;
		m_position.set(display_data->rect.left, display_data->rect.top);
		m_size.set(display_data->rect.right, display_data->rect.bottom);
		m_physical_size.set(display_data->mil_size_x, display_data->mil_size_y);
		m_ppmm.set((float)display_data->rect.right / display_data->mil_size_x, (float)display_data->rect.bottom / display_data->mil_size_y);
		m_ppi = m_ppmm * 25.4f;
	}
	return true;
}
bool Display::close()
{
	if(m_reserved)
	{
		m_index = -1;
		((DisplayData*)this->m_reserved)->set = true;
		return true;
	}
	return false;
}
bool Display::isConfigurable() const
{
	return true;
}

int Display::count()
{
	return GetSystemMetrics(SM_CMONITORS);
}

} // axl::gl
} // axl
