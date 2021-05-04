#pragma once
#include "../../lib.hpp"
#include <axl.gl/gfx/UI/UIElement.hpp>
#include <axl.util/ds/List.hpp>
#include <axl.math/vec/Vec2i.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Group;

class AXLGLCXXAPI Layout
{
	public:
		Layout();
		virtual ~Layout();
		virtual axl::math::Vec2i getMaxSize(const axl::gl::gfx::ui::Group& group, const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children) const = 0;
		virtual void organize(axl::gl::gfx::ui::Group& group, axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children) = 0;
		
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
