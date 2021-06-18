#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/camera/Camera3D.hpp>
#include <axl.math/rules.hpp>
#include <axl.math/angle.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/basic.hpp>
#include <axl.math/util.hpp>
#include <axl.math/vec/Vec2d.hpp>
#include <axl.math/mat/transform4.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace axl {
namespace gl {
namespace camera {

//
// Camera3D
//

axl::gl::projection::Projection DefaultCamera3DProjection = axl::gl::projection::Projection();
axl::math::Rules::Axis DefaultCamera3DAxisRule = axl::math::Rules::Axis::RHS;

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
	view_matrix(axl::math::Mat4d::Identity)
{
	this->updateTransform();
}

bool Camera3D::makeCurrent(axl::gl::Context* context, bool set_viewport) const
{
	if(!context || !context->isValid() || !(context->isCurrent() || context->makeCurrent())) return false;
	using namespace axl::glfl::core::GL;
	GLCLEARERROR();
	if(set_viewport)
		glViewport((axl::glfl::GLint)this->viewport_position.x, (axl::glfl::GLint)this->viewport_position.y,
			(axl::glfl::GLsizei)(this->viewport_size.x < 0 ? context->view->size.x : this->viewport_size.x),
			(axl::glfl::GLsizei)(this->viewport_size.y < 0 ? context->view->size.y : this->viewport_size.y));
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
		case axl::math::Rules::Axis::RHS: angle.y = -(axl::math::atan2(-delta.x, -delta.z)); break;
		case axl::math::Rules::Axis::LHS: angle.y = -(axl::math::atan2(-delta.x, delta.z)); break;
	}
	angle.x = (axl::math::atan2(hyp, delta.y) - axl::math::Constants::D_HALF_PI);
	angle.z = -this->roll_angle;
	Camera3D::view_matrix = axl::math::Transform4::rotateZXY(angle, this->axis_rule) * axl::math::Transform4::translateScale(-this->position, this->scale);
}

void Camera3D::lookAt(const axl::math::Vec3d& p_position, const axl::math::Vec3d& p_target, double p_roll_angle)
{
	this->target = p_target;
	this->position = p_position;
	this->roll_angle = p_roll_angle;
	this->updateTransform();
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
	this->updateTransform();
}

axl::math::Vec3d Camera3D::screenToViewport(const axl::math::Vec2i& vec2) const
{
	return axl::math::Vec3d(
		((double)vec2.x - viewport_position.x) / (double)viewport_size.x * 2.f - 1.f,
		((double)vec2.y - viewport_position.y) / (double)viewport_size.y * 2.f - 1.f,
		0.f
	);
}

axl::math::Vec2i Camera3D::viewportToScreen(const axl::math::Vec3d& vec3) const
{
	return axl::math::Vec2i(
		(int)axl::math::round((double)(vec3.x + 1.f) / 2.f * viewport_size.x + (double)viewport_position.x),
		(int)axl::math::round((double)(vec3.y + 1.f) / 2.f * viewport_size.y + (double)viewport_position.y)
	);
}

axl::math::Vec3d Camera3D::viewportToWorld(const axl::math::Vec3d& vec3) const
{
	if(this->projection)
	{
		axl::math::Vec4d world_coord = ((this->projection->matrix * this->view_matrix).inverse() * axl::math::Vec4d(vec3, 1.));
		return axl::math::Vec3d(world_coord.x, world_coord.y, world_coord.z / world_coord.w);
	}
	else
		return (this->view_matrix.inverse() * axl::math::Vec4d(vec3, 1.)).toVec3(axl::math::Vec4d::XYZ);
}

axl::math::Vec3d Camera3D::worldToViewport(const axl::math::Vec3d& vec3) const
{
	if(this->projection)
	{
		axl::math::Vec4d viewport_coord = (this->projection->matrix * (this->view_matrix * axl::math::Vec4d(vec3, 1.)));
		return axl::math::Vec3d(viewport_coord.x, viewport_coord.y, viewport_coord.z);
	}
	else
		return (this->view_matrix * axl::math::Vec4d(vec3, 1.)).toVec3(axl::math::Vec4d::XYZ);
}

axl::math::Vec3d Camera3D::screenToWorld(const axl::math::Vec2i& vec2) const
{
	if(this->projection)
		return ((this->projection->matrix * this->view_matrix).inverse() * axl::math::Vec4d(this->screenToViewport(vec2), 1.)).toVec3(axl::math::Vec4d::XYZ);
	else
		return (this->view_matrix.inverse() * axl::math::Vec4d(this->screenToViewport(vec2), 1.)).toVec3(axl::math::Vec4d::XYZ);
}

axl::math::Vec2i Camera3D::worldToScreen(const axl::math::Vec3d& vec3) const
{
	if(this->projection)
		return this->viewportToScreen((this->projection->matrix * (this->view_matrix * axl::math::Vec4d(vec3, 1.f))).toVec3(axl::math::Vec4d::XYZ));
	else
		return this->viewportToScreen((this->view_matrix * axl::math::Vec4d(vec3, 1.f)).toVec3(axl::math::Vec4d::XYZ));
}

//
// Camera3Df
//

axl::gl::projection::Projectionf DefaultCamera3DfProjection = axl::gl::projection::Projectionf();
axl::math::Rules::Axis DefaultCamera3DfAxisRule = axl::math::Rules::Axis::RHS;

Camera3Df::Camera3Df(const axl::math::Vec3f& p_position,
				const axl::math::Vec3f& p_target,
				float p_roll_angle,
				const axl::math::Vec3f& p_scale,
				const axl::math::Vec2i& p_viewport_position,
				const axl::math::Vec2i& p_viewport_size,
				axl::gl::projection::Projectionf* p_projection,
				axl::math::Rules::Axis p_axis_rule) :
	axis_rule(p_axis_rule),
	position(p_position),
	target(p_target),
	roll_angle(p_roll_angle),
	scale(p_scale),
	viewport_position(p_viewport_position),
	viewport_size(p_viewport_size),
	projection(p_projection),
	view_matrix(axl::math::Mat4f::Identity)
{
	this->updateTransform();
}

bool Camera3Df::makeCurrent(axl::gl::Context* context, bool set_viewport) const
{
	if(!context || !context->isValid() || !(context->isCurrent() || context->makeCurrent())) return false;
	using namespace axl::glfl::core::GL;
	GLCLEARERROR();
	if(set_viewport)
	{
		axl::glfl::GLsizei viewport_width = (axl::glfl::GLsizei)(this->viewport_size.x < 0 ? context->view->size.x : this->viewport_size.x);
		axl::glfl::GLsizei viewport_height = (axl::glfl::GLsizei)(this->viewport_size.y < 0 ? context->view->size.y : this->viewport_size.y);
		glViewport((axl::glfl::GLint)this->viewport_position.x, (axl::glfl::GLint)this->viewport_position.y, viewport_width, viewport_height);
		glScissor((axl::glfl::GLint)this->viewport_position.x, (axl::glfl::GLint)this->viewport_position.y, viewport_width, viewport_height);
	}
	return glGetError() == GL_NO_ERROR;
}


void Camera3Df::updateTransform()
{
	axl::math::Vec3f delta = (this->target - this->position);
	axl::math::Vec3f angle;
	float hyp = axl::math::Vec2f(delta.x, delta.z).magn();
	switch (this->axis_rule)
	{
		default:
		case axl::math::Rules::Axis::RHS: angle.y = -(axl::math::atan2(-delta.x, -delta.z)); break;
		case axl::math::Rules::Axis::LHS: angle.y = -(axl::math::atan2(-delta.x, delta.z)); break;
	}
	angle.x = (axl::math::atan2(hyp, delta.y) - axl::math::Constants::F_HALF_PI);
	angle.z = -this->roll_angle;
	this->view_matrix = axl::math::Transform4::rotateZXY(angle, this->axis_rule) * axl::math::Transform4::scaleTranslate(this->scale, -this->position);
}

void Camera3Df::lookAt(const axl::math::Vec3f& p_position, const axl::math::Vec3f& p_target, float p_roll_angle)
{
	this->target = p_target;
	this->position = p_position;
	this->roll_angle = p_roll_angle;
	this->updateTransform();
}

void Camera3Df::set(const axl::math::Vec3f& p_position, const axl::math::Vec3f& p_target, float p_roll_angle, const axl::math::Vec3f& p_scale, const axl::math::Vec2i& p_viewport_position, const axl::math::Vec2i& p_viewport_size, axl::gl::projection::Projectionf* p_projection, axl::math::Rules::Axis p_axis_rule)
{
	this->position = p_position;
	this->target = p_target;
	this->roll_angle = p_roll_angle;
	this->scale = p_scale;
	this->viewport_position = p_viewport_position;
	this->viewport_size = p_viewport_size;
	this->projection = p_projection;
	this->axis_rule = p_axis_rule;
	this->updateTransform();
}

axl::math::Vec3f Camera3Df::screenToViewport(const axl::math::Vec2i& vec2) const
{
	return axl::math::Vec3f(
		((float)vec2.x - viewport_position.x) / (float)viewport_size.x * 2.f - 1.f,
		((float)vec2.y - viewport_position.y) / (float)viewport_size.y * 2.f - 1.f,
		0.f
	);
}

axl::math::Vec2i Camera3Df::viewportToScreen(const axl::math::Vec3f& vec3) const
{
	return axl::math::Vec2i(
		(int)axl::math::round((float)(vec3.x + 1.f) / 2.f * viewport_size.x + (float)viewport_position.x),
		(int)axl::math::round((float)(vec3.y + 1.f) / 2.f * viewport_size.y + (float)viewport_position.y)
	);
}

axl::math::Vec3f Camera3Df::viewportToWorld(const axl::math::Vec3f& vec3) const
{
	if(this->projection)
	{
		axl::math::Vec4f world_coord = ((this->projection->matrix * this->view_matrix).inverse() * axl::math::Vec4f(vec3, 1.f));
		return axl::math::Vec3f(world_coord.x, world_coord.y, world_coord.z / world_coord.w);
	}
	else
		return (this->view_matrix.inverse() * axl::math::Vec4f(vec3, 1.f)).toVec3(axl::math::Vec4f::XYZ);
}

axl::math::Vec3f Camera3Df::worldToViewport(const axl::math::Vec3f& vec3) const
{
	if(this->projection)
	{
		axl::math::Vec4f viewport_coord = (this->projection->matrix * (this->view_matrix * axl::math::Vec4f(vec3, 1.f)));
		return axl::math::Vec3f(viewport_coord.x, viewport_coord.y, viewport_coord.z);
	}
	else
		return (this->view_matrix * axl::math::Vec4f(vec3, 1.f)).toVec3(axl::math::Vec4f::XYZ);
}

axl::math::Vec3f Camera3Df::screenToWorld(const axl::math::Vec2i& vec2) const
{
	if(this->projection)
		return ((this->projection->matrix * this->view_matrix).inverse() * axl::math::Vec4f(this->screenToViewport(vec2), 1.f)).toVec3(axl::math::Vec4f::XYZ);
	else
		return (this->view_matrix.inverse() * axl::math::Vec4f(this->screenToViewport(vec2), 1.f)).toVec3(axl::math::Vec4f::XYZ);
}

axl::math::Vec2i Camera3Df::worldToScreen(const axl::math::Vec3f& vec3) const
{
	if(this->projection)
		return this->viewportToScreen((this->projection->matrix * (this->view_matrix * axl::math::Vec4f(vec3, 1.f))).toVec3(axl::math::Vec4f::XYZ));
	else
		return this->viewportToScreen((this->view_matrix * axl::math::Vec4f(vec3, 1.f)).toVec3(axl::math::Vec4f::XYZ));
}


} // namespace axl.gl.camera
} // namespace axl.gl
} // namespace axl
