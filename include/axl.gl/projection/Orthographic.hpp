#pragma once

#include "../lib.hpp"
#include "Projection.hpp"

namespace axl {
namespace gl {
namespace projection {

class AXLGLCXXAPI Orthographic : public Projection
{
	public:
		Orthographic(double left = -1.0, double right = 1.0, double bottom = -1.0, double top = 1.0, double zNear = -1.0, double zFar = 1.0);
		~Orthographic();
		void updateTransform();
		void set(double left, double right, double bottom, double top, double zNear, double zFar);
	public:
		const double& left;
		const double& right;
		const double& bottom;
		const double& top;
		const double& zNear;
		const double& zFar;
	private:
		double m_left;
		double m_right;
		double m_bottom;
		double m_top;
		double m_zNear;
		double m_zFar;
};

class AXLGLCXXAPI Orthographicf : public Projectionf
{
	public:
		Orthographicf(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f, float zNear = -1.0f, float zFar = 1.0f);
		~Orthographicf();
		void updateTransform();
		void set(float left, float right, float bottom, float top, float zNear, float zFar);
	public:
		const float& left;
		const float& right;
		const float& bottom;
		const float& top;
		const float& zNear;
		const float& zFar;
	private:
		float m_left;
		float m_right;
		float m_bottom;
		float m_top;
		float m_zNear;
		float m_zFar;
};

} // axl::gl::projection
} // axl::gl
} // axl
