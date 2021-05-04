#include <axl.gl/gfx/UI/Group.hpp>
#include <axl.gl/gfx/UI/layouts/Linear.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace layouts {

Linear::Linear(Linear::Orientation orientation, Linear::Alignment alignment, const axl::math::Vec2f& spacing, const axl::math::Vec4f& padding) :
	axl::gl::gfx::ui::Layout(),
	lin_orientation(orientation),
	lin_alignment(alignment),
	lin_spacing(spacing),
	lin_padding(padding)
{}
Linear::~Linear()
{}
axl::math::Vec2i Linear::getMaxSize(const axl::gl::gfx::ui::Group& group, const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children) const
{
	axl::math::Vec4f group_border_size = group.getBorderSize();
	axl::math::Vec2i max_size(
			(int)(group_border_size.x + group_border_size.z),
			(int)(group_border_size.y + group_border_size.w)
			);
	axl::math::Vec2i elem_size, max_elem_size(0,0);
	axl::math::Vec4f group_padding = group.getPadding();
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = group_children.first();
	unsigned int child_count = group_children.count();
	while(!it.isNull())
	{
		elem_size = (*it)->getMaxSize();
		switch(this->lin_orientation)
		{
			default:
			case OR_HORIZONTAL:
				if(elem_size.y > max_elem_size.y)
					max_elem_size.y = elem_size.y;
				max_size.x += elem_size.x;
				break;
			case OR_VERTICAL:
				if(elem_size.x > max_elem_size.x)
					max_elem_size.x = elem_size.x;
				max_size.y += elem_size.y;
				break;
		}
		++it;
	}
	switch(this->lin_orientation)
	{
		default:
		case OR_HORIZONTAL:
			max_size.x += (int)((float)(child_count > 1 ? ((child_count - 1) * lin_spacing.x) : 0) + (child_count * (lin_padding.x + lin_padding.z)) + group_padding.x + group_padding.z);
			max_size.y += max_elem_size.y;
			break;
		case OR_VERTICAL:
			max_size.x += max_elem_size.x;
			max_size.y += (int)((float)(child_count > 1 ? ((child_count - 1) * lin_spacing.y) : 0) + (child_count * (lin_padding.y + lin_padding.w)) + group_padding.y + group_padding.w);
			break;
	}
	return max_size;
}
void Linear::organize(axl::gl::gfx::ui::Group& group, axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children)
{
	axl::math::Vec3f new_position(0.0f,0.0f);
	axl::math::Vec4f _border_size = group.getBorderSize(), group_padding = group.getPadding();
	axl::math::Vec2i new_size(0,0), group_size = group.getSize(), border_size((int)(_border_size.x + _border_size.z), (int)(_border_size.y + _border_size.w));
	unsigned int child_count = group_children.count();
	axl::math::Vec2i client_size = axl::math::Vec2i(
			group_size.x - (int)(group_padding.x + group_padding.z) - (int)(child_count * (lin_padding.x + lin_padding.z)) - border_size.x,
			group_size.y - (int)(group_padding.y + group_padding.w) - (int)(child_count * (lin_padding.y + lin_padding.w)) - border_size.y
			);
	switch(this->lin_orientation)
	{
		default:
		case OR_HORIZONTAL:
			client_size.x -= ((int)(child_count <= 1 ? 0 : lin_spacing.x * (child_count - 1)));
			break;
		case OR_VERTICAL:
			client_size.y -= ((int)(child_count <= 1 ? 0 : lin_spacing.y * (child_count - 1)));
			break;
	}
	axl::math::Vec2i elem_size, current_position(0,0);
	elem_size = child_count == 0 ? axl::math::Vec2i(0,0) : (client_size / child_count);
	switch(this->lin_orientation)
	{
		default:
		case OR_HORIZONTAL:
			// elem_size.x -= (int)(lin_spacing.x);
			current_position.set((int)(group_padding.x + lin_padding.x + _border_size.x), (int)(group_padding.y + lin_padding.y + _border_size.y));
			break;
		case OR_VERTICAL:
			// elem_size.y -= (int)(lin_spacing.y);
			current_position.set((int)(group_padding.x + lin_padding.x + _border_size.x), group_size.y - (child_count == 0 ? 0 : (elem_size.y)) - (int)(group_padding.w + lin_padding.w + _border_size.w));
			break;
	}
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = group_children.first();
	while(!it.isNull())
	{
		switch(this->lin_orientation)
		{
			default:
			case OR_HORIZONTAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(elem_size.x, client_size.y);
				current_position.x += new_size.x + (int)(lin_spacing.x + lin_padding.x + lin_padding.z);
				break;
			case OR_VERTICAL:
				new_position.set((float)current_position.x, (float)current_position.y);
				new_size.set(client_size.x, elem_size.y);
				current_position.y -= new_size.y + (int)(lin_spacing.y + lin_padding.y + lin_padding.w);
				break;
		}
		(*it)->transform.setPosition(new_position);
		(*it)->setSize(new_size);
		++it;
	}
}
// set methods
void Linear::set(Orientation orientation, Alignment alignment, const axl::math::Vec2f& spacing, const axl::math::Vec4f& padding)
{
	this->lin_orientation = orientation;
	this->lin_alignment = alignment;
	this->lin_spacing = spacing;
	this->lin_padding = padding;
}
void Linear::setOrientation(Linear::Orientation orientation)
{
	this->lin_orientation = orientation;
}
void Linear::setAlignment(Linear::Alignment alignment)
{
	this->lin_alignment = alignment;
}
void Linear::setSpacing(const axl::math::Vec2f& spacing)
{
	this->lin_spacing = spacing;
}
void Linear::setPadding(const axl::math::Vec4f& padding)
{
	this->lin_padding = padding;
}
// get methods
Linear::Orientation Linear::getOrientation() const
{
	return this->lin_orientation;
}
Linear::Alignment Linear::getAlignment() const
{
	return this->lin_alignment;
}
const axl::math::Vec2f& Linear::getSpacing() const
{
	return this->lin_spacing;
}
const axl::math::Vec4f& Linear::getPadding() const
{
	return this->lin_padding;
}

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
