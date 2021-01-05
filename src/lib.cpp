
#include <axl.gl/lib.hpp>

namespace axl {
namespace gl {
namespace lib {

const Version VERSION = { AXLGL_VERSION_MAJOR, AXLGL_VERSION_MINOR, AXLGL_VERSION_PATCH };

#ifdef AXLGL_SHARED
const Build BUILD = Build::SHARED;
#else
const Build BUILD = Build::STATIC;
#endif

} // axl::gl::lib
} // axl::gl
} // axl