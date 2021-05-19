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

class AXLGLCXXAPI TextInput : public axl::gl::gfx::ui::Element
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
		TextInput(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT);
		virtual ~TextInput();
	public:
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual bool isValid() const;
		const axl::util::WString& getText() const;
		bool setText(const axl::util::WString& label);
		void setFont(const axl::gl::gfx::Font* font);
		bool setTextProgram(const axl::gl::gfx::Text::Program* text_input_program);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual void setHorizontalAlignment(HorizontalTextAlignment horizontal_text_input_alignment);
		virtual void setVerticalAlignment(VerticalTextAlignment vertical_text_input_alignment);
		virtual void setForegroundColor(const axl::math::Vec4f& foreground_color);
		HorizontalTextAlignment getHorizontalAlignment() const;
		VerticalTextAlignment getVerticalAlignment() const;
	protected:
		virtual bool iCreate();
		virtual bool iDestroy();
		virtual bool iRender(axl::gl::camera::Camera3Df* camera);
		virtual axl::math::Vec2i getContentSize() const;
		virtual void onAlignmentChange();
		bool onInputFocusRequest() const;
		virtual void onInputFocusGain();
		virtual void onInputFocusLost();
	public:
		virtual void onChar(axl::util::WString::char_t char_code);
		bool setCursorIndex(axl::util::size_t index);
		axl::util::size_t getCursorIndex() const;
	private:
		void onCursorPositionChange();
	protected:
		HorizontalTextAlignment text_input_horizontal_text_alignment;
		VerticalTextAlignment text_input_vertical_text_alignment;
		axl::gl::gfx::Text text_input_text;
	private:
		bool m_draw_cursor;
		axl::util::size_t m_cursor_index;
		axl::gl::gfx::Text text_input_cursor;
};

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
