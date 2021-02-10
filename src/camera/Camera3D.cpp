#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/camera/Camera3D.hpp>
#include <axl.math/angle.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/vec/Vec2d.hpp>
#include <axl.math/mat/transform4.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace axl {
namespace gl {
namespace camera {

axl::gl::projection::Projection Camera3D::DefaultProjection = axl::gl::projection::Projection();

Camera3D::Camera3D(const axl::math::Vec3d& p_position,
				const axl::math::Vec3d& p_target,
				double p_roll_angle,
				const axl::math::Vec3d& p_scale,
				const axl::math::Vec2i& p_viewport_position,
				const axl::math::Vec2i& p_viewport_size,
				axl::gl::projection::Projection* p_projection,
				axl::math::Rules::Axis p_axis_rule) :
	axis_rule(p_axis_rule),
	position(p_position),
	target(p_target),
	roll_angle(p_roll_angle),
	scale(p_scale),
	viewport_position(p_viewport_position),
	viewport_size(p_viewport_size),
	projection(p_projection),
	view_transform(axl::math::Mat4d::Identity)
{}

Camera3D::~Camera3D()
{}

bool Camera3D::makeCurrent(axl::gl::Context* context, bool set_viewport) const
{
	if(!context || !context->isValid() || !context->makeCurrent()) return false;
	using namespace axl::glfl::core::GL;
	GLCLEARERROR();
	if(set_viewport)
		glViewport((axl::glfl::GLint)Camera3D::viewport_position.x, (axl::glfl::GLint)Camera3D::viewport_position.y,
			(axl::glfl::GLsizei)(Camera3D::viewport_size.x < 0 ? context->view->size.x : Camera3D::viewport_size.x),
			(axl::glfl::GLsizei)(Camera3D::viewport_size.y < 0 ? context->view->size.y : Camera3D::viewport_size.y));
	return glGetError() == GL_NO_ERROR;
}


void Camera3D::updateTransform()
{
	axl::math::Vec3d delta = (this->target - this->position);
	axl::math::Vec3d angle;
	double hyp = axl::math::Vec2d(delta.x, delta.z).magn();
	switch (this->axis_rule)
	{
		default:
		case axl::math::Rules::Axis::AXIS_RHS: angle.y = -(axl::math::atan2(-delta.x, -delta.z)); break;
		case axl::math::Rules::Axis::AXIS_LHS: angle.y = -(axl::math::atan2(-delta.x, delta.z)); break;
	}
	angle.x = (axl::math::atan2(hyp, delta.y) - axl::math::Constants::D_HALF_PI);
	angle.z = -this->roll_angle;
	Camera3D::view_transform = axl::math::Transform4::rotateZXY(angle, this->axis_rule) * axl::math::Transform4::scaleTranslate(this->scale, -this->position);
}

void Camera3D::lookAt(const axl::math::Vec3d& p_position, const axl::math::Vec3d& p_target, double p_roll_angle)
{
	Camera3D::target = p_target;
	Camera3D::position = p_position;
	Camera3D::roll_angle = p_roll_angle;
	Camera3D::updateTransform();
}

void Camera3D::set(const axl::math::Vec3d& p_position, const axl::math::Vec3d& p_target, double p_roll_angle, const axl::math::Vec3d& p_scale, const axl::math::Vec2i& p_viewport_position, const axl::math::Vec2i& p_viewport_size, axl::gl::projection::Projection* p_projection, axl::math::Rules::Axis p_axis_rule)
{
	this->position = p_position;
	this->target = p_target;
	this->roll_angle = p_roll_angle;
	this->scale = p_scale;
	this->viewport_position = p_viewport_position;
	this->viewport_size = p_viewport_size;
	this->projection = p_projection;
	this->axis_rule = p_axis_rule;
}


} // namespace axl.gl.camera
} // namespace axl.gl
} // namespace axl
