#include <climits>
#include "common.hpp"
#include <axl.gl/lib.hpp>
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/float.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/util.hpp>
#include <axl.util/WString.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.glfl/win/wglext.hpp>
#include <axl.gl/Display.hpp>
#include <axl.gl/Application.hpp>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/input/Mouse.hpp>
#include <axl.gl/input/Keyboard.hpp>
#include <axl.gl/input/Touch.hpp>
#include <axl.gl/gfx/UI.hpp>
#include "ViewData.hpp"

namespace axl {
namespace gl {
using namespace axl::math;
using namespace axl::glfl;
namespace GL {
using namespace axl::glfl::WGL;
using namespace axl::glfl::core::GL;
}


//////////////////
// ViewConfig

ViewConfig::ViewConfig(long id_, PixelType pixel_type_, 
		char bits_color_, char bits_red_, char bits_green_, char bits_blue_, char bits_alpha_, 
		char bits_depth_, char bits_stencil_, 
		char samples_, bool double_buffered_, bool stereo_) :
	id(id_),
	pixel_type(pixel_type_),
	bits_color(bits_color_),
	bits_red(bits_red_),
	bits_green(bits_green_),
	bits_blue(bits_blue_),
	bits_alpha(bits_alpha_),
	bits_depth(bits_depth_),
	bits_stencil(bits_stencil_),
	samples(samples_),
	double_buffered(double_buffered_),
	stereo(stereo_)
{}

ViewConfig::ViewConfig(const ViewConfig& config) :
	id(config.id),
	pixel_type(config.pixel_type),
	bits_color(config.bits_color),
	bits_red(config.bits_red),
	bits_green(config.bits_green),
	bits_blue(config.bits_blue),
	bits_alpha(config.bits_alpha),
	bits_depth(config.bits_depth),
	bits_stencil(config.bits_stencil),
	samples(config.samples),
	double_buffered(config.double_buffered),
	stereo(config.stereo)
{}

bool ViewConfig::operator==(const ViewConfig& rhs) const
{
	return	(
		pixel_type == rhs.pixel_type &&
		bits_color == rhs.bits_color &&
		bits_red == rhs.bits_red &&
		bits_green == rhs.bits_green &&
		bits_blue == rhs.bits_blue &&
		bits_alpha == rhs.bits_alpha &&
		bits_depth == rhs.bits_depth &&
		bits_stencil == rhs.bits_stencil &&
		samples == rhs.samples &&
		double_buffered == rhs.double_buffered &&
		stereo == rhs.stereo
		);
}

bool ViewConfig::operator!=(const ViewConfig& rhs) const
{
	return	(
		pixel_type != rhs.pixel_type ||
		bits_color != rhs.bits_color ||
		bits_red != rhs.bits_red ||
		bits_green != rhs.bits_green ||
		bits_blue != rhs.bits_blue ||
		bits_alpha != rhs.bits_alpha ||
		bits_depth != rhs.bits_depth ||
		bits_stencil != rhs.bits_stencil ||
		samples != rhs.samples ||
		double_buffered != rhs.double_buffered ||
		stereo != rhs.stereo
		);
}
ViewConfig DefaultViewConfig = ViewConfig(
	0, // id
	ViewConfig::PT_RGB, // pixel_type
	24, // bits_color
	8, // bits_red
	8, // bits_green
	8, // bits_blue
	0, // bits_alpha
	24, // bits_depth
	8, // bits_stencil
	0, // samples
	false, // double_buffered
	false // stereo
);
const ViewConfig NullViewConfig = ViewConfig(
	-1, // id
	ViewConfig::PT_RGB, // pixel_type
	-1, // bits_color
	-1, // bits_red
	-1, // bits_green
	-1, // bits_blue
	-1, // bits_alpha
	-1, // bits_depth
	-1, // bits_stencil
	-1, // samples
	false, // double_buffered
	false // stereo
);

/////////
// View

static bool _cursors_loaded = false;
static HCURSOR _hcur_arrow = NULL;
static HCURSOR _hcur_cross = NULL;
static HCURSOR _hcur_hand = NULL;
static HCURSOR _hcur_wait = NULL;
static HCURSOR _hcur_help = NULL;
static HCURSOR _hcur_ibeam = NULL;
static HCURSOR _hcur_no = NULL;
static HBRUSH _hbrush_black = NULL;

LRESULT CALLBACK MWindowProc(HWND, UINT, WPARAM, LPARAM);

View::View(const axl::util::WString& title_, const axl::math::Vec2i& position_, const axl::math::Vec2i& size_, const Cursor& cursor_) :
	display(m_display),
	position(m_position),
	size(m_size),
	min_size(m_min_size),
	max_size(m_max_size),
	title(m_title),
	config(m_config),
	cursor(m_cursor),
	visiblity(m_visiblity),
	is_paused(m_is_paused),
	is_ui_processing_enabled(m_is_ui_processing_enabled),
	pointers(m_pointers),
	reserved(m_reserved),
	m_display(),
	m_position(position_),
	m_size(size_),
	m_min_size(axl::math::Vec2i(0,0)),
	m_max_size(axl::math::Vec2i::filled(INT_MAX/2)),
	m_title(title_),
	m_config(DefaultViewConfig),
	m_cursor(cursor_),
	m_visiblity(View::VS_HIDDEN),
	m_is_paused(false),
	m_is_ui_processing_enabled(false),
	m_contexts(),
	m_reserved((void*)new ViewData())
{
	for(int i=0; i<MAX_POINTERS; ++i) m_pointers[i] = false;
	setCursor(m_cursor);
	if(m_reserved)
	{
		((ViewData*)m_reserved)->hinst = GetModuleHandleW(0);
	}
}

View::~View()
{
	this->destroy();
	if(this->m_display) this->m_display->removeView(this);
	if(m_reserved)
	{
		delete ((ViewData*)m_reserved);
		m_reserved = (void*)0;
	}
}

bool View::isValid() const
{
	return (m_reserved && ((ViewData*)m_reserved)->hwnd && ((ViewData*)m_reserved)->hdc);
}

bool View::create(Display& _display, bool recreate, const ViewConfig* configs_, int configs_count_, unsigned long view_flags)
{
	if(!_cursors_loaded) // load cursors
	{
		_hcur_arrow = LoadCursor(NULL, IDC_ARROW);
		_hcur_cross = LoadCursor(NULL, IDC_CROSS);
		_hcur_hand = LoadCursor(NULL, IDC_HAND);
		_hcur_wait = LoadCursor(NULL, IDC_WAIT);
		_hcur_help = LoadCursor(NULL, IDC_HELP);
		_hcur_ibeam = LoadCursor(NULL, IDC_IBEAM);
		_hcur_no = LoadCursor(NULL, IDC_NO);
		_cursors_loaded = true;
	}
	if(!_hbrush_black)
		_hbrush_black = CreateSolidBrush(RGB(0,0,0));
	if(!m_reserved) m_reserved = (void*)new ViewData();
	if((view_flags & VF_TRANSPARENT) && !IsWindowsVistaOrGreater())
		return false;
	if(m_reserved)
	{
		ViewData* view_data = (ViewData*)m_reserved;
		((ViewData*)m_reserved)->is_recreating = recreate;
		if(recreate && axl::gl::View::isValid()) this->destroy();
		m_display = &_display;
		m_display->addView(this);
		if(!recreate)
		{
			if(((ViewData*)m_reserved)->hwnd && ((ViewData*)m_reserved)->hdc) return true;
			if(((ViewData*)m_reserved)->hwnd && !((ViewData*)m_reserved)->hdc)
			{
				((ViewData*)m_reserved)->hdc = GetDC(((ViewData*)m_reserved)->hwnd);
				if(((ViewData*)m_reserved)->hdc) return true;
			};
		}
		axl::util::WString class_name(L"AXL.GL.VIEW.");
		class_name += m_title.substring(12);
		DWORD style = 0, ex_style = 0;
		int view_type = view_flags & 0x3;
		switch(view_type)
		{
			default:
			case VF_FIXED:
				style = WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX|WS_BORDER|WS_CAPTION;
				break;
			case VF_RESIZABLE:
				style = WS_OVERLAPPEDWINDOW|WS_BORDER;
				break;
			case VF_POPUP:
				style = WS_POPUP;
				break;
		}
		if(view_flags & VF_TOPMOST)
			ex_style |= WS_EX_TOPMOST;
		if(view_flags & VF_TRANSPARENT)
			ex_style |= WS_EX_LAYERED;
		if(view_flags & VF_PASSTHROUGH)
			ex_style |= WS_EX_TRANSPARENT;
		HINSTANCE hinst = (HINSTANCE)GetModuleHandleW(0);
		HWND hwnd = NULL;
		HDC hdc = NULL;
		WNDCLASSEXW wc;
		bool registered = FALSE != GetClassInfoExW(hinst, class_name.cwstr(), &wc);
		if(!registered)
		{
			ZeroMemory(&wc, sizeof(WNDCLASSEXW));
			wc.cbSize = sizeof(WNDCLASSEXW);
			wc.hInstance = hinst;
			wc.lpszClassName = class_name.cwstr();
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = MWindowProc;
			wc.hbrBackground = (HBRUSH)_hbrush_black;
			registered = FALSE != RegisterClassExW(&wc);
		}
		if(registered)
		{
			RECT rc = {0, 0, m_size.x, m_size.y};
			AdjustWindowRectEx(&rc, style, FALSE, ex_style);
			hwnd = CreateWindowExW(ex_style, class_name.cwstr(), m_title.cwstr(), style,
				m_position.x+rc.left, m_position.y+rc.top,
				rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, hinst, NULL);
			if(hwnd)
			{
				SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
				if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED)
				{
					BOOL composition, opaque;
					DWORD color;
					SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
					if (IsWindowsVistaOrGreater() &&
						SUCCEEDED(DwmIsCompositionEnabled(&composition)) &&
						composition)
					{
						if (IsWindows8OrGreater() ||
							(SUCCEEDED(DwmGetColorizationColor(&color, &opaque)) &&
							!opaque))
						{
							HRGN region = CreateRectRgn(0,0,-1,-1);
							DWM_BLURBEHIND bb = {0};
							bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
							bb.hRgnBlur = region;
							bb.fEnable = TRUE;
							DwmEnableBlurBehindWindow(hwnd, &bb);
							DeleteObject(region);
						}
						else
						{
							DWM_BLURBEHIND bb = {0};
							bb.dwFlags = DWM_BB_ENABLE;
							DwmEnableBlurBehindWindow(hwnd, &bb);
						}
					}
				}
				hdc = GetDC(hwnd);
			}
		}
		else return false;
		view_data->hinst = hinst;
		view_data->hwnd = hwnd;
		view_data->hdc = hdc;
		view_data->style = style;
		view_data->destroying = false;
		view_data->destroy_from_message = false;
		if(!view_data->hcursor) setCursor(m_cursor);
		if(view_data->hwnd)
		{
			if(view_data->hicon_small)
			{
				SendMessageW(view_data->hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)view_data->hicon_small);
			}
			if(view_data->hicon_big)
			{
				SendMessageW(view_data->hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)view_data->hicon_big);
			}
		}
		if(axl::gl::View::isValid())
		{
			WGL::load();
			// set the pixelformat
			const ViewConfig *config;
			BOOL set = FALSE;
			for(int i=0; i<configs_count_+1; ++i)
			{
				if(i<configs_count_) config = &configs_[i];
				else config = &DefaultViewConfig;
				if(WGL::WGL_ARB_pixel_format)
				{
					UINT num_formats;
					const UINT max_formats = 32;
					int formats[max_formats];
					int pixel_type;
					switch(config->pixel_type)
					{
						default:
						case ViewConfig::PT_RGBA: pixel_type = WGL::WGL_TYPE_RGBA_ARB; break;
						case ViewConfig::PT_RGBA_FLOAT: pixel_type = WGL::WGL_TYPE_RGBA_FLOAT_ARB; break;
						case ViewConfig::PT_COLORINDEX: pixel_type = WGL::WGL_TYPE_COLORINDEX_ARB; break;
					}
					int attribs[] = {
						WGL::WGL_DRAW_TO_WINDOW_ARB, (int)GL::GL_TRUE,
						WGL::WGL_SUPPORT_OPENGL_ARB, (int)GL::GL_TRUE,
						WGL::WGL_STEREO_ARB, (int)(!config->stereo ? GL::GL_FALSE : GL::GL_TRUE),
						WGL::WGL_ACCELERATION_ARB, (int)WGL::WGL_FULL_ACCELERATION_ARB,
						WGL::WGL_PIXEL_TYPE_ARB, pixel_type,
						WGL::WGL_COLOR_BITS_ARB, config->bits_color,
						WGL::WGL_RED_BITS_ARB, config->bits_red,
						WGL::WGL_GREEN_BITS_ARB, config->bits_green,
						WGL::WGL_BLUE_BITS_ARB, config->bits_blue,
						WGL::WGL_ALPHA_BITS_ARB, config->bits_alpha,
						WGL::WGL_DEPTH_BITS_ARB, config->bits_depth,
						WGL::WGL_STENCIL_BITS_ARB, config->bits_stencil,
						WGL::WGL_DOUBLE_BUFFER_ARB, (int)(!config->double_buffered ? GL::GL_FALSE : GL::GL_TRUE),
						WGL::WGL_SAMPLE_BUFFERS_ARB, (int)(config->samples > 0 ? GL::GL_TRUE : GL::GL_FALSE),
						WGL::WGL_SAMPLES_ARB, config->samples,
						0
					};
					if(FALSE != WGL::wglChoosePixelFormatARB(hdc, attribs, NULL, max_formats, formats, &num_formats))
					{
						if(num_formats > 0)
						{
							PIXELFORMATDESCRIPTOR pfd;
							for(UINT i=0; i<num_formats; ++i)
							{
								DescribePixelFormat(hdc, formats[i], sizeof(pfd), &pfd);
								if(FALSE != (set = SetPixelFormat(hdc, formats[i], &pfd))) break;
							}
						}
					}
					else return false;
				}
				else if(config->pixel_type != ViewConfig::PT_RGBA_FLOAT)
				{
					BYTE pixel_type;
					switch(config->pixel_type)
					{
						default:
						case ViewConfig::PT_RGBA: pixel_type = PFD_TYPE_RGBA; break;
						case ViewConfig::PT_COLORINDEX: pixel_type = PFD_TYPE_COLORINDEX; break;
					}
					PIXELFORMATDESCRIPTOR pfd;
					ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
					pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
					pfd.nVersion = 1;
					pfd.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|(!config->double_buffered?0:PFD_DOUBLEBUFFER)|(!config->stereo?0:PFD_STEREO);
					pfd.iPixelType = pixel_type;
					pfd.cColorBits = config->bits_color;
					pfd.cRedBits = config->bits_red;
					pfd.cGreenBits = config->bits_green;
					pfd.cBlueBits = config->bits_blue;
					pfd.cAlphaBits = config->bits_alpha;
					pfd.cDepthBits = config->bits_depth;
					pfd.cStencilBits = config->bits_stencil;
					set = SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
				}
				if(set != FALSE)
				{
					m_config = *config;
					break;
				}
			}
			if(set == FALSE)
				m_config = DefaultViewConfig;
			SendMessageW(hwnd, WM_USER+1, 0, (LPARAM)this);
			return set != FALSE;
		}
	}
	return false;
}

