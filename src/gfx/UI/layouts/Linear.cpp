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
	axl::math::Vec2i new_size(0,0), container_size = container.getSize();
	const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& container_children = container.getComponents();
	unsigned int child_count = container_children.count();
	axl::math::Vec2i client_size = axl::math::Vec2i(
			container_size.x - (int)(container_padding.x + container_padding.z),
			container_size.y - (int)(container_padding.y + container_padding.w)
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
	axl::math::Vec2i current_position((int)(container_padding.x), (int)(container_padding.y));
	axl::math::Vec2i element_size(
		(int)((float)(child_count == 0 ? 0.f : ((client_size.x - ((child_count-1) * lin_spacing.x)) / child_count))),
		(int)((float)(child_count == 0 ? 0.f : ((client_size.y - ((child_count-1) * lin_spacing.y)) / child_count)))
		);
	axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = container_children.first();
	while(!it.isNull())
	{
		switch(this->lin_orientation)
		{
			default:
			case OR_HORIZONTAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(element_size.x, client_size.y);
				current_position.x += new_size.x + (int)(lin_spacing.x);
				break;
			case OR_VERTICAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(client_size.x, element_size.y);
				current_position.y += new_size.y + (int)(lin_spacing.y);
				break;
		}
		(*it)->transform.setPosition(new_position);
		(*it)->setSize(new_size);
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
