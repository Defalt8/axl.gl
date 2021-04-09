#pragma once
#include "../lib.hpp"
#include "../projections.hpp"
#include "../Context.hpp"
#include <axl.math/rules.hpp>
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec3d.hpp>
#include <axl.math/mat/Mat4d.hpp>

namespace axl {
namespace gl {
namespace camera {

class AXLGLCXXAPI Context;

AXLGLAPI axl::gl::projection::Projection DefaultCamera3DProjection;
AXLGLAPI axl::math::Rules::Axis DefaultCamera3DAxisRule;

class AXLGLCXXAPI Camera3D
{
	public:
		Camera3D(const axl::math::Vec3d& position = axl::math::Vec3d(0.0,0.0,1.0),
				const axl::math::Vec3d& target = axl::math::Vec3d(0.0,0.0,0.0),
				double roll_angle = 0.0,
				const axl::math::Vec3d& scale = axl::math::Vec3d(1.0,1.0,1.0),
				const axl::math::Vec2i& viewport_position = axl::math::Vec2i(0,0),
				const axl::math::Vec2i& viewport_size = axl::math::Vec2i(-1,-1),
				axl::gl::projection::Projection* projection = &DefaultCamera3DProjection,
				axl::math::Rules::Axis axis_rule = DefaultCamera3DAxisRule);
	public:
		bool makeCurrent(axl::gl::Context* context, bool set_viewport = true) const;
		void updateTransform();
		void lookAt(const axl::math::Vec3d& position, const axl::math::Vec3d& target, double roll_angle = 0.0);
		void set(const axl::math::Vec3d& position, const axl::math::Vec3d& target, double roll_angle, const axl::math::Vec3d& scale, const axl::math::Vec2i& viewport_position, const axl::math::Vec2i& viewport_size, axl::gl::projection::Projection* projection, axl::math::Rules::Axis axis_rule = DefaultCamera3DAxisRule);
		axl::math::Vec3d screenToWorld(const axl::math::Vec2i& vec2) const;
		axl::math::Vec2i worldToScreen(const axl::math::Vec3d& vec3) const;
	public:
		axl::math::Rules::Axis axis_rule;
		axl::math::Vec3d position;
		axl::math::Vec3d target;
		double roll_angle;
		axl::math::Vec3d scale;
		axl::math::Vec2i viewport_position;
		axl::math::Vec2i viewport_size;
		axl::gl::projection::Projection* projection;
		axl::math::Mat4d view_transform;
};

AXLGLAPI axl::gl::projection::Projectionf DefaultCamera3DfProjection;
AXLGLAPI axl::math::Rules::Axis DefaultCamera3DfAxisRule;

class AXLGLCXXAPI Camera3Df
{
	public:
		Camera3Df(const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,1.0f),
				const axl::math::Vec3f& target = axl::math::Vec3f(0.0f,0.0f,0.0f),
				float roll_angle = 0.0f,
				const axl::math::Vec3f& scale = axl::math::Vec3f(1.0f,1.0f,1.0f),
				const axl::math::Vec2i& viewport_position = axl::math::Vec2i(0,0),
				const axl::math::Vec2i& viewport_size = axl::math::Vec2i(-1,-1),
				axl::gl::projection::Projectionf* projection = &DefaultCamera3DfProjection,
				axl::math::Rules::Axis axis_rule = DefaultCamera3DfAxisRule);
	public:
		bool makeCurrent(axl::gl::Context* context, bool set_viewport = true) const;
		void updateTransform();
		void lookAt(const axl::math::Vec3f& position, const axl::math::Vec3f& target, float roll_angle = 0.0f);
		void set(const axl::math::Vec3f& position, const axl::math::Vec3f& target, float roll_angle, const axl::math::Vec3f& scale, const axl::math::Vec2i& viewport_position, const axl::math::Vec2i& viewport_size, axl::gl::projection::Projectionf* projection, axl::math::Rules::Axis axis_rule = DefaultCamera3DfAxisRule);
		axl::math::Vec3f screenToWorld(const axl::math::Vec2i& vec2) const;
		axl::math::Vec2i worldToScreen(const axl::math::Vec3f& vec3) const;
	public:
		axl::math::Rules::Axis axis_rule;
		axl::math::Vec3f position;
		axl::math::Vec3f target;
		float roll_angle;
		axl::math::Vec3f scale;
		axl::math::Vec2i viewport_position;
		axl::math::Vec2i viewport_size;
		axl::gl::projection::Projectionf* projection;
		axl::math::Mat4f view_transform;
};


} // namespace axl.gl.camera
} // namespace axl.gl
} // namespace axl
