
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/lib.hpp>
#include <axl.gl/Display.hpp>
#include <axl.gl/Application.hpp>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/Texture2D.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.util/uc/Clock.hpp>
#include <axl.util/uc/Time.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/util.hpp>
#include <gl/GL.h>
#include "Assert.hpp"
#include "GLC.h"

class GameView : public axl::gl::View
{
public:
	GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const Cursor& _cursor = View::DefaultCursor) :
		axl::gl::View(_title, _position, _size, _cursor)
	{}
	~GameView()
	{}
	void onDestroy(bool recreating)
	{
		axl::gl::View::onDestroy(recreating);
		if(!recreating)
			axl::gl::Application::quit();
	}
	void onKey(axl::gl::input::KeyCode key, bool down)
	{
		using namespace axl::gl::input;
		switch (key)
		{
		case KeyCode::KEY_ESCAPE:
			axl::gl::Application::quit(0);
			break;
		case KeyCode::KEY_C:
			if(this->display && *this->display) (*this->display)->close();
			break;
		default:
			axl::gl::View::onKey(key, down);
		}
	}
};


/*****
 * 
 *****/

axl::gl::Display display;

void onQuit(int quit_code)
{
	display.close();
}

void onExit(int exit_code)
{
	axl::gl::View::cleanup();
	axl::gl::Display::restoreSettings();
}

int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl Display test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);

		axl::math::Vec2i size(640, 480);

		// Display::Settings display_settings(32, size.x, size.y, 60);
		// if(Display::setSettings(display_settings, true))
		// 	Display::setSettings(display_settings);

		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i position = (display.size - size) / 2;
		GameView view(L"axl.gl.View", position, size);
		Assertv(view.create(display), verbose);
		Assertv(view.isValid(), verbose);
		GameView::Config view_configs[] = {
			GameView::Config(1, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 16, true, false),
			GameView::Config(2, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 8, true, false),
			GameView::Config(3, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
			GameView::Config(4, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 2, true, false),
			GameView::Config(5, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 1, true, false),
			GameView::Config(6, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
			GameView::Config(7, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0, false, false),
			GameView::Config(8, GameView::Config::PT_RGBA, 32,8,8,8,8, 0,0, 0, false, false),
			GameView::Config(9, GameView::Config::PT_RGB, 24,8,8,8,0, 0,0, 0, false, false),
		};
		Assertv(view.create(display, true, view_configs, sizeof(view_configs)/sizeof(GameView::Config), GameView::VF_RESIZABLE), verbose);
		Assertv(view.isValid(), verbose);
		printf(".. View.Config %d selected.\n", view.config.id);
		if(view.isValid())
		{
			Context::Config context_configs[] = {
				Context::Config(3, 1, 5, Context::Config::GLP_COMPATIBLITY),	
				Context::Config(2, 2, 1, Context::Config::GLP_COMPATIBLITY),	
				Context::Config(1, 3, 3, Context::Config::GLP_CORE),	
			};
			Context context;
			Assertv(context.create(false, &view, context_configs, sizeof(context_configs)/sizeof(Context::Config)), verbose);
			printf(".. Context.Config %d selected. %d.%d\n", context.config.id, context.config.major_version, context.config.minor_version);
			Assertv(context.makeCurrent(), verbose);
			Assertv(context.isCurrent(), verbose);
			Assertve(view.show(GameView::SM_SHOW), verbose);
			///
			Assertv(context.makeCurrent(), verbose);
			if(context.isCurrent())
			{
				// using namespace axl::glfl;
				// using namespace axl::glfl::core;
				axl::gl::gfx::Texture2D texture;
				Assertv(!texture.create(0, GL_LUMINANCE8, axl::math::Vec2i(16, 16)), verbose);
				Assertv(texture.create(&context, GL_LUMINANCE8, axl::math::Vec2i(5, 5), axl::gl::gfx::TextureParams(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT)), verbose);
				GLubyte image[25] = {
					 0,0,0,0,0,
					0,200,250,200,0,
					0,250,0,250,0,
					0,200,250,200,0,
					 0,0,0,0,0
				};
				Assertv(texture.setImage(0, axl::math::Vec2i(0,0), axl::math::Vec2i(5,5), GL_RED, GL_UNSIGNED_BYTE, image, 1), verbose);
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glEnable(GL_TEXTURE_2D);
				Assertv(texture.bind(), verbose);
				glBegin(GL_QUADS);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					float min = -1.0f, max = 1.0f;
					glTexCoord2f( min, min); glVertex2f(-1.0f, -1.0f);
					glTexCoord2f( max, min); glVertex2f( 1.0f, -1.0f);
					glTexCoord2f( max, max); glVertex2f( 1.0f,  1.0f);
					glTexCoord2f( min, max); glVertex2f(-1.0f,  1.0f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				Assertv(texture.unbind(), verbose);
				Assertv(texture.destroy(), verbose);
				Assertv(view.swap(), verbose);
			}
			///
			axl::util::uc::Time time;
			float R = 0.0f, G = 0.0f, B = 0.5f;
			while(!Application::IS_QUITTING)
			{
				Application::pollEvents(display);
				// update
				R = axl::math::Util::map(axl::math::sin(time.deltaTime() * axl::math::Constants::F_PI), -1.0f, 1.0f, 0.0f, 1.0f);
				G = axl::math::Util::map(axl::math::sin(time.deltaTime() * axl::math::Constants::F_PI * 0.31f), -1.0f, 1.0f, 0.0f, 0.5f);
				B = axl::math::Util::map(axl::math::sin(time.deltaTime() * axl::math::Constants::F_PI * 1.31f), -1.0f, 1.0f, 0.5f, 1.0f);
				// render
				// if(context.makeCurrent())
				// {
				// 	using namespace axl::glfl::core;
				// 	GL::glClearColor(R, G, B, 1.0f);
				// 	GL::glClear(GL_COLOR_BUFFER_BIT);
				// 	Assertv(view.swap(), verbose);
				// }
			}
			// Application::loopEvents(display);
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}