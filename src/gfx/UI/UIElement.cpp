#include <cstdio>
#include <axl.gl/gfx/UI/UIElement.hpp>
#include <axl.gl/gfx/UI/UIManager.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.math/mat/transform4.hpp>

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {

//
// UIElement
//

UIElement::UIElement(Type type, axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	type(type),
	transform(),
	uielement_size(0,0),
	uielement_border_size(0.0f,0.0f,0.0f,0.0f),
	uielement_border_color(0.1f,0.1f,0.1f,1.0f),
	uielement_frame_buffer(ptr_context),
	uielement_texture(ptr_context),
	m_program(ptr_context),
	m_vertex_array(-1),
	m_vertex_buffer(-1),
	m_uloc_projection(-1),
	m_uloc_view(-1),
	m_uloc_model(-1),
	m_uloc_size(-1),
	m_uloc_border(-1),
	m_uloc_border_color(-1)
{}
UIElement::~UIElement()
{
	this->destroy();
}
bool UIElement::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() &&
		this->m_vertex_array != -1 && this->m_vertex_buffer != -1 &&
		this->uielement_texture.isValid() &&
		this->uielement_frame_buffer.isValid() &&
		this->m_program.isLinked();
}
void UIElement::setContext(axl::gl::Context* ptr_context)
{
	ContextObject::setContext(ptr_context);
	this->uielement_frame_buffer.setContext(ptr_context);
	this->uielement_texture.setContext(ptr_context);
	this->m_program.setContext(ptr_context);
}
bool UIElement::icreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || !this->ctx_context || this->ctx_context->config.major_version < 3 || !this->ctx_context->makeCurrent() ||
		!uielement_frame_buffer.create()
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
	GLsizei width = this->uielement_size.x;
	GLsizei height = this->uielement_size.y;
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
	if(!this->uielement_frame_buffer.setSize(axl::math::Vec2i(width, height)) ||
		GL::glGetError() != GL_NO_ERROR ||
		!this->uielement_texture.create() ||
		!this->m_program.create() ||
		!this->uielement_texture.allocate(0, width, height, GL::GL_RGBA8) ||
		!this->uielement_frame_buffer.attachTexture2D(GL_COLOR_ATTACHMENT0, &this->uielement_texture)
		)
	{
		this->destroy();
		return false;
	}
	this->m_uloc_projection = this->m_program.getUniformLocation("u_MatProjection");
	this->m_uloc_view = this->m_program.getUniformLocation("u_MatView");
	this->m_uloc_model = this->m_program.getUniformLocation("u_MatModel");
	this->m_uloc_size = this->m_program.getUniformLocation("u_Size");
	this->m_uloc_border = this->m_program.getUniformLocation("u_Border");
	this->m_uloc_border_color = this->m_program.getUniformLocation("u_BorderColor");
	return true;
}
bool UIElement::idestroy()
{
	using namespace GL;
	if(this->ctx_context && this->ctx_context->makeCurrent())
	{
		GLCLEARERROR();
		if(this->m_vertex_buffer != -1)
		{
			glDeleteBuffers(1, &this->m_vertex_buffer);
			this->m_vertex_buffer = -1;
		}
		if(this->m_vertex_array != -1)
		{
			glDeleteVertexArrays(1, &this->m_vertex_array);
			this->m_vertex_array = -1;
		}
	}
	return (glGetError() == GL_NO_ERROR &&
		this->uielement_texture.destroy() &&
		this->m_program.destroy());
}
bool UIElement::setUIManager(UIManager* ui_manager)
{
	if(ui_manager && this->ctx_context && this->ctx_context != ui_manager->getContext())
		return false;
	this->uielement_ui_manager = ui_manager;
	return true;
}
bool UIElement::render(const camera::Camera3Df* camera)
{
	using namespace GL;
	if(!this->isValid() || !camera || !camera->makeCurrent(this->ctx_context, false)) return false;
	if(!this->uielement_frame_buffer.bind(axl::gl::gfx::FrameBuffer::FBT_BOTH)) return false;
	if(!this->irender(camera))
	{
		this->uielement_frame_buffer.unbind(axl::gl::gfx::FrameBuffer::FBT_BOTH);
		return false;
	}
	this->uielement_frame_buffer.unbind(axl::gl::gfx::FrameBuffer::FBT_BOTH);
	if(!camera->makeCurrent(this->ctx_context, true)) return false;
	if(camera->projection)
		this->m_program.setUniformMat4fv(m_uloc_projection, camera->projection->matrix.values);
	this->m_program.setUniformMat4fv(m_uloc_view, camera->view_transform.values);
	this->m_program.setUniformMat4fv(m_uloc_model, this->transform.getMatrix().values);
	this->m_program.setUniform2iv(m_uloc_size, &this->uielement_size.x);
	this->m_program.setUniform4fv(m_uloc_border, &this->uielement_border_size.x);
	this->m_program.setUniform4fv(m_uloc_border_color, &this->uielement_border_color.x);
	if(!this->m_program.use() || !this->uielement_texture.bind()) return false;
	GLCLEARERROR();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->m_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	if(glGetError() != GL_NO_ERROR) return false;
	this->m_program.unuse();
	this->uielement_texture.unbind();
	return true;
}

