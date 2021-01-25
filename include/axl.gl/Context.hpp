#pragma once
#include "lib.hpp"

namespace axl {
namespace gl {

class AXLGLCXXAPI View;

class AXLGLCXXAPI Context
{
	public:
		/**
		 * OpenGL context configuration info.
		 */
		class AXLGLCXXAPI Config
		{
			public:
				// OpenGL context profiles.
				enum GLProfile { GLP_COMPATIBLITY, GLP_CORE };
			public:
				Config(long id, int major_version, int minor_version, GLProfile profile);
				Config(const Config& config = Default);
				bool operator==(const Config& config) const;
				bool operator!=(const Config& config) const;
			public:
				long id;
				int major_version;
				int minor_version;
				GLProfile profile;
			public:
				// The default OpenGL context configuration to be set at the creation of a new Config by default.
				// You can modify it to your liking before creating configurations. 
				static Config Default;
				// A Null value representaion of the Config class.
				static const Config Null;
		};
	public:
		Context();
		virtual ~Context();
		virtual bool isCurrent() const;
		virtual bool isValid() const;
		virtual bool create(bool recreate, View* view, const Context::Config* configs, int num_configs);
		virtual void destroy();
		virtual bool makeCurrent() const;
		virtual bool clearCurrent() const;
		bool getVSync() const;
		bool setVSync(bool v_sync) const;
	public:
		View*const& view;
		const Context::Config& config;
		const void*const& reserved;
	private:
		View* m_view;
		Context::Config m_config;
		void *m_reserved;
	private:
		Context(const Context& context);
		Context& operator=(const Context& context);
};

} // namespace axl.gl
} // namespace axl