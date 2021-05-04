#pragma once
#include "../../lib.hpp"
#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.util/ds/List.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Container;

class AXLGLCXXAPI Layout
{
	public:
		Layout();
		virtual ~Layout();
		virtual void organize(axl::gl::gfx::ui::Container& container, axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& components) = 0;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
