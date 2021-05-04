#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

Container::Container(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2f& size,
			const axl::math::Vec4f& margin,
			const axl::math::Vec4f& padding):
	Component(Component::CONTAINER, ptr_context, container, position, size, margin, padding),
	m_components()
{}
Container::~Container()
{
	this->destroy();
}

bool Container::render(axl::gl::camera::Camera3Df* camera)
{
	if(!component_is_visible) return false;
	return true;
}
bool Container::addComponent(axl::gl::gfx::ui::Component* component)
{
	if(m_components.positionOf(component).isNotNull()) return false;
	if(m_components.insertLast(component))
	{
		component->component_container = this;
		return true;
	}
	return false;
}
bool Container::removeComponent(axl::gl::gfx::ui::Component* component)
{
	return m_components.remove(component);
}
bool Container::containsComponent(axl::gl::gfx::ui::Component* component)
{
	return m_components.positionOf(component).isNotNull();
}
const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& Container::getComponents() const
{
	return m_components;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
