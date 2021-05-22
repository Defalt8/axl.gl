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
			status_clock(300)
		{}
		~MainView()
		{
			this->destroy();
		}
	private:
		axl::util::uc::Clock status_clock;
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::Text::Program text_program;
		axl::gl::gfx::ui::Component::Program component_program;
		axl::gl::gfx::ui::layouts::Linear linear_layout, linear_layout1, linear_layout2;
		axl::gl::gfx::ui::Container container, container1, container2;
		axl::gl::gfx::ui::elements::TextView text_views[5], text_views2[5];
	public:
		bool onCreate(bool recreating)
		{
			using namespace GL;
			if(!Test::MainView::onCreate()) return false;
			// m_default_font
			m_default_font.setContext(&context);
			Assert(m_default_font.create());
			Assert(m_default_font.loadFromFile("../../common/fonts/Montserrat-Bold.ttf", axl::math::Vec2i(36,36)) ||
				m_default_font.loadFromFile("/windows/fonts/consola.ttf", axl::math::Vec2i(22,22)));
			// text_program
			text_program.setContext(&context);
			Assert(text_program.create());
			// component_program
			component_program.setContext(&context);
			Assert(component_program.create());
			// linear_layout
			linear_layout.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL);
			linear_layout.setSpacing(axl::math::Vec2f(10.f,0.f));
			// linear_layout1
			linear_layout1.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_VERTICAL);
			linear_layout1.setSpacing(axl::math::Vec2f(0.f,10.f));
			// linear_layout2
			linear_layout2.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_VERTICAL);
			linear_layout2.setSpacing(axl::math::Vec2f(0.f,10.f));
			// container
			container.setContext(&context);
			container.setComponentProgram(&component_program);
			container.setBackgroundColor(axl::math::Vec4f(.8f,.8f,.78f,1.f));
			container.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container.setSize(axl::math::Vec2i(300, 260));
			container.setLayout(&linear_layout);
			Assert(container.create());
			// container1
			container1.setContext(&context);
			container1.setContainer(&container);
			container1.setComponentProgram(&component_program);
			container1.setBackgroundColor(axl::math::Vec4f(.5f,.5f,.5f,0.04f));
			container1.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container1.setSize(axl::math::Vec2i(300, 260));
			container1.setLayout(&linear_layout1);
			Assert(container1.create());
			// container2
			container2.setContext(&context);
			container2.setContainer(&container);
			container2.setComponentProgram(&component_program);
			container2.setBackgroundColor(axl::math::Vec4f(.5f,.5f,.5f,.04f));
			container2.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container2.setSize(axl::math::Vec2i(300, 260));
			container2.setLayout(&linear_layout2);
			Assert(container2.create());
			{ // test text_views
				axl::util::size_t element_count = sizeof(text_views)/sizeof(axl::gl::gfx::ui::elements::TextView);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					text_views[i].setContext(&context);
					text_views[i].setComponentProgram(&component_program);
					text_views[i].setContainer(&container1);
					text_views[i].setFont(&m_default_font);
					text_views[i].setTextProgram(&text_program);
					text_views[i].setBackgroundColor(axl::math::Vec4f(.9f,.1f,.4f,((float)i / (element_count-1))));
					text_views[i].setForegroundColor(axl::math::Vec4f(.1f,.1f,.1f,1.f));
					// text_views[i].setForegroundColor(axl::math::Vec4f(.99f,.99f,.99f,1.f));
					text_views[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					text_views[i].setLayoutHeight(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					text_views[i].transform.setRotationOrder(axl::math::Orders::Rotation::NONE, false);
					text_views[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, ((float)i / element_count)));
					text_views[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					Assert(text_views[i].create());
					axl::util::WString label(32);
					label.format(L"Text %d", (i+1));
					text_views[i].setText(label);
				}
			}
			{ // test text_views
				axl::util::size_t element_count = sizeof(text_views2)/sizeof(axl::gl::gfx::ui::elements::TextView);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					text_views2[i].setContext(&context);
					text_views2[i].setComponentProgram(&component_program);
					text_views2[i].setContainer(&container2);
					text_views2[i].setFont(&m_default_font);
					text_views2[i].setTextProgram(&text_program);
					text_views2[i].setBackgroundColor(axl::math::Vec4f(.9f,.1f,.4f,((float)i / (element_count-1))));
					text_views2[i].setForegroundColor(axl::math::Vec4f(.99f,.99f,.99f,1.f));
					text_views2[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					text_views2[i].setLayoutHeight(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					text_views2[i].transform.setRotationOrder(axl::math::Orders::Rotation::NONE, false);
					text_views2[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, ((float)i / element_count)));
					text_views2[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					Assert(text_views2[i].create());
					axl::util::WString label(32);
					label.format(L"Text %d", (i+1));
					text_views2[i].setText(label);
				}
			}
			container.organize();
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
				new_title.format(L"UITextView test | FPS: %.2f", FPS);
				this->setTitle(new_title);
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
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			container.setSize(this->size);
			container.organize();
		}
		void onKey(axl::gl::input::KeyCode key_code, bool down)
		{}
	
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.uitextview test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		// Create View
		Assertve(g_display.isOpen(), verbose);
		axl::math::Vec2i view_position(40, 60), view_size(640, 480);
		MainView main_view(L"UITextView test", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
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