#include "common/stdafx.hpp"
#include "common/MainView.hpp"

axl::gl::Display g_display;

class MainView : public Test::MainView
{
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
			Test::MainView(_title, _position, _size, _cursor)
		{}
		~MainView()
		{
			this->destroy();
		}
	public:
		bool initialize()
		{
			if(!Test::MainView::initialize()) return false;
			return true;
		}
		bool update()
		{
			static axl::util::WString new_title(128);
			if(!Test::MainView::update()) return false;
			new_title.format(L"Base View | FPS: %.2f", FPS);
			this->setTitle(new_title);
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			glClearColor(.1f, .1f, .4f, .0f);
			glClear(GL_COLOR_BUFFER_BIT);
			return true;
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
		MainView main_view(L"Base View", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
		Assertve(main_view.create(g_display, false, MainView::VF_FIXED), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		main_view.initialize();
		while(!axl::gl::Application::IS_QUITTING)	
		{
			axl::gl::Application::pollEvents(g_display);
			main_view.update();
			main_view.render();
			main_view.swap();
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}