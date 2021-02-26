#pragma once
#include "lib.hpp"
#include <axl.util/ds/List.hpp>
#include "ContextObject.hpp"

namespace axl {
namespace gl {

class AXLGLCXXAPI View;

template class AXLGLCXXAPI axl::util::ds::UniList<axl::gl::ContextObject*>;
template class AXLGLCXXAPI axl::util::ds::UniList<axl::gl::ContextObject*>::Iterator;

class AXLGLCXXAPI ContextConfig;
// The default OpenGL context configuration to be set at the creation of a new ContextConfig by default.
// You can modify it to your liking before creating configurations. 
AXLGLAPI axl::gl::ContextConfig DefaultContextConfig;
// A Null value representaion of the ContextConfig class.
AXLGLAPI const axl::gl::ContextConfig NullContextConfig;

/**
 * OpenGL context configuration info.
 */
class AXLGLCXXAPI ContextConfig
{
	public:
		// OpenGL context profiles.
		enum GLProfile { GLP_COMPATIBLITY, GLP_CORE };
	public:
		ContextConfig(long id, int major_version, int minor_version, GLProfile profile);
		ContextConfig(const ContextConfig& config = DefaultContextConfig);
		bool operator==(const ContextConfig& config) const;
		bool operator!=(const ContextConfig& config) const;
	public:
		long id;
		int major_version;
		int minor_version;
		GLProfile profile;
};

class AXLGLCXXAPI Context
{
	public:
	public:
		Context();
		virtual ~Context();
		virtual bool isCurrent() const;
		virtual bool isValid() const;
		virtual bool create(bool recreate, View* view, const Context::ContextConfig* configs, int num_configs);
		virtual void destroy();
		virtual bool makeCurrent() const;
		virtual bool clearCurrent() const;
		const axl::util::ds::UniList<ContextObject*>& getContextObjects() const;
		bool getVSync() const;
		bool setVSync(bool v_sync) const;
	public:
		View*const& view;
		const Context::ContextConfig& config;
		const void*const& reserved;
	private:
		View* m_view;
		Context::ContextConfig m_config;
		axl::util::ds::UniList<ContextObject*> m_context_objects;
		void *m_reserved;
	private:
		Context(const Context& context);
		Context& operator=(const Context& context);
		friend class AXLGLCXXAPI axl::gl::ContextObject;
};

} // namespace axl.gl
} // namespace axl