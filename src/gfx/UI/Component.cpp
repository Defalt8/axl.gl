#include <axl.gl/gfx/UI/Component.hpp>
#include <axl.gl/gfx/UI/Container.hpp>
#include <axl.gl/gfx/shader/Shader.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

//
// Component
//

Component::Component(Type type,
		axl::gl::Context* ptr_context,
		axl::gl::gfx::ui::Container* container,
		const axl::math::Vec3f& position,
		const axl::math::Vec2i& size,
		const axl::math::Vec4f& padding) :
	axl::gl::ContextObject(ptr_context),
	component_type(type),
	component_is_visible(true),
	component_is_modified(true),
	m_vertex_array(-1),
	m_vertex_buffer(-1),
	component_framebuffer(ptr_context),
	component_program_ptr(0),
	component_container(0),
	component_size(size),
	component_padding(padding),
	component_background_color(0.f,0.f,0.f,0.f),
	component_foreground_color(.9f,.9f,.9f,1.f)
{
	this->setContainer(container);
}
Component::~Component()
{
	this->destroy();
}

bool Component::iCreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 ||
	   !this->ctx_context ||
	   !this->ctx_context->isValid() ||
	   !component_program_ptr || !component_program_ptr->isValid() ||
	   this->ctx_context->config.major_version < 3 ||
	   !this->ctx_context->makeCurrent() ||
	   !component_framebuffer.create()
	)
		return false;
	GLCLEARERROR();
	glGenVertexArrays(1, &this->m_vertex_array);
	glGenBuffers(1, &this->m_vertex_buffer);
	if(this->m_vertex_array == -1 || this->m_vertex_buffer == -1) return false;
	glBindVertexArray(this->m_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
	if(GL::glGetError() != GL_NO_ERROR)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat) * 24), 0, GL_DYNAMIC_DRAW);
	if(GL::glGetError() != GL_NO_ERROR)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return false;
	}
	GLfloat* buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(!buffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return false;
	}
	GLsizei width = (GLsizei)component_size.x;
	GLsizei height = (GLsizei)component_size.y;
	buffer[0] = 0.0f; buffer[1] = 0.0f;
	buffer[2] = 0.0f; buffer[3] = 0.0f;
	buffer[4] = (GLfloat)width; buffer[5] = 0.0f;
	buffer[6] = 1.0f; buffer[7] = 0.0f;
	buffer[8] = (GLfloat)width; buffer[9] = (GLfloat)height;
	buffer[10] = 1.0f; buffer[11] = 1.0f;
	buffer[12] = (GLfloat)width; buffer[13] = (GLfloat)height;
	buffer[14] = 1.0f; buffer[15] = 1.0f;
	buffer[16] = 0.0f; buffer[17] = (GLfloat)height;
	buffer[18] = 0.0f; buffer[19] = 1.0f;
	buffer[20] = 0.0f; buffer[21] = 0.0f;
	buffer[22] = 0.0f; buffer[23] = 0.0f;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(GLfloat) * 4), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(GLfloat) * 4), (const void*)(sizeof(GLfloat) * 2));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if(GL::glGetError() != GL_NO_ERROR ||
		!component_framebuffer.setSize(axl::math::Vec2i(width, height))
	)
	{
		this->destroy();
		return false;
	}
	ctx_context->addComponent((Component*)this);
	component_is_modified = true;
	return true;
}
bool Component::iDestroy()
{using namespace GL;
	if(this->ctx_context && this->ctx_context->isValid() && this->ctx_context->makeCurrent())
	{
		GLCLEARERROR();
		if(m_vertex_buffer != -1)
		{
			glDeleteBuffers(1, &m_vertex_buffer);
			m_vertex_buffer = -1;
		}
		if(m_vertex_array != -1)
		{
			glDeleteVertexArrays(1, &m_vertex_array);
			m_vertex_array = -1;
		}
	}
	ctx_context->removeComponent((Component*)this);
	return glGetError() == GL_NO_ERROR &&
		component_framebuffer.destroy();
}

bool Component::isValid() const
{
	return component_program_ptr && component_program_ptr->isValid() && component_framebuffer.isValid();
}
void Component::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::ContextObject::setContext(ptr_context);
	component_framebuffer.setContext(ptr_context);
	if(component_program_ptr && component_program_ptr->getContext() != ptr_context)
		component_program_ptr = 0;
}
bool Component::setComponentProgram(const axl::gl::gfx::ui::Component::Program* ptr_program)
{
	if(ptr_program && ptr_program->getContext() != this->ctx_context)
		return false;
	component_program_ptr = ptr_program;
	return true;
}

