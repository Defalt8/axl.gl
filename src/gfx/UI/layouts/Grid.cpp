#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/UI/Element.hpp>
#include <axl.gl/gfx/UI/layouts/Grid.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/util.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace layouts {

Grid::Grid(Grid::Orientation orientation, Grid::FillMode fill_mode, const axl::math::Vec2f& spacing) :
	axl::gl::gfx::ui::Layout(),
	grid_orientation(orientation),
	grid_fill_mode(fill_mode),
	grid_spacing(spacing)
{}
Grid::~Grid()
{}
void Grid::organize(axl::gl::gfx::ui::Container& container)
{
	const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& container_children = container.getComponents();
	unsigned int child_count = container_children.count();
	if(child_count == 0)
		return;
	else if(child_count == 1)
	{
		axl::math::Vec4f container_padding = container.getPadding();
		axl::math::Vec2i container_size = container.getSize();
		axl::gl::gfx::ui::Component* component = (axl::gl::gfx::ui::Component*)(*container_children.first());
		component->transform.setPosition(axl::math::Vec3f(container_padding.x, container_padding.y, component->transform.getPosition().z));
		component->setSize(axl::math::Vec2i(
			container_size.x - (int)(container_padding.x + container_padding.z),
			container_size.y - (int)(container_padding.y + container_padding.w)
		));
	}
	else
	{
		axl::math::Vec4f container_padding = container.getPadding();
		axl::math::Vec2i container_size = container.getSize();
		axl::math::Vec2i client_size = axl::math::Vec2i(
			container_size.x - (int)(container_padding.x + container_padding.z),
			container_size.y - (int)(container_padding.y + container_padding.w)
		);
		float _sqrt = axl::math::sqrt((float)child_count);
		unsigned int column_count = (int)axl::math::ceil(_sqrt);
		unsigned int row_count = (column_count*column_count == child_count ? column_count : (int)_sqrt);
		axl::math::Vec2f cell_size(
			((float)((float)client_size.x - ((float)grid_spacing.x * (column_count - 1))) / column_count),
			((float)((float)client_size.y - ((float)grid_spacing.y * (row_count - 1))) / row_count)
		);
		if(this->grid_orientation == OR_VERTICAL)
		{
			unsigned int tmp = row_count;
			row_count = column_count;
			column_count = tmp;
		}
		unsigned int row_index = 0, column_index = 0, index = 0, last_index = child_count-1;
		float depth = 0.f;
		for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = container_children.first(); it.isNotNull(); ++it)
		{
			axl::gl::gfx::ui::Component* component = (axl::gl::gfx::ui::Component*)(*it);
			if(column_index >= column_count)
			{
				column_index = 0;
				++row_index;
			}
			switch(this->grid_orientation)
			{
				case OR_HORIZONTAL:
					component->transform.setPosition(axl::math::Vec3f(
						container_padding.x + column_index * (cell_size.x + grid_spacing.x),
						container_padding.y + row_index * (cell_size.y + grid_spacing.y),
						depth
					));
					if(index < last_index)
					{
						component->setSize(axl::math::Vec2i((int)cell_size.x, (int)cell_size.y));
					}
					else
					{
						switch(grid_fill_mode)
						{
							case KEEP_PATTERN:
								component->setSize(axl::math::Vec2i((int)cell_size.x, (int)cell_size.y));
								break;
							case STRECH_LAST:
								{
									component->setSize(axl::math::Vec2i(
										(int)((float)client_size.x - ((float)(cell_size.x + grid_spacing.x) * column_index)),
										(int)cell_size.y
									));
								}
								break;
						}
					}
					break;
				case OR_VERTICAL:
					component->transform.setPosition(axl::math::Vec3f(
						container_padding.x + row_index * ((float)cell_size.x + grid_spacing.x),
						container_padding.y + column_index * ((float)cell_size.y + grid_spacing.y),
						depth
					));
					if(index < last_index)
					{
						component->setSize(axl::math::Vec2i((int)cell_size.x, (int)cell_size.y));
					}
					else
					{
						switch(grid_fill_mode)
						{
							case KEEP_PATTERN:
								component->setSize(axl::math::Vec2i((int)cell_size.x, (int)cell_size.y));
								break;
							case STRECH_LAST:
								{
									component->setSize(axl::math::Vec2i(
										(int)cell_size.x,
										(int)((float)client_size.y - ((float)(cell_size.y + grid_spacing.y) * column_index))
									));
								}
								break;
						}
					}
					break;
			}
			depth += 0.0001f;
			++column_index;
			++index;
		}
	}
}
// set methods
void Grid::set(Orientation orientation, const axl::math::Vec2f& spacing)
{
	this->grid_orientation = orientation;
	this->grid_spacing = spacing;
}
void Grid::setOrientation(Grid::Orientation orientation)
{
	this->grid_orientation = orientation;
}
void Grid::setFillMode(Grid::FillMode fill_mode)
{
	this->grid_fill_mode = fill_mode;
}
void Grid::setSpacing(const axl::math::Vec2f& spacing)
{
	this->grid_spacing = spacing;
}
// get methods
Grid::Orientation Grid::getOrientation() const
{
	return this->grid_orientation;
}
Grid::FillMode Grid::getFillMode() const
{
	return this->grid_fill_mode;
}
const axl::math::Vec2f& Grid::getSpacing() const
{
	return this->grid_spacing;
}

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
