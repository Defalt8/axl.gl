#include "common/stdafx.hpp"
#include "common/MainView.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../3DParty/stb/stb_image.h"

axl::gl::Display g_display;

class MainView : public Test::MainView
{
	private:
		axl::gl::gfx::UIManager m_uimanager;
		axl::gl::gfx::Font m_default_font;
		axl::gl::gfx::ui::TextView::Program m_default_text_program;
		axl::gl::gfx::ui::TextView m_text;
		axl::gl::gfx::Texture2D m_button_texture;
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor) :
			Test::MainView(_title, _position, _size, _cursor),
			m_uimanager(&context),
			m_default_font(&context),
			m_default_text_program(&context),
			m_text(&context),
			m_button_texture(&context)
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
			//
			{
				Assert(m_button_texture.create());
				int txw, txh, ch;
				stbi__vertically_flip_on_load = true;
				unsigned char *image = stbi_load("../../common/textures/button_b0.png", &txw, &txh, &ch, 4);
				if(image)
				{
					Assert(m_button_texture.allocate(0, txw, txh, GL_RGBA8, 0));
					Assert(m_button_texture.setImage(0, 0, 0, txw, txh, GL_RGBA, GL_UNSIGNED_BYTE, image, 1));
				}
				stbi_image_free(image);
				m_button_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				m_button_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			m_uimanager.setContext(&context);
			Assert(m_uimanager.create());
			m_default_font.setContext(&context);
			Assert(m_default_font.create());
			Assert(m_default_font.loadFromFile("../../common/fonts/OpenSans-Regular.ttf", axl::math::Vec2i(64,64)));
			m_default_text_program.setContext(&context);
			Assert(m_default_text_program.create());
			m_text.setFont(&m_default_font);
			m_text.setContext(&context);
			m_text.setProgram(&m_default_text_program);
			m_text.setAlignment(axl::gl::gfx::ui::TextView::TAL_CENTER);
			// m_text.setBorderColor(axl::math::Vec4f(1.f,1.f,1.f,1.f));
			// m_text.setBorderSize(axl::math::Vec4f(6.f,6.f,6.f,6.f));
			// m_text.setBackgroundColor(axl::math::Vec4f(1.f,0.f,.3f,1.0f));
			m_text.setTextColor(axl::math::Vec4f(1.f,1.f,1.f,1.f));
			m_text.setTextShadowOffset(axl::math::Vec2f(1.f,-1.f));
			m_text.setTextShadowColor(axl::math::Vec4f(.1f,.1f,.6f,0.96f));
			m_text.setShadowColor(axl::math::Vec4f(.3f,.1f,.1f,0.69f));
			m_text.setShadowOffset(axl::math::Vec4f(10.f,-10.f,10.f,-10.f));
			m_text.setPadding(axl::math::Vec4f(35.f,30.f,35.f,30.f));
			Assert(m_text.setBackgroundTexture(&m_button_texture));
			m_text.enable_shadow = true;
			m_text.enable_text_shadow = true;
			Assert(m_text.create());
			Assert(m_text.setQuality(axl::gl::gfx::ui::TextView::Q_HIGH));
			Assert(m_text.setText(L"Hello World!"));
			Assert(m_text.setSize(m_text.getMaxSize()));
			axl::math::Vec2i text_position = (this->size - m_text.getSize()) / 2;
			m_text.transform.setPosition(axl::math::Vec3f((float)text_position.x,(float)text_position.y, 0.0f));
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
			new_title.format(L"UI Manager | FPS: %.2f", FPS);
			this->setTitle(new_title);
			return true;
		}
		bool render()
		{
			using namespace GL;
			if(!Test::MainView::render()) return false;
			glClearColor(.87f, .87f, .87f, .0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			m_text.render(&main_camera);
			return true;
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			axl::math::Vec2i text_position = (this->size - m_text.getSize()) / 2;
			m_text.transform.setPosition(axl::math::Vec3f((float)text_position.x,(float)text_position.y, 0.0f));
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
			axl::gl::Application::pollEvents(g_display);
			main_view.update();
			main_view.render();
			main_view.swap();
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}