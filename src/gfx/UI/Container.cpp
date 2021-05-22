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
			const axl::math::Vec4f& padding):
	Component(Component::CONTAINER, ptr_context, container, position, size, padding),
	m_components(),
	m_layout(0)
{}
Container::~Container()
{
	this->destroy();
}

bool Container::isValid() const
{
	return axl::gl::gfx::ui::Component::isValid();
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
	if(m_layout)
		m_layout->organize(*(Container*)(this));
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
}
Layout* Container::getLayout() const
{
	return m_layout;
}
void Container::setLayout(Layout* layout)
{
	m_layout = layout;
}

void Container::onViewSize(int w, int h)
{}

bool Container::iRender(axl::gl::camera::Camera3Df* camera)
{
	for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = m_components.first(); it.isNotNull(); ++it)
	{
		axl::gl::gfx::ui::Component* component = *it;
		component->render(camera, &component_render_texture, &component_depth_texture);
	}
	return true;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
