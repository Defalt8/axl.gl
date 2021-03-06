#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/texture/Texture.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {
namespace gfx {

//////////////
// Texture
//

Texture::Texture(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	txr_id(0)
{}

Texture::~Texture()
{}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
