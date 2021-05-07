#include "MainView.hpp"

namespace Test {

const axl::gl::ViewConfig view_configs[] = {
	axl::gl::ViewConfig(1, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 16, true, false),
	axl::gl::ViewConfig(2, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 8, true, false),
	axl::gl::ViewConfig(3, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
	axl::gl::ViewConfig(4, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 2, true, false),
	axl::gl::ViewConfig(5, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 1, true, false),
	axl::gl::ViewConfig(6, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
	axl::gl::ViewConfig(7, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, false, false),
	axl::gl::ViewConfig(8, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 0,0, 0, false, false),
	axl::gl::ViewConfig(9, axl::gl::ViewConfig::PT_RGB, 24,8,8,8,0, 0,0, 0, false, false),
};
const axl::gl::ContextConfig context_configs[] = {
	axl::gl::ContextConfig(1, 4, 6, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(2, 4, 5, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(3, 4, 4, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(4, 4, 3, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(5, 4, 2, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(6, 4, 1, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(7, 4, 0, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(8, 3, 3, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(9, 3, 2, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(10, 3, 1, axl::gl::ContextConfig::GLP_CORE),
	axl::gl::ContextConfig(11, 3, 0, axl::gl::ContextConfig::GLP_CORE)
};

//////////////////////////

MainView::MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
	axl::gl::View(_title, _position, _size, _cursor),
	context(),
	projection(),
	main_camera(),
	FPS(0.0f),
	m_rendered_frames(0),
	m_frame_time(),
	m_frame_clock(500),
	m_key_Alt(axl::gl::input::KEY_ALT),
	m_key_Enter(axl::gl::input::KEY_RETURN)
{}
MainView::~MainView()
{
	this->destroy();
}
bool MainView::isValid() const
{
	return axl::gl::View::isValid();
}
bool MainView::create(axl::gl::Display& _display, bool _recreate, Flags _flags)
{
	if(!axl::gl::View::create(_display, _recreate, view_configs, sizeof(view_configs)/sizeof(axl::gl::ViewConfig), _flags))
		return false;
	return true;
}
void MainView::destroy()
{
	this->context.destroy();
	axl::gl::View::destroy();
}

/////////////

bool MainView::initialize()
{
	if(!this->isValid())
		return false;
	this->main_camera.set(axl::math::Vec3f(0.f,0.f,10.f),
		axl::math::Vec3f::filled(0.f),
		0.f,
		axl::math::Vec3f::filled(1.f),
		axl::math::Vec2i(0,0),
		this->size,
		&projection,
		axl::math::Rules::Axis::RHS
		);
	this->projection.set(0, (float)this->main_camera.viewport_size.x, 0, (float)this->main_camera.viewport_size.y, .001f, 1000.f);
	FPS = 0.0f;
	m_rendered_frames = 0;
	return true;
}
bool MainView::update()
{
	if(!this->isValid())
		return false;
	if(m_frame_clock.checkAndSet(false))
	{
		if(m_rendered_frames > 0)
		{
			FPS = (float)m_rendered_frames / m_frame_time.deltaTimef();
			m_frame_time.set();
			m_rendered_frames = 0;
		}
		else
		{
			m_rendered_frames = 0;
			FPS = 0.0f;
		}
	}
	return true;
}
bool MainView::render()
{
	if(!this->isValid() || !this->main_camera.makeCurrent(&context, true))
		return false;
	++m_rendered_frames;
	return true;
}

////////////
//

void MainView::onDisplayConfig(const axl::gl::Display& _display)
{
	axl::gl::View::onDisplayConfig(_display);
	axl::math::Vec2i display_size(_display.settings.width, _display.settings.height);
	axl::math::Vec2i center_position((display_size - this->size) / 2);
	this->setPosition(center_position);
}

bool MainView::onCreate(bool _recreating)
{
	if(!axl::gl::View::onCreate(_recreating)) return false;
	return this->context.create(_recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::ContextConfig));
}

void MainView::onDestroy(bool _recreating)
{
	axl::gl::View::onDestroy(_recreating);
	this->context.destroy();
	axl::gl::Application::quit(0);
}

void MainView::onMove(int x, int y)
{
	axl::gl::View::onMove(x, y);
}

void MainView::onSize(int w,int h)
{
	axl::gl::View::onSize(w, h);
	if(this->context.makeCurrent())
	{
		this->projection.set(0, (float)w, 0, (float)h, .001f, 1000.f);
		this->main_camera.set(axl::math::Vec3f(0.f,0.f,10.f),
			axl::math::Vec3f::filled(0.f),
			0.f,
			axl::math::Vec3f::filled(1.f),
			axl::math::Vec2i(0,0),
			axl::math::Vec2i(w,h),
			&this->projection,
			axl::math::Rules::Axis::RHS
			);
	}
}

void MainView::onPause()
{
	axl::gl::View::onPause();
	m_reference_time = m_frame_time;
}

void MainView::onResume()
{
	axl::gl::View::onResume();
	m_frame_time.setFromReference(m_reference_time);
}

void MainView::onKey(axl::gl::input::KeyCode key_code, bool is_down)
{
	axl::gl::View::onKey(key_code, is_down);
	if(m_key_Enter.isPressed() && m_key_Alt.isDown())
	{
		this->show(this->visiblity == VS_FULLSCREEN ? SM_SHOW : SM_FULLSCREEN);
	}
}

void MainView::onChar(wchar_t char_code)
{
	axl::gl::View::onChar(char_code);
}

void MainView::onPointer(int index, int x, int y, bool is_down)
{
	axl::gl::View::onPointer(index, x, y, is_down);
}

void MainView::onPointerMove(int index, int x, int y)
{
	axl::gl::View::onPointerMove(index, x, y);
}

void MainView::onScroll(bool is_vertical, int delta, int x, int y)
{
	axl::gl::View::onScroll(is_vertical, delta, x, y);
}

}