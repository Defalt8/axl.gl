#pragma once
#include "lib.hpp"
#include "Context.hpp"

namespace axl {
namespace gl {

class AXLGLCXXAPI ContextObject
{
	public:
		ContextObject(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~ContextObject();
	public:
		virtual bool isValid() const;
		bool create();
		bool destroy();
		bool isCreated() const;
		bool isBeingDestroyed() const;
	protected:
		virtual bool iCreate() = 0;
		virtual bool iDestroy() = 0;
	public:
		axl::gl::Context* getContext();
		const axl::gl::Context* getContext() const;
		bool setContext(axl::gl::Context* ptr_context);
	protected:
		axl::gl::Context* ctx_context;
	private:
		bool m_created;
		bool m_being_destroyed;
};

} // axl.gl
} // axl