void View::destroy()
{
	if(m_reserved && !((ViewData*)m_reserved)->destroying)
	{
		ViewData* view_data = (ViewData*)m_reserved;
		view_data->destroying = true;
		if(view_data->hdc) this->onDestroy(view_data->is_recreating);
		while(!this->m_contexts.isEmpty())
		{
			Context* contexts = this->m_contexts.removeFirst();
			if(contexts) contexts->destroy();
		}
		if(view_data->hwnd)
		{
			ReleaseDC(view_data->hwnd, view_data->hdc);
			view_data->hdc = NULL;
			// if(!view_data->destroy_from_message)
			DestroyWindow(view_data->hwnd);
			view_data->hwnd = NULL;
		}
		view_data->destroying = false;
	}
}

const void* View::getReserved() const
{
	return this->reserved;
}

void View::cleanup()
{
	if(_hbrush_black)
	{
		DeleteObject(_hbrush_black);
		_hbrush_black = NULL;
	}
}
void View::setUIEventProcessing(bool enable)
{
	this->m_is_ui_processing_enabled = enable;
}
bool View::setPosition(const axl::math::Vec2i& position_)
{
	if(!m_reserved) return false;
	ViewData* view_data = (ViewData*)m_reserved;
	DWORD style = GetWindowLongW(view_data->hwnd, GWL_STYLE), ex_style = GetWindowLongW(view_data->hwnd, GWL_EXSTYLE);
	RECT rc = {0, 0, m_size.x, m_size.y};
	AdjustWindowRectEx(&rc, style, FALSE, ex_style);
	if(SetWindowPos(((ViewData*)m_reserved)->hwnd, NULL, position_.x+rc.left, position_.y+rc.top, 0, 0, SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER) != FALSE)
	{
		this->m_position = position_;
		return true;
	}
	return false;
}

