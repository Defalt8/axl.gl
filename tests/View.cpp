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

axl::gl::Display display;

// TestView

class TestView : public axl::gl::View
{
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

		bool onCreate(bool recreating)
		{
			return axl::gl::View::onCreate(recreating);
		}
		void onDestroy(bool recreating)
		{
			axl::gl::View::onDestroy(recreating);
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
		}
		bool onKey(axl::gl::input::KeyCode key, bool down)
		{
			if(key == axl::gl::input::KeyCode::KEY_CMD)
			{
				printf("CMD: %hhd\n", down);
				return true;
			}
			return axl::gl::View::onKey(key, down);
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
		printf(".. View.Config %d selected.\n", view.config.id);
		
		Assertve(view.show(TestView::SM_SHOW), verbose);
		Application::loopEvents(display);
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}