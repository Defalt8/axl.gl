#pragma once

#include "../lib.hpp"
#include "Projection.hpp"

namespace axl {
namespace gl {
namespace projection {

class AXLGLCXXAPI Perspective : public Projection
{
	public:
		Perspective(double zNear = 0.001, double zFar = 1000.0, double aspect_ratio = 1.0, double fov = 70.0);
		~Perspective();
		void updateTransform();
		void set(double zNear, double zFar, double aspect_ratio, double fov);
	public:
		const double& left;
		const double& right;
		const double& bottom;
		const double& top;
		const double& zNear;
		const double& zFar;
		const double& aspect_ratio;
		const double& fov;
	private:
		double m_left;
		double m_right;
		double m_bottom;
		double m_top;
		double m_zNear;
		double m_zFar;
		double m_aspect_ratio;
		double m_fov;
};

} // axl::gl::projection
} // axl::gl
} // axl
