
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/everything.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.util/uc/Clock.hpp>
#include <axl.util/uc/Time.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/angle.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/util.hpp>
#include "Assert.hpp"
#include "GLC.h"

namespace GL = axl::glfl::core::GL;

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::camera::Camera3Df camera;
		axl::gl::gfx::Program program;
		axl::glfl::GLuint vertex_array, vertex_buffer;
		axl::glfl::GLint uloc_projection, uloc_view;
	private:
		Cursor NormalCursor;
		axl::util::uc::Time time, ctime;
		axl::gl::projection::Orthographicf projection;
		axl::gl::input::Key key_Control, key_Shift, key_Alt, key_F2, key_F3;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const Cursor& _cursor = View::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			camera(),
			projection(),
			program(&this->main_context),
			NormalCursor(CUR_CROSS),
			key_Control(axl::gl::input::KeyCode::KEY_CONTROL),
			key_Shift(axl::gl::input::KeyCode::KEY_SHIFT),
			key_Alt(axl::gl::input::KeyCode::KEY_ALT),
			key_F2(axl::gl::input::KeyCode::KEY_F2),
			key_F3(axl::gl::input::KeyCode::KEY_F3)
		{}

		~GameView()
		{}

		void init()
		{
			using namespace axl::math;
			this->setCursor(NormalCursor);
			this->camera.set(Vec3f(0.0f,0.0f,4.0f), Vec3f(0.0f,0.0f,0.0f), 0.0f, Vec3f(1.0f,1.0f,1.0f), Vec2i(0,0), this->size, &this->projection);
			this->camera.updateTransform();
			program.setContext(&this->main_context);
			vertex_array = 0;
			vertex_buffer = 0;
			uloc_projection = -1;
			uloc_view = -1;
			time.set();
			ctime.set();
		}

		void update()
		{
			// update code
			this->time.set();
		}

		void render()
		{
			if(!this->camera.makeCurrent(&this->main_context)) return;
			GL::glEnable(GL::GL_DEPTH_TEST);
			GL::glDepthFunc(GL::GL_LESS);
			GL::glEnable(GL::GL_BLEND);
			GL::glBlendFunc(GL::GL_SRC_ALPHA, GL::GL_ONE_MINUS_SRC_ALPHA);

			this->render(true);
			this->swap();
			
			GL::glDisable(GL::GL_BLEND);
			GL::glDisable(GL::GL_DEPTH_TEST);
		}

		void render(bool p_clear)
		{
			if(!this->camera.makeCurrent(&this->main_context)) return;
			if(p_clear)
			{
				GL::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				GL::glClear(GL::GL_COLOR_BUFFER_BIT|GL::GL_DEPTH_BUFFER_BIT);
			}
			Assert(program.setUniformMat4f(uloc_view, this->camera.view_transform.values));
			// Draw
			if(vertex_array != 0 && vertex_buffer != 0 && program.use())
			{
				GL::glBindVertexArray(vertex_array);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, vertex_buffer);
				GL::glDrawArrays(GL::GL_TRIANGLES, 0, 3);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
				GL::glBindVertexArray(0);
			}
		}

		void onDisplayConfig(const axl::gl::Display& display)
		{
			axl::gl::View::onDisplayConfig(display);
			printf("onDisplayConfig:display.size: %d,%d\n", display.size.x, display.size.y);
			this->setPosition((display.size - this->size) / 2); // center to display
			if(this->size.x > display.size.x || this->size.y > display.size.y)
				this->setSize(display.size);
		}

		bool onCreate(bool recreating)
		{
			axl::gl::Context::Config context_configs[] = {
				axl::gl::Context::Config(1, 3, 3, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(2, 4, 6, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(3, 3, 3, axl::gl::Context::Config::GLP_CORE),
				axl::gl::Context::Config(4, 2, 1, axl::gl::Context::Config::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::Context::Config))) return false;
			this->init();
			// Create stuff here
			{
				axl::gl::gfx::Shader shader;
				Assert(!shader.isValid());
				Assert(!shader.create());
				Assert(!shader.isValid());
				Assert(!shader.compile());
				Assert(!shader.isValid());
				Assert(!shader.isCompiled());
			}
			{
				axl::gl::gfx::Shader shader(&this->main_context);
				Assert(!shader.isValid());
				Assert(!shader.create());
				Assert(!shader.isValid());
				Assert(!shader.compile());
				Assert(!shader.isValid());
				Assert(!shader.isCompiled());
			}
			{
				axl::gl::gfx::Program program;
				Assert(!program.isValid());
				Assert(!program.create());
				Assert(!program.isValid());
				Assert(!program.link());
				Assert(!program.isValid());
				Assert(!program.isLinked());
			}
			{
				axl::gl::gfx::Program program(&this->main_context);
				Assert(!program.isValid());
				Assert(program.create());
				Assert(program.isValid());
				Assert(!program.link());
				Assert(!program.isLinked());
				Assert(program.destroy());
				Assert(!program.isValid());
				Assert(!program.isLinked());
			}
			{
				// VERTEX_SHADER
				axl::gl::gfx::Shader vertex_shader(&this->main_context, GL::GL_VERTEX_SHADER);
				Assert(!vertex_shader.isValid());
				Assert(vertex_shader.create());
				Assert(vertex_shader.isValid());
				Assert(vertex_shader.getSource().length() == 0);
				Assert(!vertex_shader.compile());
				Assert(!vertex_shader.isCompiled());
				Assert(vertex_shader.setSource(
					"# version 330 core\n"
					"void main() {}\n"
				));
				Assert(vertex_shader.getSource().length() != 0);
				Assert(vertex_shader.compile());
				Assert(vertex_shader.isCompiled());

				// FRAGMENT_SHADER
				axl::gl::gfx::Shader fragment_shader(&this->main_context, GL::GL_FRAGMENT_SHADER);
				Assert(!fragment_shader.isValid());
				Assert(fragment_shader.create());
				Assert(fragment_shader.isValid());
				Assert(fragment_shader.getSource().length() == 0);
				Assert(!fragment_shader.compile());
				Assert(!fragment_shader.isCompiled());
				Assert(fragment_shader.setSource(
					"# version 330 core\n"
					"out vec4 out_Color;"
					"void main() {"
					"	out_Color = vec4(1.0,0.0,1.0,1.0);"
					"}\n"
				));
				Assert(fragment_shader.getSource().length() != 0);
				Assert(fragment_shader.compile());
				Assert(fragment_shader.isCompiled());

				// Program
				axl::gl::gfx::Program program(&this->main_context);
				Assert(!program.isValid());
				Assert(program.create());
				Assert(program.isValid());
				Assert(!program.link());
				Assert(!program.isLinked());
				Assert(program.isValid());
				// attach shaders
				Assert(vertex_shader.attach(program));
				Assert(fragment_shader.attach(program));
				Assert(program.link());
				Assert(program.isLinked());
				Assert(vertex_shader.detach(program));
				Assert(fragment_shader.detach(program));
				// destroy shaders
				Assert(vertex_shader.destroy());
				Assert(!vertex_shader.isValid());
				Assert(!vertex_shader.isCompiled());
				Assert(fragment_shader.destroy());
				Assert(!fragment_shader.isValid());
				Assert(!fragment_shader.isCompiled());
				// destroy program
				Assert(program.destroy());
				Assert(!program.isValid());
				Assert(!program.isLinked());
			}
			// VERTEX_SHADER
				axl::gl::gfx::Shader vertex_shader(&this->main_context, GL::GL_VERTEX_SHADER);
				Assert(!vertex_shader.isValid());
				Assert(vertex_shader.create());
				Assert(vertex_shader.isValid());
				Assert(vertex_shader.getSource().length() == 0);
				Assert(!vertex_shader.compile());
				Assert(!vertex_shader.isCompiled());
				Assert(vertex_shader.setSource(
					"# version 330 core\n"
					"layout(location = 0) in vec3 in_Position;\n"
					"uniform mat4 u_MatProjection = mat4(1);"
					"uniform mat4 u_MatView = mat4(1);"
					"void main() {\n"
					"	gl_Position = u_MatProjection * u_MatView * vec4(in_Position.xyz, 1.0);\n"
					"}\n"
				));
				Assert(vertex_shader.getSource().length() != 0);
				Assert(vertex_shader.compile());
				Assert(vertex_shader.isCompiled());

				// FRAGMENT_SHADER
				axl::gl::gfx::Shader fragment_shader(&this->main_context, GL::GL_FRAGMENT_SHADER);
				Assert(!fragment_shader.isValid());
				Assert(fragment_shader.create());
				Assert(fragment_shader.isValid());
				Assert(fragment_shader.getSource().length() == 0);
				Assert(!fragment_shader.compile());
				Assert(!fragment_shader.isCompiled());
				Assert(fragment_shader.setSource(
					"# version 330 core\n"
					"void main() {"
					"	gl_FragColor = vec4(1.0,0.0,1.0,1.0);"
					"}\n"
				));
				Assert(fragment_shader.compile());
				Assert(fragment_shader.isCompiled());
				if(!vertex_shader.isCompiled())
				{
					axl::util::String info_log = vertex_shader.getInfoLog();
					printf("VERTEX_SHADER_INFO_LOG:\n****\n%s****\n", info_log.cstr());
				}
				if(!fragment_shader.isCompiled())
				{
					axl::util::String info_log = fragment_shader.getInfoLog();
					printf("FRAGMENT_SHADER_INFO_LOG:\n****\n%s****\n", info_log.cstr());
				}
				// Program
				Assert(program.create());
				Assert(program.isValid());
				// attach shaders
				if(vertex_shader.isCompiled() && fragment_shader.isCompiled())
				{
					Assert(vertex_shader.attach(program));
					Assert(fragment_shader.attach(program));
					Assert(program.link());
					Assert(program.isLinked());
					if(!program.isLinked())
					{
						axl::util::String info_log = program.getInfoLog();
						printf("PROGRAM_INFO_LOG:\n****\n%s****\n", info_log.cstr());
					}
					uloc_projection = program.getUniformLocation("u_MatProjection");
					uloc_view = program.getUniformLocation("u_MatView");
					Assert(vertex_shader.detach(program));
					Assert(fragment_shader.detach(program));
				}
				else
				{
					program.destroy();
				}
				// destroy shaders
				Assert(vertex_shader.destroy());
				Assert(fragment_shader.destroy());
				// create vertext array and vertex buffer objects
				if(this->main_context.makeCurrent())
				{
					GL::glGenVertexArrays(1, &vertex_array);
					GL::glGenBuffers(1, &vertex_buffer);
					if(vertex_array != 0 && vertex_buffer != 0)
					{
						const axl::glfl::GLfloat vertices[] = {
							-1.0f, -1.0, 0.0,
							 1.0f, -1.0, 0.0,
							 0.0f,  1.0, 0.0,
						};
						GLC(GL::glBindVertexArray(vertex_array));
						GLC(GL::glBindBuffer(GL::GL_ARRAY_BUFFER, vertex_buffer));
						GLC(GL::glBufferData(GL::GL_ARRAY_BUFFER, (axl::glfl::GLsizeiptr)sizeof(vertices), vertices, GL::GL_STATIC_DRAW));
						GLC(GL::glEnableVertexAttribArray(0));
						GLC(GL::glVertexAttribPointer(0, 3, GL::GL_FLOAT, 0, 0, 0));
						GLC(GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0));
						GLC(GL::glBindVertexArray(0));
					}
				}
			return axl::gl::View::onCreate(recreating);
		}

		void onDestroy(bool recreating)
		{
			if(this->main_context.makeCurrent())
			{
				if(vertex_array != 0)
				{
					GLC(GL::glDeleteVertexArrays(1, &vertex_array));
					vertex_array = 0;
				}
				if(vertex_buffer != 0)
				{
					GLC(GL::glDeleteBuffers(1, &vertex_buffer));
					vertex_buffer = 0;
				}
			}
			axl::gl::View::onDestroy(recreating);
			if(!recreating)
				axl::gl::Application::quit(0);
		}

		void onSize(int width, int height)
		{
			axl::gl::View::onSize(width, height);
			if(this->main_context.makeCurrent())
			{
				GL::glViewport(0, 0, width, height);
				this->camera.viewport_size.set(width, height);
				float aspect_ratio = (float)this->camera.viewport_size.x / this->camera.viewport_size.y;
				if(this->camera.projection->type == axl::gl::projection::Projectionf::Type::PT_ORTHOGRAPHIC)
				{
					((axl::gl::projection::Orthographicf*)this->camera.projection)->set(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, 0.01f, 1000.0f);
				}
				this->camera.projection->updateTransform();
				Assert(!program.setUniformMat4d(uloc_projection, axl::math::Mat4d(this->camera.projection->matrix).values));
				Assert(program.setUniformMat4f(uloc_projection, this->camera.projection->matrix.values));
			}
			this->update();
			this->render();
		}

		void onKey(axl::gl::input::KeyCode key, bool down)
		{
			using namespace axl::gl::input;
			bool bk_control = key_Control.isDown(), bk_shift = key_Shift.isDown(), bk_alt = key_Alt.isDown();
			bool no_modifiers = !bk_control && !bk_shift && !bk_alt;
			if(key_F2.isPressed() && no_modifiers)
				this->show(this->visiblity == VS_FULLSCREEN ? SM_SHOW : SM_FULLSCREEN);
			if(key_F3.isPressed() && no_modifiers)
				this->setCursor(this->cursor == CUR_NONE ? this->NormalCursor : CUR_NONE);
			switch (key)
			{
				case KeyCode::KEY_ESCAPE:
					axl::gl::Application::quit(0);
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
	printf(">> axl.gl Shader test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		Assertve(axl::glfl::core::load(), verbose);
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(800, 600);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.Shader", position, size);
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
			while(!Application::IS_QUITTING)
			{
				/// -- handle realtime events
				
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