bool View::setSize(const axl::math::Vec2i& size_)
{
	if(!m_reserved) return false;
	ViewData* view_data = (ViewData*)m_reserved;
	DWORD style = GetWindowLongW(view_data->hwnd, GWL_STYLE), ex_style = GetWindowLongW(view_data->hwnd, GWL_EXSTYLE);
	RECT rc = {0, 0, size_.x, size_.y};
	AdjustWindowRectEx(&rc, style, FALSE, ex_style);
	if(SetWindowPos(((ViewData*)m_reserved)->hwnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOZORDER) != FALSE)
	{
		this->m_size = size_;
		return true;
	}
	return false;
}

bool View::setMinSize(const axl::math::Vec2i& min_size)
{
	if(min_size.x < 0 || min_size.y < 0)
		return false;
	this->m_min_size = min_size;
	return true;
}

bool View::setMaxSize(const axl::math::Vec2i& max_size)
{
	if(max_size.x < 0 || max_size.y < 0)
		return false;
	this->m_max_size = max_size;
	return true;
}

bool View::setTitle(const axl::util::WString& title_)
{
	if(!m_reserved) return false;
	if(SetWindowTextW(((ViewData*)m_reserved)->hwnd, title_.cwstr()) != FALSE)
	{
		this->m_title = title_;
		return true;
	}
	return false;
}

