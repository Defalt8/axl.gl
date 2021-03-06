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
		enum HorizontalTextAlignment {
			HAL_CENTER = 0,
			HAL_LEFT,
			HAL_RIGHT
		};
		enum VerticalTextAlignment {
			VAL_CENTER = 0,
			VAL_TOP,
			VAL_BOTTOM
		};
	public:
		Button(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT);
		virtual ~Button();
	public:
		virtual bool setContext(axl::gl::Context* ptr_context);
		virtual bool isValid() const;
		State getState() const;
		bool isHovered() const;
		const axl::util::WString& getText() const;
		bool setText(const axl::util::WString& label);
		void setFont(const axl::gl::gfx::Font* font);
		bool setTextProgram(const axl::gl::gfx::Text::Program* text_program);
		virtual bool setSize(const axl::math::Vec2f& size);
		virtual void setHorizontalAlignment(HorizontalTextAlignment horizontal_text_alignment);
		virtual void setVerticalAlignment(VerticalTextAlignment vertical_text_alignment);
		virtual void setForegroundColor(const axl::math::Vec4f& foreground_color);
		virtual void setReleasedColor(const axl::math::Vec4f& released_color);
		virtual void setPressedColor(const axl::math::Vec4f& pressed_color);
		virtual void setHoveredColor(const axl::math::Vec4f& hovered_color);
		virtual void setReleasedTextColor(const axl::math::Vec4f& released_text_color);
		virtual void setPressedTextColor(const axl::math::Vec4f& pressed_text_color);
		virtual void setHoveredTextColor(const axl::math::Vec4f& hovered_text_color);
		HorizontalTextAlignment getHorizontalAlignment() const;
		VerticalTextAlignment getVerticalAlignment() const;
		const axl::math::Vec4f& getReleasedColor() const;
		const axl::math::Vec4f& getPressedColor() const;
		const axl::math::Vec4f& getHoveredColor() const;
		const axl::math::Vec4f& getReleasedTextColor() const;
		const axl::math::Vec4f& getPressedTextColor() const;
		const axl::math::Vec4f& getHoveredTextColor() const;
	public:
		// When a pointer moves in the area of the button.
		virtual void onHover();
		// When a pointer moves out of the area of the button.
		virtual void onDrift();
		// When a pointer is pressed in the area of the button.
		virtual void onPress(int pointer_index);
		// When a pointer is released after the button was previously pressed.
		virtual void onRelease(int pointer_index);
	protected:
		virtual bool iCreate();
		virtual bool iDestroy();
		virtual bool iRender(axl::gl::camera::Camera3Df* camera);
		virtual axl::math::Vec2f getContentSize() const;
		virtual void onAlignmentChange();
	protected:
		State button_state;
		bool button_hovered;
		HorizontalTextAlignment button_horizontal_text_alignment;
		VerticalTextAlignment button_vertical_text_alignment;
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
