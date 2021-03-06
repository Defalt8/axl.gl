#include <axl.math/angle.hpp>
#include <axl.math/basic.hpp>
#include <axl.gl/projection/Projection.hpp>

namespace axl {
namespace gl {
namespace projection {

//
// Projection
//

Projection::Projection(Projection::Type p_type, axl::math::Mat4d p_matrix) :
	type(m_type),
	matrix(projection_matrix),
	m_type(p_type),
	projection_matrix(p_matrix)
{}

void Projection::updateTransform()
{
	Projection::projection_matrix = axl::math::Mat4d::Identity;
}

//
// Projectionf
//

Projectionf::Projectionf(Projectionf::Type p_type, axl::math::Mat4f p_matrix) :
	type(m_type),
	matrix(projection_matrix),
	m_type(p_type),
	projection_matrix(p_matrix)
{}

void Projectionf::updateTransform()
{
	Projectionf::projection_matrix = axl::math::Mat4f::Identity;
}

} // axl::gl::projection
} // axl::gl
} // axl
