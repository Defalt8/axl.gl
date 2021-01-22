#pragma once
#include "../lib.hpp"

namespace axl {
namespace gl {
namespace input {

class AXLGLCXXAPI Touch 
{
	public:
		static bool isPresent();
	private:
		Touch();
		~Touch();
};

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl