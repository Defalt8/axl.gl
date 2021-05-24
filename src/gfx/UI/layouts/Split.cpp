#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/UI/Element.hpp>
#include <axl.gl/gfx/UI/layouts/Split.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace layouts {

Split::Split(Split::Orientation orientation, float proportion, const axl::math::Vec2f& spacing) :
	axl::gl::gfx::ui::Layout(),
	split_orientation(orientation),
	split_proportion(.5f),
	split_spacing(spacing)
{
	this->setProportion(proportion);
}
Split::~Split()
{}
void Split::organize(axl::gl::gfx::ui::Container& container)
{
	const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& container_children = container.getComponents();
	unsigned int child_count = container_children.count();
	if(child_count == 0)
		return;
	axl::math::Vec4f container_padding = container.getPadding();
	axl::math::Vec2f container_size = container.getSize();
	axl::math::Vec2f client_size = axl::math::Vec2f(
		container_size.x - (container_padding.x + container_padding.z),
		container_size.y - (container_padding.y + container_padding.w)
	);
	axl::math::Vec3f new_positions[2];
	axl::math::Vec2f new_sizes[2];
	switch(this->split_orientation)
	{
		default:
		case OR_HORIZONTAL:
			{
				float actual_x = (client_size.x - split_spacing.x);
				new_sizes[0].set(
					actual_x * split_proportion,
					client_size.y
				);
				new_sizes[1].set(
					actual_x * (1.f - split_proportion),
					client_size.y
				);
				new_positions[0].set(container_padding.x, container_padding.y);
				new_positions[1].set((container_padding.x + new_sizes[0].x + split_spacing.x), container_padding.y);
			}
			break;
		case OR_VERTICAL:
			{
				float actual_y = (client_size.y - split_spacing.y);
				new_sizes[0].set(
					client_size.x,
					(actual_y * split_proportion)
				);
				new_sizes[1].set(
					client_size.x,
					(actual_y * (1.f - split_proportion))
				);
				new_positions[0].set(container_padding.x, container_padding.y);
				new_positions[1].set(container_padding.x, (container_padding.y + new_sizes[0].y + split_spacing.y));
			}
			break;
	}
	axl::math::Vec2f current_position(container_padding.x, container_padding.y);
	int index = 0;
	axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = container_children.first();
	float depth = 0.f;
	while(!it.isNull())
	{
		if(index >= 2)
			break;
		axl::gl::gfx::ui::Component* component = (axl::gl::gfx::ui::Component*)(*it);
		new_positions[index].z = depth;
		component->transform.setPosition(new_positions[index]);
		component->setSize(new_sizes[index]);
		depth += .0001f;
		++it;
		++index;
	}
}
// set methods
void Split::set(Orientation orientation, const axl::math::Vec2f& spacing)
{
	this->split_orientation = orientation;
	this->split_spacing = spacing;
}
void Split::setOrientation(Split::Orientation orientation)
{
	this->split_orientation = orientation;
}
bool Split::setProportion(float proportion)
{
	if(proportion < 0.f || proportion > 1.f)
		return false;
	this->split_proportion = proportion;
	return true;
}
void Split::setSpacing(const axl::math::Vec2f& spacing)
{
	this->split_spacing = spacing;
}
// get methods
Split::Orientation Split::getOrientation() const
{
	return this->split_orientation;
}
float Split::getProportion() const
{
	return this->split_proportion;
}
const axl::math::Vec2f& Split::getSpacing() const
{
	return this->split_spacing;
}

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
