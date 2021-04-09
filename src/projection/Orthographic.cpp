#include <axl.math/angle.hpp>
#include <axl.math/basic.hpp>
#include <axl.gl/projection/Projection.hpp>
#include <axl.gl/projection/Orthographic.hpp>

namespace axl {
namespace gl {
namespace projection {

//
// Orthographic
//

Orthographic::Orthographic(double p_left, double p_right, double p_bottom, double p_top, double p_zNear, double p_zFar) :
	Projection(Projection::Type::PT_ORTHOGRAPHIC, axl::math::Mat4d::Identity),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	zNear(m_zNear),
	zFar(m_zFar),
	m_left(p_left),
	m_right(p_right),
	m_bottom(p_bottom),
	m_top(p_top),
	m_zNear(p_zNear),
	m_zFar(p_zFar)
{
	this->updateTransform();
}

void Orthographic::updateTransform()
{
	Projection::projection_matrix.set(
		(2.0 / (m_right - m_left)), 0.0, 0.0, 0.0,
		0.0, (2.0 / (m_top - m_bottom)), 0.0, 0.0,
		0.0, 0.0, (-2.0 / (m_zFar - m_zNear)), 0.0,
		(-(m_right + m_left) / (m_right - m_left)), (-(m_top + m_bottom) / (m_top - m_bottom)), (-(m_zFar + m_zNear) / (m_zFar - m_zNear)), 1.0
	);
}

void Orthographic::set(double p_left, double p_right, double p_bottom, double p_top, double p_zNear, double p_zFar)
{
	m_left = p_left;
	m_right = p_right;
	m_bottom = p_bottom;
	m_top = p_top;
	m_zNear = p_zNear;
	m_zFar = p_zFar;
	this->updateTransform();
}

//
// Orthographicf
//

Orthographicf::Orthographicf(float p_left, float p_right, float p_bottom, float p_top, float p_zNear, float p_zFar) :
	Projectionf(Projectionf::Type::PT_ORTHOGRAPHIC, axl::math::Mat4f::Identity),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	zNear(m_zNear),
	zFar(m_zFar),
	m_left(p_left),
	m_right(p_right),
	m_bottom(p_bottom),
	m_top(p_top),
	m_zNear(p_zNear),
	m_zFar(p_zFar)
{
	this->updateTransform();
}

void Orthographicf::updateTransform()
{
	Projectionf::projection_matrix.set(
		(2.0f / (m_right - m_left)), 0.0f, 0.0f, 0.0f,
		0.0f, (2.0f / (m_top - m_bottom)), 0.0f, 0.0f,
		0.0f, 0.0f, (-2.0f / (m_zFar - m_zNear)), 0.0f,
		(-(m_right + m_left) / (m_right - m_left)), (-(m_top + m_bottom) / (m_top - m_bottom)), (-(m_zFar + m_zNear) / (m_zFar - m_zNear)), 1.0f
	);
}

void Orthographicf::set(float p_left, float p_right, float p_bottom, float p_top, float p_zNear, float p_zFar)
{
	m_left = p_left;
	m_right = p_right;
	m_bottom = p_bottom;
	m_top = p_top;
	m_zNear = p_zNear;
	m_zFar = p_zFar;
	this->updateTransform();
}

} // axl::gl::projection
} // axl::gl
} // axl
