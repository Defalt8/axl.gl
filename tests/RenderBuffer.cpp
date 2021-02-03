
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
#include <axl.gl/gfx/buffer/RenderBuffer.hpp>
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
		axl::gl::gfx::FrameBuffer frame_buffer, frame_buffer0;
		axl::gl::gfx::RenderBuffer render_buffer_color, render_buffer_depth_stencil;
		axl::gl::gfx::RenderBuffer render_buffer_color0, render_buffer_depth_stencil0;
		axl::glfl::GLuint samples, max_sample;
		bool offscreen;
		float theta;
	private:
		axl::util::uc::Time time, ctime;
		axl::util::uc::Clock clock_fps_update;
		axl::gl::input::Key m_key_M, m_key_O, m_key_V;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const Cursor& _cursor = View::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			frame_buffer(&this->main_context),
			frame_buffer0(&this->main_context),
			render_buffer_color(&this->main_context),
			render_buffer_color0(&this->main_context),
			render_buffer_depth_stencil(&this->main_context),
			render_buffer_depth_stencil0(&this->main_context),
			samples(0),
			offscreen(true),
			theta(0.0f),
			clock_fps_update(200),
			m_key_M(axl::gl::input::KeyCode::KEY_M),
			m_key_O(axl::gl::input::KeyCode::KEY_O),
			m_key_V(axl::gl::input::KeyCode::KEY_V)
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
			if(this->offscreen)
			{
				Assert(this->frame_buffer.bind(axl::gl::gfx::FrameBuffer::FBT_DRAW));
			}
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->render(true, false, false, false);
			
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			if(this->offscreen)
			{
				Assert(this->frame_buffer.unbind());
				Assert(this->frame_buffer.blit(&this->frame_buffer0, 0, 0, this->size.x, this->size.y, 0, 0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT, GL_NEAREST));
				Assert(this->frame_buffer0.blit(0, 0, 0, this->size.x, this->size.y, 0, 0, this->size.x, this->size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST));
			}
			this->swap();

			// update status
			static double sFrames = 0.0;
			static axl::util::uc::Time render_time;
			sFrames += 1.0;
			if(clock_fps_update.checkAndSet(true))
			{
				static axl::util::WString swTitle(64);
				double fps = sFrames / render_time.deltaTime();
				sFrames = 0.0;
				render_time.set();
				bool vsync = this->main_context.getVSync();
				swTitle.format(L"axl.gl.View | %s | %u Samples | VSync:%hhd | %4.0lf FPS", this->offscreen ? L"Offscreen" : L"View", this->samples, vsync, fps);
				this->setTitle(swTitle);
			}
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
					glClearColor(0.7f, 0.7f, 0.73f, 0.0f);
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
				// if(p_swap) this->swap();
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
			this->samples = this->config.samples;
			axl::gl::Context::Config context_configs[] = {
				axl::gl::Context::Config(1, 3, 3, axl::gl::Context::Config::GLP_COMPATIBLITY),
				axl::gl::Context::Config(1, 4, 6, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(2, 3, 3, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(3, 2, 1, axl::gl::Context::Config::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::Context::Config))) return false;
			{
				GLint max_samples;
				GLC(glGetIntegerv(GL::GL_MAX_SAMPLES, &max_samples));
				this->max_sample = max_samples <= 0 ? 0 : max_samples;
			}
			// Create stuff here
			Assert(!this->render_buffer_color.isValid());
			Assert(this->render_buffer_color.create());
			Assert(this->render_buffer_color.isValid());

			Assert(!this->render_buffer_color0.isValid());
			Assert(this->render_buffer_color0.create());
			Assert(this->render_buffer_color0.isValid());

			Assert(!this->render_buffer_depth_stencil.isValid());
			Assert(this->render_buffer_depth_stencil.create());
			Assert(this->render_buffer_depth_stencil.isValid());

			Assert(!this->render_buffer_depth_stencil0.isValid());
			Assert(this->render_buffer_depth_stencil0.create());
			Assert(this->render_buffer_depth_stencil0.isValid());
			
			Assert(!this->frame_buffer.isValid());
			Assert(this->frame_buffer.create());
			Assert(this->frame_buffer.isValid());

			Assert(!this->frame_buffer0.isValid());
			Assert(this->frame_buffer0.create());
			Assert(this->frame_buffer0.isValid());
			
			this->configureFrameBuffer();
			return axl::gl::View::onCreate(recreating);
		}
		
		bool configureFrameBuffer()
		{
			if(!this->isValid() || !this->main_context.isValid() || !this->main_context.makeCurrent()) return false;
			Assert(this->render_buffer_color.allocate(this->samples, GL_RGBA8, this->size.x, this->size.y));
			Assert(this->render_buffer_depth_stencil.allocate(this->samples, GL::GL_DEPTH24_STENCIL8, this->size.x, this->size.y));
			Assert(this->frame_buffer.attachRenderBuffer(GL::GL_COLOR_ATTACHMENT0, &this->render_buffer_color));
			Assert(this->frame_buffer.attachRenderBuffer(GL::GL_DEPTH_STENCIL_ATTACHMENT, &this->render_buffer_depth_stencil));
			if(!this->frame_buffer.isComplete())
			{
				fprintf(stderr, "ERROR: incomplete Framebuffer!\n");
				return false;
			}
			Assert(this->render_buffer_color0.allocate(this->samples, GL_RGBA8, this->size.x, this->size.y));
			Assert(this->render_buffer_depth_stencil0.allocate(this->samples, GL::GL_DEPTH24_STENCIL8, this->size.x, this->size.y));
			Assert(this->frame_buffer0.attachRenderBuffer(GL::GL_COLOR_ATTACHMENT0, &this->render_buffer_color0));
			Assert(this->frame_buffer0.attachRenderBuffer(GL::GL_DEPTH_STENCIL_ATTACHMENT, &this->render_buffer_depth_stencil0));
			if(!this->frame_buffer0.isComplete())
			{
				fprintf(stderr, "ERROR: incomplete Framebuffer[0]!\n");
				return false;
			}
			return true;
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
					left = -d * ar;
					right = d * ar;
					bottom = -d;
					top = d;
					glFrustum(left, right, bottom, top, znear, zfar);
					// glOrtho(-ar, ar, -1.0, 1.0, -100.0, 100.0);
				}
				else
				{
					double ar = (double)height/width;
					double bottom, top, left, right;
					double d = znear * axl::math::tan(axl::math::Angle::degToRad(fov) / 2.0);
					left = -d;
					right = d;
					bottom = -d * ar;
					top = d * ar;
					glFrustum(left, right, bottom, top, znear, zfar);
					// glOrtho(-1.0, 1.0, -ar, ar, -100.0, 100.0);
				}
			}
			this->configureFrameBuffer();
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
			if(this->m_key_M.isPressed())
			{
				this->samples = this->samples == 0 ? 1 : (this->samples >= max_sample ? 0 : this->samples * 2);
				this->configureFrameBuffer();
			}
			if(this->m_key_O.isPressed())
			{
				this->offscreen = !this->offscreen;
			}
			if(this->m_key_V.isPressed())
			{
				this->main_context.setVSync(!this->main_context.getVSync());
			}
		}

};

/*****
 * 
 *****/
const GameView::Config view_configs[] = {
	GameView::Config(1, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
	GameView::Config(2, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 16, true, false),
	GameView::Config(3, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 8, true, false),
	GameView::Config(4, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
	GameView::Config(5, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 2, true, false),
	GameView::Config(6, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 1, true, false),
	GameView::Config(7, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
	GameView::Config(8, GameView::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0, false, false),
	GameView::Config(9, GameView::Config::PT_RGB, 24,8,8,8,0, 24,8, 0, false, false),
};
axl::gl::Display display;

/**************
 *
 **/

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
			Assertv(view.main_context.setVSync(false), verbose);
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