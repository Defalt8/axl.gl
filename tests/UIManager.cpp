#include <thread>
#include "common/stdafx.hpp"
#include "common/MainView.hpp"

axl::gl::Display g_display;

class MyTextView : public axl::gl::gfx::ui::TextView
{
	public:
		MyTextView(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::Font* ptr_font = 0,
			axl::gl::gfx::Program* ptr_program = 0,
			axl::math::Vec2i size = axl::math::Vec2i(100,40)
			) :
			axl::gl::gfx::ui::TextView(ptr_context, ptr_font, ptr_program, size)
		{}
		~MyTextView()
		{
			this->destroy();
		}
	protected:
		bool icreate()
		{
			this->setAlignment(axl::gl::gfx::ui::TextView::TAL_CENTER);
			this->setBorderColor(axl::math::Vec4f(0.1f,0.1f,0.1f,1.f));
			this->setBorderSize(axl::math::Vec4f(3.f,2.f,3.f,2.f));
			this->setBackgroundColor(axl::math::Vec4f(1.f,0.f,.3f+((float)m_count / 4.5f * 0.8f),1.0f));
			this->setTextColor(axl::math::Vec4f(1.f,1.f,1.f,1.f));
			this->setTextShadowOffset(axl::math::Vec2f(1.f,-1.f));
			this->setTextShadowColor(axl::math::Vec4f(.1f,.1f,.6f,0.69f));
			this->setShadowColor(axl::math::Vec4f(.0f,.1f,.1f,0.69f));
			this->setShadowOffset(axl::math::Vec4f(1.f,-1.f,1.f,-1.f));
			this->setPadding(axl::math::Vec4f(9.f,9.f,9.f,9.f));
			// this->enable_shadow = true;
			this->enable_text_shadow = true;
			if(axl::gl::gfx::ui::TextView::icreate())
			{
				axl::util::WString wstring(64);
				wstring.format(L"Text_%d", m_count);
				this->setText(wstring);
				this->setSize(this->getMaxSize());
				++m_count;
				return true;
			}
			return false;
		}
		bool idestroy()
		{
			if(axl::gl::gfx::ui::TextView::idestroy())
			{
				--m_count;
				return true;
			}
			return false;
		}
	private:
		static int m_count;
};
int MyTextView::m_count = 0;

class MainView : public Test::MainView
{
	private:
		axl::gl::gfx::UIManager m_uimanager;
		axl::gl::gfx::ui::layouts::Linear m_linear_layout;
		axl::gl::gfx::ui::Group m_text_group;
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::ui::TextView::Program m_default_text_program;
		MyTextView m_texts[4];
		axl::util::uc::Clock status_clock;
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
			Test::MainView(_title, _position, _size, _cursor),
			status_clock(300)
		{}
		~MainView()
		{
			this->destroy();
		}
	public:
		bool onCreate(bool recreating)
		{
			using namespace GL;
			if(!Test::MainView::onCreate()) return false;
			// m_uimanager
			m_uimanager.setContext(&context);
			Assert(m_uimanager.create());
			// m_linear_layout
			m_linear_layout.set(axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL,
				axl::gl::gfx::ui::layouts::Linear::AL_CENTER,
				axl::math::Vec2f(1.f,1.f),
				axl::math::Vec4f(2.f,2.f,2.f,2.f));
			// m_text_group
			m_text_group.setContext(&context);
			Assert(m_text_group.create());
			m_text_group.transform.setPosition(axl::math::Vec3f(40.0f,40.f,0.f));
			m_text_group.setSize(this->size - 80);
			m_text_group.setPadding(axl::math::Vec4f(10.0f,10.f,10.f,10.f));
			m_text_group.setLayout(&m_linear_layout);
			m_text_group.setBorderColor(axl::math::Vec4f(.0f,.9f,.0f,.99f));
			m_text_group.setBorderSize(axl::math::Vec4f(2.f,2.f,2.f,2.f));
			// m_text_group.setShadowColor(axl::math::Vec4f(.0f,.0f,.0f,.69f));
			// m_text_group.setShadowOffset(axl::math::Vec4f(-1.f,-1.f,1.f,1.f));
			// m_text_group.enable_shadow = true;
			// m_default_font
			m_default_font.setContext(&context);
			Assert(m_default_font.create());
			Assert(m_default_font.loadFromFile("../../common/fonts/OpenSans-Bold.ttf", axl::math::Vec2i(22,22)));
			// m_default_text_program
			m_default_text_program.setContext(&context);
			Assert(m_default_text_program.create());
			// m_texts
			for(int i=0; i<sizeof(m_texts)/sizeof(MyTextView); ++i)
			{
				m_texts[i].setFont(&m_default_font);
				m_texts[i].setContext(&context);
				m_texts[i].setProgram(&m_default_text_program);
				Assert(m_texts[i].create());
			}
			for(int i=0; i<sizeof(m_texts)/sizeof(MyTextView); ++i)
			{
				Assert(m_text_group.add(&m_texts[i]));
			}
			m_text_group.organize();
			return true;
		}
		bool initialize()
		{
			if(!Test::MainView::initialize()) return false;
			return true;
		}
		bool update()
		{
			static axl::util::WString new_title(128);
			if(!Test::MainView::update()) return false;
			if(status_clock.checkAndSet(true))
			{
				new_title.format(L"UI Manager | FPS: %.2f", FPS);
				this->setTitle(new_title);
			}
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render() || !this->main_camera.makeCurrent(&context, true)) return false;
			glClearColor(.87f, .87f, .87f, .0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			m_text_group.render(&main_camera);
			// for(int i=0; i<sizeof(m_texts)/sizeof(MyTextView); ++i)
			// 	m_texts[i].render(&main_camera);
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			m_text_group.setSize(this->size - 80);
			m_text_group.organize();
		}
		void onKey(axl::gl::input::KeyCode key_code, bool down)
		{
			if(down)
			{
			}
			else
			{
				switch(key_code)
				{
					case axl::gl::input::KeyCode::KEY_O:
						this->m_linear_layout.setOrientation(this->m_linear_layout.getOrientation() == axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL ? axl::gl::gfx::ui::layouts::Linear::OR_VERTICAL : axl::gl::gfx::ui::layouts::Linear::OR_HORIZONTAL);
						this->m_text_group.organize();
						break;
				}
			}
		}
	
};


int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl.base test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		// Create View
		Assertve(g_display.isOpen(), verbose);
		axl::math::Vec2i view_position(40, 60), view_size(640, 480);
		MainView main_view(L"UI Manager", view_position, view_size, axl::gl::Cursor::CUR_CROSS);
		Assertve(main_view.create(g_display, false, MainView::VF_RESIZABLE), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		main_view.context.setVSync(false);
		main_view.initialize();
		while(!axl::gl::Application::IS_QUITTING)
		{
			axl::gl::Application::pollEvent(g_display);
			if(main_view.isValid() && !main_view.is_paused)
			{
				main_view.update();
				main_view.render();
				main_view.swap();
			}
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}