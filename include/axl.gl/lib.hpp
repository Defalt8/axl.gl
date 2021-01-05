#pragma once

#ifndef AXLGL_VERSION_MAJOR
#	define AXLGL_VERSION_MAJOR 0
#endif
#ifndef AXLGL_VERSION_MINOR
#	define AXLGL_VERSION_MINOR 0
#endif
#ifndef AXLGL_VERSION_PATCH
#define AXLGL_VERSION_PATCH 0
#endif

#ifdef AXLGL_SHARED
#	if defined(WIN32)
#		if defined(AXLGL_BUILD)
#			define AXLGLAPI extern __declspec(dllexport)
#			define AXLGLCXXAPI __declspec(dllexport)
#		else
#			define AXLGLAPI extern __declspec(dllimport)
#			define AXLGLCXXAPI __declspec(dllexport)
#		endif
#	elif defined(__GNUC__) && __GNUC__ > 4
#		define AXLGLAPI extern __attribute__((visibility("default")))
#		define AXLGLCXXAPI __attribute__((visibility("default")))
#	else
#   	error("Don't know how to export shared object libraries")
#		define AXLGLAPI extern
#		define AXLGLCXXAPI
#	endif
#elif defined(AXLGL_STATIC)
#	define AXLGLAPI extern
#	define AXLGLCXXAPI
#else
#	define AXLGL_STATIC
#	define AXLGLAPI extern
#	define AXLGLCXXAPI
#endif

#if __cplusplus >= 201103L
#	define AXLGL_CONSTMODIFIER constexpr
#	define AXLGL_ENUM_CLASS enum class
#else
#	define AXLGL_CONSTMODIFIER const static
#	define AXLGL_ENUM_CLASS enum
#endif

namespace axl {
namespace gl {
namespace lib {

struct _Version {
	unsigned int major;
	unsigned int minor;
	unsigned int patch;
};
typedef _Version Version;

AXLGL_ENUM_CLASS _Build { STATIC, SHARED };
typedef _Build Build;

AXLGLAPI const Version VERSION;
AXLGLAPI const Build BUILD;

} // axl::gl::lib
} // axl::gl
} // axl
