#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/UI/Element.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

Element::Element(Element::Type type,
			axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2f& size,
			const axl::math::Vec4f& padding,
			axl::gl::gfx::ui::Layout::Size layout_width,
			axl::gl::gfx::ui::Layout::Size layout_height):
	axl::gl::gfx::ui::Component(axl::gl::gfx::ui::Component::ELEMENT, ptr_context, container, position, size, padding),
	element_type(type),
	element_layout_width(layout_width),
	element_layout_height(layout_height)
{
	this->setContainer(container);
}
Element::~Element()
{
	this->destroy();
}

bool Element::isValid() const
{
	return axl::gl::gfx::ui::Component::isValid();
}
axl::math::Vec2f Element::getContentSize() const
{
	return axl::math::Vec2f(0,0);
}
axl::gl::gfx::ui::Layout::Size Element::getLayoutWidth() const
{
	return element_layout_width;
}
axl::gl::gfx::ui::Layout::Size Element::getLayoutHeight() const
{
	return element_layout_height;
}
void Element::setLayoutWidth(axl::gl::gfx::ui::Layout::Size layout_width)
{
	element_layout_width = layout_width;
}
void Element::setLayoutHeight(axl::gl::gfx::ui::Layout::Size layout_height)
{
	element_layout_width = layout_height;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
