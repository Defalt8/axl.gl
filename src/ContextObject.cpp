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

bool ContextObject::create()
{
	if(!ContextObject::m_created && this->icreate())
		ContextObject::m_created = true;
	return ContextObject::m_created;
}

bool ContextObject::destroy()
{
	if(ContextObject::m_being_destroyed) return false;
	else if(!ContextObject::m_created) return true;
	ContextObject::m_being_destroyed = true;
	if(this->idestroy())
		ContextObject::m_created = false;
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

void ContextObject::setContext(axl::gl::Context* ptr_context)
{
	if(this->ctx_context != ptr_context)
	{
		this->ctx_context = ptr_context;
		this->ctx_context->m_context_objects.insertLast(this);
	}
}

} // axl.gl
} // axl