bool View::setCursor(const Cursor& cursor_)
{
	if(!m_reserved) return false;
	m_cursor = cursor_;
	HCURSOR hcursor = NULL;
	if(m_cursor == Cursor::CUR_CUSTOM)
	{
		hcursor = ((ViewData*)m_reserved)->hcursor_custom;
	}
	else
	{
		switch (m_cursor)
		{
			case Cursor::CUR_ARROW: hcursor = _hcur_arrow; break;
			case Cursor::CUR_CROSS: hcursor = _hcur_cross; break;
			case Cursor::CUR_HAND: hcursor = _hcur_hand; break;
			case Cursor::CUR_WAIT: hcursor = _hcur_wait; break;
			case Cursor::CUR_HELP: hcursor = _hcur_help; break;
			case Cursor::CUR_IBEAM: hcursor = _hcur_ibeam; break;
			case Cursor::CUR_NO: hcursor = _hcur_no; break;
			case Cursor::CUR_NONE:
				hcursor = NULL;
				m_cursor = Cursor::CUR_NONE;
				break;
			default: return false;
		}
	}
	((ViewData*)m_reserved)->hcursor = hcursor;
	SetCursor(hcursor);
	return true;
}

bool View::setCursorFromResource(int res_id)
{
	if(!m_reserved) return false;
	m_cursor = Cursor::CUR_CUSTOM;
	HCURSOR hcursor = LoadCursorW(((ViewData*)m_reserved)->hinst, MAKEINTRESOURCEW(res_id));
	if(!hcursor) return false;
	((ViewData*)m_reserved)->hcursor = hcursor;
	SetCursor(hcursor);
	return true;
}

