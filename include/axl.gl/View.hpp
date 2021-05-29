#pragma once
#include "lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.util/WString.hpp>
#include "Display.hpp"
#include "input/KeyCodes.hpp"

namespace axl {
namespace gl {

	class AXLGLCXXAPI Context;
	class AXLGLCXXAPI ViewConfig;

} // axl.gl

namespace util {
namespace ds {

template class AXLGLCXXAPI UniList<axl::gl::Context*>;
#ifndef __GNUC__
template class AXLGLCXXAPI UniList<axl::gl::Context*>::Iterator;
#endif

} // axl.util.ds
} // axl.util

namespace gl {

// The default View configuration to be set at the creation of a new ViewConfig by default.
// You can modify it to your liking before creating configurations. 
AXLGLAPI ViewConfig DefaultViewConfig;
// A Null value representaion of the ViewConfig class.
AXLGLAPI const ViewConfig NullViewConfig;


/**
 * OpenGL view pixel-format configuration info.
 */
class AXLGLCXXAPI ViewConfig
{
	public:
		enum PixelType { PT_RGB, PT_RGBA, PT_RGBA_FLOAT, PT_COLORINDEX };
	public:
		ViewConfig(long id, PixelType pixel_type, char bits_color, char bits_red, char bits_green, char bits_blue, char bits_alpha, char bits_depth, char bits_stencil, char samples, bool double_buffered, bool stereo);
		ViewConfig(const ViewConfig& config = DefaultViewConfig);
		bool operator==(const ViewConfig& config) const;
		bool operator!=(const ViewConfig& config) const;
	public:
		long id;
		PixelType pixel_type;
		char bits_color;
		char bits_red;
		char bits_green;
		char bits_blue;
		char bits_alpha;
		char bits_depth;
		char bits_stencil;
		char samples;
		bool double_buffered; 
		bool stereo;
};

// View cursor type. Implementation varies through platforms.
enum class Cursor { CUR_NONE, CUR_CUSTOM, CUR_ARROW, CUR_HAND, CUR_CROSS, CUR_WAIT, CUR_HELP, CUR_IBEAM, CUR_NO };

AXLGLAPI Cursor DefaultCursor;

/**
 * A window in Desktop platforms and NativeActivity in Android platforms.
 * Is responsible for creating and initializing a View with an OpenGL context.
 */
class AXLGLCXXAPI View
{
	public:
		const static int MAX_TOUCHES = 10;
		const static int MAX_POINTERS = MAX_TOUCHES + 5; // don't change
		// View creation flags
		enum Flags { VF_FIXED, VF_RESIZABLE, VF_POPUP };
		// A mouse or other pointer's buttons.
		enum PointerIndex { PI_LEFT_BUTTON = 0, PI_MIDDLE_BUTTON = 1, PI_RIGHT_BUTTON = 2, PI_TOUCH = 3 };
		// View visiblity states.
		enum VisiblityState { VS_SHOWN, VS_HIDDEN, VS_FULLSCREEN };
		// View show modes.
		enum ShowMode { SM_SHOW, SM_HIDE, SM_FULLSCREEN };
	public:
		View(const axl::util::WString& title, const axl::math::Vec2i& position, const axl::math::Vec2i& size, const Cursor& cursor = DefaultCursor);
		virtual ~View();
		virtual bool isValid() const;
		virtual bool create(Display& display, bool recreate = false, const ViewConfig* configs = (const ViewConfig*)0, int configs_count = 0, Flags flags = VF_FIXED);
		virtual void destroy();
		const void* getReserved() const;
		static void cleanup();
		void setUIEventProcessing(bool enable);
		virtual bool setPosition(const axl::math::Vec2i& position);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual bool setMinSize(const axl::math::Vec2i& min_size);
		virtual bool setMaxSize(const axl::math::Vec2i& max_size);
		virtual bool setTitle(const axl::util::WString& title);
		virtual bool setCursor(const Cursor& cursor);
		virtual bool setCursorFromResource(int res_id);
		virtual bool setIcon(const axl::util::WString& icon_file);
		virtual bool setIconFromResource(int res_id);
		virtual bool isPointerCaptured() const;
		virtual bool capturePointer(bool capture) const;
		virtual bool show(ShowMode show_mode = SM_SHOW);
		virtual bool setCursorPosition(const axl::math::Vec2i& cursor_position);
		virtual bool swap() const;
		const axl::util::ds::UniList<axl::gl::Context*>& getContexts() const;
	protected:
		bool addContext(Context* view);
		bool removeContext(Context* view);
		friend class AXLGLCXXAPI axl::gl::Context;
	public: // Event callback methods
		virtual void onDisplayConfig(const Display& display);
		virtual bool onCreate(bool recreating = false);
		virtual void onDestroy(bool recreating = false);
		virtual void onMove(int x, int y);
		virtual void onSize(int w, int h);
		virtual void onPause();
		virtual void onResume();
		virtual void onKey(input::KeyCode key_code, bool is_down);
		virtual void onChar(wchar_t char_code);
		virtual void onPointer(int index, int x, int y, bool is_down);
		virtual void onPointerMove(int index, int x, int y);
		virtual void onScroll(bool is_vertical, int delta, int x, int y);
	public:
		Display**const display;
		const axl::math::Vec2i& position;
		const axl::math::Vec2i& size;
		const axl::math::Vec2i& min_size;
		const axl::math::Vec2i& max_size;
		const axl::util::WString& title;
		const ViewConfig& config;
		const Cursor& cursor;
		const VisiblityState& visiblity;
		const bool& is_paused;
		const bool& is_ui_processing_enabled;
		const bool*const pointers;
		void *const& reserved;
	private:
		Display* m_display;
		axl::math::Vec2i m_position;
		axl::math::Vec2i m_size;
		axl::math::Vec2i m_min_size;
		axl::math::Vec2i m_max_size;
		axl::util::WString m_title;
		ViewConfig m_config;
		Cursor m_cursor;
		VisiblityState m_visiblity;
		bool m_is_paused;
		bool m_is_ui_processing_enabled;
		bool m_pointers[MAX_POINTERS];
		axl::util::ds::UniList<axl::gl::Context*> m_contexts;
		void *m_reserved;
	private:
		View(const View& view);
		View& operator=(const View& view);
};

} // axl::gl
} // axl