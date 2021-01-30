#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/texture/Texture.hpp>

namespace axl {
namespace gl {
namespace gfx {

//////////////
// Texture
//

Texture::Texture(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	txr_id(-1)
{}

Texture::~Texture()
{}

axl::glfl::GLuint Texture::getId() const
{
	return this->txr_id;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
