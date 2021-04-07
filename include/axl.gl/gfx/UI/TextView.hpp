#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../shader/Program.hpp"
#include "../text.hpp"
#include "UIFrameBuffer.hpp"
#include "UIElement.hpp"
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI TextView : public axl::gl::gfx::UIElement, public axl::gl::gfx::Text
{
	public:
		TextView(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::Font* ptr_font = 0,
			axl::gl::gfx::Program* ptr_program = 0,
			axl::math::Vec2i size = axl::math::Vec2i(100,100)
			);
		~TextView();
		bool isValid() const;
		void setContext(axl::gl::Context* ptr_context);
		bool icreate();
		bool idestroy();
	protected:
		void irender(const axl::gl::camera::Camera3Df* camera);
	public:
		void setBackgroundColor(const axl::math::Vec4f& color);
		const axl::math::Vec4f& getBackgroundColor() const;
	protected:
		axl::math::Vec4f txtv_background_color;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
