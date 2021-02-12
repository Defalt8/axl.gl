#include <axl.math/angle.hpp>
#include <axl.math/basic.hpp>
#include <axl.gl/projection/Projection.hpp>
#include <axl.gl/projection/Perspective.hpp>

namespace axl {
namespace gl {
namespace projection {

//
// Perspective
//

Perspective::Perspective(double p_zNear, double p_zFar, double p_aspect_ratio, double p_fov) :
	Projection(Projection::Type::PT_PERSPECTIVE, axl::math::Mat4d::Identity),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	zNear(m_zNear),
	zFar(m_zFar),
	aspect_ratio(m_aspect_ratio),
	fov(m_fov),
	m_left(-1.0),
	m_right(1.0),
	m_bottom(-1.0),
	m_top(1.0),
	m_zNear(p_zNear),
	m_zFar(p_zFar),
	m_aspect_ratio(p_aspect_ratio),
	m_fov(aspect_ratio)
{}

Perspective::~Perspective()
{}

void Perspective::updateTransform()
{
	double d = m_zNear * axl::math::tan(axl::math::Angle::degToRad(m_fov) / 2.0);
	m_left = -d * m_aspect_ratio;
	m_right = d * m_aspect_ratio;
	m_bottom = -d;
	m_top = d;
	Projection::projection_matrix.set(
		(2.0 * m_zNear / (m_right - m_left)), 0.0, 0.0, 0.0,
		0.0, (2.0 * m_zNear / (m_top - m_bottom)), 0.0, 0.0,
		((m_right + m_left) / (m_right - m_left)), ((m_top + m_bottom) / (m_top - m_bottom)), (-(m_zFar + m_zNear) / (m_zFar - m_zNear)), -1.0,
		0.0, 0.0, (-(2.0 * m_zFar * m_zNear) / (m_zFar - m_zNear)), 0.0
	);
}

void Perspective::set(double p_zNear, double p_zFar, double p_aspect_ratio, double p_fov)
{
	m_zNear = p_zNear;
	m_zFar = p_zFar;
	m_aspect_ratio = p_aspect_ratio;
	m_fov = p_fov;
}


//
// Perspectivef
//

Perspectivef::Perspectivef(float p_zNear, float p_zFar, float p_aspect_ratio, float p_fov) :
	Projectionf(Projectionf::Type::PT_PERSPECTIVE, axl::math::Mat4f::Identity),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	zNear(m_zNear),
	zFar(m_zFar),
	aspect_ratio(m_aspect_ratio),
	fov(m_fov),
	m_left(-1.0f),
	m_right(1.0f),
	m_bottom(-1.0f),
	m_top(1.0f),
	m_zNear(p_zNear),
	m_zFar(p_zFar),
	m_aspect_ratio(p_aspect_ratio),
	m_fov(aspect_ratio)
{}

Perspectivef::~Perspectivef()
{}

void Perspectivef::updateTransform()
{
	float d = m_zNear * axl::math::tan(axl::math::Angle::degToRad(m_fov) / 2.0f);
	m_left = -d * m_aspect_ratio;
	m_right = d * m_aspect_ratio;
	m_bottom = -d;
	m_top = d;
	Projectionf::projection_matrix.set(
		(2.0f * m_zNear / (m_right - m_left)), 0.0f, 0.0f, 0.0f,
		0.0f, (2.0f * m_zNear / (m_top - m_bottom)), 0.0f, 0.0f,
		((m_right + m_left) / (m_right - m_left)), ((m_top + m_bottom) / (m_top - m_bottom)), (-(m_zFar + m_zNear) / (m_zFar - m_zNear)), -1.0f,
		0.0f, 0.0f, (-(2.0f * m_zFar * m_zNear) / (m_zFar - m_zNear)), 0.0f
	);
}

void Perspectivef::set(float p_zNear, float p_zFar, float p_aspect_ratio, float p_fov)
{
	m_zNear = p_zNear;
	m_zFar = p_zFar;
	m_aspect_ratio = p_aspect_ratio;
	m_fov = p_fov;
}

} // axl::gl::projection
} // axl::gl
} // axl
