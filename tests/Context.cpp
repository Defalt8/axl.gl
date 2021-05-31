#include "common/stdafx.hpp"

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

const axl::gl::ViewConfig view_configs[] = {
	axl::gl::ViewConfig(1, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 16, true, false),
	axl::gl::ViewConfig(2, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 8, true, false),
	axl::gl::ViewConfig(3, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
	axl::gl::ViewConfig(4, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 2, true, false),
	axl::gl::ViewConfig(5, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 1, true, false),
	axl::gl::ViewConfig(6, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
	axl::gl::ViewConfig(7, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, false, false),
	axl::gl::ViewConfig(8, axl::gl::ViewConfig::PT_RGB, 24,8,8,8,0, 24,8, 0, false, false)
};

const axl::gl::ContextConfig context_configs[] = {
	axl::gl::ContextConfig(1, 4, 6, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(2, 4, 5, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(3, 4, 4, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(4, 4, 3, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(5, 4, 2, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(6, 4, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(7, 4, 0, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(8, 3, 3, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(9, 3, 2, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(10, 3, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(11, 3, 0, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(12, 2, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(13, 2, 0, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(14, 1, 5, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(15, 1, 4, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(16, 1, 3, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(17, 1, 2, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(18, 1, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY),
	axl::gl::ContextConfig(19, 1, 0, axl::gl::ContextConfig::GLP_COMPATIBLITY),
};

axl::gl::Display display;

// TestView

class TestView : public axl::gl::View
{
	public:
		axl::gl::Context context;
	public:
		TestView() :
			axl::gl::View(L"axl.gl View test", axl::math::Vec2i(0,0), axl::math::Vec2i(640,480), axl::gl::Cursor::CUR_HAND)
		{}
		~TestView()
		{
			this->destroy();
		}
		bool create(axl::gl::Display& display, bool recreate)
		{
			return axl::gl::View::create(display, recreate,
				view_configs, sizeof(view_configs)/sizeof(axl::gl::ViewConfig),
				TestView::VF_RESIZABLE);
		}
		bool update()
		{
			return true;
		}
		bool render()
		{
			if(!context.makeCurrent()) return false;
			using namespace GL;
			glClearColor(0.1f, 0.5f, 1.f, 0.4f);
			// glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			return true;
		}
		bool process()
		{
			if(!this->isValid() || this->is_paused) return false;
			this->update();
			if(this->render() && this->config.double_buffered)
				this->swap();
			return true;
		}

		
		bool onCreate(bool recreating)
		{
			if(!context.create(false, (axl::gl::View*)this, context_configs, sizeof(context_configs)/sizeof(axl::gl::ContextConfig)))
				return false;
			return axl::gl::View::onCreate(recreating);
		}

		void onDestroy(bool recreating)
		{
			axl::gl::View::onDestroy(recreating);
			context.destroy();
			if(!recreating)
				axl::gl::Application::quit(0);
		}
		
		void onDisplayConfig(const axl::gl::Display& display)
		{
			axl::gl::View::onDisplayConfig(display);
			this->setSize(display.size / 2); // set half the display size
			this->setPosition((display.size - this->size) / 2); // center to display
		}

		void onSize(int width, int height)
		{
			axl::gl::View::onSize(width, height);
			if(context.isValid() && context.makeCurrent())
			{
				using namespace GL;
				glViewport(0, 0, (GLsizei)width, (GLsizei)height);
				this->process();
			}
		}

};

void onQuit(int quit_code)
{
	display.close();
}

void onExit(int exit_code)
{
	axl::gl::View::cleanup();
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	printf("-- exiting with code %d --\n", exit_code);
}


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	printf(">> axl.gl View test -- axl.gl %s library %u.%u.%u\n", (lib::BUILD == lib::Build::SHARED ? "SHARED" : "STATIC"), lib::VERSION.major, lib::VERSION.minor, lib::VERSION.patch);
	puts("----------------------------------------");
	{
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		TestView view;
		Assertve(view.create(display, true), verbose);
		Assertve(view.isValid(), verbose);
		Assertve(view.show(TestView::SM_SHOW), verbose);

		printf(".. View.Config %d selected.\n", view.config.id);
		printf(".. Context.Config %d selected.\n", view.context.config.id);

		while(!Application::IS_QUITTING)
		{
			Application::pollEvents(display);
			view.process();
		}
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}