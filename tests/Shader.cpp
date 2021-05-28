#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>
#include <axl.math/everything.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include "common/Assert.hpp"
#include "common/GLC.h"

namespace GL = axl::glfl::core::GL;

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::camera::Camera3Df camera;
		axl::gl::gfx::Program program;
		axl::gl::gfx::Texture2D texture;
		axl::glfl::GLuint vertex_array, vertex_buffer;
		axl::glfl::GLint uloc_projection, uloc_view, uloc_model, uloc_color;
		axl::math::Mat4f model_transform;
		axl::math::Vec4f color;
		
	private:
		axl::gl::Cursor NormalCursor;
		axl::util::uc::Time time, ctime;
		axl::gl::projection::Orthographicf projection;
		axl::gl::input::Key key_Control, key_Shift, key_Alt, key_F2, key_F3, key_Space;
		bool is_animating;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			camera(),
			projection(),
			program(),
			texture(),
			is_animating(false),
			NormalCursor(axl::gl::Cursor::CUR_CROSS),
			key_Control(axl::gl::input::KeyCode::KEY_CONTROL),
			key_Shift(axl::gl::input::KeyCode::KEY_SHIFT),
			key_Alt(axl::gl::input::KeyCode::KEY_ALT),
			key_F2(axl::gl::input::KeyCode::KEY_F2),
			key_F3(axl::gl::input::KeyCode::KEY_F3),
			key_Space(axl::gl::input::KeyCode::KEY_SPACE)
		{}

		~GameView()
		{}

		void init()
		{
			using namespace axl::math;
			this->setCursor(NormalCursor);
			this->camera.set(Vec3f(0.0f,0.0f,4.0f), Vec3f(0.0f,0.0f,0.0f), 0.0f, Vec3f(1.0f,1.0f,1.0f), Vec2i(0,0), this->size, &this->projection);
			this->camera.updateTransform();
			this->is_animating = false;
			program.setContext(&this->main_context);
			texture.setContext(&this->main_context);
			vertex_array = 0;
			vertex_buffer = 0;
			uloc_projection = -1;
			uloc_view = -1;
			uloc_model = -1;
			uloc_color = -1;
			model_transform = Transform4::scale(Vec3f::filled(0.8f));
			color.set(1.0f);
			time.set();
			ctime.set();
		}

		void update()
		{
			using namespace axl::math;
			// update code
			if(this->is_animating)
			{
				this->model_transform = Transform4::scale(Vec3f::filled(0.7f)) * Transform4::rotateZ(ctime.deltaTimef() * Constants::F_QUARTER_PI);
				float color_value = map(axl::math::sin(ctime.deltaTimef() * Constants::F_HALF_PI), -1.0f, 1.0f, 0.0f, 1.0f);
				this->color.set(color_value, 0.2f, 1.0f-color_value, 1.0f);
			}
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
			program.setUniformMat4fv(uloc_projection, this->camera.projection->matrix.values);
			program.setUniformMat4fv(uloc_view, this->camera.view_matrix.values);
			program.setUniformMat4fv(uloc_model, this->model_transform.values);
			program.setUniform4fv(uloc_color, &this->color.x);
			// Draw
			if(vertex_array != 0 && vertex_buffer != 0 && program.use())
			{
				this->texture.bind();
				GL::glBindVertexArray(vertex_array);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, vertex_buffer);
				GL::glDrawArrays(GL::GL_TRIANGLES, 0, 3);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
				GL::glBindVertexArray(0);
				this->texture.unbind();
			}
		}

		bool loadShaders(const axl::util::String& vertex_shader_file, const axl::util::String& fragment_shader_file)
		{
			bool success = false;
			// VERTEX_SHADER
			axl::gl::gfx::Shader vertex_shader(&this->main_context, GL::GL_VERTEX_SHADER);
			if(vertex_shader.create())
			{
				vertex_shader.setSource(axl::util::File::getStringContent(vertex_shader_file));
				vertex_shader.compile();
				if(!vertex_shader.isCompiled())
					fprintf(stderr, "VERTEX_SHADER_INFO_LOG:\n****\n%s****\n", vertex_shader.getInfoLog().cstr());
			}
			// FRAGMENT_SHADER
			axl::gl::gfx::Shader fragment_shader(&this->main_context, GL::GL_FRAGMENT_SHADER);
			if(fragment_shader.create())
			{
				fragment_shader.setSource(axl::util::File::getStringContent(fragment_shader_file));
				fragment_shader.compile();
				if(!fragment_shader.isCompiled())
					fprintf(stderr, "FRAGMENT_SHADER_INFO_LOG:\n****\n%s****\n", fragment_shader.getInfoLog().cstr());
			}
			// Program
			if(!this->program.isCreated())
				program.create();
			if(this->program.isValid())
			{
				// attach shaders
				if(vertex_shader.isCompiled() && vertex_shader.attach(program) &&
				   fragment_shader.isCompiled() && fragment_shader.attach(program)
				)
				{
					if(program.link())
					{
						uloc_projection = program.getUniformLocation("u_MatProjection");
						uloc_view = program.getUniformLocation("u_MatView");
						uloc_model = program.getUniformLocation("u_MatModel");
						uloc_color = program.getUniformLocation("u_VecColor");
						vertex_shader.detach(program);
						fragment_shader.detach(program);
						success = true;
					}
					else
					{
						axl::util::String info_log = program.getInfoLog();
						printf("PROGRAM_INFO_LOG:\n****\n%s****\n", info_log.cstr());
					}
				}
				else
				{
					program.destroy();
				}
			}
			// destroy shaders
			vertex_shader.destroy();
			fragment_shader.destroy();
			return success;
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
			axl::gl::ContextConfig context_configs[] = {
				axl::gl::ContextConfig(1, 3, 3, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(2, 4, 6, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(3, 3, 3, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(4, 2, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::ContextConfig))) return false;
			this->init();
			// Create stuff here
			{ // Shader tests
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
					Assert(!program.use());
					Assert(program.unuse());
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
						"uniform vec4 u_Color = vec4(1.0,0.0,1.0,1.0);\n"
						"void main() {\n"
						"	gl_FragColor = u_Color;\n"
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
					// attach shaders and link program
					Assert(vertex_shader.attach(program));
					Assert(fragment_shader.attach(program));
					Assert(program.link());
					Assert(program.isLinked());
					// detach and destroy shaders
					Assert(vertex_shader.detach(program));
					Assert(fragment_shader.detach(program));
					Assert(vertex_shader.destroy());
					Assert(!vertex_shader.isValid());
					Assert(!vertex_shader.isCompiled());
					Assert(fragment_shader.destroy());
					Assert(!fragment_shader.isValid());
					Assert(!fragment_shader.isCompiled());
					if(program.isLinked())
					{
						axl::glfl::GLint uloc_color;
						axl::math::Vec4f color(0.45f, 0.1f, 0.0f, 1.0f), rcolor;
						Assert(program.getUniformLocation("u_COLOR") == -1);
						uloc_color = program.getUniformLocation("u_Color");
						Assert(uloc_color != -1);
						if(uloc_color != -1)
						{
							Assert(program.setUniform4fv(uloc_color, &color.x));
							Assert(program.getUniformfv(uloc_color, &rcolor.x));
							Assert(rcolor.equals(color, 0.001f));
						}
					}
					// destroy program
					Assert(program.destroy());
					Assert(!program.isValid());
					Assert(!program.isLinked());
				}
			}
			// END-Shader tests
			if(this->loadShaders("tests/shaders/330/shader_test.vert", "tests/shaders/330/shader_test.frag"))
				printf(":) Shaders loading successful!\n");
			else
				fprintf(stderr, ":( Shaders loading failed!\n");
			// create vertext array and vertex buffer objects
			if(this->main_context.makeCurrent())
			{
				GL::glGenVertexArrays(1, &vertex_array);
				GL::glGenBuffers(1, &vertex_buffer);
				if(vertex_array != 0 && vertex_buffer != 0)
				{
					const axl::glfl::GLfloat vertices[] = {
						-1.0f, -1.0, 0.0, 0.0, 0.0,
						 1.0f, -1.0, 0.0, 1.0, 0.0,
						 0.0f,  1.0, 0.0, 0.5, 1.0,
					};
					GLC(GL::glBindVertexArray(vertex_array));
					GLC(GL::glBindBuffer(GL::GL_ARRAY_BUFFER, vertex_buffer));
					GLC(GL::glBufferData(GL::GL_ARRAY_BUFFER, (axl::glfl::GLsizeiptr)sizeof(vertices), vertices, GL::GL_STATIC_DRAW));
					GLC(GL::glEnableVertexAttribArray(0));
					GLC(GL::glEnableVertexAttribArray(1));
					GLC(GL::glVertexAttribPointer(0, 3, GL::GL_FLOAT, 0, (axl::glfl::GLsizei)(sizeof(axl::glfl::GLfloat)*5), 0));
					GLC(GL::glVertexAttribPointer(1, 2, GL::GL_FLOAT, 0, (axl::glfl::GLsizei)(sizeof(axl::glfl::GLfloat)*5), (axl::glfl::GLvoid*)(sizeof(axl::glfl::GLfloat)*3)));
					GLC(GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0));
					GLC(GL::glBindVertexArray(0));
				}
			}
			Assert(this->texture.create());
			Assert(this->texture.setParami(GL::GL_TEXTURE_MIN_FILTER, GL::GL_LINEAR));
			Assert(this->texture.setParami(GL::GL_TEXTURE_MAG_FILTER, GL::GL_LINEAR));
			const axl::glfl::GLsizei txw = 128, txh = 128;
			if(this->texture.allocate(0, txw, txh, GL::GL_RGB8))
			{
				axl::glfl::GLsizei size = txw * txh * 3 * sizeof(axl::glfl::GLubyte);
				axl::glfl::GLubyte *image = new axl::glfl::GLubyte[size];
				if(image)
				{
					for(axl::glfl::GLsizei j = 0; j < txh; ++j)
					{
						for(axl::glfl::GLsizei i = 0; i < txw; ++i)
						{
							axl::glfl::GLsizei index = (j*txw + i) * 3;
							image[index] = (axl::glfl::GLubyte)((float)(i+1)/txw * 255.f * (float)(txh-j)/txh);
							image[index + 1] = (axl::glfl::GLubyte)((float)(txw-i)/txw * 255.f * (float)(txh-j)/txh);
							image[index + 2] = (axl::glfl::GLubyte)((float)j/txh * 255.f);
						}
					}
					Assert(this->texture.setImage(0, 0, 0, txw, txh, GL::GL_RGB, GL::GL_UNSIGNED_BYTE, image, 1));
					delete[] image;
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
				this->setCursor(this->cursor == axl::gl::Cursor::CUR_NONE ? this->NormalCursor : axl::gl::Cursor::CUR_NONE);
			if(key_Space.isPressed() && no_modifiers)
			{
				static axl::util::uc::Time ptime = ctime;
				this->is_animating = !this->is_animating;
				if(this->is_animating)
				{
					this->time.set();
					this->ctime.setFromReference(ptime);
				} else ptime.set();
			}
			if(down) // KEY_DOWN
			{
				switch (key)
				{
					case KeyCode::KEY_ESCAPE:
						if(no_modifiers)
						{
							axl::gl::Application::quit(0);
						}
						break;
					default:
						axl::gl::View::onKey(key, down);
				}
			}
			else // KEY_UP
			{
				switch (key)
				{
					case KeyCode::KEY_L:
						if(bk_control && !bk_shift && !bk_alt)
						{
							if(this->loadShaders("tests/shaders/330/shader_test.vert", "tests/shaders/330/shader_test.frag"))
								printf(":) Shaders loading successful!\n");
							else
								fprintf(stderr, ":( Shaders loading failed!\n");
						}
						break;
					default:
						axl::gl::View::onKey(key, down);
				}
			}
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