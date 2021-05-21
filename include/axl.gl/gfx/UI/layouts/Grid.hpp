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

class AXLGLCXXAPI Grid : public axl::gl::gfx::ui::Layout
{
	public:
		enum Orientation {
			OR_HORIZONTAL,
			OR_VERTICAL
		};
		enum FillMode {
			STRECH_LAST,
			KEEP_PATTERN
		};
	public:
		Grid(Orientation orientation = OR_HORIZONTAL,
			FillMode fill_mode = KEEP_PATTERN,
			const axl::math::Vec2f& spacing = axl::math::Vec2f(0.0f,0.0f)
			);
		virtual ~Grid();
		virtual void organize(axl::gl::gfx::ui::Container& container);
		// set methods
		void set(Orientation orientation, const axl::math::Vec2f& spacing);
		void setOrientation(Orientation orientation);
		void setFillMode(FillMode fill_mode);
		void setSpacing(const axl::math::Vec2f& spacing);
		// get methods
		Orientation getOrientation() const;
		FillMode getFillMode() const;
		const axl::math::Vec2f& getSpacing() const;
	private:
		Orientation grid_orientation;
		FillMode grid_fill_mode;
		axl::math::Vec2f grid_spacing;
};

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
