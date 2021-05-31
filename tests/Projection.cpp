#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common/Legacy.h"
#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>
#include <axl.math/everything.hpp>
#include "common/Assert.hpp"
#include "common/GLC.h"

namespace GL = axl::glfl::core::GL;

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::projection::Projection* projection;
		axl::gl::projection::Orthographic orthographic_projection;
		axl::gl::projection::Perspective perspective_projection;
		double fov;
		float theta;
	private:
		axl::util::uc::Time time, ctime;
		axl::gl::input::Key key_P;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			projection(&perspective_projection),
			orthographic_projection(),
			perspective_projection(),
			fov(70.0),
			theta(0.0f),
			key_P(axl::gl::input::KeyCode::KEY_P)
		{}

		~GameView()
		{}

		void update()
		{
			this->theta += 45.0f * this->time.deltaTimef();
			this->time.set();
		}

		void update_projection()
		{
			if(this->main_context.makeCurrent())
			{
				glMatrixMode(GL_PROJECTION);
				double znear = 0.01, zfar = 1000.0;
				double ar = (double)this->size.x/this->size.y;
				if(this->projection)
				{
					switch(this->projection->type)
					{
						default:
						case axl::gl::projection::Projection::Type::PT_ORTHOGRAPHIC:
							this->orthographic_projection.set(-ar, ar, -1.0, 1.0, znear, zfar);
							this->orthographic_projection.updateTransform();
							break;
						case axl::gl::projection::Projection::Type::PT_PERSPECTIVE:
							this->perspective_projection.set(znear, zfar, ar, fov);
							this->perspective_projection.updateTransform();
							break;
					}
					glLoadMatrixd(projection->matrix.values);
				}
				else
				{
					glLoadIdentity();
					glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
				}
			}
		}

		void render()
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->render(true, false, false, false);
			this->swap();
			
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
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
				glScaled(0.6, 0.6, 0.6);
				if(p_clear)
				{
					glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
					glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				}
				// Draw
				if(!p_exclude_opaque)
				{
					for(int i=-10; i<=10; ++i)
					{
						double axes_min = -1000.0, axes_max = 1000.0;
						double offset = (double)i * 1.0;
						glLineWidth(0.8f);
						glBegin(GL_LINES);
							glColor4ub(255,0,0,127);
							glVertex3d(axes_min, 0.0, offset);
							glVertex3d(axes_max, 0.0, offset);
						glEnd();
						glBegin(GL_LINES);
							glColor4ub(0,0,255,127);
							glVertex3d(offset, 0.0, axes_min);
							glVertex3d(offset, 0.0, axes_max);
						glEnd();
					}
					glBegin(GL_QUADS);
						glColor4ub(255,127,127,255);
						glVertex3d(-1.0, 1.0+-1.0, 0.0);
						glVertex3d( 1.0, 1.0+-1.0, 0.0);
						glVertex3d( 1.0, 1.0+ 1.0, 0.0);
						glVertex3d(-1.0, 1.0+ 1.0, 0.0);
					glEnd();
					glBegin(GL_QUADS);
						glColor4ub(0,255,0,255);
						glVertex3d(-1.0, 1.0+-1.0, -1.0);
						glVertex3d( 1.0, 1.0+-1.0, -1.0);
						glVertex3d( 1.0, 1.0+-1.0,  1.0);
						glVertex3d(-1.0, 1.0+-1.0,  1.0);
					glEnd();
				}
				if(!p_exclude_alpha)
				{
					glBegin(GL_QUADS);
						glColor4ub(0,0,255,127);
						glVertex3d(0.0, 1.0+-1.0, -1.0);
						glVertex3d(0.0, 1.0+ 1.0, -1.0);
						glVertex3d(0.0, 1.0+ 1.0,  1.0);
						glVertex3d(0.0, 1.0+-1.0,  1.0);
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
			axl::gl::ContextConfig context_configs[] = {
				axl::gl::ContextConfig(1, 3, 3, axl::gl::ContextConfig::GLP_COMPATIBLITY),
				axl::gl::ContextConfig(1, 4, 6, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(2, 3, 3, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(3, 2, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::ContextConfig))) return false;
			// Create stuff here
			
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
				this->update_projection();
			}
			this->update();
			this->render();
		}

		bool onKey(axl::gl::input::KeyCode key, bool down)
		{
			using namespace axl::gl::input;
			if(key_P.isPressed())
			{
				if(this->projection == 0)
				{
					this->projection = &this->orthographic_projection;
				}
				else
				{
					switch(this->projection->type)
					{
						case axl::gl::projection::Projection::Type::PT_ORTHOGRAPHIC:
							this->projection = &this->perspective_projection;
							break;
						default:
						case axl::gl::projection::Projection::Type::PT_PERSPECTIVE:
							this->projection = &this->orthographic_projection;
							break;
					}
				}
				this->update_projection();
			}
			switch (key)
			{
			case KeyCode::KEY_ESCAPE:
				axl::gl::Application::quit(0);
				return true;
			}
			return axl::gl::View::onKey(key, down);
		}
};


/*****
 * 
 *****/
const axl::gl::ViewConfig view_configs[] = {
	axl::gl::ViewConfig(1, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 16, true, false),
	axl::gl::ViewConfig(2, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 8, true, false),
	axl::gl::ViewConfig(3, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
	axl::gl::ViewConfig(4, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 2, true, false),
	axl::gl::ViewConfig(5, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 1, true, false),
	axl::gl::ViewConfig(6, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, true, false),
	axl::gl::ViewConfig(7, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 0, false, false),
	axl::gl::ViewConfig(8, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 0,0, 0, false, false),
	axl::gl::ViewConfig(9, axl::gl::ViewConfig::PT_RGB, 24,8,8,8,0, 0,0, 0, false, false),
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
	printf(">> axl.gl Projection test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(640, 480);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.Projection", position, size);
		Assertv(view.create(display, true, view_configs, sizeof(view_configs)/sizeof(axl::gl::ViewConfig), GameView::VF_RESIZABLE), verbose);
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
			input::Key key_Control(axl::gl::input::KeyCode::KEY_CONTROL);
			input::Key key_Shift(axl::gl::input::KeyCode::KEY_SHIFT);
			input::Key key_O(axl::gl::input::KeyCode::KEY_O);
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
				static axl::util::uc::Time time0;
				if(key_O.isDown()) {
					double factor = (key_Control.isDown() ? -1.0 : 1.0) * (key_Shift.isDown() ? 1.0 : 5.0);
					view.fov = axl::math::clamp(view.fov + factor * 10.0 * (double)time0.deltaTime(), 1.0, 179.0);
					view.update_projection();
					printf("FOV: %6.1lf\r", view.fov);
				}
				time0.set();
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