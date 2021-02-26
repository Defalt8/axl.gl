#include "common.hpp"
#include <axl.gl/Display.hpp>
#include <axl.gl/View.hpp>
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
			display_data->device_name[0] = '\0';
			display_data->set = true;
			display_enum_counter = 0;
			_display_enum_index = -1;
			_display_enum_count = 0;
			ReleaseDC(desktop, hdc);
			DISPLAY_DEVICEA dev;
			dev.cb = sizeof(DISPLAY_DEVICEA);
			if (FALSE != EnumDisplayDevicesA(NULL, 0, &dev, 0))
			{
#				if defined(_MSC_VER)
					strncpy_s(display_data->device_name, 128, dev.DeviceString, 128);
#				else
					strncpy(display_data->device_name, dev.DeviceString, 128);
#				endif
			}
			return FALSE;
		}
	}
	++display_enum_counter;
	ReleaseDC(desktop, hdc);
	return TRUE;
}

Display::Settings::Settings(int bits_per_pixel, int width, int height, int frequency, Display::Settings::Rotation rotation) :
	bits_per_pixel(bits_per_pixel), width(width), height(height), frequency(frequency), rotation(rotation), orientation(width >= height ? OR_LANDSCAPE : OR_PORTRAIT)
{}

Display::Settings& Display::Settings::operator=(const Display::Settings& settings)
{
	bits_per_pixel = settings.bits_per_pixel;
	width = settings.width;
	height = settings.height;
	frequency = settings.frequency;
	rotation = settings.rotation;
	orientation = width >= height ? OR_LANDSCAPE : OR_PORTRAIT;
	return *this;
}

