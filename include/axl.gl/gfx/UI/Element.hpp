#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../../ContextObject.hpp"
#include "../../camera/Camera3D.hpp"
#include <axl.util/ds/List.hpp>
#include <axl.math/vec/Vec2f.hpp>
#include <axl.math/vec/Vec3f.hpp>
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Container;

class AXLGLCXXAPI Element : public axl::gl::gfx::ui::Component
{
	public:
		enum Type {
			OTHER,
			TEXT_VIEW,
			TEXT_INPUT,
			PUSH_BUTTON
		};
	public:
		Element(Element::Type type,
			axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(0.0f,0.0f),
			const axl::math::Vec4f& margin = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f));
		virtual ~Element();
	public:
		virtual bool render(axl::gl::camera::Camera3Df* camera = 0) = 0;
	public:
		const axl::gl::gfx::ui::Element::Type element_type;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
