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

class AXLGLCXXAPI Button : public axl::gl::gfx::ui::Element
{
	public:
		enum State {
			RELEASED = 0,
			PRESSED,
		};
	public:
		Button(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT);
		virtual ~Button();
	public:
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual bool isValid() const;
		State getState() const;
		bool isHovered() const;
		const axl::util::WString& getLabel() const;
		bool setLabel(const axl::util::WString& label);
		void setFont(const axl::gl::gfx::Font* font);
		void setProgram(const axl::gl::gfx::Program* program);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual void setForegroundColor(const axl::math::Vec4f& foreground_color);
		virtual void setReleaseColor(const axl::math::Vec4f& released_color);
		virtual void setPressedColor(const axl::math::Vec4f& pressed_color);
		virtual void setHoveredColor(const axl::math::Vec4f& hovered_color);
		virtual void setReleaseTextColor(const axl::math::Vec4f& released_text_color);
		virtual void setPressedTextColor(const axl::math::Vec4f& pressed_text_color);
		virtual void setHoveredTextColor(const axl::math::Vec4f& hovered_text_color);
	public:
		// When a pointer is on the button.
		virtual void onHover();
		virtual void onDrift();
		virtual void onPress(int pointer_index);
		virtual void onRelease(int pointer_index);
	protected:
		virtual bool icreate();
		virtual bool idestroy();
		virtual bool iRender(axl::gl::camera::Camera3Df* camera);
	protected:
		State button_state;
		bool button_hovered;
		axl::gl::gfx::Text button_text;
		axl::math::Vec4f button_released_color;
		axl::math::Vec4f button_released_text_color;
		axl::math::Vec4f button_pressed_color;
		axl::math::Vec4f button_pressed_text_color;
		axl::math::Vec4f button_hovered_color;
		axl::math::Vec4f button_hovered_text_color;
};

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