Display::Display(int p_index) :
	index(m_index),
	position(m_position),
	size(m_size),
	physical_size(m_physical_size),
	ppmm(m_ppmm),
	ppi(m_ppi),
	name(m_name),
	settings(m_settings),
	m_index(-1),
	m_position(0,0),
	m_size(0,0),
	m_physical_size(0.0f,0.0f),
	m_ppmm(0.0f,0.0f),
	m_ppi(0.0f,0.0f),
	m_name(""),
	m_settings(),
	m_views(),
	m_reserved(new DisplayData())
{
	if(p_index >= 0 && p_index < Display::count())
		Display::reopen(p_index);
}
Display::~Display()
{
	this->close();
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
		m_physical_size.set((float)display_data->mil_size_x, (float)display_data->mil_size_y);
		m_ppmm.set((float)display_data->rect.right / display_data->mil_size_x, (float)display_data->rect.bottom / display_data->mil_size_y);
		m_ppi = m_ppmm * 25.4f;
#		if defined(_MSC_VER)
			strncpy_s(m_name, 128, display_data->device_name, 128);
#		else
			strncpy(m_name, display_data->device_name, 128);
#		endif
	}
	this->m_settings.bits_per_pixel = display_data->color_bits;
	this->m_settings.width = display_data->rect.right;
	this->m_settings.height = display_data->rect.bottom;
	this->m_settings.frequency = display_data->refresh_rate;
	this->m_settings.rotation = Settings::ROT_DEFAULT;
	this->m_settings.orientation = this->m_settings.width >= this->m_settings.height ? Settings::OR_LANDSCAPE : Settings::OR_PORTRAIT;
	for(axl::util::ds::UniList<axl::gl::View*>::Iterator it =  this->m_views.first(); it != this->m_views.end(); ++it)
	{
		if(*it) (*it)->onDisplayConfig(*this);
	}
	return true;
}
bool Display::close()
{
	if(m_reserved)
	{
		while(!this->m_views.isEmpty())
		{
			View* view = this->m_views.removeFirst();
			if(view) view->destroy();
		}
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
axl::math::Vec2i Display::getDefaultSize()
{
	return axl::math::Vec2i(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}
bool Display::enumSettings(Display::Settings* display_settings, int i)
{
	if(!display_settings || i < -1) return false;
	DEVMODEA devmode, prev_devmode;
	ZeroMemory(&devmode, sizeof(DEVMODEA));
	devmode.dmSize = sizeof(DEVMODEA);
	prev_devmode = devmode;
	if(i == Settings::DI_CURRENT || i == Settings::DI_DEFAULT)
	{
		if(FALSE != EnumDisplaySettingsA(NULL, (i == Settings::DI_DEFAULT ? ENUM_REGISTRY_SETTINGS : ENUM_CURRENT_SETTINGS), &devmode))
		{
			display_settings->bits_per_pixel = (int)devmode.dmBitsPerPel;
			display_settings->width = (int)devmode.dmPelsWidth;
			display_settings->height = (int)devmode.dmPelsHeight;
			display_settings->frequency = (int)devmode.dmDisplayFrequency;
			switch(devmode.dmDisplayOrientation)
			{
				default:
				case DMDO_DEFAULT: display_settings->rotation = Settings::ROT_DEFAULT; break;
				case DMDO_90: display_settings->rotation = Settings::ROT_CCW_90; break;
				case DMDO_180: display_settings->rotation = Settings::ROT_CCW_180; break;
				case DMDO_270: display_settings->rotation = Settings::ROT_CCW_270; break;
			}
			display_settings->orientation = display_settings->width >= display_settings->height ? Settings::OR_LANDSCAPE : Settings::OR_PORTRAIT;
			return true;
		}
		return false;
	}
	DWORD index = 0, true_index = 0;
	while(FALSE != EnumDisplaySettingsA(NULL, index, &devmode))
	{
		if(	devmode.dmBitsPerPel != prev_devmode.dmBitsPerPel ||
			devmode.dmPelsWidth != prev_devmode.dmPelsWidth ||
			devmode.dmPelsHeight != prev_devmode.dmPelsHeight ||
			devmode.dmDisplayFrequency != prev_devmode.dmDisplayFrequency ||
			devmode.dmDisplayOrientation != prev_devmode.dmDisplayOrientation )
		{
			if(i == true_index)
			{
				display_settings->bits_per_pixel = (int)devmode.dmBitsPerPel;
				display_settings->width = (int)devmode.dmPelsWidth;
				display_settings->height = (int)devmode.dmPelsHeight;
				display_settings->frequency = (int)devmode.dmDisplayFrequency;
				switch(devmode.dmDisplayOrientation)
				{
					default:
					case DMDO_DEFAULT: display_settings->rotation = Settings::ROT_DEFAULT; break;
					case DMDO_90: display_settings->rotation = Settings::ROT_CCW_90; break;
					case DMDO_180: display_settings->rotation = Settings::ROT_CCW_180; break;
					case DMDO_270: display_settings->rotation = Settings::ROT_CCW_270; break;
				}
				display_settings->orientation = display_settings->width >= display_settings->height ? Settings::OR_LANDSCAPE : Settings::OR_PORTRAIT;
				return true;
			}
			++true_index;
		}
		++index;
		prev_devmode = devmode;
	}
	return false;
}
bool Display::setSettings(const Display::Settings& display_settings, bool test)
{
	Settings Default;
	if(!enumSettings(&Default, Settings::DI_DEFAULT) && setSettings(Default))
		return false;
	DEVMODEA devmode;
	ZeroMemory(&devmode, sizeof(DEVMODEA));
	devmode.dmSize = sizeof(DEVMODEW);
	devmode.dmBitsPerPel = (DWORD)display_settings.bits_per_pixel;
	devmode.dmPelsWidth = (DWORD)display_settings.width;
	devmode.dmPelsHeight = (DWORD)display_settings.height;
	devmode.dmDisplayFrequency = (DWORD)display_settings.frequency;
	switch (display_settings.rotation)
	{
		default:
		case Settings::ROT_DEFAULT: devmode.dmDisplayOrientation = DMDO_DEFAULT; break;
		case Settings::ROT_CCW_90: devmode.dmDisplayOrientation = DMDO_90; break;
		case Settings::ROT_CCW_180: devmode.dmDisplayOrientation = DMDO_180; break;
		case Settings::ROT_CCW_270: devmode.dmDisplayOrientation = DMDO_270; break;
	}
	devmode.dmFields = (display_settings.bits_per_pixel >= 0 ? DM_BITSPERPEL : 0) |
		(display_settings.width >= 0 ? DM_PELSWIDTH : 0) |
		(display_settings.height >= 0 ? DM_PELSHEIGHT : 0) |
		(display_settings.frequency >= 0 ? DM_DISPLAYFREQUENCY : 0) |
		(display_settings.rotation >= 0 ? DM_DISPLAYORIENTATION : 0);
	return DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsA(&devmode, (test ? CDS_TEST : 0));
}
bool Display::restoreSettings()
{
	Settings Default;
	if(enumSettings(&Default, Settings::DI_DEFAULT) && setSettings(Default))
		return true;
	// last resort display size restore
	axl::math::Vec2i display_size = getDefaultSize();
	Default.width = display_size.x;
	Default.height = display_size.y;
	return setSettings(Default, false);
}

int Display::count()
{
	return GetSystemMetrics(SM_CMONITORS);
}
const axl::util::ds::UniList<axl::gl::View*>& Display::getViews() const
{
	return this->m_views;
}

bool Display::addView(View* view)
{
	if(!view || !this->isOpen()) return false;
	for(axl::util::ds::UniList<axl::gl::View*>::Iterator it = this->m_views.first(); it != this->m_views.end(); ++it)
	{
		if(*it && *it == view) return true;
	}
	return this->m_views.insertLast(view);
}

bool Display::removeView(View* view)
{
	if(!this->isOpen()) return false;
	return this->m_views.remove(view);
}


} // axl::gl
} // axl
