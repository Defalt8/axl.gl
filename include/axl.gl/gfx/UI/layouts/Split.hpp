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

// A proportional split layout for only for organizing two components.
class AXLGLCXXAPI Split : public axl::gl::gfx::ui::Layout
{
	public:
		enum Orientation {
			OR_HORIZONTAL,
			OR_VERTICAL
		};
	public:
		Split(Orientation orientation = OR_HORIZONTAL,
			float proportion = 0.5f,
			const axl::math::Vec2f& spacing = axl::math::Vec2f(0.0f,0.0f)
			);
		virtual ~Split();
		virtual void organize(axl::gl::gfx::ui::Container& container);
		// set methods
		void set(Orientation orientation, const axl::math::Vec2f& spacing);
		void setOrientation(Orientation orientation);
		bool setProportion(float proportion);
		void setSpacing(const axl::math::Vec2f& spacing);
		// get methods
		Orientation getOrientation() const;
		float getProportion() const;
		const axl::math::Vec2f& getSpacing() const;
	private:
		Orientation split_orientation;
		float split_proportion;
		axl::math::Vec2f split_spacing;
};

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
