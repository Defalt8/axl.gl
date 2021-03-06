#include "common/stdafx.hpp"
#include "common/MainView.hpp"

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

axl::gl::Display g_display;

//
// TestElement
//

class TestElement : public axl::gl::gfx::ui::Element
{
	public:
		TestElement(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(60.f,30.f),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f)
		) :
			axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::ET_OTHER, ptr_context, container, position, size, padding)
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
		axl::gl::gfx::FrameBuffer component_framebuffer;
		axl::gl::gfx::ui::Component::Program component_program;
		axl::gl::gfx::ui::Container container, container1, container2;
		axl::gl::gfx::ui::layouts::Linear linear_layout, linear_layout1, linear_layout2;
		TestElement elements1[5], elements2[3];
	public:
		bool onCreate(bool recreating)
		{
			using namespace GL;
			if(!Test::MainView::onCreate()) return false;
			// component_framebuffer
			component_framebuffer.setContext(&context);
			Assert(component_framebuffer.create());
			// component_program
			component_program.setContext(&context);
			Assert(component_program.create());
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
			container.setComponentFrameBuffer(&component_framebuffer);
			container.setComponentProgram(&component_program);
			container.setBackgroundColor(axl::math::Vec4f(.8f,.8f,.78f,1.f));
			container.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container.setLayout(&linear_layout);
			Assert(container.create());
			// container1
			container1.setContext(&context);
			container1.setComponentFrameBuffer(&component_framebuffer);
			container1.setComponentProgram(&component_program);
			container1.setContainer(&container);
			container1.setBackgroundColor(axl::math::Vec4f(0.9f,.9f,.85f,1.f));
			container1.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container1.setLayout(&linear_layout1);
			Assert(container1.create());
			// container2
			container2.setContext(&context);
			container2.setComponentFrameBuffer(&component_framebuffer);
			container2.setComponentProgram(&component_program);
			container2.setContainer(&container);
			container2.setBackgroundColor(axl::math::Vec4f(.85f,0.9f,.9f,1.f));
			container2.setPadding(axl::math::Vec4f(10.f,10.f,10.f,10.f));
			container2.setLayout(&linear_layout2);
			Assert(container2.create());
			{ // test elements1
				axl::util::size_t element_count = sizeof(elements1)/sizeof(TestElement);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					elements1[i].setContext(&context);
					elements1[i].setComponentFrameBuffer(&component_framebuffer);
					elements1[i].setComponentProgram(&component_program);
					elements1[i].setContainer(&container1);
					elements1[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, -((float)i / element_count)));
					elements1[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					elements1[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					elements1[i].setBackgroundColor(axl::math::Vec4f(.1f, 0.5f+0.5f*((float)i / element_count), .1f, 1.f));
					Assert(elements1[i].create());
				}
			}
			{ // test elements2
				axl::util::size_t element_count = sizeof(elements2)/sizeof(TestElement);
				for(axl::util::size_t i=0; i<element_count; ++i)
				{
					elements2[i].setContext(&context);
					elements2[i].setComponentFrameBuffer(&component_framebuffer);
					elements2[i].setComponentProgram(&component_program);
					elements2[i].setContainer(&container2);
					elements2[i].transform.setPosition(axl::math::Vec3f(0.f,0.f, -((float)i / element_count)));
					elements2[i].setLayoutWidth(axl::gl::gfx::ui::Layout::MATCH_PARENT);
					elements2[i].setPadding(axl::math::Vec4f(5.f,5.f,5.f,5.f));
					elements2[i].setBackgroundColor(axl::math::Vec4f(.1f, .1f, 0.5f+0.5f*((float)i / element_count), 1.f));
					Assert(elements2[i].create());
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
				new_title.format(L"UIContainer test | FPS: %.2f", FPS);
				this->setTitle(new_title);
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			if(main_camera.makeCurrent(&context, false))
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