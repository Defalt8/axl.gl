#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/UI/Element.hpp>
#include <axl.gl/gfx/UI/layouts/Linear.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace layouts {

Linear::Linear(Linear::Orientation orientation, const axl::math::Vec2f& spacing) :
	axl::gl::gfx::ui::Layout(),
	lin_orientation(orientation),
	lin_spacing(spacing)
{}
Linear::~Linear()
{}
void Linear::organize(axl::gl::gfx::ui::Container& container)
{
	axl::math::Vec3f new_position(0.0f,0.0f);
	axl::math::Vec4f container_padding = container.getPadding();
	axl::math::Vec2f new_size(0,0), container_size = container.getSize();
	const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& container_children = container.getComponents();
	unsigned int child_count = container_children.count();
	axl::math::Vec2f total_padding((container_padding.x + container_padding.z) , (container_padding.y + container_padding.w));
	axl::math::Vec2f client_size(
		(container_size.x <= total_padding.x ? 0.0f : (container_size.x - total_padding.x)),
		(container_size.y <= total_padding.y ? 0.0f : (container_size.y - total_padding.y))
	);
	switch(this->lin_orientation)
	{
		default:
		case OR_HORIZONTAL:
			new_size.y = client_size.y;
			break;
		case OR_VERTICAL:
			new_size.x = client_size.x;
			break;
	}
	axl::math::Vec2f current_position(container_padding.x, container_padding.y);
	axl::math::Vec2f element_size(
		(child_count == 0 ? 0.f : ((float)((client_size.x <= (float)(child_count-1) * lin_spacing.x) ? 0.0f : (client_size.x - ((float)(child_count-1) * lin_spacing.x))) / (float)child_count)),
		(child_count == 0 ? 0.f : ((float)((client_size.y <= (float)(child_count-1) * lin_spacing.y) ? 0.0f : (client_size.y - ((float)(child_count-1) * lin_spacing.y))) / (float)child_count))
	);
	float depth = 0.f;
	axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = container_children.first();
	while(!it.isNull())
	{
		switch(this->lin_orientation)
		{
			default:
			case OR_HORIZONTAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(element_size.x, client_size.y);
				current_position.x += new_size.x + (lin_spacing.x);
				break;
			case OR_VERTICAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(client_size.x, element_size.y);
				current_position.y += new_size.y + (lin_spacing.y);
				break;
		}
		axl::gl::gfx::ui::Component* component = (axl::gl::gfx::ui::Component*)(*it);
		new_position.z = depth;
		component->transform.setPosition(new_position);
		component->setSize(new_size);
		depth += 0.0001f;
		++it;
	}
}
// set methods
void Linear::set(Orientation orientation, const axl::math::Vec2f& spacing)
{
	this->lin_orientation = orientation;
	this->lin_spacing = spacing;
}
void Linear::setOrientation(Linear::Orientation orientation)
{
	this->lin_orientation = orientation;
}
void Linear::setSpacing(const axl::math::Vec2f& spacing)
{
	this->lin_spacing = spacing;
}
// get methods
Linear::Orientation Linear::getOrientation() const
{
	return this->lin_orientation;
}
const axl::math::Vec2f& Linear::getSpacing() const
{
	return this->lin_spacing;
}

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
