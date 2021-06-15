#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>

namespace axl {
namespace gl {

ContextObject::ContextObject(axl::gl::Context* ptr_context) :
	ctx_context(),
	m_created(false),
	m_being_destroyed(false)
{
	this->setContext(ptr_context);
}

ContextObject::~ContextObject()
{
	if(this->ctx_context)
		this->ctx_context->m_context_objects.remove(this);
}

 bool ContextObject::isValid() const
 {
	 return ctx_context && ctx_context->isValid() && m_created;
 }
bool ContextObject::create()
{
	if(this->ctx_context && !ContextObject::m_created && this->iCreate())
	{
		this->ctx_context->m_context_objects.insertFirst((ContextObject*)this);
		ContextObject::m_created = true;
	}
	return ContextObject::m_created;
}

bool ContextObject::destroy()
{
	if(!this->ctx_context || ContextObject::m_being_destroyed) return false;
	else if(!ContextObject::m_created) return true;
	ContextObject::m_being_destroyed = true;
	if(this->iDestroy())
	{
		ContextObject::m_created = false;
	}
	this->ctx_context->m_context_objects.remove((ContextObject*)this);
	ContextObject::m_being_destroyed = false;
	return !ContextObject::m_created;
}

bool ContextObject::isCreated() const
{
	return this->m_created;
}

bool ContextObject::isBeingDestroyed() const
{
	return this->m_being_destroyed;
}


axl::gl::Context* ContextObject::getContext()
{
	return this->ctx_context;
}

const axl::gl::Context* ContextObject::getContext() const
{
	return this->ctx_context;
}

bool ContextObject::setContext(axl::gl::Context* ptr_context)
{
	if(this->ctx_context != ptr_context && this->m_created) return false;
	if(!ptr_context && !this->destroy()) return false;
	this->ctx_context = ptr_context;
	return true;
}

} // axl.gl
} // axl
