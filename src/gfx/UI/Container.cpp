#include <axl.glfl/glCoreARB.hpp>
#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/UI/Element.hpp>

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

Container::Container(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			Layout* layout,
			const axl::math::Vec3f& position,
			const axl::math::Vec2i& size,
			const axl::math::Vec4f& margin,
			const axl::math::Vec4f& padding):
	Component(Component::CONTAINER, ptr_context, container, position, size, margin, padding),
	m_components()
{}
Container::~Container()
{
	this->destroy();
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
void Container::organize() const
{
	for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = m_components.first(); it.isNotNull(); ++it)
	{
		axl::gl::gfx::ui::Component* component = *it;
		switch(component->component_type)
		{
			case axl::gl::gfx::ui::Component::CONTAINER:
				((axl::gl::gfx::ui::Container*)component)->organize();
				break;
			case axl::gl::gfx::ui::Component::ELEMENT:
				break;
		}
	}
	m_layout->organize(*(Container*)(this));
}

bool Container::iRender(axl::gl::camera::Camera3Df* camera)
{
	for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = m_components.first(); it.isNotNull(); ++it)
	{
		(*it)->render(camera, &m_framebuffer);
	}
	return true;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
