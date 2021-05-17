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
// MainContainer
//

class MainContainer : public axl::gl::gfx::ui::Container
{
	public:
		MainContainer(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			axl::gl::gfx::ui::Layout* layout = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f)
		) :
			axl::gl::gfx::ui::Container(ptr_context, container, layout, position, size, padding)
		{}
		void onViewSize(int w, int h)
		{
			this->setSize(axl::math::Vec2i(w, h));
			this->organize();
		}
};

//
// TestElement
//

class TestElement : public axl::gl::gfx::ui::Element
{
	public:
		TestElement(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(60,30),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT
		) :
			axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::IMAGE_VIEW, ptr_context, container, position, size, padding, layout_width, layout_height)
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

//
// TestButton
//

class TestButton : public axl::gl::gfx::ui::elements::Button
{
	public:
		TestButton(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(60,30),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT
		) :
			axl::gl::gfx::ui::elements::Button(ptr_context, container, position, size, padding, layout_width, layout_height)
		{}
		~TestButton()
		{
			this->destroy();
		}
	protected:
		void onHover()
		{
			axl::gl::gfx::ui::elements::Button::onHover();
			wprintf(L"%s::onHover()\n", this->button_text.getText().cwstr());
		}
		void onDrift()
		{
			axl::gl::gfx::ui::elements::Button::onDrift();
			wprintf(L"%s::onDrift()\n", this->button_text.getText().cwstr());
		}
		void onPress(int pointer_index)
		{
			if(pointer_index != axl::gl::View::PI_LEFT_BUTTON)
				return;
			axl::gl::gfx::ui::elements::Button::onPress(pointer_index);
			wprintf(L"%s::onPressed()\n", this->button_text.getText().cwstr());
		}
		void onRelease(int pointer_index)
		{
			if(pointer_index != axl::gl::View::PI_LEFT_BUTTON)
				return;
			axl::gl::gfx::ui::elements::Button::onRelease(pointer_index);
			wprintf(L"%s::onReleased()\n", this->button_text.getText().cwstr());
		}
};

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
		axl::gl::gfx::Program text_program;
		MainContainer container;
		axl::gl::gfx::ui::Container container1, container2;
		axl::gl::gfx::ui::layouts::Linear linear_layout, linear_layout1, linear_layout2;
		TestButton buttons[5];
		TestElement elements2[3];
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
			// text_program
			text_program.setContext(&context);
			Assert(text_program.create());
			Assert(GL::loadVFShaders(text_program, "tests/shaders/330/text_vuPVMs.vert", "tests/shaders/330/text_vuPVMs.frag"));
			// linear_layout
			linear_layout.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL);
			linear_layout.setSpacing(axl::math::Vec2f(10.f,0.f));
			// linear_layout1
			linear_layout1.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_VERTICAL);
			linear_layout1.setSpacing(axl::math::Vec2f(0.f,5.f));
			// linear_layout2
			linear_layout2.setOrientation(axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL);
			linear_layout2.setSpacing(axl::math::Vec2f(5.f,0.f));
			// container
			container.setContext(&context);
			container.setBackgroundColor(axl::math::Vec4f(.8f,.8f,.78f,1.f));
			container.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container.setSize(axl::math::Vec2i(300, 260));
			container.setLayout(&linear_layout);
			Assert(container.create());
			// container1
			container1.setContext(&context);
			container1.setContainer(&container);
			container1.setBackgroundColor(axl::math::Vec4f(0.6f,.8f,.1f,1.f));
			container1.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container1.setLayout(&linear_layout1);
			Assert(container1.create());
			// container2
			container2.setContext(&context);
			container2.setContainer(&container);
			container2.setBackgroundColor(axl::math::Vec4f(.9f,0.9f,.9f,1.f));
			container2.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container2.setLayout(&linear_layout2);
			Assert(container2.create());
			{ // test buttons
				axl::util::size_t element_count = sizeof(buttons)/sizeof(axl::gl::gfx::ui::elements::Button);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					buttons[i].setContext(&context);
					buttons[i].setContainer(&container1);
					buttons[i].setFont(&m_default_font);
					buttons[i].setProgram(&text_program);
					buttons[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, -((float)i / element_count)));
					buttons[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					buttons[i].setLayoutHeight(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					buttons[i].setSize(axl::math::Vec2i(100,40));
					buttons[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					Assert(buttons[i].create());
					axl::util::WString label(64);
					label.format(L"Button %d", (i+1));
					buttons[i].setLabel(label);
				}
			}
			{ // test elements2
				axl::util::size_t element_count = sizeof(elements2)/sizeof(TestElement);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					elements2[i].setContext(&context);
					elements2[i].setContainer(&container2);
					elements2[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, -((float)i / element_count)));
					elements2[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					elements2[i].setLayoutHeight(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					elements2[i].setSize(axl::math::Vec2i(100,40));
					elements2[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					elements2[i].setBackgroundColor(axl::math::Vec4f(.1f, .1f, 0.5f+0.5f*((float)i / element_count), 1.f));
					Assert(elements2[i].create());
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
				new_title.format(L"UIContainer test | FPS: %.2f", FPS);
				this->setTitle(new_title);
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			glClearColor(.07f, .07f, .13f, .0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			container.render(&main_camera);
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
		}
		void onKey(axl::gl::input::KeyCode key_code, bool down)
		{}
	
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.uicontainer test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		// Create View
		Assertve(g_display.isOpen(), verbose);
		axl::math::Vec2i view_position(40, 60), view_size(640, 480);
		MainView main_view(L"UIContainer test", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
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