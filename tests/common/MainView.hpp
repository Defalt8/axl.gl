#pragma once

#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>

namespace Test {

class MainView : public axl::gl::View
{
	public:
		MainView(const axl::util::WString& title, const axl::math::Vec2i& position, const axl::math::Vec2i& size, const axl::gl::Cursor& cursor = axl::gl::DefaultCursor);
		virtual ~MainView();
		virtual bool isValid() const;
		virtual bool create(axl::gl::Display& display, bool recreate = false, Flags flags = VF_FIXED);
		virtual void destroy();
	public:
		virtual bool initialize();
		virtual bool update();
		virtual bool render();
	protected:
		virtual void onRender();
		virtual void onDisplayConfig(const axl::gl::Display& display);
		virtual bool onCreate(bool recreating = false);
		virtual void onDestroy(bool recreating = false);
		virtual void onMove(int x, int y);
		virtual void onSize(int w, int h);
		virtual void onPause();
		virtual void onResume();
		virtual bool onKey(axl::gl::input::KeyCode key_code, bool is_down);
		virtual void onChar(wchar_t char_code);
		virtual void onPointer(int index, int x, int y, bool is_down);
		virtual void onPointerMove(int index, int x, int y);
		virtual void onScroll(bool is_vertical, int delta, int x, int y);
	public:
		axl::gl::Context context;
		axl::gl::projection::Orthographicf projection;
		axl::gl::camera::Camera3Df main_camera;
		float FPS;
	private:
		int m_rendered_frames;
		axl::util::uc::Time m_reference_time, m_frame_time;
		axl::util::uc::Clock m_frame_clock;
};

};
