#include <thread>
#include "common/stdafx.hpp"
#include "common/MainView.hpp"

axl::gl::Display g_display;

class MainView : public Test::MainView
{
	private:
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::ui::TextView::Program m_default_text_program;
		axl::util::uc::Clock status_clock;
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
			Test::MainView(_title, _position, _size, _cursor),
			status_clock(300)
		{}
		~MainView()
		{
			this->destroy();
		}
	public:
		bool onCreate(bool recreating)
		{
			using namespace GL;
			if(!Test::MainView::onCreate()) return false;
			// m_default_font
			m_default_font.setContext(&context);
			Assert(m_default_font.create());
			Assert(m_default_font.loadFromFile("../../common/fonts/OpenSans-Bold.ttf", axl::math::Vec2i(22,22)));
			// m_default_text_program
			m_default_text_program.setContext(&context);
			Assert(m_default_text_program.create());
			return true;
		}
		bool initialize()
		{
			if(!Test::MainView::initialize()) return false;
			return true;
		}
		bool update()
		{
			static axl::util::WString new_title(128);
			if(!Test::MainView::update()) return false;
			if(status_clock.checkAndSet(true))
			{
				new_title.format(L"UI Manager | FPS: %.2f", FPS);
				this->setTitle(new_title);
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render() || !this->main_camera.makeCurrent(&context, true)) return false;
			glClearColor(.87f, .87f, .93f, .0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
		}
		void onKey(axl::gl::input::KeyCode key_code, bool down)
		{
			if(down)
			{
			}
			else
			{
			}
		}
	
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.base test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		// Create View
		Assertve(g_display.isOpen(), verbose);
		axl::math::Vec2i view_position(40, 60), view_size(640, 480);
		MainView main_view(L"UI Manager", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
		Assertve(main_view.create(g_display, false, MainView::VF_RESIZABLE), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		main_view.context.setVSync(false);
		main_view.initialize();
		while(!axl::gl::Application::IS_QUITTING)
		{
			axl::gl::Application::pollEvent(g_display);
			if(main_view.isValid() && !main_view.is_paused)
			{
				main_view.update();
				main_view.render();
				main_view.swap();
			}
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}