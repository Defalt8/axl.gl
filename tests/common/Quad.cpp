#include "Quad.hpp"
namespace Test {


Quad::Quad(axl::gl::Context* ptr_context, const axl::math::Vec2f& _size) :
	axl::gl::ContextObject(ptr_context),
	size(m_size),
	texture(0),
	color(1.f,1.f,1.f,1.f),
	m_program_ptr(0),
	m_size(_size),
	m_vertex_array(0),
	m_vertex_buffer(0)
{}

Quad::~Quad()
{
	this->destroy();
}

bool Quad::iCreate()
{
	if(!(ctx_context->isCurrent() || ctx_context->makeCurrent())) return false;
	using namespace GL;
	GLuint vertex_array = 0, vertex_buffer = 0;
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer);
	if(vertex_array == 0 || vertex_buffer == 0)
	{
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteBuffers(1, &vertex_buffer);
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	const GLfloat vertices[] = {
		0.f, 0.f, 0.f,				0.f, 0.f,
		m_size.x, 0.f, 0.f,			1.f, 0.f,
		m_size.x, m_size.y, 0.f,	1.f, 1.f,
		m_size.x, m_size.y, 0.f,	1.f, 1.f,
		0.f, m_size.y, 0.f,			0.f, 1.f,
		0.f, 0.f, 0.f,				0.f, 0.f
	};
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindVertexArray(vertex_array);
	GLsizei stride = (GLsizei)sizeof(GLfloat) * 5;
	GLint aloc_position = m_program_ptr && m_program_ptr->isLinked() ? m_program_ptr->aloc_position : 0;
	GLint aloc_texture_coord = m_program_ptr && m_program_ptr->isLinked() ? m_program_ptr->aloc_texture_coord : 1;
	glVertexAttribPointer(aloc_position, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(aloc_position);
	glVertexAttribPointer(aloc_texture_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(aloc_texture_coord);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_vertex_array = vertex_array;
	m_vertex_buffer = vertex_buffer;
	return true;
}

bool Quad::iDestroy()
{
	if(m_vertex_array == 0 && m_vertex_buffer == 0) return true;
	if((ctx_context->isCurrent() || ctx_context->makeCurrent()))
	{
		using namespace GL;
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if(m_vertex_array != 0)
			glDeleteVertexArrays(1, &m_vertex_array);
		if(m_vertex_buffer != 0)
			glDeleteBuffers(1, &m_vertex_buffer);
	}
	m_vertex_array = 0;
	m_vertex_buffer = 0;
	return true;
}

bool Quad::isValid() const
{
	return axl::gl::ContextObject::isValid() && m_program_ptr && m_vertex_array != 0 && m_vertex_buffer != 0;
}

bool Quad::setQuadProgram(Quad::Program* quad_program_ptr)
{
	if(!this->ctx_context || !quad_program_ptr || this->ctx_context != quad_program_ptr->getContext()) return false;
	this->m_program_ptr = quad_program_ptr;
	this->updateQuadProgram();
	return true;
}
bool Quad::updateQuadProgram()
{
	if(!this->ctx_context || !m_program_ptr || !m_program_ptr->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	using namespace GL;
	glBindVertexArray(m_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	GLsizei stride = (GLsizei)sizeof(GLfloat) * 5;
	if(m_program_ptr->aloc_position != -1)
	{
		glVertexAttribPointer(m_program_ptr->aloc_position, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(m_program_ptr->aloc_position);
	}
	if(m_program_ptr->aloc_texture_coord != -1)
	{
		glVertexAttribPointer(m_program_ptr->aloc_texture_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
		glEnableVertexAttribArray(m_program_ptr->aloc_texture_coord);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

bool Quad::setSize(const axl::math::Vec2f& _size)
{
	if(this->isValid())
	{
		using namespace GL;
		if(!(ctx_context->isCurrent() || ctx_context->makeCurrent())) return false;
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
		GLfloat* vertices = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(!vertices)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return false;
		}
		vertices[5] = _size.x;
		vertices[10] = _size.x;
		vertices[11] = _size.y;
		vertices[15] = _size.x;
		vertices[16] = _size.y;
		vertices[21] = _size.y;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	this->m_size = _size;
	return true;
}

bool Quad::render(axl::gl::camera::Camera3Df* camera)
{
	if(!this->isValid() || !m_program_ptr->isLinked() || !(ctx_context->isCurrent() || ctx_context->makeCurrent())) return false;
	using namespace GL;
	if(camera)
	{
		if(camera->projection)
			m_program_ptr->setUniformMat4fv(m_program_ptr->uloc_projection_matrix, camera->projection->matrix.values);
		m_program_ptr->setUniformMat4fv(m_program_ptr->uloc_view_matrix, camera->view_matrix.values);
	}
	m_program_ptr->setUniformMat4fv(m_program_ptr->uloc_model_matrix, this->transform.getMatrix().values);
	m_program_ptr->setUniform4fv(m_program_ptr->uloc_object_color, &this->color.x);
	m_program_ptr->setUniform1i(m_program_ptr->uloc_sample_texture, texture != 0);
	if(texture) texture->bind(0);
	if(!m_program_ptr->use()) return false;
	glBindVertexArray(m_vertex_array);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	m_program_ptr->unuse();
	if(texture) texture->unbind(0);
	return true;
}

//
// Quad::Program
//

Quad::Program::Program(axl::gl::Context* ptr_context) :
	axl::gl::gfx::Program(ptr_context),
	aloc_position(m_aloc_position),
	aloc_texture_coord(m_aloc_texture_coord),
	uloc_projection_matrix(m_uloc_projection_matrix),
	uloc_view_matrix(m_uloc_view_matrix),
	uloc_model_matrix(m_uloc_model_matrix),
	uloc_texture0(m_uloc_texture0),
	uloc_sample_texture(m_uloc_sample_texture),
	uloc_object_color(m_uloc_object_color),
	m_aloc_position(-1),
	m_aloc_texture_coord(-1),
	m_uloc_projection_matrix(-1),
	m_uloc_view_matrix(-1),
	m_uloc_model_matrix(-1),
	m_uloc_texture0(-1),
	m_uloc_sample_texture(-1),
	m_uloc_object_color(-1)
{
	vertex_shader_code = "#version 330 core\n"
	"layout (location = 0) in vec3 atr_Position;\n"
	"layout (location = 1) in vec2 atr_TextureCoord;\n"
	"uniform mat4 u_MatProjection = mat4(1);\n"
	"uniform mat4 u_MatView = mat4(1);\n"
	"uniform mat4 u_MatModel = mat4(1);\n"
	"out vec2 v_TextureCoord;\n"
	"void main() {\n"
	"	gl_Position = u_MatProjection * (u_MatView * (u_MatModel * vec4(atr_Position, 1.0)));\n"
	"	v_TextureCoord = atr_TextureCoord;\n"
	"}\n";
	fragment_shader_code = "#version 330 core\n"
	"uniform sampler2D u_Texture0;\n"
	"uniform bool u_SampleTexture = false;\n"
	"uniform vec4 u_ObjectColor = vec4(1.0,1.0,1.0,1.0);\n"
	"in vec2 v_TextureCoord;\n"
	"out vec4 o_FragColor;\n"
	"void main() {\n"
	"if(u_SampleTexture)\n"
	"	o_FragColor = texture2D(u_Texture0, v_TextureCoord);\n"
	"else"
	"	o_FragColor = u_ObjectColor;\n"
	"}\n";
}
Quad::Program::~Program()
{
	this->destroy();
}

bool Quad::Program::iCreate()
{
	if(!axl::gl::gfx::Program::iCreate() || !(ctx_context->isCurrent() || ctx_context->makeCurrent())) return false;
	using namespace GL;
	bool success = true;
	axl::gl::gfx::Shader vertex_shader(ctx_context, GL_VERTEX_SHADER), fragment_shader(ctx_context, GL_FRAGMENT_SHADER);
	if(
		!vertex_shader.create() || !fragment_shader.create() ||
		!vertex_shader.setSource(vertex_shader_code) || !fragment_shader.setSource(fragment_shader_code) ||
		!vertex_shader.compile() || !fragment_shader.compile() ||
		!vertex_shader.attach(*this) || !fragment_shader.attach(*this) ||
		!this->link()
	)
	{
		vertex_shader_info_log = vertex_shader.getInfoLog();
		fragment_shader_info_log = fragment_shader.getInfoLog();
		this->destroy();
		success = false;
	}
	else
	{
		m_aloc_position = glGetAttribLocation(program_id, "atr_Position");
		m_aloc_texture_coord = glGetAttribLocation(program_id, "atr_TextureCoord");
		m_uloc_projection_matrix = glGetUniformLocation(program_id, "u_MatProjection");
		m_uloc_view_matrix = glGetUniformLocation(program_id, "u_MatView");
		m_uloc_model_matrix = glGetUniformLocation(program_id, "u_MatModel");
		m_uloc_texture0 = glGetUniformLocation(program_id, "u_Texture0");
		m_uloc_sample_texture = glGetUniformLocation(program_id, "u_SampleTexture");
		m_uloc_object_color = glGetUniformLocation(program_id, "u_ObjectColor");
		success = m_aloc_position != -1;
	}
	vertex_shader.detach(*this);
	vertex_shader.destroy();
	fragment_shader.detach(*this);
	fragment_shader.destroy();
	return success;
}
bool Quad::Program::iDestroy()
{
	m_aloc_position = -1;
	m_aloc_texture_coord = -1;
	m_uloc_projection_matrix = -1;
	m_uloc_view_matrix = -1;
	m_uloc_model_matrix = -1;
	m_uloc_texture0 = -1;
	m_uloc_sample_texture = -1;
	m_uloc_object_color = -1;
	return axl::gl::gfx::Program::iDestroy();
}
bool Quad::Program::isValid() const
{
	return axl::gl::ContextObject::isValid() && m_aloc_position != -1;
}

} // Test
