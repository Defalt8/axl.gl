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
		void calculate();
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

} // axl::gl::projection
} // axl::gl
} // axl
