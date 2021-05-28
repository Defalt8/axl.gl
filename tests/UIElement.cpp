#include "common/stdafx.hpp"
#include "common/MainView.hpp"

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

axl::gl::Display g_display;

class TestElement : public axl::gl::gfx::ui::Element
{
	public:
		TestElement(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(60.f,30.f),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f)
		) :
			axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::ET_IMAGE_VIEW, ptr_context, container, position, size, padding)
		{}
		~TestElement()
		{
			this->destroy();
		}
	protected:
		bool iRender(axl::gl::camera::Camera3Df* camera)
		{
			using namespace GL;
			return true;
		}
};

class MainView : public Test::MainView
{
	private:
		axl::util::uc::Clock status_clock, update_clock;
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::FrameBuffer component_framebuffer;
		axl::gl::gfx::ui::Component::Program component_program;
		TestElement test_element;
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
			Test::MainView(_title, _position, _size, _cursor),
			status_clock(300),
			update_clock(15)
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
			Assert(m_default_font.loadFromFile("../../common/fonts/consola.ttf", axl::math::Vec2i(22,22)) ||
				m_default_font.loadFromFile("/windows/fonts/consola.ttf", axl::math::Vec2i(22,22)));
			// component_framebuffer
			component_framebuffer.setContext(&context);
			Assert(component_framebuffer.create());
			// component_program
			component_program.setContext(&context);
			Assert(component_program.create());
			// test element
			test_element.setContext(&context);
			test_element.setComponentFrameBuffer(&component_framebuffer);
			test_element.setComponentProgram(&component_program);
			test_element.transform.setPosition(axl::math::Vec3f(100.f,80.f, 0.f));
			test_element.setSize(axl::math::Vec2f(200.f,160.f));
			Assert(test_element.create());
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			this->update();
			this->render();
			this->swap();
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
				new_title.format(L"UIElement test | FPS: %.2f", FPS);
				this->setTitle(new_title);
			}
			if(update_clock.checkAndSet(true))
			{
				static axl::util::uc::Time ctime;
				float elapsed_time = ctime.deltaTimef();
				test_element.setBackgroundColor(
					axl::math::Vec4f(
						axl::math::map(axl::math::sin(elapsed_time * axl::math::Constants::F_PI), -1.f, 1.f, 0.f, 1.f),
						axl::math::map(axl::math::cos(elapsed_time * axl::math::Constants::F_PI*0.5f), -1.f, 1.f, 1.f, 0.5f),
						axl::math::map(axl::math::sin(elapsed_time * axl::math::Constants::F_PI*2.f), -1.f, 1.f, 1.f, 0.5f),
						1.f
					)
				);
				test_element.transform.setPosition(
					axl::math::Vec3f(
						200.f+axl::math::map(axl::math::cos(elapsed_time * axl::math::Constants::F_PI*.4f), -1.f, 1.f, -150.f, 150.f),
						200.f+axl::math::map(axl::math::sin(elapsed_time * axl::math::Constants::F_PI), -1.f, 1.f, -150.f, 100.f),
						0.f
					)
				);
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			glClearColor(.07f, .4f, .8f, .0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			test_element.render(&main_camera);
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			return true;
		}	
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.uielement test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
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