#include <cstdio>
#include "common/stdafx.hpp"
#include "common/MainView.hpp"

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

axl::gl::Display g_display;

//
// MainView
//

class MainView : public Test::MainView
{
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
	private:
		axl::util::uc::Clock status_clock, update_clock;
		axl::util::uc::Time dtime, ctime;
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::Text::Program text_program;
		axl::gl::gfx::FrameBuffer component_framebuffer;
		axl::gl::gfx::ui::Component::Program component_program;
		axl::gl::gfx::ui::elements::Slider::Program slider_program;
		axl::gl::gfx::ui::Container container;
		axl::gl::gfx::ui::layouts::Linear linear_layout;
		axl::gl::gfx::ui::elements::Slider sliders[12];
	public:
		bool onCreate(bool recreating)
		{
			using namespace GL;
			if(!Test::MainView::onCreate()) return false;
			// m_default_font
			m_default_font.setContext(&context);
			Assert(m_default_font.create());
			// Assert(m_default_font.loadFromFile("../../common/fonts/consola.ttf", axl::math::Vec2i(16,18)) ||
			// 	m_default_font.loadFromFile("/windows/fonts/consola.ttf", axl::math::Vec2i(16,18)));
			// text_program
			text_program.setContext(&context);
			Assert(text_program.create());
			// component_program
			component_program.setContext(&context);
			Assert(component_program.create());
			// slider_program
			slider_program.setContext(&context);
			Assert(slider_program.create());
			// component_framebuffer
			component_framebuffer.setContext(&context);
			Assert(component_framebuffer.create());
			// linear_layout
			linear_layout.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_VERTICAL);
			linear_layout.setSpacing(axl::math::Vec2f(0.f,5.f));
			// container
			container.setContext(&context);
			container.setComponentProgram(&component_program);
			container.setComponentFrameBuffer(&component_framebuffer);
			container.setBackgroundColor(axl::math::Vec4f(.8f,.8f,.78f,1.f));
			container.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container.setLayout(&linear_layout);
			Assert(container.create());
			// sliders
			{
				int slider_count = sizeof(sliders)/sizeof(axl::gl::gfx::ui::elements::Slider);
				for(int i=0; i<slider_count; ++i)
				{
					sliders[i].setContext(&context);
					sliders[i].setContainer(&container);
					sliders[i].setComponentProgram(&component_program);
					sliders[i].setComponentFrameBuffer(&component_framebuffer);
					sliders[i].setSliderProgram(&slider_program);
					sliders[i].setBackgroundColor(axl::math::Vec4f(.1f,.3f,.6f,1.f));
					sliders[i].setForegroundColor(axl::math::Vec4f(1.f,1.f,1.f,1.f));
					sliders[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					sliders[i].setMinValue(0.0f);
					sliders[i].setMaxValue((float)50.f*(i+1));
					sliders[i].setValue((float)50.f);
					Assert(sliders[i].create());
				}
			}
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			container.setSize(axl::math::Vec2f((float)w, (float)h));
			container.organize();
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
				new_title.format(L"UISlider test | FPS: %.2f", FPS);
				this->setTitle(new_title);
				int slider_count = sizeof(sliders)/sizeof(axl::gl::gfx::ui::elements::Slider);
				printf("Slider values: ");
				for(int i=0; i<slider_count; ++i)
					printf("%.2f ", sliders[i].getValue());
				printf("                 \r");
			}
			if(update_clock.checkAndSet(true))
			{
				float elapsed_time = ctime.deltaTimef();
				if(sliders[0].isValid())
				{
					sliders[0].setValue(axl::math::map(axl::math::sin(elapsed_time * axl::math::Constants::F_PI), -1.0f, 1.0f, sliders[0].getMinValue(), sliders[0].getMaxValue()));
				}
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			if(main_camera.makeCurrent(&context, true))
			{
				glClearColor(.07f, .07f, .13f, .0f);
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				container.render(&main_camera);
				glDisable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
			}
			return true;
		}
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.uislider test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		// Create View
		Assertve(g_display.isOpen(), verbose);
		axl::math::Vec2i view_position(40, 60), view_size(640, 480);
		MainView main_view(L"UISlider test", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
		Assertve(main_view.create(g_display, false, MainView::VF_RESIZABLE), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		main_view.context.setVSync(false);
		main_view.initialize();
		main_view.setUIEventProcessing(true);
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