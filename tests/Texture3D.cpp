#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common/Legacy.h"
#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>
#include <axl.math/everything.hpp>
#include "common/Assert.hpp"
#include "common/GLC.h"

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::gfx::Texture3D texture;
		float theta;
		double y;
	private:
		axl::util::uc::Time time, ctime;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			texture(&this->main_context),
			theta(0.0f),
			y(0.0)
		{}

		~GameView()
		{}

		void update()
		{
			theta += 45.0f * time.deltaTimef();
			y = axl::math::sin(axl::math::Constants::D_QUARTER_PI * (double)ctime.deltaTime());
			printf("%lf\r", y);
			time.set();
		}

		void render()
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			this->render(true, false, true);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			this->render(false, true, false);
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}

		void render(bool p_clear, bool p_swap, bool p_exclude_alpha = false)
		{
			if(this->main_context.makeCurrent())
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslated(0.0, 0.0, -4.0);
				glRotated(45.0, 1.0, 0.0, 0.0);
				glRotatef(theta, 0.0, 1.0, 0.0);
				// glScaled(0.6, 0.6, 0.6);
				if(p_clear)
				{
					glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				}
				glEnable(axl::glfl::core::GL1::GL_TEXTURE_3D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				this->texture.bind();
				double txr_min = 0.0,  txr_max = 1.0;
				if(p_exclude_alpha)
				{
					glBegin(GL_QUADS);
						glTexCoord3d(txr_min, txr_min, txr_min); glVertex3d(-1.0, -1.0, -1.0);
						glTexCoord3d(txr_max, txr_min, txr_min); glVertex3d( 1.0, -1.0, -1.0);
						glTexCoord3d(txr_max, txr_max, txr_min); glVertex3d( 1.0,  1.0, -1.0);
						glTexCoord3d(txr_min, txr_max, txr_min); glVertex3d(-1.0,  1.0, -1.0);
						
						glTexCoord3d(txr_min, txr_min, txr_min); glVertex3d(-1.0, -1.0, -1.0);
						glTexCoord3d(txr_min, txr_max, txr_min); glVertex3d(-1.0,  1.0, -1.0);
						glTexCoord3d(txr_min, txr_max, txr_max); glVertex3d(-1.0,  1.0,  1.0);
						glTexCoord3d(txr_min, txr_min, txr_max); glVertex3d(-1.0, -1.0,  1.0);

						glTexCoord3d(txr_min, txr_min, txr_min); glVertex3d(-1.0, -1.0, -1.0);
						glTexCoord3d(txr_max, txr_min, txr_min); glVertex3d( 1.0, -1.0, -1.0);
						glTexCoord3d(txr_max, txr_min, txr_max); glVertex3d( 1.0, -1.0,  1.0);
						glTexCoord3d(txr_min, txr_min, txr_max); glVertex3d(-1.0, -1.0,  1.0);
					glEnd();
				}
				else
				{
					double my = axl::math::map(y, -1.0, 1.0, 0.0, 1.0);
					glBegin(GL_QUADS);
						glTexCoord3d(txr_min, my, txr_min); glVertex3d(-1.0, y, -1.0);
						glTexCoord3d(txr_max, my, txr_min); glVertex3d( 1.0, y, -1.0);
						glTexCoord3d(txr_max, my, txr_max); glVertex3d( 1.0, y,  1.0);
						glTexCoord3d(txr_min, my, txr_max); glVertex3d(-1.0, y,  1.0);

						glTexCoord3d(txr_min, txr_min, my); glVertex3d(-1.0, -1.0, y);
						glTexCoord3d(txr_max, txr_min, my); glVertex3d( 1.0, -1.0, y);
						glTexCoord3d(txr_max, txr_max, my); glVertex3d( 1.0,  1.0, y);
						glTexCoord3d(txr_min, txr_max, my); glVertex3d(-1.0,  1.0, y);
						
						glTexCoord3d(my, txr_min, txr_min); glVertex3d(y, -1.0, -1.0);
						glTexCoord3d(my, txr_max, txr_min); glVertex3d(y,  1.0, -1.0);
						glTexCoord3d(my, txr_max, txr_max); glVertex3d(y,  1.0,  1.0);
						glTexCoord3d(my, txr_min, txr_max); glVertex3d(y, -1.0,  1.0);
					glEnd();
				}
				glFinish();
				this->texture.unbind();
				glDisable(axl::glfl::core::GL1::GL_TEXTURE_3D);
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
			Assert(this->texture.create());
			Assert(this->texture.isValid());
			Assert(this->texture.allocate(0, 4, 8, 16, GL_RGBA8));
			axl::glfl::GLint tex_w, tex_h, tex_d;
			Assert(this->texture.getLevelParamiv(0, GL_TEXTURE_WIDTH, &tex_w));
			Assert(this->texture.getLevelParamiv(0, GL_TEXTURE_HEIGHT, &tex_h));
			Assert(this->texture.getLevelParamiv(0, axl::glfl::core::GL::GL_TEXTURE_DEPTH, &tex_d));
			GLubyte *image = new GLubyte[tex_w * tex_h * tex_d * 4];
			if(image)
			{
				int tex_w_h = tex_h*tex_w;
				for(int k=0; k<tex_d; ++k)
				{
					for(int j=0; j<tex_h; ++j)
					{
						for(int i=0; i<tex_w; ++i)
						{
							int index = ((k*(tex_w_h))+(j*tex_w)+i)*4;
							image[index+0] = (GLubyte)(255.f * ((float)(i+1)/tex_w));
							image[index+1] = (GLubyte)(255.f * ((float)(j+1)/tex_h));
							image[index+2] = (GLubyte)(255.f * ((float)(k+1)/tex_d));
							image[index+3] = 55+(GLubyte)(200.0f * ( ((float)(k+1)/tex_d) + ((float)(j+1)/tex_h) + ((float)(i+1)/tex_w) ) / 3.0f) ;;;
						}
					}
				}
				Assert(this->texture.setImage(0, 0, 0, 0, tex_w, tex_h, tex_d, GL_RGBA, GL_UNSIGNED_BYTE, image, 1));
				delete[] image;
				image = 0;
				Assert(this->texture.generateMipmaps());
				Assert(this->texture.setParami(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
				// Assert(this->texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				this->texture.setParami(axl::glfl::core::GL::GL_TEXTURE_LOD_BIAS, 3);
			}
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
			this->update();
			this->render();
		}

		bool onKey(axl::gl::input::KeyCode key, bool down)
		{
			using namespace axl::gl::input;
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
	printf(">> axl.gl Texture3D test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(640, 480);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.Texture3D", position, size);
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