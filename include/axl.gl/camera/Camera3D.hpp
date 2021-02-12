#pragma once
#include "../lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec3d.hpp>
#include <axl.math/mat/Mat4d.hpp>
#include "../projections.hpp"

namespace axl {
namespace gl {
namespace camera {

class AXLGLCXXAPI Context;

class AXLGLCXXAPI Camera3D
{
	public:
		Camera3D(const axl::math::Vec3d& position = axl::math::Vec3d(0.0,0.0,1.0),
				const axl::math::Vec3d& target = axl::math::Vec3d(0.0,0.0,0.0),
				double roll_angle = 0.0,
				const axl::math::Vec3d& scale = axl::math::Vec3d(1.0,1.0,1.0),
				const axl::math::Vec2i& viewport_position = axl::math::Vec2i(0,0),
				const axl::math::Vec2i& viewport_size = axl::math::Vec2i(-1,-1),
				axl::gl::projection::Projection* projection = &Camera3D::DefaultProjection,
				axl::math::Rules::Axis axis_rule = axl::math::Rules::DefaultAxisRule);
		virtual ~Camera3D();
	public:
		bool makeCurrent(axl::gl::Context* context, bool set_viewport = true) const;
		void updateTransform();
		void lookAt(const axl::math::Vec3d& position, const axl::math::Vec3d& target, double roll_angle = 0.0);
		void set(const axl::math::Vec3d& position, const axl::math::Vec3d& target, double roll_angle, const axl::math::Vec3d& scale, const axl::math::Vec2i& viewport_position, const axl::math::Vec2i& viewport_size, axl::gl::projection::Projection* projection, axl::math::Rules::Axis axis_rule = axl::math::Rules::DefaultAxisRule);
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
	public:
		static axl::gl::projection::Projection DefaultProjection;
};


class AXLGLCXXAPI Camera3Df
{
	public:
		Camera3Df(const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,1.0f),
				const axl::math::Vec3f& target = axl::math::Vec3f(0.0f,0.0f,0.0f),
				float roll_angle = 0.0f,
				const axl::math::Vec3f& scale = axl::math::Vec3f(1.0f,1.0f,1.0f),
				const axl::math::Vec2i& viewport_position = axl::math::Vec2i(0,0),
				const axl::math::Vec2i& viewport_size = axl::math::Vec2i(-1,-1),
				axl::gl::projection::Projectionf* projection = &Camera3Df::DefaultProjection,
				axl::math::Rules::Axis axis_rule = axl::math::Rules::DefaultAxisRule);
		virtual ~Camera3Df();
	public:
		bool makeCurrent(axl::gl::Context* context, bool set_viewport = true) const;
		void updateTransform();
		void lookAt(const axl::math::Vec3f& position, const axl::math::Vec3f& target, float roll_angle = 0.0f);
		void set(const axl::math::Vec3f& position, const axl::math::Vec3f& target, float roll_angle, const axl::math::Vec3f& scale, const axl::math::Vec2i& viewport_position, const axl::math::Vec2i& viewport_size, axl::gl::projection::Projectionf* projection, axl::math::Rules::Axis axis_rule = axl::math::Rules::DefaultAxisRule);
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
	public:
		static axl::gl::projection::Projectionf DefaultProjection;
};


} // namespace axl.gl.camera
} // namespace axl.gl
} // namespace axl
