#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

Component::Component(Type type,
		axl::gl::Context* ptr_context,
		axl::gl::gfx::ui::Container* container,
		const axl::math::Vec3f& position,
		const axl::math::Vec2f& size,
		const axl::math::Vec4f& margin,
		const axl::math::Vec4f& padding) :
	axl::gl::ContextObject(ptr_context),
	component_type(type),
	component_position(position),
	component_size(size),
	component_margin(margin),
	component_padding(padding)
{
	this->setContainer(container);
}
Component::~Component()
{
	this->destroy();
}

bool Component::setContainer(axl::gl::gfx::ui::Container* container)
{
	return container->addComponent(this);
}
void Component::setVisiblity(bool is_visible)
{
	component_is_visible = is_visible;
}
void Component::setPosition(const axl::math::Vec3f& position)
{
	component_position = position;
}
void Component::setSize(const axl::math::Vec2f& size)
{
	component_size = size;
}
void Component::setMargin(const axl::math::Vec4f& margin)
{
	component_margin = margin;
}
void Component::setPadding(const axl::math::Vec4f& padding)
{
	component_padding = padding;
}
axl::gl::gfx::ui::Container* Component::getContainer() const
{
	return component_container;
}
bool Component::isVisible() const
{
	return component_is_visible;
}
const axl::math::Vec3f& Component::getPosition() const
{
	return component_position;
}
const axl::math::Vec2f& Component::getSize() const
{
	return component_size;
}
const axl::math::Vec4f& Component::getMargin() const
{
	return component_margin;
}
const axl::math::Vec4f& Component::getPadding() const
{
	return component_padding;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl

