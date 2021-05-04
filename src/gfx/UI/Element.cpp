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
			const axl::math::Vec4f& margin,
			const axl::math::Vec4f& padding):
	axl::gl::gfx::ui::Component(axl::gl::gfx::ui::Component::ELEMENT, ptr_context, container, position, size, margin, padding),
	element_type(type)
{
	this->setContainer(container);
}
Element::~Element()
{
	this->destroy();
}

bool Element::render(axl::gl::camera::Camera3Df* camera)
{
	if(!component_is_visible) return false;
	return true;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
