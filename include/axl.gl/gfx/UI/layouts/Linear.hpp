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

class AXLGLCXXAPI Group;

class AXLGLCXXAPI Linear : public axl::gl::gfx::ui::Layout
{
	public:
		enum Orientation {
			OR_HORIZONTAL,
			OR_VERTICAL
		};
		enum Alignment {
			AL_CENTER = 0,
			AL_LEFT = 1,
			AL_RIGHT = 2,
			AL_BOTTOM = 1,
			AL_TOP = 2,
		};
	public:
		Linear(Orientation orientation = OR_HORIZONTAL,
			Alignment alignment = AL_CENTER,
			const axl::math::Vec2f& spacing = axl::math::Vec2f(0.0f,0.0f),
			const axl::math::Vec4f& padding = axl::math::Vec2f(0.0f,0.0f)
			);
		virtual ~Linear();
		virtual axl::math::Vec2i getMaxSize(const axl::gl::gfx::ui::Group& group, const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children) const;
		virtual void organize(axl::gl::gfx::ui::Group& group, axl::util::ds::UniList<axl::gl::gfx::UIElement*>& group_children);
		// set methods
		void set(Orientation orientation, Alignment alignment, const axl::math::Vec2f& spacing, const axl::math::Vec4f& padding);
		void setOrientation(Orientation orientation);
		void setAlignment(Alignment alignment);
		void setSpacing(const axl::math::Vec2f& spacing);
		void setPadding(const axl::math::Vec4f& padding);
		// get methods
		Orientation getOrientation() const;
		Alignment getAlignment() const;
		const axl::math::Vec2f& getSpacing() const;
		const axl::math::Vec4f& getPadding() const;
	private:
		Orientation lin_orientation;
		Alignment lin_alignment;
		axl::math::Vec2f lin_spacing;
		axl::math::Vec4f lin_padding;
};

} // axl.gl.gfx.ui.layouts
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
