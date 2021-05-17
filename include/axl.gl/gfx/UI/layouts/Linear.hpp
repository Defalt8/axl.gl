#pragma once
#include "../../../lib.hpp"
#include "../Layout.hpp"
#include <axl.math/vec/Vec2f.hpp>
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace layouts {

class AXLGLCXXAPI Linear : public axl::gl::gfx::ui::Layout
{
	public:
		enum Orientation {
			OR_HORIZONTAL,
			OR_VERTICAL
		};
	public:
		Linear(Orientation orientation = OR_HORIZONTAL,
			const axl::math::Vec2f& spacing = axl::math::Vec2f(0.0f,0.0f)
			);
		virtual ~Linear();
		virtual void organize(axl::gl::gfx::ui::Container& container);
		// set methods
		void set(Orientation orientation, const axl::math::Vec2f& spacing);
		void setOrientation(Orientation orientation);
		void setSpacing(const axl::math::Vec2f& spacing);
		// get methods
		Orientation getOrientation() const;
		const axl::math::Vec2f& getSpacing() const;
	private:
		Orientation lin_orientation;
		axl::math::Vec2f lin_spacing;
};

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
