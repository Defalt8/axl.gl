#pragma once
#include "../../../lib.hpp"
#include "../Element.hpp"
#include "../../text/Font.hpp"
#include "../../text/Text.hpp"
#include <axl.util/WString.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace elements {

class AXLGLCXXAPI Slider : public axl::gl::gfx::ui::Element
{
	public:
		enum Orientation {
			OR_HORIZONTAL,
			OR_VERTICAL
		};
		class AXLGLCXXAPI Program : public axl::gl::gfx::Program
		{
			public:
				Program(axl::gl::Context* ptr_context = 0);
				~Program();
				bool iCreate();
				bool iDestroy();
				axl::glfl::GLint getMVPLocation() const;
				axl::glfl::GLint getColorLocation() const;
			protected:
				axl::util::String vertex_shader_code, fragment_shader_code;
				axl::glfl::GLint uloc_mvp;
				axl::glfl::GLint uloc_color;
		};
	public:
		Slider(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::FIXED_SIZE,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::FIXED_SIZE);
		virtual ~Slider();
	public:
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual bool isValid() const;
		virtual bool setSize(const axl::math::Vec2f& size);
		bool setSliderProgram(axl::gl::gfx::ui::elements::Slider::Program* slider_program_ptr);
		bool setSliderOrientation(Orientation slider_orientation);
		bool setValue(float value);
		bool setMinValue(float min_value);
		bool setMaxValue(float max_value);
		Orientation getSliderOrientation() const;
		float getValue() const;
		float getMinValue() const;
		float getMaxValue() const;
		bool update();
	public:
		// When the knob is being moved.
		virtual void onSlide(float value);
	protected:
		virtual bool iCreate();
		virtual bool iDestroy();
		virtual bool iRender(axl::gl::camera::Camera3Df* camera);
		virtual axl::math::Vec2f getContentSize() const;
		virtual bool onSliderOrientationChange(Orientation slider_orientation);
	public:
		bool slider_is_sliding; // Do NOT modify
	protected:
		axl::gl::gfx::ui::elements::Slider::Program* slider_program_ptr;
		Orientation slider_orientation;
		float slider_value;
		float slider_min_value;
		float slider_max_value;
	private:
		axl::glfl::GLuint m_vertex_array_id;
		axl::glfl::GLuint m_vertex_buffer_id;
};

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
