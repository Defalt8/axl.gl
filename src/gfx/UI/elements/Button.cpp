#include <axl.gl/gfx/ui/elements/Button.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace elements {

Button::Button(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2i& size,
			const axl::math::Vec4f& padding,
			axl::gl::gfx::ui::Layout::Size layout_width,
			axl::gl::gfx::ui::Layout::Size layout_height) :
	axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::BUTTON, ptr_context, container, position, size, padding, layout_width, layout_height),
	button_state(Button::RELEASED),
	button_hovered(false),
	button_horizontal_text_alignment(HAL_CENTER),
	button_vertical_text_alignment(VAL_CENTER),
	button_text(ptr_context),
	button_released_color(.95f,.95f,.95f,1.f),
	button_released_text_color(.1f,.1f,.1f,1.f),
	button_pressed_color(.1f,.1f,.1f,1.f),
	button_pressed_text_color(.98f,.98f,.98f,1.f),
	button_hovered_color(1.0f,1.0f,1.0f,1.f),
	button_hovered_text_color(.1f,.1f,.1f,1.f)
{}
Button::~Button()
{
	this->destroy();
}

void Button::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::ui::Element::setContext(ptr_context);
	button_text.setContext(ptr_context);
}
bool Button::isValid() const
{
	return axl::gl::gfx::ui::Element::isValid() && button_text.isValid();
}
Button::State Button::getState() const
{
	return button_state;
}
bool Button::isHovered() const
{
	return button_hovered;
}
const axl::util::WString& Button::getText() const
{
	return button_text.getText();
}
bool Button::setText(const axl::util::WString& label)
{
	if(!button_text.setText(label)) return false;
	this->onModify();
	return true;
}
void Button::setFont(const axl::gl::gfx::Font* font)
{
	button_text.setFont(font);
}
bool Button::setTextProgram(const axl::gl::gfx::Text::Program* text_program)
{
	return button_text.setTextProgram(text_program);
}
bool Button::setSize(const axl::math::Vec2i& size)
{
	if(!axl::gl::gfx::ui::Element::setSize(size))
		return false;
		this->onAlignmentChange();
	return true;
}

void Button::setHorizontalAlignment(HorizontalTextAlignment horizontal_text_alignment)
{
	button_horizontal_text_alignment = horizontal_text_alignment;
	this->onAlignmentChange();
}
void Button::setVerticalAlignment(VerticalTextAlignment vertical_text_alignment)
{
	button_vertical_text_alignment = vertical_text_alignment;
	this->onAlignmentChange();
}
void Button::setForegroundColor(const axl::math::Vec4f& foreground_color)
{
	axl::gl::gfx::ui::Element::setForegroundColor(foreground_color);
	button_text.setColor(this->component_foreground_color);
}
void Button::setReleasedColor(const axl::math::Vec4f& released_color)
{
	button_released_color = released_color;
	this->onModify();
}
void Button::setPressedColor(const axl::math::Vec4f& pressed_color)
{
	button_pressed_color = pressed_color;
	this->onModify();
}
void Button::setHoveredColor(const axl::math::Vec4f& hovered_color)
{
	button_hovered_color = hovered_color;
	this->onModify();
}
void Button::setReleasedTextColor(const axl::math::Vec4f& released_text_color)
{
	button_released_text_color = released_text_color;
	this->onModify();
}
void Button::setPressedTextColor(const axl::math::Vec4f& pressed_text_color)
{
	button_pressed_text_color = pressed_text_color;
	this->onModify();
}
void Button::setHoveredTextColor(const axl::math::Vec4f& hovered_text_color)
{
	button_hovered_text_color = hovered_text_color;
	this->onModify();
}

Button::HorizontalTextAlignment Button::getHorizontalAlignment() const
{
	return button_horizontal_text_alignment;
}
Button::VerticalTextAlignment Button::getVerticalAlignment() const
{
	return button_vertical_text_alignment;
}
const axl::math::Vec4f& Button::getReleasedColor() const
{
	return button_released_color;
}
const axl::math::Vec4f& Button::getPressedColor() const
{
	return button_pressed_color;
}
const axl::math::Vec4f& Button::getHoveredColor() const
{
	return button_hovered_color;
}
const axl::math::Vec4f& Button::getReleasedTextColor() const
{
	return button_released_text_color;
}
const axl::math::Vec4f& Button::getPressedTextColor() const
{
	return button_pressed_text_color;
}
const axl::math::Vec4f& Button::getHoveredTextColor() const
{
	return button_hovered_text_color;
}

void Button::onHover()
{
	button_hovered = true;
	this->setBackgroundColor(button_hovered_color);
	this->setForegroundColor(button_hovered_text_color);
}
void Button::onDrift()
{
	button_hovered = false;
	this->setBackgroundColor(button_released_color);
	this->setForegroundColor(button_released_text_color);
}
void Button::onPress(int pointer_index)
{
	button_state = PRESSED;
	this->setBackgroundColor(button_pressed_color);
	this->setForegroundColor(button_pressed_text_color);
}
void Button::onRelease(int pointer_index)
{
	button_state = RELEASED;
	this->setBackgroundColor(button_hovered_color);
	this->setForegroundColor(button_hovered_text_color);
}

bool Button::iCreate()
{
	if(!axl::gl::gfx::ui::Element::iCreate() || !button_text.create()) return false;
	this->setBackgroundColor(button_released_color);
	this->setForegroundColor(button_released_text_color);
	return true;
}
bool Button::iDestroy()
{
	return axl::gl::gfx::ui::Element::iDestroy() && button_text.destroy();
}
bool Button::iRender(axl::gl::camera::Camera3Df* camera)
{
	return button_text.render(camera);
}
axl::math::Vec2i Button::getContentSize() const
{
	const axl::math::Vec2f& text_box = button_text.getBox();
	return axl::math::Vec2i((int)text_box.x, (int)text_box.y);
}

void Button::onAlignmentChange()
{
	// if(!axl::gl::gfx::ui::elements::Button::isValid())
	// 	return;
	axl::math::Vec2f text_size = button_text.getBox();
	axl::math::Vec3f position(0.f,0.f,0.f);
	switch(button_horizontal_text_alignment)
	{
		case HAL_CENTER:
			button_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER);
			position.x = (float)this->component_size.x / 2.f;
			break;
		case HAL_LEFT:
			button_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_LEFT);
			position.x = component_padding.x;
			break;
		case HAL_RIGHT:
			button_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_RIGHT);
			position.x = (float)this->component_size.x - component_padding.z;
			break;
	}
	switch(button_vertical_text_alignment)
	{
		case VAL_CENTER:
			button_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_VERTICAL_CENTER);
			position.y = (float)this->component_size.y / 2.f;
			break;
		case VAL_BOTTOM:
			button_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_BOTTOM);
			position.y = component_padding.y;
			break;
		case VAL_TOP:
			button_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_TOP);
			position.y = (float)this->component_size.y - component_padding.w;
			break;
	}
	button_text.text_transform.setPosition(position);
}

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
