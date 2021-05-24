#include "common/stdafx.hpp"
#include "common/MainView.hpp"

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

void onExit(int exit_code)
{
	axl::gl::View::cleanup();
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	printf("-- exiting with code %d --\n", exit_code);
}

class MainView : public Test::MainView
{
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			Test::MainView(_title, _position, _size, _cursor)
		{}
		~MainView()
		{
			this->destroy();
		}
	public:
		bool initialize()
		{
			return true;
		}
		bool update()
		{
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!context.isValid() || !context.makeCurrent())
				return false;
			glEnable(GL_SCISSOR_TEST);
			glViewport(0, 0, (GLsizei)this->size.x/2, (GLsizei)this->size.y);
			glScissor(0, 0, (GLsizei)this->size.y, (GLsizei)this->size.y);
			glClearColor(.5f, .1f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glViewport((GLint)this->size.x/2, 0, (GLsizei)this->size.x/2, (GLsizei)this->size.y);
			glScissor((GLint)this->size.x/2, 0, (GLsizei)this->size.y, (GLsizei)this->size.y);
			glClearColor(.1f, .5f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glDisable(GL_SCISSOR_TEST);
			return true;
		}
	protected:
		bool onCreate(bool recreating)
		{
			if(!Test::MainView::onCreate(recreating))
				return false;
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			this->update();
			if(this->render())
				this->swap();
		}
};

int main(int argc, char *argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	printf(">> axl.gl Viewports test -- axl.gl %s library %u.%u.%u\n", (axl::gl::lib::BUILD == axl::gl::lib::Build::SHARED ? "SHARED" : "STATIC"), axl::gl::lib::VERSION.major, axl::gl::lib::VERSION.minor, axl::gl::lib::VERSION.patch);
	puts("----------------------------------------");
	{
		axl::gl::Display display;
		MainView main_view(L"axl.gl.Viewports test", axl::math::Vec2i(0,0), axl::math::Vec2i(640,480));
		Assertve(display.isOpen(), verbose);
		Assertve(axl::gl::Application::init(), verbose);
		Assertve(axl::glfl::core::load(), verbose);
		Assertve(main_view.create(display, false, MainView::VF_RESIZABLE), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		while(!axl::gl::Application::IS_QUITTING)
		{
			axl::gl::Application::pollEvents(display);
			main_view.update();
			if(main_view.render())
				main_view.swap();
		}
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}