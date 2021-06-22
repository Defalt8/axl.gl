#include <cstdlib>
#include <csignal>
#include "common.hpp"
#include <axl.gl/Application.hpp>
#include <axl.gl/Display.hpp>
#include <axl.gl/View.hpp>
#include <axl.gl/gfx/text/Font.hpp>

void __cdecl _on_exit()
{
	axl::gl::Application::exit(0);
}
void __cdecl _sig_handler(int signal)
{
	axl::gl::Application::exit(signal);
}

namespace axl {
namespace gl {
namespace Application {
	
bool APP_IS_QUITTING = false, ON_QUIT_CALLED = false, ON_EXIT_CALLED = false;
const bool& IS_QUITTING = APP_IS_QUITTING;
void (*onQuit) (int quit_code) = 0;
void (*onExit) (int exit_code) = 0;

bool init()
{
	signal(SIGABRT, _sig_handler);
	signal(SIGFPE, _sig_handler);
	signal(SIGILL, _sig_handler);
	signal(SIGINT, _sig_handler);
	signal(SIGSEGV, _sig_handler);
	axl::gl::gfx::Font::init();
	return 0 == std::atexit(_on_exit);
}
void quit(int quit_code)
{
	APP_IS_QUITTING = true;
	PostQuitMessage(quit_code);
	if(!ON_QUIT_CALLED && onQuit)
	{
		ON_QUIT_CALLED = true;
		onQuit(quit_code);
	}
}
void exit(int exit_code)
{
	if(!ON_QUIT_CALLED) quit(exit_code);
	if(!ON_EXIT_CALLED && onExit)
	{
		ON_EXIT_CALLED = true;
		onExit(exit_code);
	}
	axl::gl::View::cleanup();
	axl::gl::gfx::Font::cleanup();
	std::exit(exit_code);
}

bool pollEvent(const Display& display)
{
	MSG message;
	if (FALSE != PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
		return true;
	}
	return false;
}
void pollEvents(const Display& display)
{
	MSG message;
	while (FALSE != PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}
void loopEvents(const Display& display)
{
	MSG message;
	while (FALSE != GetMessageW(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

} // axl::gl::Application
} // axl::gl
} // axl
