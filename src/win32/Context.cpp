#include <cstdlib>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.glfl/win/wglext.hpp>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include "ViewData.hpp"

namespace axl {
namespace gl {
////////////
// Context

struct ContextData
{
	HDC hdc;
	HGLRC hglrc;
	ContextData() : hdc(NULL), hglrc(NULL) {}
};
typedef struct ContextData ContextData;

Context::Context() :
	view(m_view),
	config(m_config),
	reserved(m_reserved),
	m_config(Context::Config::Default),
	m_reserved(new ContextData())
{}

Context::~Context()
{
	this->destroy();
	if(m_reserved)
	{
		delete((ContextData*)m_reserved);
		m_reserved = (void*)0;
	}
}

bool Context::isCurrent() const
{
	return ((ContextData*)m_reserved)->hglrc == wglGetCurrentContext();
}

bool Context::isValid() const
{
	return (((ContextData*)m_reserved)->hdc && ((ContextData*)m_reserved)->hglrc);
}

bool Context::create(bool recreate, const View* view_, const Context::Config* configs_, int num_configs_)
{
	if(!m_reserved) m_reserved = new ContextData();
	if(!view_->isValid() || !m_reserved) return false;
	if(!axl::glfl::core::load() || !axl::glfl::WGL::load())
		return false;
	if(recreate)
		this->destroy();
	((ViewData*)this->m_reserved)->hwnd = ((ViewData*)(view_->m_reserved))->hwnd;
	HDC hdc = ((ContextData*)m_reserved)->hdc = GetDC(((ViewData*)(view_->m_reserved))->hwnd);
	HGLRC hglrc = NULL;
	if(!hdc) return false;
	if(axl::glfl::WGL::WGL_ARB_create_context)
	{
		if(configs_ && num_configs_ > 0)
		{
			const Context::Config* cur_config;
			for(int i=0; i<num_configs_; ++i)
			{
				cur_config = &configs_[i];
				if(cur_config->major_version <= 0)
				{
					int attribs[] = {
						axl::glfl::WGL::WGL_CONTEXT_PROFILE_MASK_ARB, (cur_config->profile==Config::GLP_CORE ? axl::glfl::WGL::WGL_CONTEXT_CORE_PROFILE_BIT_ARB : axl::glfl::WGL::WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB),
						0
					};
					hglrc = axl::glfl::WGL::wglCreateContextAttribsARB(hdc, NULL, attribs);
				}
				else
				{
					int attribs[] = {
						axl::glfl::WGL::WGL_CONTEXT_MAJOR_VERSION_ARB, cur_config->major_version,
						axl::glfl::WGL::WGL_CONTEXT_MINOR_VERSION_ARB, cur_config->minor_version,
						axl::glfl::WGL::WGL_CONTEXT_PROFILE_MASK_ARB, (cur_config->profile==Config::GLP_CORE ? axl::glfl::WGL::WGL_CONTEXT_CORE_PROFILE_BIT_ARB : axl::glfl::WGL::WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB),
						0
					};
					hglrc = axl::glfl::WGL::wglCreateContextAttribsARB(hdc, NULL, attribs);
				}
				if(hglrc)
				{
					if(cur_config) m_config = *cur_config;
					break;
				}
			}
		}
		else
		{
			if(config.major_version <= 0)
			{
				int attribs[] = {
					axl::glfl::WGL::WGL_CONTEXT_PROFILE_MASK_ARB, (config.profile==Config::GLP_CORE ? axl::glfl::WGL::WGL_CONTEXT_CORE_PROFILE_BIT_ARB : axl::glfl::WGL::WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB),
					0
				};
				hglrc = axl::glfl::WGL::wglCreateContextAttribsARB(hdc, NULL, attribs);
			}
			else
			{
				int attribs[] = {
					axl::glfl::WGL::WGL_CONTEXT_MAJOR_VERSION_ARB, config.major_version,
					axl::glfl::WGL::WGL_CONTEXT_MINOR_VERSION_ARB, config.minor_version,
					axl::glfl::WGL::WGL_CONTEXT_PROFILE_MASK_ARB, (config.profile==Config::GLP_CORE ? axl::glfl::WGL::WGL_CONTEXT_CORE_PROFILE_BIT_ARB : axl::glfl::WGL::WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB),
					0
				};
				hglrc = axl::glfl::WGL::wglCreateContextAttribsARB(hdc, NULL, attribs);
			}
		}
	}
	if(hglrc == NULL)
	{
		hglrc = wglCreateContext(hdc);
		m_config.profile = Config::GLP_COMPATIBLITY;
		m_config.major_version = 0;
		m_config.minor_version = 0;
	}
	((ContextData*)m_reserved)->hglrc = hglrc;
	if(hglrc && this->makeCurrent())
	{
		using namespace axl::glfl::core::GL;
		const char* gl_ver = (const char*)glGetString(GL_VERSION);
		if(gl_ver)
		{
			if((int)gl_ver[0]-'0' >= 3)
			{
				glGetIntegerv(GL_MAJOR_VERSION, &m_config.major_version);
				glGetIntegerv(GL_MINOR_VERSION, &m_config.minor_version);
			}
			else
			{
				int dot_index = (gl_ver[1] == '.' ? 1 : (gl_ver[2] == '.' ? 2 : (gl_ver[3] == '.' ? 3 : (gl_ver[4] == '.' ? 4 : 5))));
				m_config.major_version = (int)atof(gl_ver);
				m_config.minor_version = (int)atof(&gl_ver[dot_index+1]);
			}
		}
	}
	return ((ContextData*)m_reserved)->hglrc != NULL;
}

void Context::destroy()
{
	if(m_reserved)
	{
		if(((ContextData*)m_reserved)->hglrc)
		{
			if(((ContextData*)m_reserved)->hdc)
			{
				wglMakeCurrent(((ContextData*)m_reserved)->hdc, NULL);
				wglDeleteContext(((ContextData*)m_reserved)->hglrc);
				((ContextData*)m_reserved)->hglrc = NULL;
				ReleaseDC(((ViewData*)this->m_reserved)->hwnd, ((ViewData*)this->m_reserved)->hdc);
				((ContextData*)m_reserved)->hdc = NULL;
			}
		}
	}
}

bool Context::makeCurrent() const
{
	return wglMakeCurrent(((ContextData*)m_reserved)->hdc, ((ContextData*)m_reserved)->hglrc) != FALSE;
}

bool Context::clearCurrent() const
{
	return wglMakeCurrent(((ContextData*)m_reserved)->hdc, NULL) != FALSE;
}
bool Context::getVSync() const
{
	if (axl::glfl::WGL::WGL_EXT_swap_control && this->makeCurrent())
		return axl::glfl::WGL::wglGetSwapIntervalEXT();
	return false;
}
bool Context::setVSync(bool v_sync) const
{
	if (axl::glfl::WGL::WGL_EXT_swap_control && this->makeCurrent())
		return axl::glfl::WGL::wglSwapIntervalEXT((int)v_sync);
	return false;
}
//////////////////
// Context::Config

Context::Config::Config(long id_, int version_major_, int version_minor_, GLProfile profile_) :
	id(id_),
	major_version(version_major_),
	minor_version(version_minor_),
	profile(profile_)
{}

Context::Config::Config(const Config& config) :
	id(config.id),
	major_version(config.major_version),
	minor_version(config.minor_version),
	profile(config.profile)
{}

bool Context::Config::operator==(const Config& rhs) const
{
	return	(
		major_version == rhs.major_version &&
		minor_version == rhs.minor_version &&
		profile == rhs.profile
		);
}

bool Context::Config::operator!=(const Config& rhs) const
{
	return	(
		major_version != rhs.major_version ||
		minor_version != rhs.minor_version ||
		profile != rhs.profile
		);
}

Context::Config Context::Config::Default(
	0, // id
	0, // major_version
	0, // minor_version
	GLP_COMPATIBLITY // profile
);

const Context::Config Context::Config::Null(
	-1, // id
	-1, // major_version
	-1, // minor_version
	GLP_CORE // profile
);

} // namespace axl.gl
} // namespace axl