bool Component::setContainer(axl::gl::gfx::ui::Container* container)
{
	if(!container) return false;
	return container->addComponent((Component*)this);
}
void Component::setVisiblity(bool is_visible)
{
	component_is_visible = is_visible;
}
bool Component::setSize(const axl::math::Vec2i& size)
{
	if(size.x < 0 || size.y < 0 || !component_framebuffer.setSize(size))
		return false;
	component_size = size;
	if(ctx_context && ctx_context->makeCurrent())
	{
		using namespace GL;
		glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
		GLfloat* buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(buffer)
		{
			GLsizei width = (GLsizei)component_size.x;
			GLsizei height = (GLsizei)component_size.y;
			buffer[4] = (GLfloat)width; buffer[5] = 0.0f;
			buffer[8] = (GLfloat)width; buffer[9] = (GLfloat)height;
			buffer[12] = (GLfloat)width; buffer[13] = (GLfloat)height;
			buffer[16] = 0.0f; buffer[17] = (GLfloat)height;
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	component_is_modified = true;
	return true;
}
bool Component::setPadding(const axl::math::Vec4f& padding)
{
	if(
		padding.x < 0.f || padding.y < 0.f || padding.z < 0.f || padding.w < 0.f
	)
		return false;
	component_padding = padding;
	component_is_modified = true;
	return true;
}
void Component::setBackgroundColor(const axl::math::Vec4f& background_color)
{
	component_background_color = background_color;
	component_is_modified = true;
}
void Component::setForegroundColor(const axl::math::Vec4f& foreground_color)
{
	component_foreground_color = foreground_color;
	component_is_modified = true;
}
const axl::gl::gfx::ui::Component::Program* Component::getComponentProgram() const
{
	return component_program_ptr;
}
axl::gl::gfx::ui::Container* Component::getContainer() const
{
	return component_container;
}
bool Component::isVisible() const
{
	return component_is_visible;
}
const axl::math::Vec2i& Component::getSize() const
{
	return component_size;
}
const axl::math::Vec4f& Component::getPadding() const
{
	return component_padding;
}
const axl::math::Vec4f& Component::getBackgroundColor() const
{
	return component_background_color;
}
const axl::math::Vec4f& Component::getForegroundColor() const
{
	return component_foreground_color;
}
const axl::gl::gfx::Texture2D& Component::getTexture() const
{
	return component_framebuffer.fb_render_texture;
}
axl::math::Vec2i Component::getClientSize() const
{
	return axl::math::Vec2i(component_size.x - (int)(component_padding.x + component_padding.z), component_size.y - (int)(component_padding.y + component_padding.w));
}
bool Component::render(axl::gl::camera::Camera3Df* camera, const axl::gl::gfx::FrameBuffer* ptr_frame_buffer)
{
	using namespace GL;
	if(!camera || !component_is_visible || !this->isValid()) return false;
	if(camera)
	{
		if(!camera->makeCurrent(this->ctx_context, true))
			return false;
	}
	bool fully_rendered = true;
	// render component offscreen if the component has been modified
	if(!component_is_modified && component_type == CONTAINER)
	{
		axl::gl::gfx::ui::Container* container = (axl::gl::gfx::ui::Container*)this;
		for(axl::util::ds::UniList<axl::gl::gfx::ui::Component*>::Iterator it = container->getComponents().first(); it.isNotNull(); ++it)
		{
			if((*it)->component_is_modified)
			{
				this->component_is_modified = true;
				break;
			}
		}
	}
	if(component_is_modified)
	{
		if(!component_framebuffer.bind(FrameBuffer::FBT_BOTH))
			return false;
		glClearColor(component_background_color.x, component_background_color.y, component_background_color.z, component_background_color.w);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		fully_rendered = this->iRender(camera);
		component_framebuffer.unbind(FrameBuffer::FBT_BOTH);
		component_is_modified = false;
	}
	// render the rendered texture onto a quad
	if(ptr_frame_buffer)
	{
		if(!ptr_frame_buffer->bind(axl::gl::gfx::FrameBuffer::FBT_BOTH))
			return false;
	}
	if(camera->projection)
	{
		component_program_ptr->setUniformMat4fv(component_program_ptr->uloc_projection, camera->projection->matrix.values);
	}
	component_program_ptr->setUniformMat4fv(component_program_ptr->uloc_view, camera->view_transform.values);
	component_program_ptr->setUniformMat4fv(component_program_ptr->uloc_model, this->transform.getMatrix().values);
	if(component_framebuffer.fb_render_texture.bind(0))
	{
		GLCLEARERROR();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glBindVertexArray(this->m_vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
		component_program_ptr->use();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		if(glGetError() != GL_NO_ERROR)
			fully_rendered = false;
		component_program_ptr->unuse();
		component_framebuffer.fb_render_texture.unbind(0);
	}
	else
		fully_rendered = false;
	if(ptr_frame_buffer)
		ptr_frame_buffer->unbind(axl::gl::gfx::FrameBuffer::FBT_BOTH);
	return fully_rendered;
}

//
// Component::FrameBuffer
//

Component::FrameBuffer::FrameBuffer(axl::gl::Context* ptr_context) :
	axl::gl::gfx::FrameBuffer(ptr_context),
	fb_render_texture(ptr_context),
	fb_render_buffer_depth(ptr_context),
	fb_size(0,0)
{}
Component::FrameBuffer::~FrameBuffer()
{
	this->destroy();
}
bool Component::FrameBuffer::isValid() const
{
	return axl::gl::gfx::FrameBuffer::isValid() &&
		this->fb_render_texture.isValid() &&
		this->fb_render_buffer_depth.isValid();
}
void Component::FrameBuffer::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::FrameBuffer::setContext(ptr_context);
	this->fb_render_texture.setContext(ptr_context);
	this->fb_render_buffer_depth.setContext(ptr_context);
}
bool Component::FrameBuffer::iCreate()
{
	using namespace GL;
	if(!axl::gl::gfx::FrameBuffer::iCreate() ||
		!this->fb_render_texture.create() ||
		!this->fb_render_buffer_depth.create()
	)
	{
		this->destroy();
		return false;
	}
	fb_render_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	fb_render_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	fb_render_texture.setParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	fb_render_texture.setParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return this->fb_render_texture.allocate(0, this->fb_size.x, this->fb_size.y, GL_RGBA8) &&
		this->fb_render_buffer_depth.allocate(this->fb_size.x, this->fb_size.y, GL_DEPTH_COMPONENT24, 0) &&
		this->attachTexture2D(GL_COLOR_ATTACHMENT0, &this->fb_render_texture) &&
		this->attachRenderBuffer(GL_DEPTH_ATTACHMENT, &this->fb_render_buffer_depth);
}
bool Component::FrameBuffer::iDestroy()
{
	using namespace GL;
	return this->attachTexture2D(GL_COLOR_ATTACHMENT0, 0) &&
		   this->attachRenderBuffer(GL_DEPTH_ATTACHMENT, 0) &&
		   this->fb_render_texture.destroy() &&
		   this->fb_render_buffer_depth.destroy() &&
		   axl::gl::gfx::FrameBuffer::iDestroy();
}
bool Component::FrameBuffer::setSize(const axl::math::Vec2i& size)
{
	using namespace GL;
	if(size.x < 0 || size.y < 0)
		return false;
	if(this->isValid())
	{
		if(!this->fb_render_texture.allocate(0, size.x, size.y, GL_RGBA8) ||
		   !this->fb_render_buffer_depth.allocate(size.x, size.y, GL_DEPTH_COMPONENT24, 0))
			return false;
	}
	this->fb_size = size;
	return true;
}
const axl::math::Vec2i& Component::FrameBuffer::getSize() const
{
	return this->fb_size;
}

//
// Component::Program
//

Component::Program::Program(axl::gl::Context* ptr_context) :
	axl::gl::gfx::Program(ptr_context),
	uloc_texture0(-1),
	uloc_projection(-1),
	uloc_view(-1),
	uloc_model(-1)
{}
Component::Program::~Program()
{
	this->destroy();
}
bool Component::Program::iCreate()
{
	using namespace GL;
	if(!axl::gl::gfx::Program::iCreate()) return false;
	Shader vertex_shader(this->ctx_context, GL_VERTEX_SHADER), fragment_shader(this->ctx_context, GL_FRAGMENT_SHADER);
	if(!vertex_shader.create() || !fragment_shader.create()) return false;
	vertex_shader.setSource(
		"# version 330 core\n"
		"layout(location = 0) in vec2 in_Position;\n"
		"layout(location = 1) in vec2 in_UV;\n"
		"uniform mat4 u_MatProjection = mat4(1.0);\n"
		"uniform mat4 u_MatView = mat4(1.0);\n"
		"uniform mat4 u_MatModel = mat4(1.0);\n"
		"out vec2 v_TexCoord0;"
		"void main() {\n"
		"	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position, 0.0, 1.0);\n"
		"	v_TexCoord0 = in_UV;\n"
		"}\n"
		);
	fragment_shader.setSource(
		"# version 330 core\n"
		"in vec2 v_TexCoord0;\n"
		"uniform sampler2D u_Texture0;\n"
		"void main() {\n"
		"	gl_FragColor = texture2D(u_Texture0, v_TexCoord0);\n"
		"}\n"
		);
	if(!vertex_shader.compile() || !fragment_shader.compile())
	{
		return false;
	}
	vertex_shader.attach(*this);
	fragment_shader.attach(*this);
	if(!this->link()) return false;
	vertex_shader.detach(*this);
	fragment_shader.detach(*this);
	uloc_texture0 = this->getUniformLocation("u_Texture0");
	uloc_projection = this->getUniformLocation("u_MatProjection");
	uloc_view = this->getUniformLocation("u_MatView");
	uloc_model = this->getUniformLocation("u_MatModel");
	return uloc_texture0 != -1 &&
		this->setUniform1i(uloc_texture0, 0) &&
		uloc_projection != -1 &&
		uloc_view != -1 &&
		uloc_model != -1;
}
bool Component::Program::iDestroy()
{
	uloc_texture0 = -1;
	uloc_projection = -1;
	uloc_view = -1;
	uloc_model = -1;
	return axl::gl::gfx::Program::iDestroy();
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