bool View::setIcon(const axl::util::WString& icon_file)
{
	if(!m_reserved) return false;
	HICON icon_small = (HICON)NULL;
	HICON icon_big = (HICON)NULL;
	if(!icon_file.isNull())
	{
		icon_small = (HICON)LoadImageW(NULL, icon_file.cwstr(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		icon_big = (HICON)LoadImageW(NULL, icon_file.cwstr(), IMAGE_ICON, 48, 48, LR_LOADFROMFILE);
		if(!icon_small) return false;
		if(!icon_big) icon_big = icon_small;
	}
	if(((ViewData*)m_reserved)->hwnd)
	{
		SendMessageW(((ViewData*)m_reserved)->hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon_small);
		SendMessageW(((ViewData*)m_reserved)->hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon_big);
	}
	((ViewData*)m_reserved)->hicon_small = icon_small;
	((ViewData*)m_reserved)->hicon_big = icon_big;
	return true;
}

bool View::setIconFromResource(int res_id)
{
	if(!m_reserved) return false;
	LPWSTR res_str = MAKEINTRESOURCEW(res_id);
	HICON icon_small = (HICON)LoadImageW(((ViewData*)m_reserved)->hinst, res_str, IMAGE_ICON, 32, 32, 0);
	HICON icon_big = (HICON)LoadImageW(((ViewData*)m_reserved)->hinst, res_str, IMAGE_ICON, 48, 48, 0);
	if(!icon_small) return false;
	if(!icon_big) icon_big = icon_small;
	if(((ViewData*)m_reserved)->hwnd)
	{
		SendMessageW(((ViewData*)m_reserved)->hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon_small);
		SendMessageW(((ViewData*)m_reserved)->hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon_big);
	}
	((ViewData*)m_reserved)->hicon_small = icon_small;
	((ViewData*)m_reserved)->hicon_big = icon_big;
	return true;
}

bool View::isPointerCaptured() const
{
	return m_reserved && GetCapture() == ((ViewData*)m_reserved)->hwnd;
}

bool View::capturePointer(bool capture) const
{
	if(!m_reserved) return false;
	if(capture)
	{
		SetCapture(((ViewData*)m_reserved)->hwnd);
		return GetCapture() == ((ViewData*)m_reserved)->hwnd;
	}
	else
	{
		if(GetCapture() != ((ViewData*)m_reserved)->hwnd)
			return true;
		return FALSE != ReleaseCapture();
	}
}

bool View::show(ShowMode show_mode)
{
	ViewData* view_data = ((ViewData*)m_reserved);
	if(!m_reserved || !((ViewData*)m_reserved)->hwnd) return false;
	switch (show_mode)
	{
		case View::SM_HIDE:
			switch(m_visiblity)
			{
				case View::VS_HIDDEN:
				case View::VS_SHOWN:
				case View::VS_FULLSCREEN:
					ShowWindow(((ViewData*)m_reserved)->hwnd, SW_HIDE);
					break;
			}
			m_visiblity = View::VS_HIDDEN;
			break;
		case View::SM_SHOW:
			if(((ViewData*)m_reserved)->style & WS_POPUP)
			{
				this->onSize(m_size.x, m_size.y);
			}
			switch(m_visiblity)
			{
				case View::VS_HIDDEN:
					ShowWindow(((ViewData*)m_reserved)->hwnd, SW_NORMAL);
					break;
				case View::VS_SHOWN:
					ShowWindow(((ViewData*)m_reserved)->hwnd, SW_NORMAL);
					break;
				case View::VS_FULLSCREEN:
					{
						// restore the style and size
						SetWindowLongW(((ViewData*)m_reserved)->hwnd, GWL_STYLE, ((ViewData*)m_reserved)->style);
						if(SetWindowPos(((ViewData*)m_reserved)->hwnd, NULL, ((ViewData*)m_reserved)->prev_rect.left, ((ViewData*)m_reserved)->prev_rect.top, ((ViewData*)m_reserved)->prev_rect.right-((ViewData*)m_reserved)->prev_rect.left, ((ViewData*)m_reserved)->prev_rect.bottom-((ViewData*)m_reserved)->prev_rect.top, SWP_SHOWWINDOW))
						{
							m_visiblity = VS_SHOWN;
							return true;
						}
						else return false;
					}
					break;
			}
			m_visiblity = View::VS_SHOWN;
			break;
		case View::SM_FULLSCREEN:
			switch(m_visiblity)
			{
				case View::VS_SHOWN:
					//save the rect data
					GetWindowRect(((ViewData*)m_reserved)->hwnd, &((ViewData*)m_reserved)->prev_rect);
				case View::VS_FULLSCREEN:
					if(this->m_display->size ==  this->size) return true;
				case View::VS_HIDDEN:
					{
						// get the screen's size
						int screen_width = this->m_display->size.x;
						int screen_height = this->m_display->size.y;
						// set the window style and size
						SetWindowLongW(((ViewData*)m_reserved)->hwnd, GWL_STYLE, ((ViewData*)m_reserved)->style & ~(WS_CAPTION|WS_THICKFRAME|WS_SYSMENU));
						if(SetWindowPos(((ViewData*)m_reserved)->hwnd, NULL, 0, 0, screen_width, screen_height, SWP_SHOWWINDOW))
						{
							m_visiblity = View::VS_FULLSCREEN;
							return true;
						}
						else return false;
					}
					break;
			}
			m_visiblity = View::VS_FULLSCREEN;
			break;
	}
	return true;
}

bool View::setCursorPosition(const axl::math::Vec2i& pointer_position)
{
	if(!m_reserved) return false;
	return SetCursorPos(m_position.x + pointer_position.x, m_position.y + pointer_position.y) != FALSE;
}

bool View::swap() const
{
	if(!m_reserved) return false;
	return SwapBuffers(((ViewData*)m_reserved)->hdc) != FALSE;
}

const axl::util::ds::UniList<axl::gl::Context*>& View::getContexts() const
{
	return this->m_contexts;
}
///////////
// protected

bool View::addContext(Context* context)
{
	if(!context || !this->isValid()) return false;
	for(axl::util::ds::UniList<axl::gl::Context*>::Iterator it = this->m_contexts.first(); it != this->m_contexts.end(); ++it)
	{
		if(*it && *it == context) return true;
	}
	return this->m_contexts.insertFirst(context);
}
bool View::removeContext(Context* context)
{
	if(!context || !this->isValid()) return false;
	return this->m_contexts.remove(context);
}

//
// local functions
//

bool isInArea(axl::math::Vec2f point, axl::math::Vec2f size, axl::math::Mat4f transform, axl::gl::gfx::ui::Container* parent)
{
    using namespace axl::math;
    if(size.x == 0 || size.y == 0)
        return false;
    Mat4f final_transform = transform;
    // calculate the final transform matrix
    while(parent)
    {
        final_transform = parent->transform.getMatrix() * final_transform;
        parent = parent->getContainer();
    }
    Vec4f verteces[4];
    verteces[0] = final_transform * Vec4f(0.f, 0.f, 0.f, 1.f);
    verteces[1] = final_transform * Vec4f(size.x, 0.f, 0.f, 1.f);
    verteces[2] = final_transform * Vec4f(size.x, size.y, 0.f, 1.f);
    verteces[3] = final_transform * Vec4f(0.f, size.y, 0.f, 1.f);
    // Calculate the internal angles check if any of them are greater than 180 degrees.
    // If so, then the point is outside, or else it is inside.
    axl::math::Vec2f vector_a, vector_b;
    for(int i=0; i<4; ++i)
    {
        int next_i = (i+1)%4;
        vector_a.set(point.x - verteces[i].x, point.y - verteces[i].y);
        vector_b.set(point.x - verteces[next_i].x, point.y - verteces[next_i].y);
        float angle = vector_b.fullAngle(vector_a);
        if(angle < 0.f) // absolute value
            angle = -angle;
        if(angle > axl::math::Constants::F_PI)
            return false;
    }
    return true;
}

///////////
/// Events

void View::onDisplayConfig(const Display& _display)
{
	if(this->m_visiblity == VS_FULLSCREEN)
	{
		this->show(SM_FULLSCREEN);
	}
}

bool View::onCreate(bool recreating)
{
	if(this->isValid())
	{
		this->onDisplayConfig(*this->m_display);
		m_is_paused = false;
		return true;
	}
	return false;
}

void View::onDestroy(bool recreating)
{
	m_is_paused = true;
}

void View::onMove(int x, int y)
{
	this->m_position.set(x, y);
}

void View::onSize(int w,int h)
{
	this->m_size.set(w, h);
}

void View::onPause()
{
	this->m_is_paused = true;
}

void View::onResume()
{
	this->m_is_paused = false;
}

bool View::onKey(input::KeyCode key_code, bool is_down)
{
	bool kLeft = false, kRight = false;
	switch(key_code)
	{
		case axl::gl::input::KeyCode::KEY_LEFT: kLeft = is_down; break;
		case axl::gl::input::KeyCode::KEY_RIGHT: kRight = is_down; break;
	}
	if(this->m_is_ui_processing_enabled && (kLeft ^ kRight))
	{
		for(axl::util::ds::UniList<axl::gl::Context*>::Iterator ctx_it = m_contexts.first(); ctx_it.isNotNull(); ++ctx_it)
		{
			const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& components = (*ctx_it)->getComponents();
			for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = components.first(); it.isNotNull(); ++it)
			{
				axl::gl::gfx::ui::Component* component = *it;
				if(!component->isVisible()) continue;
				switch(component->component_type)
				{
					case axl::gl::gfx::ui::Component::CONTAINER:
						break;
					case axl::gl::gfx::ui::Component::ELEMENT:
						{
							axl::gl::gfx::ui::Element* element = (axl::gl::gfx::ui::Element*)component;
							switch(element->element_type)
							{
								case axl::gl::gfx::ui::Element::ET_OTHER:
								case axl::gl::gfx::ui::Element::ET_IMAGE_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_INPUT:
									{
										if(element->hasInputFocus())
										{
											axl::gl::gfx::ui::elements::TextInput* text_input = (axl::gl::gfx::ui::elements::TextInput*)element;
											{
												if(kLeft ^ kRight)
												{
													axl::util::size_t index = text_input->getCursorIndex();
													if(kLeft)
													{
														if(index != -1)
															--index;
													}
													else if(kRight)
													{
														if(index == -1)
															index = 0;
														else
															++index;
													}
													text_input->setCursorIndex(index);
												}
											}
										}
									}
									break;
								case axl::gl::gfx::ui::Element::ET_BUTTON:
									break;
							}
						}
						break;
				}
			}
		}
		return true;
	}
	return false;
}

void View::onChar(wchar_t char_code)
{
	if(this->m_is_ui_processing_enabled)
	{
		switch(char_code)
		{
			case (axl::util::WString::char_t)127:
				return;
		}
		for(axl::util::ds::UniList<axl::gl::Context*>::Iterator ctx_it = m_contexts.first(); ctx_it.isNotNull(); ++ctx_it)
		{
			const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& components = (*ctx_it)->getComponents();
			for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = components.first(); it.isNotNull(); ++it)
			{
				axl::gl::gfx::ui::Component* component = *it;
				if(!component->isVisible()) continue;
				switch(component->component_type)
				{
					case axl::gl::gfx::ui::Component::CONTAINER:
						break;
					case axl::gl::gfx::ui::Component::ELEMENT:
						{
							axl::gl::gfx::ui::Element* element = (axl::gl::gfx::ui::Element*)component;
							switch(element->element_type)
							{
								case axl::gl::gfx::ui::Element::ET_OTHER:
								case axl::gl::gfx::ui::Element::ET_IMAGE_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_INPUT:
									{
										if(element->hasInputFocus())
										{
											axl::gl::gfx::ui::elements::TextInput* text_input = (axl::gl::gfx::ui::elements::TextInput*)element;
											text_input->onChar(char_code);
										}
									}
									break;
								case axl::gl::gfx::ui::Element::ET_BUTTON:
									break;
							}
						}
						break;
				}
			}
		}
	}
}

