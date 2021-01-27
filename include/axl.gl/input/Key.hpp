#pragma once
#include "../lib.hpp"
#include "KeyCodes.hpp"
#include "Keyboard.hpp"

namespace axl {
namespace gl {
namespace input {

class AXLGLCXXAPI Key
{
public:
	Key(axl::gl::input::KeyCode key_code);
	bool isDown() const;
	bool isUp() const;
	bool isPressed();
public:
	axl::gl::input::KeyCode m_key_code;
	bool m_locked;
};

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl
