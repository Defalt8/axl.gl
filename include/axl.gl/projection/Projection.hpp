#pragma once

#include "../lib.hpp"
#include <axl.math/mat/mat4f.hpp>
#include <axl.math/mat/mat4d.hpp>

namespace axl {
namespace gl {
namespace projection {

class AXLGLCXXAPI Projection
{
	public:
		AXLGL_ENUM_CLASS Type { PT_IDENTITY, PT_ORTHOGRAPHIC, PT_PERSPECTIVE, PT_OTHER };
	public:
		Projection(Projection::Type type = Projection::Type::PT_IDENTITY, axl::math::Mat4d matrix = axl::math::Mat4d::Identity);
		virtual ~Projection();
		virtual void calculate();
	public:
		const Type& type;
		const axl::math::Mat4d& matrix;
	private:
		Type m_type;
	protected:
		axl::math::Mat4d projection_matrix;
};

} // axl::gl::projection
} // axl::gl
} // axl
