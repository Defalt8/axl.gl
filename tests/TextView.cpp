/***********************************
 * axl::gl::gfx::UIElement test    *
 *                                 *
 * @author Natnael Eshetu          *
 ***********************************/

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
		axl::gl::gfx::Font font_status, font1, font2;
		axl::gl::gfx::ui::TextView::Program text_view_program;
		axl::gl::gfx::ui::TextView status_text;
		axl::gl::gfx::ui::TextView text_view[4];
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
			text_view_program.setContext(&this->main_context);
			font_status.setContext(&this->main_context);
			font1.setContext(&this->main_context);
			font2.setContext(&this->main_context);
			status_text.setContext(&this->main_context);
			status_text.setFont(&this->font_status);
			status_text.setProgram(&this->text_view_program);
			for(int i=0; i < sizeof(text_view)/sizeof(axl::gl::gfx::ui::TextView); ++i)
			{
				text_view[i].setContext(&this->main_context);
				text_view[i].setProgram(&this->text_view_program);
			}
			is_animating = false;
			time.set();
			ctime.set();
			size_delta = 0;
		}

		void update()
		{
			using namespace axl::math;
			// update code
			if(this->is_animating)
			{
			}
			this->time.set();
		}

		void render()
		{
			if(!this->isValid())
				return;
			this->render(true);
			this->swap();
			GL::glFinish();
		}

		void render(bool p_clear)
		{
			if(!this->camera.makeCurrent(&this->main_context)) return;
			if(p_clear)
			{
				GL::glClearColor(0.85f, 0.8f, 0.8f, 0.0f);
				GL::glClear(GL::GL_COLOR_BUFFER_BIT|GL::GL_DEPTH_BUFFER_BIT);
			}
			this->status_text.render(&this->camera);
			this->text_view[0].render(&this->camera);
			this->text_view[1].render(&this->camera);
			this->text_view[2].render(&this->camera);
			this->text_view[3].render(&this->camera);
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
			if(this->font_status.create())
			{
				Assert(this->font_status.loadFromFile("../../common/fonts/Roboto-Bold.ttf", axl::math::Vec2i(12,12)));
				Assert(this->font_status.setQuality(axl::gl::gfx::Font::Q_HIGH));
			}
			if(this->font1.create())
			{
				Assert(this->font1.loadFromFile("../../common/fonts/Montserrat-Bold.ttf", axl::math::Vec2i(26,26)));
				Assert(this->font1.setQuality(axl::gl::gfx::Font::Q_HIGH));
			}
			if(this->font2.create())
			{
				Assert(this->font2.loadFromFile("../../common/fonts/Roboto-Light.ttf", axl::math::Vec2i(22,22)));
				Assert(this->font2.setQuality(axl::gl::gfx::Font::Q_HIGH));
			}
			Assert(this->text_view_program.create());
			//
			{
				this->status_text.setAlignment(axl::gl::gfx::Text::Alignment::TAL_TOP_RIGHT);
				this->status_text.setTextColor(axl::math::Vec4f(0.0f,1.0f,0.0f,1.0f));
				this->status_text.setBackgroundColor(axl::math::Vec4f(0.0f,0.0f,0.8f,0.8f));
				this->status_text.setBorderColor(axl::math::Vec4f(1.0f,0.0f,0.0f,1.0f));
				this->status_text.setBorderSize(axl::math::Vec4f(1.0f,1.0f,2.0f,1.0f));
				this->status_text.setSpacing(axl::math::Vec2f(0.0f,0.0f));
				this->status_text.setPadding(axl::math::Vec4f(5.0f,5.0f,5.0f,5.0f));
				this->status_text.setSize(axl::math::Vec2i(300,220));
				Assert(this->status_text.create());
				Assert(this->status_text.setStorageSize(256));
				Assert(this->status_text.setText(L"{TODO:status}"));
				Assert(this->status_text.setQuality(axl::gl::gfx::UIElement::Q_HIGH));
			}
			//
			{
				this->text_view[0].setFont(&this->font2);
				this->text_view[0].transform.setPosition(axl::math::Vec3f(69.0f,69.0f,-0.01f));
				this->text_view[0].setTextColor(axl::math::Vec4f(0.1f,0.1f,0.1f,1.0f));
				this->text_view[0].setBackgroundColor(axl::math::Vec4f(0.94f,0.94f,0.94f,1.0f));
				this->text_view[0].setBorderColor(axl::math::Vec4f(0.9f,0.0f,0.1f,1.0f));
				this->text_view[0].setBorderSize(axl::math::Vec4f(0.0f,2.0f,0.0f,0.0f));
				this->text_view[0].setPadding(axl::math::Vec4f(4.0f,4.0f,4.0f,6.0f));
				this->text_view[0].setSpacing(axl::math::Vec2f(2.0f,0.0f));
				Assert(this->text_view[0].create());
				Assert(this->text_view[0].setQuality(axl::gl::gfx::UIElement::Q_MEDIUM));
				this->text_view[0].setText(L"Hello World!");
				this->text_view[0].setSize(this->text_view[0].getMaxSize() * axl::math::Vec2i(2,1));
			}
			{
				this->text_view[1].setFont(&this->font2);
				this->text_view[1].transform.setPosition(axl::math::Vec3f(69.0f,129.0f,0.0f));
				this->text_view[1].setTextColor(axl::math::Vec4f(1.0f,0.6f,0.0f,1.0f));
				this->text_view[1].setBackgroundColor(axl::math::Vec4f(0.1f,0.1f,0.3f,0.9f));
				this->text_view[1].setBorderColor(axl::math::Vec4f(1.0f,0.6f,0.0f,1.0f));
				this->text_view[1].setBorderSize(axl::math::Vec4f(4.0f,4.0f,4.0f,4.0f));
				this->text_view[1].setPadding(axl::math::Vec4f(10.0f,6.0f,10.0f,6.0f));
				this->text_view[1].setSpacing(axl::math::Vec2f(1.0f,0.0f));
				Assert(this->text_view[1].create());
				Assert(this->text_view[1].setQuality(axl::gl::gfx::UIElement::Q_MEDIUM));
				this->text_view[1].setText(L"3.1415 @defaltAxel");
				this->text_view[1].setSize(this->text_view[1].getMaxSize() + 25);
			}
			{
				this->text_view[2].setFont(&this->font1);
				this->text_view[2].transform.setPosition(axl::math::Vec3f(69.0f,209.0f,0.0f));
				this->text_view[2].enable_shadow = true;
				this->text_view[2].setShadowOffset(axl::math::Vec2f(1.0f,-1.0f));
				this->text_view[2].setTextColor(axl::math::Vec4f(0.96f,1.0f,0.0f,1.0f));
				this->text_view[2].setBackgroundColor(axl::math::Vec4f(0.3f,0.9f,0.9f,0.8f));
				this->text_view[2].setBorderColor(axl::math::Vec4f(0.96f,1.0f,0.0f,1.0f));
				this->text_view[2].setBorderSize(axl::math::Vec4f(10.0f,1.0f,1.0f,1.0f));
				this->text_view[2].setPadding(axl::math::Vec4f(18.0f,2.0f,2.0f,2.0f));
				this->text_view[2].setSpacing(axl::math::Vec2f(1.0f,2.0f));
				Assert(this->text_view[2].create());
				Assert(this->text_view[2].setQuality(axl::gl::gfx::UIElement::Q_MEDIUM));
				this->text_view[2].setText(L"To be or not to be!\nThat is the question.\nOne can only wonder.");
				this->text_view[2].setSize(this->text_view[2].getMaxSize() + 50);
			}
			{
				this->text_view[3].setFont(&this->font1);
				this->text_view[3].transform.setPosition(axl::math::Vec3f(69.0f,379.0f,0.0f));
				this->text_view[3].setTextColor(axl::math::Vec4f(1.0f,1.0f,1.0f,1.0f));
				this->text_view[3].enable_shadow = true;
				this->text_view[3].setShadowOffset(axl::math::Vec2f(1.0f,-2.0f));
				this->text_view[3].setShadowColor(axl::math::Vec4f(1.0f,0.0f,0.0f,0.9f));
				this->text_view[3].setBackgroundColor(axl::math::Vec4f(0.0f,0.0f,0.0f,0.1f));
				this->text_view[3].setBorderColor(axl::math::Vec4f(1.0f,1.0f,1.0f,1.0f));
				this->text_view[3].setBorderSize(axl::math::Vec4f(2.0f,0.0f,0.0f,0.0f));
				this->text_view[3].setPadding(axl::math::Vec4f(10.0f,10.0f,10.0f,10.0f));
				this->text_view[3].setSpacing(axl::math::Vec2f(1.0f,2.0f));
				Assert(this->text_view[3].create());
				Assert(this->text_view[3].setQuality(axl::gl::gfx::UIElement::Q_MEDIUM));
				this->text_view[3].setText(L"- Learn\n- Adapt\n- Overcome");
				this->text_view[3].setSize(this->text_view[3].getMaxSize());
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
					this->projection.set(0.0f, (float)this->camera.viewport_size.x, 0.0f, (float)this->camera.viewport_size.y,-10.0f, 10.0f);
				}
				this->projection.updateTransform();
				axl::math::Vec2i status_text_size = this->status_text.getSize();
				this->status_text.transform.setPosition(axl::math::Vec3f(
					(float)this->camera.viewport_size.x - status_text_size.x - 10.0f, 
					(float)this->camera.viewport_size.y - status_text_size.y - 10.0f, 0.0f)
					);
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
			if(key_F5.isPressed() && no_modifiers) // Reset
			{
				this->camera.position.set(0.0f);
			}
			bool bp_left = key_Left.isPressed(), bp_right = key_Right.isPressed();
			bool bp_up = key_Up.isPressed(), bp_down = key_Down.isPressed();
			if(bk_shift && !bk_alt && !bk_control)
			{
				if(bp_left ^ bp_right)
				{
					if(bp_left)
					{
						for(int i=0; i < sizeof(text_view)/sizeof(axl::gl::gfx::ui::TextView); ++i)
						{
							switch(this->text_view[i].getHorizontalAlignment())
							{
								case axl::gl::gfx::Text::TAL_LEFT:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_RIGHT);
									break;
								case axl::gl::gfx::Text::TAL_RIGHT:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_HORIZONTAL_CENTER);
									break;
								case axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_LEFT);
									break;
							}
						}
					}
					else if(bp_right)
					{
						for(int i=0; i < sizeof(text_view)/sizeof(axl::gl::gfx::ui::TextView); ++i)
						{
							switch(this->text_view[i].getHorizontalAlignment())
							{
								case axl::gl::gfx::ui::TextView::TAL_LEFT:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_HORIZONTAL_CENTER);
									break;
								case axl::gl::gfx::ui::TextView::TAL_RIGHT:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_LEFT);
									break;
								case axl::gl::gfx::ui::TextView::TAL_HORIZONTAL_CENTER:
									this->text_view[i].setHorizontalAlignment(axl::gl::gfx::ui::TextView::TAL_RIGHT);
									break;
							}
						}
					}
				}
				if(bp_up ^ bp_down)
				{
					if(bp_up)
					{
						for(int i=0; i < sizeof(text_view)/sizeof(axl::gl::gfx::ui::TextView); ++i)
						{
							switch(this->text_view[i].getVerticalAlignment())
							{
								case axl::gl::gfx::ui::TextView::TAL_TOP:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_BOTTOM);
									break;
								case axl::gl::gfx::ui::TextView::TAL_BOTTOM:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_VERTICAL_CENTER);
									break;
								case axl::gl::gfx::ui::TextView::TAL_VERTICAL_CENTER:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_TOP);
									break;
							}
						}
					}
					else if(bp_down)
					{
						for(int i=0; i < sizeof(text_view)/sizeof(axl::gl::gfx::ui::TextView); ++i)
						{
							switch(this->text_view[i].getVerticalAlignment())
							{
								case axl::gl::gfx::ui::TextView::TAL_TOP:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_VERTICAL_CENTER);
									break;
								case axl::gl::gfx::ui::TextView::TAL_BOTTOM:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_TOP);
									break;
								case axl::gl::gfx::ui::TextView::TAL_VERTICAL_CENTER:
									this->text_view[i].setVerticalAlignment(axl::gl::gfx::ui::TextView::TAL_BOTTOM);
									break;
							}
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
						break;
					case KeyCode::KEY_OPENBRACE:
					case KeyCode::KEY_CLOSEBRACE:
						if(bk_control && (!bk_shift && !bk_alt)) 
						{
							size_delta += (key == KEY_CLOSEBRACE ? 1 : -1);
							size_delta = this->font1.size.x + size_delta <= 5 ? 5 - this->font1.size.x : size_delta;
							printf("Font.size(%3d)\r", this->font1.size.x + size_delta);
						}
						break;
					default:
						axl::gl::View::onKey(key, down);
				}
			}
			else
			{
				switch (key)
				{
					case KeyCode::KEY_CONTROL:
						if(size_delta != 0)
						{
							font1.setSize(axl::math::Vec2i::filled(this->font1.size.x + size_delta));
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
					axl::math::Vec2i font_size = view.font1.size;
					axl::math::Vec3f text_scale = view.text_view[0].transform.getScale();
					axl::math::Vec2i texture_size = view.font1.texture.getSize();
					unsigned int glyph_count = view.font1.glyphs.count();
					const wchar_t *halignment_str = L"-", *valignment_str = L"-";
					switch(view.text_view[0].getHorizontalAlignment())
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
					switch(view.text_view[0].getVerticalAlignment())
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
						view.text_view[0].getText().length(),
						halignment_str, valignment_str
					);
					view.status_text.setText(fps_string);
					axl::math::Vec2i status_text_size = view.status_text.getMinSize();
					axl::math::Vec4f status_text_padding = view.status_text.getPadding();
					axl::math::Vec2i new_size(view.status_text.getMaxSize());
					view.status_text.setSize(new_size);
					view.status_text.transform.setPosition(axl::math::Vec3f(
						(float)view.camera.viewport_size.x - new_size.x - 10.0f, 
						(float)view.camera.viewport_size.y - new_size.y - 10.0f, 0.0f
					));
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