void View::onPointer(int index, int x, int y, bool is_down)
{
	if(index >= 0 && index < MAX_POINTERS)
		m_pointers[index] = is_down;
	if(this->m_is_ui_processing_enabled)
	{
		axl::math::Vec2f point((float)x, (float)(this->size.y - y));
		for(axl::util::ds::UniList<axl::gl::Context*>::Iterator ctx_it = m_contexts.first(); ctx_it.isNotNull(); ++ctx_it)
		{
			const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& components = (*ctx_it)->getComponents();
			for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = components.first(); it.isNotNull(); ++it)
			{
				axl::gl::gfx::ui::Component* component = *it;
				if(!component->isVisible()) continue;
				switch(component->component_type)
				{
					case axl::gl::gfx::ui::Component::CONTAINER:
						break;
					case axl::gl::gfx::ui::Component::ELEMENT:
						{
							axl::gl::gfx::ui::Element* element = (axl::gl::gfx::ui::Element*)component;
							switch(element->element_type)
							{
								case axl::gl::gfx::ui::Element::ET_OTHER:
								case axl::gl::gfx::ui::Element::ET_IMAGE_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_VIEW:
									break;
								case axl::gl::gfx::ui::Element::ET_TEXT_INPUT:
									{
										axl::gl::gfx::ui::elements::TextInput* text_input = (axl::gl::gfx::ui::elements::TextInput*)element;
										if(is_down)
										{
											if(isInArea(point, text_input->getSize(), text_input->transform.getMatrix(), text_input->getContainer()))
												text_input->requestInputFocus();
										}
									}
									break;
								case axl::gl::gfx::ui::Element::ET_BUTTON:
									{
										axl::gl::gfx::ui::elements::Button* button = (axl::gl::gfx::ui::elements::Button*)element;
										if(is_down)
										{
											if(isInArea(point, button->getSize(), button->transform.getMatrix(), button->getContainer()))
												button->onPress(index);
										}
										else
										{
											if(button->getState() == axl::gl::gfx::ui::elements::Button::PRESSED)
												button->onRelease(index);
										}
									}
									break;
								case axl::gl::gfx::ui::Element::ET_SLIDER:
									{
										axl::gl::gfx::ui::elements::Slider* slider = (axl::gl::gfx::ui::elements::Slider*)element;
										if((index == PI_LEFT_BUTTON) || (index == PI_TOUCH))
										{
											if(is_down)
											{
												axl::math::Vec3f _position = slider->transform.getPosition();
												axl::math::Vec2f client_size(slider->getSize());
												axl::math::Vec4f padding(slider->getPadding());
												axl::gl::gfx::ui::Container* parent = slider->getContainer();
												axl::math::Mat4f final_transform = slider->transform.getMatrix();
												// calculate the final transform matrix
												while(parent)
												{
													final_transform = parent->transform.getMatrix() * final_transform;
													parent = parent->getContainer();
												}
												axl::math::Vec4f position = final_transform * axl::math::Vec4f(_position, 1.0f);
												axl::math::Vec2f client_point(point.x - position.x - padding.x, point.y - position.y - padding.y);
												if(isInArea(point, client_size, final_transform, 0))
												{
													float min_value = slider->getMinValue(), max_value = slider->getMaxValue();
													float slide_value = slider->getSliderOrientation() == axl::gl::gfx::ui::elements::Slider::OR_HORIZONTAL ?
														(client_point.x / (client_size.x - (padding.x + padding.z))) :
														(client_point.y / (client_size.y - (padding.y + padding.w)));
													if(axl::gl::input::Keyboard::isKeyDown(axl::gl::input::KeyCode::KEY_CONTROL))
														slide_value = (slide_value >= 0.5f) ? max_value : min_value;
													else
														slide_value = axl::math::clamp((slide_value * (max_value - min_value)), min_value, max_value);
													slider->onSlide(slide_value);
													slider->slider_is_sliding = true;
												}
											}
											else
											{
												slider->slider_is_sliding = false;
											}
										}
									}
									break;
							}
						}
						break;
				}
			}
		}
	}
}

