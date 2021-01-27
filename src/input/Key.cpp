#include <axl.gl/input/Key.hpp>

namespace axl {
namespace gl {
namespace input {

Key::Key(axl::gl::input::KeyCode key_code) :
	m_key_code(key_code),
	m_locked(false)
{}

bool Key::isDown() const
{
	return axl::gl::input::Keyboard::isKeyDown(this->m_key_code);
}

bool Key::isUp() const
{
	return !axl::gl::input::Keyboard::isKeyDown(this->m_key_code);
}

bool Key::isPressed()
{
	if(axl::gl::input::Keyboard::isKeyDown(this->m_key_code))
	{
		if(!this->m_locked)
		{
			return this->m_locked = true;
		}
	}
	else if(this->m_locked)
		this->m_locked = false;
	return false;
}

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl
