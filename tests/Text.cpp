/******************************
 * axl::gl::gfx::Text test    *
 *                            *
 * @author Natnael Eshetu     *
 ******************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/everything.hpp>
#include <axl.util/everything.hpp>
#include <axl.math/everything.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include "common/GL.hpp"
#include "common/Assert.hpp"


namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

class GameView : public axl::gl::View
{
	public:
		axl::gl::Context main_context;
		axl::gl::camera::Camera3Df camera;
		axl::gl::gfx::Text::Program text_program;
		axl::gl::gfx::Font font, font1, font2;
		axl::gl::gfx::Text text, status_text;
	private:
		axl::gl::Cursor NormalCursor;
		axl::util::uc::Time time, ctime;
		axl::gl::projection::Orthographicf projection;
		axl::gl::input::Key key_Control, key_Shift, key_Alt, key_F2, key_F3, key_F4, key_F5, key_Space;
		axl::gl::input::Key key_Left, key_Right, key_Down, key_Up;
		bool is_animating;
		int size_delta;
	public:
		GameView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			axl::gl::View(_title, _position, _size, _cursor),
			main_context(),
			camera(),
			projection(),
			text_program(),
			text(),
			status_text(),
			NormalCursor(axl::gl::Cursor::CUR_CROSS),
			key_Control(axl::gl::input::KeyCode::KEY_CONTROL),
			key_Shift(axl::gl::input::KeyCode::KEY_SHIFT),
			key_Alt(axl::gl::input::KeyCode::KEY_ALT),
			key_F2(axl::gl::input::KeyCode::KEY_F2),
			key_F3(axl::gl::input::KeyCode::KEY_F3),
			key_F4(axl::gl::input::KeyCode::KEY_F4),
			key_F5(axl::gl::input::KeyCode::KEY_F5),
			key_Space(axl::gl::input::KeyCode::KEY_SPACE),
			key_Left(axl::gl::input::KeyCode::KEY_LEFT),
			key_Right(axl::gl::input::KeyCode::KEY_RIGHT),
			key_Down(axl::gl::input::KeyCode::KEY_DOWN),
			key_Up(axl::gl::input::KeyCode::KEY_UP)
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
			text_program.setContext(&this->main_context);
			font.setContext(&this->main_context);
			font1.setContext(&this->main_context);
			font2.setContext(&this->main_context);
			text.setContext(&this->main_context);
			text.setFont(&this->font);
			text.setTextProgram(&this->text_program);
			status_text.setContext(&this->main_context);
			status_text.setFont(&this->font2);
			status_text.setTextProgram(&this->text_program);
			is_animating = false;
			size_delta = 0;
			time.set();
			ctime.set();
		}

		void update()
		{
			using namespace axl::math;
			// update code
			if(this->is_animating)
			{
				float C1 = map(axl::math::sin(ctime.deltaTimef() * axl::math::Constants::F_HALF_PI), -1.0f, 1.0f, 0.0f, 1.0f);
				float C2 = map(axl::math::cos(ctime.deltaTimef() * axl::math::Constants::F_QUARTER_PI), -1.0f, 1.0f, 0.0f, 1.0f);
				this->text.setColor(Vec4f(C1,C2,1.0f-C1,1.0f));
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
			
			GL::glFinish();
			GL::glDisable(GL::GL_BLEND);
			GL::glDisable(GL::GL_DEPTH_TEST);
		}

		void render(bool p_clear)
		{
			if(!this->camera.makeCurrent(&this->main_context)) return;
			if(p_clear)
			{
				GL::glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
				GL::glClear(GL::GL_COLOR_BUFFER_BIT|GL::GL_DEPTH_BUFFER_BIT);
			}
			this->text.render(&this->camera);
			this->status_text.render(&this->camera);
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
				axl::gl::ContextConfig(1, 4, 6, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(2, 3, 3, axl::gl::ContextConfig::GLP_CORE),
				axl::gl::ContextConfig(3, 3, 0, axl::gl::ContextConfig::GLP_COMPATIBLITY),
				axl::gl::ContextConfig(4, 2, 1, axl::gl::ContextConfig::GLP_COMPATIBLITY)
			};
			if(!this->main_context.create(recreating, this, context_configs, sizeof(context_configs)/sizeof(axl::gl::ContextConfig))) return false;
			this->init();
			// Create stuff here
			Assert(this->text_program.create());
			if(this->font.create())
			{
				Assert(this->font.loadFromFile("/Windows/Fonts/consola.ttf", axl::math::Vec2i(16,16)));
			}
			if(this->font1.create())
			{
				Assert(this->font1.loadFromFile("../../common/fonts/Roboto-Italic.ttf", axl::math::Vec2i(16,16)));
			}
			if(this->font2.create())
			{
				Assert(this->font2.loadFromFile("/Windows/Fonts/consolab.ttf", axl::math::Vec2i(11,11)));
			}
			if(this->text.create())
			{
				this->text.setAlignment(axl::gl::gfx::Text::Alignment::TAL_TOP_LEFT);
				this->text.setSpacing(axl::math::Vec2f(1.0f, 1.0f));
				// this->text.setText(axl::util::File::getWStringContent("tests/sample_text.txt"));
				this->text.setText(L"To be, or not to be, that is the question:\n"
					"Whether 'tis nobler in the mind to suffer\n"
					"The slings and arrows of outrageous fortune,\n"
					"Or to take arms against a sea of troubles\n"
					"And by opposing end them. To die-to sleep,\n"
					"No more; and by a sleep to say we end\n"
					"The heart-ache and the thousand natural shocks\n"
					"That flesh is heir to: 'tis a consummation\n"
					"Devoutly to be wish'd. To die, to sleep;\n"
					"To sleep, perchance to dream-ay, there's the rub:\n"
					"For in that sleep of death what dreams may come,\n"
					"When we have shuffled off this mortal coil,\n"
					"Must give us pause-there's the respect\n"
					"That makes calamity of so long life.\n"
					"For who would bear the whips and scorns of time,\n"
					"Th'oppressor's wrong, the proud man's contumely,\n"
					"The pangs of dispriz'd love, the law's delay,\n"
					"The insolence of office, and the spurns\n"
					"That patient merit of th'unworthy takes,\n"
					"When he himself might his quietus make\n"
					"With a bare bodkin? Who would fardels bear,\n"
					"To grunt and sweat under a weary life,\n"
					"But that the dread of something after death,\n"
					"The undiscovere'd country, from whose bourn\n"
					"No traveller returns, puzzles the will,\n"
					"And makes us rather bear those ills we have\n"
					"Than fly to others that we know not of?\n"
					"Thus conscience doth make cowards of us all,\n"
					"And thus the native hue of resolution\n"
					"Is sicklied o'er with the pale cast of thought,\n"
					"And enterprises of great pith and moment\n"
					"With this regard their currents turn awry\n"
					"And lose the name of action."
					);
				this->text.text_transform.setScale(axl::math::Vec3f(1.0f,1.0f,1.0f), false);
				this->text.text_transform.setPosition(axl::math::Vec3f(5.0f, (float)this->camera.viewport_size.y - 5.0f, -0.1f));
				this->text.setColor(axl::math::Vec4f(0.2f,0.79f,0.5f,1.0f));
			}
			if(this->status_text.create())
			{
				this->status_text.setAlignment(axl::gl::gfx::Text::Alignment::TAL_TOP_RIGHT);
				Assert(this->status_text.setStorageSize(128));
				Assert(this->status_text.setText(L"{TODO:status}"));
				this->status_text.setColor(axl::math::Vec4f(0.9f,0.1f,0.9f,0.9f));
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
				GL::glViewport(0, 0, width, height);
				this->camera.viewport_size.set(width, height);
				if(this->camera.projection->type == axl::gl::projection::Projectionf::Type::PT_ORTHOGRAPHIC)
				{
					((axl::gl::projection::Orthographicf*)this->camera.projection)->set(0.0f, (float)this->camera.viewport_size.x, 0.0f, (float)this->camera.viewport_size.y, 0.01f, 1000.0f);
				}
				this->camera.projection->updateTransform();
				this->status_text.text_transform.setPosition(axl::math::Vec3f((float)this->camera.viewport_size.x - 10.0f, (float)this->camera.viewport_size.y - 10.0f, 0.0f));
			}
			this->update();
			this->render();
		}

		bool onKey(axl::gl::input::KeyCode key, bool down)
		{
			using namespace axl::gl::input;
			bool bk_control = key_Control.isDown(), bk_shift = key_Shift.isDown(), bk_alt = key_Alt.isDown();
			bool no_modifiers = !bk_control && !bk_shift && !bk_alt;
			if(key_F2.isPressed() && no_modifiers)
				this->show(this->visiblity == VS_FULLSCREEN ? SM_SHOW : SM_FULLSCREEN);
			if(key_F3.isPressed() && no_modifiers)
				this->setCursor(this->cursor == axl::gl::Cursor::CUR_NONE ? this->NormalCursor : axl::gl::Cursor::CUR_NONE);
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
				this->camera.position.set(0.0f);
				this->text.text_transform.setPosition(axl::math::Vec3f(5.0f, (float)this->camera.viewport_size.y - 5.0f, -0.1f), false);
				this->text.text_transform.setScale(axl::math::Vec3f::filled(1.0f), false);
				this->text.text_transform.setRotation(axl::math::Vec3f::filled(0.0f));
			}
			bool bp_left = key_Left.isPressed(), bp_right = key_Right.isPressed();
			bool bp_up = key_Up.isPressed(), bp_down = key_Down.isPressed();
			if(bk_shift && !bk_alt && !bk_control)
			{
				if(bp_left ^ bp_right)
				{
					if(bp_left)
					{
						switch(this->text.getHorizontalAlignment())
						{
							case axl::gl::gfx::Text::TAL_LEFT:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_RIGHT);
								break;
							case axl::gl::gfx::Text::TAL_RIGHT:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER);
								break;
							case axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_LEFT);
								break;
						}
					}
					else if(bp_right)
					{
						switch(this->text.getHorizontalAlignment())
						{
							case axl::gl::gfx::Text::TAL_LEFT:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER);
								break;
							case axl::gl::gfx::Text::TAL_RIGHT:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_LEFT);
								break;
							case axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER:
								this->text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_RIGHT);
								break;
						}
					}
				}
				if(bp_up ^ bp_down)
				{
					if(bp_up)
					{
						switch(this->text.getVerticalAlignment())
						{
							case axl::gl::gfx::Text::TAL_TOP:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_BOTTOM);
								break;
							case axl::gl::gfx::Text::TAL_BOTTOM:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_VERTICAL_CENTER);
								break;
							case axl::gl::gfx::Text::TAL_VERTICAL_CENTER:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_TOP);
								break;
						}
					}
					else if(bp_down)
					{
						switch(this->text.getVerticalAlignment())
						{
							case axl::gl::gfx::Text::TAL_TOP:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_VERTICAL_CENTER);
								break;
							case axl::gl::gfx::Text::TAL_BOTTOM:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_TOP);
								break;
							case axl::gl::gfx::Text::TAL_VERTICAL_CENTER:
								this->text.setVerticalAlignment(axl::gl::gfx::Text::TAL_BOTTOM);
								break;
						}
					}
				}
			}
			if(down)
			{
				switch (key)
				{
					case KeyCode::KEY_ESCAPE:
						if(no_modifiers) axl::gl::Application::quit(0);
						return true;
					case KeyCode::KEY_OPENBRACE:
					case KeyCode::KEY_CLOSEBRACE:
						if(bk_control && (!bk_shift && !bk_alt)) 
						{
							size_delta += (key == KeyCode::KEY_CLOSEBRACE ? 1 : -1);
							size_delta = this->font.size.x + size_delta <= 5 ? 5 - this->font.size.x : size_delta;
							printf("Font.size(%3d)\r", this->font.size.x + size_delta);
						}
						return true;
				}
			}
			else
			{
				switch (key)
				{
					case KeyCode::KEY_1:
						this->text.setFont(&this->font);
						return true;
					case KeyCode::KEY_2:
						this->text.setFont(&this->font1);
						return true;
					case KeyCode::KEY_CONTROL:
						if(size_delta != 0)
						{
							axl::gl::gfx::Font* current_font = const_cast<axl::gl::gfx::Font*>(this->text.getFont());
							if(current_font)
							{
								current_font->setSize(axl::math::Vec2i::filled(this->font.size.x + size_delta));
								this->text.updateBuffers();
							}
							size_delta = 0;
						}
						return true;
				}
			}
			return axl::gl::View::onKey(key, down);
		}

		void onChar(wchar_t ch)
		{
			axl::gl::View::onChar(ch);
			if(ch == 13) return;
			bool bk_control = key_Control.isDown(), bk_shift = key_Shift.isDown(), bk_alt = key_Alt.isDown();
			if(bk_control || bk_alt) return;
		} 

};


/*****
 * 
 *****/
const axl::gl::ViewConfig view_configs[] = {
	axl::gl::ViewConfig(1, axl::gl::ViewConfig::PT_RGBA, 32,8,8,8,8, 24,8, 4, true, false),
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

void onExit(int exit_code)
{
	display.close();
	axl::gl::View::cleanup();
	axl::gl::gfx::Font::cleanup();
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
		Assertve(Application::init(), verbose);
		Assertve(display.isOpen(), verbose);

		axl::math::Vec2i size(800, 600);
		axl::math::Vec2i position = (display.size - size) / 2;

		GameView view(L"axl.gl.Text", position, size);
		Assertv(view.create(display, true, view_configs, sizeof(view_configs)/sizeof(axl::gl::ViewConfig), GameView::VF_RESIZABLE), verbose);
		Assertv(view.isValid(), verbose);
		printf(".. View.Config %d selected.\n", view.config.id);
		if(view.isValid())
		{
			printf(".. Context.Config %d selected. %d.%d\n", view.main_context.config.id, view.main_context.config.major_version, view.main_context.config.minor_version);
			view.main_context.setVSync(false);
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
						axl::math::Vec3f text_scale = view.text.text_transform.getScale();
						float delta = 0.8f * (bk_up ? 1.0f : -1.0f) * delta_time;
						view.text.text_transform.setScale(text_scale + delta);
					}
					else if(no_modifiers)
					{
						axl::math::Vec3f text_position = view.text.text_transform.getPosition();
						float delta = 1.0f * view.camera.viewport_size.y * (bk_up ? 1.0f : -1.0f) * delta_time;
						view.text.text_transform.setPosition(axl::math::Vec3f(text_position.x, text_position.y - delta, text_position.z));
					}
				}
				if(bk_left ^ bk_right)
				{
					if(no_modifiers)
					{
						axl::math::Vec3f text_position = view.text.text_transform.getPosition();
						float delta = 1.0f * view.camera.viewport_size.y * (bk_right ? 1.0f : -1.0f) * delta_time;
						view.text.text_transform.setPosition(axl::math::Vec3f(text_position.x - delta, text_position.y, text_position.z));
					}
				}
				/// -- update
				view.update();
				/// -- render
				static axl::util::uc::Clock frame_clock(200); // 200ms clock
				static axl::util::uc::Time frame_time;
				static int rendered_frames = 0;
				view.render();
				++rendered_frames;
				if(frame_clock.checkAndSet(false)) // no first time
				{
					float fps = rendered_frames / frame_time.deltaTimef();
					static axl::util::WString fps_string(1024);
					const axl::gl::gfx::Font *current_font = view.text.getFont();
					axl::math::Vec2i font_size = current_font ? current_font->size : axl::math::Vec2i(0,0);
					axl::math::Vec3f text_scale = view.text.text_transform.getScale();
					axl::math::Vec2i texture_size = current_font ? current_font->texture.getSize() : axl::math::Vec2i(0,0);
					unsigned int glyph_count = current_font ? current_font->glyphs.count() : 0;
					const wchar_t *halignment_str = L"-", *valignment_str = L"-";
					switch(view.text.getHorizontalAlignment())
					{
						case axl::gl::gfx::Text::TAL_LEFT:
							halignment_str = L"Left";
							break;
						case axl::gl::gfx::Text::TAL_RIGHT:
							halignment_str = L"Right";
							break;
						case axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER:
							halignment_str = L"Center";
							break;
					}
					switch(view.text.getVerticalAlignment())
					{
						case axl::gl::gfx::Text::TAL_TOP:
							valignment_str = L"Top";
							break;
						case axl::gl::gfx::Text::TAL_BOTTOM:
							valignment_str = L"Bottom";
							break;
						case axl::gl::gfx::Text::TAL_VERTICAL_CENTER:
							valignment_str = L"Center";
							break;
					}
					fps_string.format(L"FPS: %.1f\n"
						"GlyphCount:%u\n"
						"FontSize: %d,%d\n"
						"TextScale: %.1f,%.1f,%.1f\n"
						"TextureSize: %d,%d\n"
						"TextLength: %u\n"
						"Horizontal-Alignment:%s\n"
						"Vertical-Alignment:%s", 
						fps, glyph_count,
						font_size.x, font_size.y, 
						text_scale.x, text_scale.y, text_scale.z,
						texture_size.x, texture_size.y, 
						view.text.getText().length(),
						halignment_str, valignment_str
					);
					view.status_text.setText(fps_string);
					frame_time.set();
					rendered_frames = 0;
				}
				/// -- check events
				Application::pollEvents(display);
			}
		}
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}