void View::onPointerMove(int index, int x, int y)
{
	HCURSOR hcursor = ((ViewData*)this->m_reserved)->hcursor;
	if(GetCursor() != hcursor)
	{
		SetCursor(hcursor);
	}
	if(this->m_is_ui_processing_enabled)
	{
		axl::math::Vec2f point((float)x, (float)(this->size.y - y));
		for(axl::util::ds::UniList<axl::gl::Context*>::Iterator ctx_it = m_contexts.first(); ctx_it.isNotNull(); ++ctx_it)
		{
			const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& components = (*ctx_it)->getComponents();
			for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = components.first(); it.isNotNull(); ++it)
			{
				axl::gl::gfx::ui::Component* component = *it;
				if(!component->isVisible()) continue;
				switch(component->component_type)
				{
					case axl::gl::gfx::ui::Component::CONTAINER:
						break;
					case axl::gl::gfx::ui::Component::ELEMENT:
						{
							axl::gl::gfx::ui::Element* element = (axl::gl::gfx::ui::Element*)component;
							switch(element->element_type)
							{
								case axl::gl::gfx::ui::Element::ET_OTHER:
								case axl::gl::gfx::ui::Element::ET_IMAGE_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_VIEW:
								case axl::gl::gfx::ui::Element::ET_TEXT_INPUT:
									break;
								case axl::gl::gfx::ui::Element::ET_BUTTON:
									{
										axl::gl::gfx::ui::elements::Button* button = (axl::gl::gfx::ui::elements::Button*)element;
										if(isInArea(point, button->getSize(), button->transform.getMatrix(), button->getContainer()))
										{
											if(!button->isHovered())
												button->onHover();
										}
										else if(button->isHovered())
										{
											button->onDrift();
										}
									}
									break;
								case axl::gl::gfx::ui::Element::ET_SLIDER:
									{
										axl::gl::gfx::ui::elements::Slider* slider = (axl::gl::gfx::ui::elements::Slider*)element;
										if(slider->slider_is_sliding)
										{
											if(axl::gl::input::Mouse::isButtonDown(axl::gl::input::MouseButton::MBTN_LEFT) || pointers[PI_TOUCH])
											{
												axl::math::Vec3f _position = slider->transform.getPosition();
												axl::math::Vec2f client_size(slider->getSize());
												axl::math::Vec4f padding(slider->getPadding());
												axl::gl::gfx::ui::Container* parent = slider->getContainer();
												axl::math::Mat4f final_transform = slider->transform.getMatrix();
												// calculate the final transform matrix
												while(parent)
												{
													final_transform = parent->transform.getMatrix() * final_transform;
													parent = parent->getContainer();
												}
												axl::math::Vec4f position = final_transform * axl::math::Vec4f(_position, 1.0f);
												axl::math::Vec2f client_point(point.x - position.x - padding.x, point.y - position.y - padding.y);
												float min_value = slider->getMinValue(), max_value = slider->getMaxValue();
												float slide_value = slider->getSliderOrientation() == axl::gl::gfx::ui::elements::Slider::OR_HORIZONTAL ?
													(client_point.x / (client_size.x - (padding.x + padding.z))) :
													(client_point.y / (client_size.y - (padding.y + padding.w)));
												if(axl::gl::input::Keyboard::isKeyDown(axl::gl::input::KeyCode::KEY_CONTROL))
													slide_value = (slide_value >= 0.5f) ? max_value : min_value;
												else
													slide_value = axl::math::clamp((slide_value * (max_value - min_value)), min_value, max_value);
												slider->onSlide(slide_value);
											}
											else
												slider->slider_is_sliding = false;
										}
									}
									break;
							}
						}
						break;
				}
			}
		}
	}
}

void View::onScroll(bool is_vertical, int delta, int x, int y)
{}

///////////
// 

