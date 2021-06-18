#include "common/stdafx.hpp"
#include "common/MainView.hpp"
#include "common/Quad.hpp"

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

void onExit(int exit_code)
{
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	printf("-- exiting with code %d --\n", exit_code);
}

class MainView : public Test::MainView
{
	public:
		MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::gl::Cursor& _cursor = axl::gl::DefaultCursor) :
			Test::MainView(_title, _position, _size, _cursor),
			title_update_clock(500),
			update_clock(15)
		{}
		~MainView()
		{
			this->destroy();
		}
	public:
		bool initialize()
		{
			Test::MainView::initialize();
			axl::media::image::DIB bitmap;
			if(bitmap.loadFromFile(AXLGL_TEST_RES_DIR"/image/Alien.bmp"))
				image_alien = bitmap.toImage2D();
			return true;
		}
		bool update()
		{
			Test::MainView::update();
			if(title_update_clock.checkAndSet(false))
			{
				axl::util::WString new_title(96);
				new_title.format(L"axl.gl.Camera3D test | FPS: %.0f", this->FPS);
				this->setTitle(new_title);
			}
			if(update_clock.checkAndSet(true))
			{
				float delta_time = dtime.deltaTimef(), elapsed_time = etime.deltaTimef();
				dtime.set();
				quad.transform.setRotation(axl::math::Vec3f(0.f, elapsed_time * axl::math::Angle::degToRad(69.f), 0.f));
			}
			return true;
		}
		bool render()
		{
			if(!context.isValid() || !(context.isCurrent() || context.makeCurrent())) return false;
			using namespace GL;
			glEnable(GL_SCISSOR_TEST);
			// camera 1
			camera1.makeCurrent(&context, true);
			if(selected_camera == &camera1) glClearColor(1.f, .8f, .8f, 1.f);
			else glClearColor(1.f, .1f, .5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			quad.render(&camera1);
			// camera 2
			camera2.makeCurrent(&context, true);
			if(selected_camera == &camera2) glClearColor(.8f, 1.f, .8f, 1.f);
			else glClearColor(.5f, 1.f, .1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			quad.render(&camera2);
			// camera 3
			camera3.makeCurrent(&context, true);
			if(selected_camera == &camera3) glClearColor(.8f, .8f, 1.f, 1.f);
			else glClearColor(.1f, .5f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			quad.render(&camera3);
			glDisable(GL_SCISSOR_TEST);
			this->onRender();
			return true;
		}
	protected:
		void printPoints(int x, int y)
		{
			if(!selected_camera) return;
			axl::math::Vec2i screen_space(x, size.y-y);
			axl::math::Vec3f viewport_space = selected_camera->screenToViewport(screen_space);
			axl::math::Vec3f world_space = selected_camera->viewportToWorld(viewport_space);
			printf("ScreenSpace[%3d, %3d] -> ViewportSpace[%6.2f, %6.2f, %6.2f] -> WorldSpace[%6.2f, %6.2f, %6.2f]  \r",
				screen_space.x, screen_space.y,
				viewport_space.x, viewport_space.y, viewport_space.z,
				world_space.x, world_space.y, world_space.z
			);
		}
		bool onCreate(bool recreating)
		{
			if(!Test::MainView::onCreate(recreating)) return false;
			using namespace GL;
			// context.setVSync(true);
			dtime.set();
			etime.set();
			// projections
			projection1 = &orthographic_projection1;
			projection2 = &orthographic_projection2;
			projection3 = &orthographic_projection3;
			// quad_texture
			quad_texture.setContext(&context);
			quad_texture.create();
			if(image_alien.isValid())
			{
				quad_texture.allocate(0, image_alien.width(), image_alien.height(), GL_RGB8);
				quad_texture.setImage(0, 0, 0, image_alien.width(), image_alien.height(), GL_RGB, GL_UNSIGNED_BYTE, image_alien.cimage(), 1);
				quad_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				quad_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			// quad_program
			quad_program.setContext(&context);
			quad_program.create();
			// quad
			quad.setContext(&context);
			quad.setQuadProgram(&quad_program);
			quad.setSize(axl::math::Vec2f(1.f, 1.f));
			quad.transform.setTransformOrder(axl::math::Orders::Transform::STR);
			quad.transform.setRotationOrder(axl::math::Orders::Rotation::Y);
			quad.transform.setPosition(axl::math::Vec3f(-quad.size.x / 2.f, -quad.size.y / 2.f, 0.f));
			quad.color.set(.1f, .1f, 0.2f, 1.f);
			quad.texture = &quad_texture;
			quad.create();
			return true;
		}
		void onPointer(int index, int x, int y, bool down)
		{
			Test::MainView::onPointer(index, x, y, down);
			if(index == PI_LEFT_BUTTON)
			{
				if(down)
				{
					this->printPoints(x, y);
					this->capturePointer(true);
				}
				else this->capturePointer(false); 
			}
		}
		void onPointerMove(int index, int x, int y)
		{
			Test::MainView::onPointerMove(index, x, y);
			if(pointers[index])
			{
				if(index == PI_LEFT_BUTTON)
				{
					this->printPoints(x, y);
				}
			}
		}
		bool onKey(axl::gl::input::KeyCode key, bool down)
		{
			if(down)
			{
				switch(key)
				{
					case axl::gl::input::KeyCode::KEY_O:
						if(projection1 != &orthographic_projection1)
						{
							camera1.projection = projection1 = &orthographic_projection1;
							camera2.projection = projection2 = &orthographic_projection2;
							camera3.projection = projection3 = &orthographic_projection3;
							puts("\n> Projection method switched to Orthographic.");
						}
						return true;
					case axl::gl::input::KeyCode::KEY_P:
						if(projection1 != &perspective_projection1)
						{
							camera1.projection = projection1 = &perspective_projection1;
							camera2.projection = projection2 = &perspective_projection2;
							camera3.projection = projection3 = &perspective_projection3;
							puts("\n> Projection method switched to Perspective.");
						}
						return true;
					case axl::gl::input::KeyCode::KEY_1:
						if(selected_camera != &camera1)
						{
							selected_camera = &camera1;
							printf("\n> Camera 1 selected.\n");
						}
						return true;
					case axl::gl::input::KeyCode::KEY_2:
						if(selected_camera != &camera2)
						{
							selected_camera = &camera2;
							printf("\n> Camera 2 selected.\n");
						}
						return true;
					case axl::gl::input::KeyCode::KEY_3:
						if(selected_camera != &camera3)
						{
							selected_camera = &camera3;
							printf("\n> Camera 3 selected.\n");
						}
						return true;
				}
			}
			return Test::MainView::onKey(key, down);
		}
		void onSize(int w, int h)
		{
			Test::MainView::onSize(w, h);
			camera1.set(
				axl::math::Vec3f(0.f, 0.f, 2.5f),
				axl::math::Vec3f(0.f, 0.f, 0.f),
				0.f,
				axl::math::Vec3f::filled(1.f),
				axl::math::Vec2i(0,0),
				axl::math::Vec2i(w/2,h),
				projection1
			);
			float ratio1 = (float)camera1.viewport_size.x / camera1.viewport_size.y;
			perspective_projection1.set(.001f, 100.f, ratio1, 70.f);
			orthographic_projection1.set(
				-ratio1, ratio1, //(float)camera1.viewport_size.x,
				-1.f, 1.f, // (float)camera1.viewport_size.y,
				-100.f, 100.f);
			camera2.set(
				axl::math::Vec3f(0.f, 0.f, 2.5f),
				axl::math::Vec3f(0.f, 0.f, 0.f),
				0.f,
				axl::math::Vec3f::filled(1.f),
				axl::math::Vec2i(w/2,0),
				axl::math::Vec2i(w/2,h/2),
				projection2
			);
			float ratio2 = (float)camera2.viewport_size.x / camera2.viewport_size.y;
			perspective_projection2.set(.001f, 100.f, ratio2, 54.4f);
			orthographic_projection2.set(
				-ratio2, ratio2, //(float)camera2.viewport_size.x,
				-1.f, 1.f, //(float)camera2.viewport_size.y,
				-100.f, 100.f);
			camera3.set(
				axl::math::Vec3f(0.f, 0.f, 2.5f),
				axl::math::Vec3f(0.f, 0.f, 0.f),
				0.f,
				axl::math::Vec3f::filled(1.f),
				axl::math::Vec2i(w/2,h/2),
				axl::math::Vec2i(w/2,h/2),
				projection3
			);
			float ratio3 = (float)camera3.viewport_size.x / camera3.viewport_size.y;
			perspective_projection3.set(.001f, 100.f, ratio3, 30.f);
			orthographic_projection3.set(
				-ratio3, ratio3, //(float)camera3.viewport_size.x,
				-1.f, 1.f, //(float)camera3.viewport_size.y,
				-100.f, 100.f);
			this->update();
			if(this->render())
				this->swap();
		}
	private:
		axl::util::uc::Clock title_update_clock, update_clock;
		axl::util::uc::Time dtime, etime;
		axl::gl::projection::Orthographicf orthographic_projection1, orthographic_projection2, orthographic_projection3;
		axl::gl::projection::Perspectivef perspective_projection1, perspective_projection2, perspective_projection3;
		axl::gl::projection::Projectionf *projection1, *projection2, *projection3;
		axl::gl::camera::Camera3Df camera1, camera2, camera3;
		axl::gl::camera::Camera3Df *selected_camera = &camera1;
		Test::Quad::Program quad_program;
		Test::Quad quad;
		axl::gl::gfx::Texture2D quad_texture;
		axl::media::image::Image2D image_alien;
};

int main(int argc, char *argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	printf(">> axl.gl Viewports test -- axl.gl %s library %u.%u.%u\n", (axl::gl::lib::BUILD == axl::gl::lib::Build::SHARED ? "SHARED" : "STATIC"), axl::gl::lib::VERSION.major, axl::gl::lib::VERSION.minor, axl::gl::lib::VERSION.patch);
	puts("----------------------------------------");
	puts(" Press Keys '1','2' or '3' to select the corresponding camera.");
	puts(" Press Key 'P' and 'O' keys to switch to perspective and orthogographic camera respectively.");
	puts("----------------------------------------");
	{
		axl::gl::Display display;
		MainView main_view(L"axl.gl.Camera3D test", axl::math::Vec2i(0,0), axl::math::Vec2i(640,480));
		Assertve(display.isOpen(), verbose);
		Assertve(axl::gl::Application::init(), verbose);
		Assertve(axl::glfl::core::load(), verbose);
		Assertve(main_view.initialize(), verbose);
		Assertve(main_view.create(display, false, MainView::VF_RESIZABLE), verbose);
		Assertve(main_view.show(MainView::SM_SHOW), verbose);
		while(!axl::gl::Application::IS_QUITTING)
		{
			axl::gl::Application::pollEvents(display);
			main_view.update();
			if(main_view.render())
				main_view.swap();
		}
	}
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}
