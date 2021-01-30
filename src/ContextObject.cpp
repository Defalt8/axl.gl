#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>

namespace axl {
namespace gl {

ContextObject::ContextObject(axl::gl::Context* ptr_context) :
	ctx_context(),
	ctx_created(false),
	ctx_destroyed(false)
{
	this->setContext(ptr_context);
}

ContextObject::~ContextObject()
{
	if(this->ctx_context)
		this->ctx_context->m_context_objects.remove(this);
}

bool ContextObject::create()
{
	if(this->ctx_created && !this->ctx_destroyed) return true;
	if(!this->ctx_context || !this->icreate()) return false;
	this->ctx_destroyed = false;
	return this->ctx_created = true;
}

bool ContextObject::destroy()
{
	if(this->ctx_destroyed) return true;
	if(!this->ctx_created || !this->idestroy()) return false;
	this->ctx_created = false;
	return this->ctx_destroyed = true;
}

axl::gl::Context* ContextObject::getContext()
{
	return this->ctx_context;
}

void ContextObject::setContext(axl::gl::Context* ptr_context)
{
	this->ctx_context = ptr_context;
	if(this->ctx_context)
	{
		this->ctx_context->m_context_objects.insertLast(this);
	}
}

} // axl.gl
} // axl