LRESULT CALLBACK MWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	View* view = (View*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	UINT scancode = (lparam & 0x00ff0000) >> 16;
    int extended  = (lparam & 0x01000000) != 0;
	static int bottom_index = 0;
	static int ids[View::MAX_TOUCHES];
	static bool init_ids = false;
	if(!init_ids)
	{
		for(int i=0; i<View::MAX_TOUCHES; ++i)
			ids[i] = -1;
		init_ids = true;
	}
	switch (message)
	{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
			if(view)
			{
				bool is_down = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
				switch(wparam)
				{
					case VK_CONTROL:
						if(!view->onKey(input::Keyboard::mapPlatformKeyCode((int)wparam), is_down) ||
							!view->onKey(input::Keyboard::mapPlatformKeyCode((int)(extended ? VK_RCONTROL : VK_LCONTROL)), is_down)
							) return DefWindowProcW(hwnd, message, wparam, lparam);
						return 0;
					case VK_SHIFT:
						if(!view->onKey(input::Keyboard::mapPlatformKeyCode((int)wparam), is_down) ||
							!view->onKey(input::Keyboard::mapPlatformKeyCode((int)(MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX))), (message == WM_KEYDOWN))
							) return DefWindowProcW(hwnd, message, wparam, lparam);
						return 0;
					case VK_MENU:
						if(!view->onKey(axl::gl::input::KeyCode::KEY_ALT, is_down) ||
							!view->onKey(input::Keyboard::mapPlatformKeyCode((int)(extended ? VK_RMENU : VK_LMENU)), is_down)
							) return DefWindowProcW(hwnd, message, wparam, lparam);
						return 0;
					case VK_LWIN:
					case VK_RWIN:
						if(!view->onKey(axl::gl::input::KeyCode::KEY_CMD, is_down) ||
							!view->onKey(input::Keyboard::mapPlatformKeyCode((int)wparam), is_down)
							) return DefWindowProcW(hwnd, message, wparam, lparam);
						return 0;
					default:
						if(!view->onKey(input::Keyboard::mapPlatformKeyCode((int)wparam), is_down))
							return DefWindowProcW(hwnd, message, wparam, lparam);
						else
							return 0;
				}
			}
			return 0;
		case WM_CHAR:
			if(view)
			{
				view->onChar((wchar_t)wparam);
			}
			break;
		case WM_UNICHAR:
			if(wparam == UNICODE_NOCHAR) return TRUE;
			else return FALSE;
			break;
		case WM_POINTERUPDATE:
			if(view)
			{
				const int id = GET_POINTERID_WPARAM(wparam);
				int index = -1;
				for(int i=0; i<View::MAX_TOUCHES; ++i)
				{
					if(ids[i] == id)
					{
						index = i;
						break;
					}
				}
				POINT point = {(int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam)};
				ScreenToClient(hwnd, &point);
				view->onPointerMove(View::PI_TOUCH + index, point.x, point.y);
			}
			break;
		case WM_POINTERDOWN:
			if(view)
			{
				const int id = GET_POINTERID_WPARAM(wparam);
				int index = -1;
				if(IS_POINTER_PRIMARY_WPARAM(wparam))
				{
					ids[bottom_index] = id;
					index = 0;
				}
				else
				{
					for(int i=0; i<View::MAX_TOUCHES; ++i)
					{
						if(ids[i] == -1)
						{
							index = i;
							break;
						}
					}
				}
				ids[index] = id;
				POINT point = {(int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam)};
				ScreenToClient(hwnd, &point);
				view->onPointer(View::PI_TOUCH + index, point.x, point.y, true);
			}
			break;
		case WM_POINTERUP:
			if(view)
			{
				int id = GET_POINTERID_WPARAM(wparam);
				int index = -1;
				for(int i=0; i<View::MAX_TOUCHES; ++i)
				{
					if(ids[i] == id)
					{
						index = i;
						break;
					}
				}
				ids[index] = -1;
				POINT point = {(int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam)};
				ScreenToClient(hwnd, &point);
				view->onPointer(View::PI_TOUCH + index, point.x, point.y, false);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			if(view)
			{
				view->onPointer(View::PI_LEFT_BUTTON, (int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam), (message == WM_LBUTTONDOWN));
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if(view)
			{
				view->onPointer(View::PI_RIGHT_BUTTON, (int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam), (message == WM_RBUTTONDOWN));
			}
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			if(view)
			{
				view->onPointer(View::PI_MIDDLE_BUTTON, (int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam), (message == WM_MBUTTONDOWN));
			}
			break;
		case WM_MOUSEMOVE:
			if(view)
			{
				view->onPointerMove(View::PI_LEFT_BUTTON, (int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam));
			}
			break;
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			if(view)
			{
				POINT point = {(int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam)};
				ScreenToClient(hwnd, &point);
				view->onScroll((message == WM_MOUSEWHEEL), (int)GET_WHEEL_DELTA_WPARAM(wparam), point.x, point.y);
			}
			break;
		case WM_MOVE:
			if(view)
			{
				view->onMove((int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam));
			}
			break;
		case WM_SIZE:
			if(view)
			{
				switch(wparam)
				{
					case SIZE_MINIMIZED:
						view->onPause();
						break;
					default:
						{
							if(view->is_paused) view->onResume();
							view->onSize((int)GET_X_LPARAM(lparam), (int)GET_Y_LPARAM(lparam));
						}
						break;
				}
			}
			break;
		case WM_GETMINMAXINFO:
			if(view)
			{
				LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
				RECT rc_min = {0, 0, view->min_size.x, view->min_size.y};
				RECT rc_max = {0, 0, view->max_size.x, view->max_size.y};
				ViewData* view_data = (ViewData*)view->getReserved();
				DWORD style = view_data->style;
				AdjustWindowRect(&rc_min, style, FALSE);
				AdjustWindowRect(&rc_max, style, FALSE);
				lpMMI->ptMinTrackSize.x = (rc_min.right - rc_min.left);
				lpMMI->ptMinTrackSize.y = (rc_min.bottom - rc_min.top);
				lpMMI->ptMaxTrackSize.x = (rc_max.right - rc_max.left);
				lpMMI->ptMaxTrackSize.y = (rc_max.bottom - rc_max.top);
			}
			break;
		case WM_USER+1: // window created and configured
			if(view != (View*)lparam)
			{
				DestroyWindow(hwnd);
			}
			if(view)
			{
				if(!view->onCreate(((ViewData*)view->getReserved())->is_recreating))
				{
					((ViewData*)view->getReserved())->is_recreating = false;
					DestroyWindow(hwnd);
				}
				((ViewData*)view->getReserved())->is_recreating = false;
			}
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			if(view)
			{
				if(!((ViewData*)view->reserved)->destroying)
				{
					((ViewData*)view->reserved)->destroy_from_message = true;
					view->destroy();
					((ViewData*)view->reserved)->destroy_from_message = false;
				}
			}
			break;
		default:
			return DefWindowProcW(hwnd, message, wparam, lparam);
	}
	return 0;
}

Cursor DefaultCursor = Cursor::CUR_ARROW;


} // axl::gl
} // axl