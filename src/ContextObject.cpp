#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>

namespace axl {
namespace gl {

ContextObject::ContextObject(axl::gl::Context* context, bool create) :
	ctx_context(),
	ctx_created(false),
	ctx_destroyed(false)
{
	this->setContext(context);
	if(create) this->create();
}

ContextObject::~ContextObject()
{
	this->destroy();
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

void ContextObject::setContext(axl::gl::Context* context)
{
	this->ctx_context = context;
	if(this->ctx_context)
	{
		this->ctx_context->m_context_objects.insertLast(this);
	}
}

} // axl.gl
} // axl