bool UIElement::setSize(const axl::math::Vec2i& size)
{
	using namespace GL;
	if(size.x < 0 || size.y < 0) return false;
	if(this->isValid())
	{
		if(!this->uielement_texture.allocate(0, (GLsizei)size.x, (GLsizei)size.y, GL_RGBA8) ||
			!this->uielement_frame_buffer.setSize(size))
			return false;
		glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer);
		if(GL::glGetError() != GL_NO_ERROR) return false;
		GLfloat* buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(!buffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return false;
		}
		GLfloat width = (GLfloat)size.x;
		GLfloat height = (GLfloat)size.y;
		buffer[0] = 0.0f; buffer[1] = 0.0f;
		buffer[2] = 0.0f; buffer[3] = 0.0f;
		buffer[4] = width; buffer[5] = 0.0f;
		buffer[6] = 1.0f; buffer[7] = 0.0f;
		buffer[8] = width; buffer[9] = height;
		buffer[10] = 1.0f; buffer[11] = 1.0f;
		buffer[12] = width; buffer[13] = height;
		buffer[14] = 1.0f; buffer[15] = 1.0f;
		buffer[16] = 0.0f; buffer[17] = height;
		buffer[18] = 0.0f; buffer[19] = 1.0f;
		buffer[20] = 0.0f; buffer[21] = 0.0f;
		buffer[22] = 0.0f; buffer[23] = 0.0f;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	this->uielement_size = size;
	return true;
}
bool UIElement::setQuality(UIElement::Quality quality)
{
	using namespace GL;
	if(!this->isValid())
		return false;
	switch(quality)
	{
		default:
		case Q_LOW:
			return this->uielement_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_NEAREST) &&
				this->uielement_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		case Q_MEDIUM:
			return this->uielement_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_NEAREST) &&
				this->uielement_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		case Q_HIGH:
			return this->uielement_texture.setParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR) &&
				this->uielement_texture.setParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	return false;
}

bool UIElement::setBorderSize(const axl::math::Vec4f& border_size)
{
	if(border_size.x < 0.0f || border_size.y < 0.0f || border_size.z < 0.0f || border_size.w < 0.0f)
		return false;
	this->uielement_border_size = border_size;
	return true;
}
bool UIElement::setBorderColor(const axl::math::Vec4f& border_color)
{
	if(border_color.x < 0.0f || border_color.y < 0.0f || border_color.z < 0.0f || border_color.w < 0.0f)
		return false;
	this->uielement_border_color = border_color;
	return true;
}
const axl::math::Vec2i& UIElement::getSize() const
{
	return this->uielement_size;
}
const axl::math::Vec4f& UIElement::getBorderSize() const
{
	return this->uielement_border_size;
}
const axl::math::Vec4f& UIElement::getBorderColor() const
{
	return this->uielement_border_color;
}

//
// UIElement::Program
//

UIElement::Program::Program(axl::gl::Context* ptr_context) :
	axl::gl::gfx::Program(ptr_context)
{}
UIElement::Program::~Program()
{
	this->destroy();
}
bool UIElement::Program::icreate()
{
	using namespace GL;
	if(!axl::gl::gfx::Program::icreate()) return false;
	Shader vertex_shader(this->ctx_context, GL_VERTEX_SHADER), fragment_shader(this->ctx_context, GL_FRAGMENT_SHADER);
	if(!vertex_shader.create() || !fragment_shader.create()) return false;
	vertex_shader.setSource(
		"# version 330 core\n"
		"layout(location = 0) in vec2 in_Position;\n"
		"layout(location = 1) in vec2 in_UV;\n"
		"uniform mat4 u_MatProjection = mat4(1);\n"
		"uniform mat4 u_MatView = mat4(1);\n"
		"uniform mat4 u_MatModel = mat4(1);\n"
		"out vec2 v_TexCoord;"
		"void main() {\n"
		"	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4(in_Position, 0.0, 1.0);\n"
		"	v_TexCoord = in_UV;\n"
		"}\n"
		);
	fragment_shader.setSource(
		"# version 330 core\n"
		"in vec2 v_TexCoord;\n"
		"uniform sampler2D texture;\n"
		"uniform ivec2 u_Size = ivec2(0,0);"
		"uniform vec4 u_Border = vec4(0,0,0,0);"
		"uniform vec4 u_BorderColor = vec4(0,0,0,0);"
		"void main() {\n"
		"	vec4 sample = texture2D(texture, v_TexCoord);\n"
		"	gl_FragColor = (v_TexCoord.x <= (u_Border.x / u_Size.x) || \n"
		"		v_TexCoord.x >= ((u_Size.x - u_Border.z) / u_Size.x) || \n"
		"		v_TexCoord.y <= (u_Border.y / u_Size.y) || \n"
		"		v_TexCoord.y >= ((u_Size.y - u_Border.w) / u_Size.y) \n"
		"		) ? u_BorderColor : sample;\n"
		"}\n"
		);
	if(!vertex_shader.compile() || !fragment_shader.compile())
	{
		printf("===== UIElement::VIL:\n %s\n", vertex_shader.getInfoLog().cstr());
		printf("===== UIElement::FIL:\n %s\n", fragment_shader.getInfoLog().cstr());
		return false;
	}
	vertex_shader.attach(*this);
	fragment_shader.attach(*this);
	if(!this->link()) return false;
	vertex_shader.detach(*this);
	fragment_shader.detach(*this);
	return true;
}
bool UIElement::Program::idestroy()
{
	return axl::gl::gfx::Program::idestroy();
}

} // axl.gl.gfx
} // axl.gl
} // axl
