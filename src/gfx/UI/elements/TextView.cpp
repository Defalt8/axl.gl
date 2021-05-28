#include <axl.gl/gfx/ui/elements/TextView.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace elements {

TextView::TextView(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2f& size,
			const axl::math::Vec4f& padding,
			axl::gl::gfx::ui::Layout::Size layout_width,
			axl::gl::gfx::ui::Layout::Size layout_height) :
	axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::ET_TEXT_VIEW, ptr_context, container, position, size, padding, layout_width, layout_height),
	text_view_horizontal_text_alignment(HAL_CENTER),
	text_view_vertical_text_alignment(VAL_CENTER),
	text_view_text(ptr_context)
{}
TextView::~TextView()
{
	this->destroy();
}

void TextView::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::ui::Element::setContext(ptr_context);
	text_view_text.setContext(ptr_context);
}
bool TextView::isValid() const
{
	return axl::gl::gfx::ui::Element::isValid() && text_view_text.isValid();
}
const axl::util::WString& TextView::getText() const
{
	return text_view_text.getText();
}
bool TextView::setText(const axl::util::WString& text)
{
	if(!text_view_text.setText(text)) return false;
	this->onModify();
	return true;
}
void TextView::setFont(const axl::gl::gfx::Font* font)
{
	text_view_text.setFont(font);
}
bool TextView::setTextProgram(const axl::gl::gfx::Text::Program* text_program)
{
	return text_view_text.setTextProgram(text_program);
}
bool TextView::setSize(const axl::math::Vec2f& size)
{
	if(!axl::gl::gfx::ui::Element::setSize(size))
		return false;
		this->onAlignmentChange();
	return true;
}

void TextView::setHorizontalAlignment(HorizontalTextAlignment horizontal_text_alignment)
{
	text_view_horizontal_text_alignment = horizontal_text_alignment;
	this->onAlignmentChange();
}
void TextView::setVerticalAlignment(VerticalTextAlignment vertical_text_alignment)
{
	text_view_vertical_text_alignment = vertical_text_alignment;
	this->onAlignmentChange();
}
void TextView::setForegroundColor(const axl::math::Vec4f& foreground_color)
{
	axl::gl::gfx::ui::Element::setForegroundColor(foreground_color);
	text_view_text.setColor(this->component_foreground_color);
}

TextView::HorizontalTextAlignment TextView::getHorizontalAlignment() const
{
	return text_view_horizontal_text_alignment;
}
TextView::VerticalTextAlignment TextView::getVerticalAlignment() const
{
	return text_view_vertical_text_alignment;
}

bool TextView::iCreate()
{
	if(!axl::gl::gfx::ui::Element::iCreate() || !text_view_text.create()) return false;
	return true;
}
bool TextView::iDestroy()
{
	return axl::gl::gfx::ui::Element::iDestroy() && text_view_text.destroy();
}
bool TextView::iRender(axl::gl::camera::Camera3Df* camera)
{
	return text_view_text.render(camera);
}
axl::math::Vec2f TextView::getContentSize() const
{
	const axl::math::Vec2f& text_box = text_view_text.getBox();
	return axl::math::Vec2f(text_box.x, text_box.y);
}

void TextView::onAlignmentChange()
{
	// if(!axl::gl::gfx::ui::elements::TextView::isValid())
	// 	return;
	axl::math::Vec2f text_view_size = text_view_text.getBox();
	axl::math::Vec3f position(0.f,0.f,0.f);
	switch(text_view_horizontal_text_alignment)
	{
		case HAL_CENTER:
			text_view_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER);
			position.x = (float)this->component_size.x / 2.f;
			break;
		case HAL_LEFT:
			text_view_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_LEFT);
			position.x = component_padding.x;
			break;
		case HAL_RIGHT:
			text_view_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_RIGHT);
			position.x = (float)this->component_size.x - component_padding.z;
			break;
	}
	switch(text_view_vertical_text_alignment)
	{
		case VAL_CENTER:
			text_view_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_VERTICAL_CENTER);
			position.y = (float)this->component_size.y / 2.f;
			break;
		case VAL_BOTTOM:
			text_view_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_BOTTOM);
			position.y = component_padding.y;
			break;
		case VAL_TOP:
			text_view_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_TOP);
			position.y = (float)this->component_size.y - component_padding.w;
			break;
	}
	text_view_text.text_transform.setPosition(position);
}

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
