
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>
#include <axl.math/everything.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include "Assert.hpp"
#include "GLC.h"

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::camera::Camera3Df camera;
		axl::gl::gfx::Program program;
		axl::gl::gfx::Font font;
		axl::glfl::GLuint vertex_array, vertex_buffer;
		axl::glfl::GLint uloc_projection, uloc_view, uloc_model, uloc_TextColor;
		axl::math::Mat4f model_transform;
		axl::math::Vec4f font_color;
	private:
		Cursor NormalCursor;
		axl::util::uc::Time time, ctime;
		axl::gl::projection::Orthographicf projection;
		axl::gl::input::Key key_Control, key_Shift, key_Alt, key_F2, key_F3, key_F4, key_F5, key_Space;
		bool is_animating;
		int size_delta;
		unsigned int current_glyph_index; 
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const Cursor& _cursor = View::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			camera(),
			projection(),
			program(),
			font(),
			NormalCursor(CUR_CROSS),
			key_Control(axl::gl::input::KeyCode::KEY_CONTROL),
			key_Shift(axl::gl::input::KeyCode::KEY_SHIFT),
			key_Alt(axl::gl::input::KeyCode::KEY_ALT),
			key_F2(axl::gl::input::KeyCode::KEY_F2),
			key_F3(axl::gl::input::KeyCode::KEY_F3),
			key_F4(axl::gl::input::KeyCode::KEY_F4),
			key_F5(axl::gl::input::KeyCode::KEY_F5),
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
			font.setContext(&this->main_context);
			vertex_array = 0;
			vertex_buffer = 0;
			uloc_projection = -1;
			uloc_view = -1;
			uloc_model = -1;
			uloc_TextColor = -1;
			model_transform = Transform4::scale(Vec3f::filled(0.8f));
			font_color = axl::math::Vec4f(0.1f,0.1f,0.1f,1.0f);
			is_animating = false;
			size_delta = 0;
			current_glyph_index = -1;
			time.set();
			ctime.set();
		}

		void update()
		{
			// update code
			if(this->is_animating)
			{
				model_transform = axl::math::Transform4::scale(axl::math::Vec3f::filled(0.7f)) * axl::math::Transform4::rotateZ(ctime.deltaTimef() * axl::math::Constants::F_HALF_PI);
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
				GL::glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
				GL::glClear(GL::GL_COLOR_BUFFER_BIT|GL::GL_DEPTH_BUFFER_BIT);
			}
			program.setUniformMat4fv(uloc_projection, this->camera.projection->matrix.values);
			program.setUniformMat4fv(uloc_view, this->camera.view_transform.values);
			program.setUniformMat4fv(uloc_model, this->model_transform.values);
			// Draw
			if(vertex_array != 0 && vertex_buffer != 0 && program.use())
			{
				this->font.texture.bind();
				GL::glBindVertexArray(vertex_array);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, vertex_buffer);
				GL::glDrawArrays(GL::GL_TRIANGLES, 0, 6);
				GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
				GL::glBindVertexArray(0);
				this->font.texture.unbind();
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
			{ // program
				// VERTEX_SHADER
				axl::gl::gfx::Shader vertex_shader(&this->main_context, GL::GL_VERTEX_SHADER);
				if(vertex_shader.create())
				{
					axl::util::String vertex_code = axl::util::File::getStringContent("tests/shaders/330/text_vpvms.vert");
					vertex_shader.setSource(vertex_code.cstr());
					vertex_shader.compile();
				}
				if(!vertex_shader.isCompiled())
				{
					axl::util::String info_log = vertex_shader.getInfoLog();
					printf("VERTEX_SHADER_INFO_LOG:\n****\n%s****\n", info_log.cstr());
				}
				// FRAGMENT_SHADER
				axl::gl::gfx::Shader fragment_shader(&this->main_context, GL::GL_FRAGMENT_SHADER);
				if(fragment_shader.create())
				{
					axl::util::String fragment_code = axl::util::File::getStringContent("tests/shaders/330/text_vpvms.frag");
					fragment_shader.setSource(fragment_code.cstr());
					fragment_shader.compile();
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
					uloc_model = program.getUniformLocation("u_MatModel");
					uloc_TextColor = program.getUniformLocation("u_TextColor");
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
			}
			// create vertext array and vertex buffer objects
			axl::math::Vec4f char_uv;
			if(this->font.create())
			{
				Assert(this->font.loadFromFile("/Windows/Fonts/consola.ttf", axl::math::Vec2i(64,64)));
				char_uv.set(0.0, 0.0, 1.0, 1.0);
			}
			if(this->main_context.makeCurrent())
			{
				GL::glGenVertexArrays(1, &vertex_array);
				GL::glGenBuffers(1, &vertex_buffer);
				if(vertex_array != 0 && vertex_buffer != 0)
				{
					axl::math::Vec2i texture_size = this->font.texture.getSize();
					axl::math::Vec2i font_size = this->font.size;
					GL::GLfloat tminx = char_uv.x;
					GL::GLfloat tmaxx = char_uv.z;
					GL::GLfloat tminy = char_uv.y;
					GL::GLfloat tmaxy = char_uv.w;
					const axl::glfl::GLfloat vertices[] = {
						-(float)this->size.y, -(float)this->size.y, 0.0, tminx, tminy,
						 (float)this->size.y, -(float)this->size.y, 0.0, tmaxx, tminy,
						 (float)this->size.y,  (float)this->size.y, 0.0, tmaxx, tmaxy,
						 (float)this->size.y,  (float)this->size.y, 0.0, tmaxx, tmaxy,
						-(float)this->size.y,  (float)this->size.y, 0.0, tminx, tmaxy,
						-(float)this->size.y, -(float)this->size.y, 0.0, tminx, tminy,
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
			program.setUniform4fv(uloc_TextColor, &this->font_color.x);
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
					((axl::gl::projection::Orthographicf*)this->camera.projection)->set((float)-width, (float)width, (float)-height, (float)height, 0.01f, 1000.0f);
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
				this->setCursor(this->cursor == CUR_NONE ? this->NormalCursor : CUR_NONE);
			if(key_F4.isPressed() && no_modifiers)
			{
				static axl::util::uc::Time ptime = ctime;
				this->is_animating = !this->is_animating;
				if(this->is_animating)
				{
					this->time.set();
					this->ctime.setFromReference(ptime);
				} else ptime.set();
			}
			if(key_F5.isPressed() && no_modifiers)
			{
				axl::math::Vec4f char_uv(0.0f, 0.0f, 1.0f, 1.0f);
				if(this->main_context.makeCurrent())
				{
					GL::glBindBuffer(GL::GL_ARRAY_BUFFER, this->vertex_buffer);
					GL::GLfloat *verteces = (GL::GLfloat*)GL::glMapBuffer(GL::GL_ARRAY_BUFFER, GL::GL_WRITE_ONLY);
					if(verteces)
					{
						verteces[0 * 5] = (float)-this->camera.viewport_size.x;
						verteces[0 * 5 + 1] = (float)-this->camera.viewport_size.x;
						verteces[1 * 5] = (float)this->camera.viewport_size.x;
						verteces[1 * 5 + 1] = (float)-this->camera.viewport_size.x;
						verteces[2 * 5] = (float)this->camera.viewport_size.x;
						verteces[2 * 5 + 1] = (float)this->camera.viewport_size.x;
						verteces[3 * 5] = (float)this->camera.viewport_size.x;
						verteces[3 * 5 + 1] = (float)this->camera.viewport_size.x;
						verteces[4 * 5] = (float)-this->camera.viewport_size.x;
						verteces[4 * 5 + 1] = (float)this->camera.viewport_size.x;
						verteces[5 * 5] = (float)-this->camera.viewport_size.x;
						verteces[5 * 5 + 1] = (float)-this->camera.viewport_size.x;
						verteces[0 * 5 + 3] = char_uv.x; verteces[0 * 5 + 4] = char_uv.y;
						verteces[1 * 5 + 3] = char_uv.z; verteces[1 * 5 + 4] = char_uv.y;
						verteces[2 * 5 + 3] = char_uv.z; verteces[2 * 5 + 4] = char_uv.w;
						verteces[3 * 5 + 3] = char_uv.z; verteces[3 * 5 + 4] = char_uv.w;
						verteces[4 * 5 + 3] = char_uv.x; verteces[4 * 5 + 4] = char_uv.w;
						verteces[5 * 5 + 3] = char_uv.x; verteces[5 * 5 + 4] = char_uv.y;
					}
					GL::glUnmapBuffer(GL::GL_ARRAY_BUFFER);
					GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
				}
			}
			if(down)
			{
				switch (key)
				{
					case KeyCode::KEY_ESCAPE:
						if(no_modifiers) axl::gl::Application::quit(0);
						break;
					case KeyCode::KEY_OPENBRACE:
					case KeyCode::KEY_CLOSEBRACE:
						if(bk_control && (!bk_shift && !bk_alt)) 
						{
							size_delta += (key == KEY_CLOSEBRACE ? 1 : -1);
							size_delta = this->font.size.x + size_delta <= 5 ? 5 - this->font.size.x : size_delta;
							printf("Font.size(%3d)\r", this->font.size.x + size_delta);
						}
						break;
					case KeyCode::KEY_LEFT:
						if(current_glyph_index == -1) current_glyph_index = this->font.glyphs.count() - 1;
						else if(bk_alt && (!bk_shift && !bk_control) && current_glyph_index > 0) current_glyph_index -= 1;
						else current_glyph_index = 0;
						break;
					case KeyCode::KEY_RIGHT:
						if(current_glyph_index == -1) current_glyph_index = 0;
						else if(bk_alt && (!bk_shift && !bk_control) && current_glyph_index < this->font.glyphs.count() - 1) current_glyph_index += 1;
						else current_glyph_index = this->font.glyphs.count() - 1;
						break;
					default:
						axl::gl::View::onKey(key, down);
				}
				if((key == KeyCode::KEY_LEFT || key == KeyCode::KEY_RIGHT) && bk_alt && (!bk_shift && !bk_control))
				{
					if(this->font.isValid())
					{
						unsigned int glyph_index = current_glyph_index;
						if(glyph_index != -1 && this->main_context.makeCurrent())
						{
							GL::glBindBuffer(GL::GL_ARRAY_BUFFER, this->vertex_buffer);
							GL::GLfloat *verteces = (GL::GLfloat*)GL::glMapBuffer(GL::GL_ARRAY_BUFFER, GL::GL_WRITE_ONLY);
							if(verteces)
							{
								float sz_x = this->font.glyphs[glyph_index].width;
								float sz_y = this->font.glyphs[glyph_index].height;
								float x_off = (float)this->font.glyphs[glyph_index].horiBearingX;
								float y_off = (float)this->font.glyphs[glyph_index].horiBearingY;
								verteces[0 * 5] = x_off;
								verteces[0 * 5 + 1] = y_off;
								verteces[1 * 5] = x_off + sz_x;
								verteces[1 * 5 + 1] = y_off;
								verteces[2 * 5] = x_off + sz_x;
								verteces[2 * 5 + 1] = y_off + sz_y;
								verteces[3 * 5] = x_off + sz_x;
								verteces[3 * 5 + 1] = y_off + sz_y;
								verteces[4 * 5] = x_off;
								verteces[4 * 5 + 1] = y_off + sz_y;
								verteces[5 * 5] = x_off;
								verteces[5 * 5 + 1] = y_off;
								verteces[0 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[0 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
								verteces[1 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[1 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
								verteces[2 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[2 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
								verteces[3 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[3 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
								verteces[4 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[4 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
								verteces[5 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[5 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
							}
							GL::glUnmapBuffer(GL::GL_ARRAY_BUFFER);
							GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
						}
					}
				}
			}
			else
			{
				switch (key)
				{
					case KeyCode::KEY_CONTROL:
						if(size_delta != 0)
						{
							this->font.setSize(axl::math::Vec2i::filled(this->font.size.x + size_delta));
							size_delta = 0;
						}
						break;
					default:
						axl::gl::View::onKey(key, down);
				}
			}
		}

		void onChar(wchar_t ch)
		{
			axl::gl::View::onChar(ch);
			if(ch == 13) return;
			bool bk_control = key_Control.isDown(), bk_shift = key_Shift.isDown(), bk_alt = key_Alt.isDown();
			if(bk_control || bk_alt) return;
			if(this->font.isValid())
			{
				unsigned int glyph_index = this->font.getCharIndex(ch);
				if(glyph_index != -1 && this->main_context.makeCurrent())
				{
					current_glyph_index = glyph_index;
					GL::glBindBuffer(GL::GL_ARRAY_BUFFER, this->vertex_buffer);
					GL::GLfloat *verteces = (GL::GLfloat*)GL::glMapBuffer(GL::GL_ARRAY_BUFFER, GL::GL_WRITE_ONLY);
					if(verteces)
					{
						float sz_x = this->font.glyphs[glyph_index].width;
						float sz_y = this->font.glyphs[glyph_index].height;
						float x_off = (float)this->font.glyphs[glyph_index].horiBearingX;
						float y_off = (float)this->font.glyphs[glyph_index].horiBearingY;
						verteces[0 * 5] = x_off;
						 verteces[0 * 5 + 1] = y_off;
						verteces[1 * 5] = x_off + sz_x;
						 verteces[1 * 5 + 1] = y_off;
						verteces[2 * 5] = x_off + sz_x;
						 verteces[2 * 5 + 1] = y_off + sz_y;
						verteces[3 * 5] = x_off + sz_x;
						 verteces[3 * 5 + 1] = y_off + sz_y;
						verteces[4 * 5] = x_off;
						 verteces[4 * 5 + 1] = y_off + sz_y;
						verteces[5 * 5] = x_off;
						 verteces[5 * 5 + 1] = y_off;
						verteces[0 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[0 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
						verteces[1 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[1 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
						verteces[2 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[2 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
						verteces[3 * 5 + 3] = this->font.glyphs[glyph_index].UV.z; verteces[3 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
						verteces[4 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[4 * 5 + 4] = this->font.glyphs[glyph_index].UV.w;
						verteces[5 * 5 + 3] = this->font.glyphs[glyph_index].UV.x; verteces[5 * 5 + 4] = this->font.glyphs[glyph_index].UV.y;
					}
					GL::glUnmapBuffer(GL::GL_ARRAY_BUFFER);
					GL::glBindBuffer(GL::GL_ARRAY_BUFFER, 0);
				}
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
	axl::gl::gfx::Font::cleanup();
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
	printf(">> axl.gl Text test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		Assertve(axl::glfl::core::load(), verbose);
		Application::onExit = onExit;
		Application::onQuit = onQuit;
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(800, 600);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.Text", position, size);
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
			axl::gl::input::Key key_Control(axl::gl::input::KeyCode::KEY_CONTROL), key_Shift(axl::gl::input::KeyCode::KEY_SHIFT), key_Alt(axl::gl::input::KeyCode::KEY_ALT);
			axl::gl::input::Key key_Up(axl::gl::input::KeyCode::KEY_UP), key_Down(axl::gl::input::KeyCode::KEY_DOWN), key_Left(axl::gl::input::KeyCode::KEY_LEFT), key_Right(axl::gl::input::KeyCode::KEY_RIGHT);
			while(!Application::IS_QUITTING)
			{
				/// -- handle realtime events
				bool bk_control = key_Control.isDown(), bk_shift = key_Shift.isDown(), bk_alt = key_Alt.isDown();
				bool bk_up = key_Up.isDown(), bk_down = key_Down.isDown(), bk_left = key_Left.isDown(), bk_right = key_Right.isDown();
				bool no_modifiers = !bk_control && !bk_shift && !bk_alt;
				float delta_time = time.deltaTimef();
				time.set();
				if(bk_up ^ bk_down)
				{
					if(bk_control && (!bk_shift && !bk_alt))
					{
						view.camera.scale += 2.0f * (bk_up ? 1.0f : -1.0f) * delta_time;
						view.camera.updateTransform();
					}
					else if(no_modifiers)
					{
						float delta = 1.0f * view.camera.viewport_size.y * (bk_up ? 1.0f : -1.0f) * delta_time;
						view.camera.position.y += delta;
						view.camera.target.y += delta;
						view.camera.updateTransform();
					}
				}
				if(bk_left ^ bk_right)
				{
					if(no_modifiers)
					{
						float delta = 1.0f * view.camera.viewport_size.y * (bk_right ? 1.0f : -1.0f) * delta_time;
						view.camera.position.x += delta;
						view.camera.target.x += delta;
						view.camera.updateTransform();
					}
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