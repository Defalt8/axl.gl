
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/lib.hpp>
#include <axl.gl/Display.hpp>
#include <axl.gl/Application.hpp>
#include <axl.gl/input/Keyboard.hpp>
#include <axl.gl/input/Key.hpp>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>
#include <axl.gl/gfx/textures.hpp>
#include <axl.gl/gfx/buffer/FrameBuffer.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.util/uc/Clock.hpp>
#include <axl.util/uc/Time.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/angle.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/util.hpp>
#include <gl/GL.h>
#include "Assert.hpp"
#include "GLC.h"

namespace GL = axl::glfl::core::GL;

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::gfx::Texture2D texture, texture_color, texture_depth;
		axl::gl::gfx::FrameBuffer frame_buffer;
		float theta;
	private:
		axl::util::uc::Time time, ctime;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const Cursor& _cursor = View::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			texture(&this->main_context),
			texture_color(&this->main_context),
			texture_depth(&this->main_context),
			frame_buffer(&this->main_context),
			theta(0.0f)
		{}

		~GameView()
		{}

		void update()
		{
			this->theta += 45.0f * this->time.deltaTime();
			this->time.set();
		}

		void render()
		{
			Assert(this->frame_buffer.bind());
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->render(true, false, false, false);
			// this->swap();
			
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			Assert(this->frame_buffer.unbind());
			Assert(this->frame_buffer.bind(axl::gl::gfx::FrameBuffer::FBT_READ));
			GL::glBlitFramebuffer(0, 0, this->size.x, this->size.y, 0, 0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			Assert(this->frame_buffer.unbind());

			// glMatrixMode(GL_PROJECTION);
			// glPushMatrix();
			// glLoadIdentity();
			// glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);
			// glMatrixMode(GL_MODELVIEW);
			// glLoadIdentity();
			// glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
			// glClear(GL_COLOR_BUFFER_BIT);
			// this->texture1.bind();
			// glEnable(GL_TEXTURE_2D);
			// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			// glBegin(GL_QUADS);
			// 	// glColor4ub(255,255,255,255);
			// 	glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, -1.0);
			// 	glTexCoord2d(1.0, 0.0); glVertex2d( 1.0, -1.0);
			// 	glTexCoord2d(1.0, 1.0); glVertex2d( 1.0,  1.0);
			// 	glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, 1.0);
			// glEnd();
			// glFinish();
			// glDisable(GL_TEXTURE_2D);
			
			// glMatrixMode(GL_PROJECTION);
			// glPopMatrix();
			// glMatrixMode(GL_MODELVIEW);
			// this->texture1.unbind();
			this->swap();
			// GL::glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, );
		}

		void render(bool p_clear, bool p_swap, bool p_exclude_opaque = false, bool p_exclude_alpha = false)
		{
			if(this->main_context.makeCurrent())
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslated(0.0, 0.0, -4.0);
				glRotated(30.0, 1.0, 0.0, 0.0);
				glRotatef(theta, 0.0, 1.0, 0.0);
				// glScaled(0.6, 0.6, 0.6);
				if(p_clear)
				{
					glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
					glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				}
				// Draw
				if(!p_exclude_opaque)
				{
					glBegin(GL_QUADS);
						glColor4ub(255,127,127,255);
						glVertex3d(-1.0, -1.0, 0.0);
						glVertex3d( 1.0, -1.0, 0.0);
						glVertex3d( 1.0,  1.0, 0.0);
						glVertex3d(-1.0,  1.0, 0.0);
					glEnd();
					glBegin(GL_QUADS);
						glColor4ub(0,255,0,255);
						glVertex3d(-1.0, -1.0, -1.0);
						glVertex3d( 1.0, -1.0, -1.0);
						glVertex3d( 1.0, -1.0,  1.0);
						glVertex3d(-1.0, -1.0,  1.0);
					glEnd();
				}
				if(!p_exclude_alpha)
				{
					glBegin(GL_QUADS);
						glColor4ub(0,0,255,127);
						glVertex3d(0.0, -1.0, -1.0);
						glVertex3d(0.0,  1.0, -1.0);
						glVertex3d(0.0,  1.0,  1.0);
						glVertex3d(0.0, -1.0,  1.0);
					glEnd();
				}
				if(p_swap) this->swap();
			}
		}

		void onDisplayConfig(const axl::gl::Display& display)
		{
			axl::gl::View::onDisplayConfig(display);
			printf("onDisplayConfig:display.size: %d,%d\n", display.size.x, display.size.y);
			this->setPosition((display.size - this->size) / 2); // center to display
		}

		bool onCreate(bool recreating)
		{
			axl::gl::Context::Config context_configs[] = {
				axl::gl::Context::Config(1, 3, 3, axl::gl::Context::Config::GLP_COMPATIBLITY),
				axl::gl::Context::Config(1, 4, 6, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(2, 3, 3, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(3, 2, 1, axl::gl::Context::Config::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::Context::Config))) return false;
			// Create stuff here
			Assert(!this->texture.isValid());
			Assert(this->texture.create());
			Assert(this->texture.isValid());
			Assert(this->texture.allocate(this->size.x, this->size.y, GL_RGBA, 0));
			
			Assert(!this->texture_color.isValid());
			Assert(this->texture_color.create());
			Assert(this->texture_color.isValid());
			Assert(this->texture_color.allocate(this->size.x, this->size.y, GL_RGBA, 0));

			Assert(!this->texture_depth.isValid());
			Assert(this->texture_depth.create());
			Assert(this->texture_depth.isValid());
			Assert(this->texture_depth.allocate(this->size.x, this->size.y, GL::GL_DEPTH_COMPONENT24, 0));

			Assert(!this->frame_buffer.isValid());
			Assert(this->frame_buffer.create());
			Assert(this->frame_buffer.isValid());
			Assert(this->frame_buffer.bind());
			GLC(GL::glFramebufferTexture(GL::GL_DRAW_FRAMEBUFFER, GL::GL_COLOR_ATTACHMENT0, this->texture_color.getId(), 0));
			GLC(GL::glFramebufferTexture(GL::GL_DRAW_FRAMEBUFFER, GL::GL_DEPTH_ATTACHMENT, this->texture_depth.getId(), 0));
			GLC(GL::glFramebufferTexture(GL::GL_READ_FRAMEBUFFER, GL::GL_COLOR_ATTACHMENT0, this->texture_color.getId(), 0));
			GLC(GL::glFramebufferTexture(GL::GL_READ_FRAMEBUFFER, GL::GL_DEPTH_ATTACHMENT, this->texture_depth.getId(), 0));
			Assert(this->frame_buffer.unbind());
			return axl::gl::View::onCreate(recreating);
		}

		void onDestroy(bool recreating)
		{
			axl::gl::View::onDestroy(recreating);
			if(!recreating)
				axl::gl::Application::quit(0);
		}

		void onSize(int width, int height)
		{
			axl::gl::View::onSize(width, height);
			if(this->main_context.makeCurrent())
			{
				glViewport(0, 0, width, height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				double fov = 70.0, znear = 0.01, zfar = 1000.0;
				if(width >= height)
				{
					double ar = (double)width/height;
					double bottom, top, left, right;
					double d = znear * axl::math::tan(axl::math::Angle::degToRad(fov) / 2.0);
					left = -d;
					right = d;
					bottom = -d / ar;
					top = d / ar;
					glFrustum(left, right, bottom, top, znear, zfar);
					// glOrtho(-ar, ar, -1.0, 1.0, -100.0, 100.0);
				}
				else
				{
					double ar = (double)height/width;
					double bottom, top, left, right;
					double d = znear * axl::math::tan(axl::math::Angle::degToRad(fov) / 2.0);
					left = -d / ar;
					right = d / ar;
					bottom = -d;
					top = d;
					glFrustum(left, right, bottom, top, znear, zfar);
					// glOrtho(-1.0, 1.0, -ar, ar, -100.0, 100.0);
				}
			}
			axl::math::Vec2i texture_size = this->texture.getSize();
			if(texture_size != this->size) { Assert(this->texture.allocate(this->size.x, this->size.y, GL_RGBA, 0)); }
			axl::math::Vec2i texture_color_size = this->texture_color.getSize();
			if(texture_color_size != this->size) Assert(this->texture_color.allocate(this->size.x, this->size.y, GL_RGBA, 0));
			axl::math::Vec2i texture_depth_size = this->texture_depth.getSize();
			if(texture_depth_size != this->size) Assert(this->texture_depth.allocate(this->size.x, this->size.y, GL::GL_DEPTH_COMPONENT24, 0));
			this->update();
			this->render();
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
const GameView::Config view_configs[] = {
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
axl::gl::Display display;

void onQuit(int quit_code)
{
	display.close();
}

void onExit(int exit_code)
{
	axl::gl::View::cleanup();
	axl::gl::Display::restoreSettings();
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	printf("-- exiting with code %d --\n", exit_code);
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
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(640, 480);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.View", position, size);
		// Assertv(view.create(display), verbose);
		// Assertv(view.isValid(), verbose);
		Assertv(view.create(display, true, view_configs, sizeof(view_configs)/sizeof(GameView::Config), GameView::VF_RESIZABLE), verbose);
		Assertv(view.isValid(), verbose);
		printf(".. View.Config %d selected.\n", view.config.id);
		if(view.isValid())
		{
			printf(".. Context.Config %d selected. %d.%d\n", view.main_context.config.id, view.main_context.config.major_version, view.main_context.config.minor_version);
			Assertv(view.main_context.makeCurrent(), verbose);
			Assertv(view.main_context.isCurrent(), verbose);
			Assertve(view.show(GameView::SM_SHOW), verbose);
			///
			axl::util::uc::Time time;
			input::Key key_0(axl::gl::input::KeyCode::KEY_0);
			input::Key key_1(axl::gl::input::KeyCode::KEY_1);
			input::Key key_F(axl::gl::input::KeyCode::KEY_F);
			input::Key key_UP(axl::gl::input::KeyCode::KEY_UP);
			input::Key key_DOWN(axl::gl::input::KeyCode::KEY_DOWN);
			while(!Application::IS_QUITTING)
			{
				/// -- handle events	
				if(key_F.isPressed())
				{
					view.show(view.visiblity == axl::gl::View::VS_FULLSCREEN ? axl::gl::View::SM_SHOW : axl::gl::View::SM_FULLSCREEN);
				}
				if(key_0.isPressed())
				{
					Display::Settings display_settings;
					Display::enumSettings(&display_settings, Display::Settings::DI_DEFAULT);
					if(Display::setSettings(display_settings, true))
					{
						Assertv(Display::setSettings(display_settings), verbose);
					}
					else fprintf(stderr, "Failed to set display settings!\n");
					display.reopen(0);
				}		
				if(key_1.isPressed())
				{
					Display::Settings display_settings(-1, 1280, 720);
					if(Display::setSettings(display_settings, true))
					{
						Assertv(Display::setSettings(display_settings), verbose);
					}
					else fprintf(stderr, "Failed to set display settings!\n");
					display.reopen(0);
				}
				/// -- update
				view.update();
				/// -- render
				view.render();
				/// -- check events
				Application::pollEvents(display);
			}
		}
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}