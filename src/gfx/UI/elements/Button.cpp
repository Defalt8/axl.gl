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
const axl::util::WString& Button::getLabel() const
{
	return button_text.getText();
}
bool Button::setLabel(const axl::util::WString& label)
{
	if(!button_text.setText(label)) return false;
	this->component_is_modified = true;
	return true;
}
void Button::setFont(const axl::gl::gfx::Font* font)
{
	button_text.setFont(font);
}
void Button::setProgram(const axl::gl::gfx::Program* program)
{
	button_text.setProgram(program);
}
bool Button::setSize(const axl::math::Vec2i& size)
{
	if(!axl::gl::gfx::ui::Element::setSize(size))
		return false;
	button_text.text_transform.setPosition(axl::math::Vec3f((float)this->component_size.x / 2.f, (float)this->component_size.y / 2.f));
	return true;
}
void Button::setForegroundColor(const axl::math::Vec4f& foreground_color)
{
	axl::gl::gfx::ui::Element::setForegroundColor(foreground_color);
	button_text.setColor(this->component_foreground_color);
}
void Button::setReleaseColor(const axl::math::Vec4f& released_color)
{
	button_released_color = released_color;
	component_is_modified = true;
}
void Button::setPressedColor(const axl::math::Vec4f& pressed_color)
{
	button_pressed_color = pressed_color;
	component_is_modified = true;
}
void Button::setHoveredColor(const axl::math::Vec4f& hovered_color)
{
	button_hovered_color = hovered_color;
	component_is_modified = true;
}
void Button::setReleaseTextColor(const axl::math::Vec4f& released_text_color)
{
	button_released_text_color = released_text_color;
	component_is_modified = true;
}
void Button::setPressedTextColor(const axl::math::Vec4f& pressed_text_color)
{
	button_pressed_text_color = pressed_text_color;
	component_is_modified = true;
}
void Button::setHoveredTextColor(const axl::math::Vec4f& hovered_text_color)
{
	button_hovered_text_color = hovered_text_color;
	component_is_modified = true;
}

void Button::onHover()
{
	button_hovered = true;
	this->setBackgroundColor(button_hovered_color);
	this->setForegroundColor(button_hovered_text_color);
	this->component_is_modified = true;
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

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
