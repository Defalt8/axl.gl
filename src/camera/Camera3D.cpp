#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/camera/Camera3D.hpp>
#include <axl.math/rules.hpp>
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
	if(!context || !context->isValid() || !context->makeCurrent()) return false;
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

axl::math::Vec3d Camera3D::screenToWorld(const axl::math::Vec2i& vec2) const
{
	axl::math::Vec3d world_coord(
		((double)(vec2.x - this->viewport_position.x) / this->viewport_size.x),
		((double)((double)this->viewport_size.y - (vec2.y - this->viewport_position.y)) / this->viewport_size.y),
		0.0);
	if(this->projection)
	{
		switch(this->projection->type)
		{
			case axl::gl::projection::Projection::Type::PT_ORTHOGRAPHIC:
				{
					axl::gl::projection::Orthographic* op = (axl::gl::projection::Orthographic*)this->projection;
					world_coord.x = op->left + world_coord.x * (op->right - op->left);
					world_coord.y = op->bottom + world_coord.y * (op->top - op->bottom);
				}
				break;
			case axl::gl::projection::Projection::Type::PT_PERSPECTIVE:
				{
					axl::gl::projection::Perspective* pp = (axl::gl::projection::Perspective*)this->projection;
					world_coord.x = pp->left + world_coord.x * (pp->right - pp->left);
					world_coord.y = pp->bottom + world_coord.y * (pp->top - pp->bottom);
				}
				break;
			case axl::gl::projection::Projection::Type::PT_IDENTITY:
			case axl::gl::projection::Projection::Type::PT_OTHER:
			default:
				break;
		}
	}
	world_coord = this->view_matrix.affineInvert(world_coord);
	world_coord.z = 0.0;
	return world_coord;
}

axl::math::Vec2i Camera3D::worldToScreen(const axl::math::Vec3d& vec3) const
{
	axl::math::Vec4d world_coord;
	if(this->projection)
		world_coord = this->view_matrix * this->projection->matrix * axl::math::Vec4d(vec3.x, vec3.y, vec3.z, 1.0);
	else
		world_coord = this->view_matrix * axl::math::Vec4d(vec3.x, vec3.y, vec3.z, 1.0);
	return axl::math::Vec2i((int)world_coord.x, (int)world_coord.y);
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
	if(!context || !context->isValid() || !context->makeCurrent()) return false;
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

axl::math::Vec3f Camera3Df::screenToWorld(const axl::math::Vec2i& vec2) const
{
	axl::math::Vec3f world_coord(
		((float)(vec2.x - this->viewport_position.x) / this->viewport_size.x),
		((float)((float)this->viewport_size.y - (vec2.y - this->viewport_position.y)) / this->viewport_size.y),
		0.0f);
	if(this->projection)
	{
		switch(this->projection->type)
		{
			case axl::gl::projection::Projectionf::Type::PT_ORTHOGRAPHIC:
				{
					axl::gl::projection::Orthographicf* op = (axl::gl::projection::Orthographicf*)this->projection;
					world_coord.x = op->left + world_coord.x * (op->right - op->left);
					world_coord.y = op->bottom + world_coord.y * (op->top - op->bottom);
				}
				break;
			case axl::gl::projection::Projectionf::Type::PT_PERSPECTIVE:
				{
					axl::gl::projection::Perspectivef* pp = (axl::gl::projection::Perspectivef*)this->projection;
					world_coord.x = pp->left + world_coord.x * (pp->right - pp->left);
					world_coord.y = pp->bottom + world_coord.y * (pp->top - pp->bottom);
				}
				break;
			case axl::gl::projection::Projectionf::Type::PT_IDENTITY:
			case axl::gl::projection::Projectionf::Type::PT_OTHER:
			default:
				break;
		}
	}
	world_coord = this->view_matrix.affineInvert(world_coord);
	world_coord.z = 0.0f;
	return world_coord;
}

axl::math::Vec2i Camera3Df::worldToScreen(const axl::math::Vec3f& vec3) const
{
	axl::math::Vec4f world_coord;
	if(this->projection)
		world_coord = this->view_matrix * this->projection->matrix * axl::math::Vec4f(vec3.x, vec3.y, vec3.z, 1.0f);
	else
		world_coord = this->view_matrix * axl::math::Vec4f(vec3.x, vec3.y, vec3.z, 1.0f);
	return axl::math::Vec2i((int)world_coord.x, (int)world_coord.y);
}


} // namespace axl.gl.camera
} // namespace axl.gl
} // namespace axl
