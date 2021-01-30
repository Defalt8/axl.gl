#pragma once
#include "lib.hpp"

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

class AXLGLCXXAPI ContextObject
{
	public:
		ContextObject(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~ContextObject();
		bool create();
		bool destroy();
		virtual bool icreate() = 0;
		virtual bool idestroy() = 0;
		axl::gl::Context* getContext();
		void setContext(axl::gl::Context* ptr_context);
	protected:
		axl::gl::Context* ctx_context;
		bool ctx_created;
		bool ctx_destroyed;
};

} // axl.gl
} // axl
