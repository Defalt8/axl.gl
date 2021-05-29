#include <axl.gl/gfx/ui/elements/Slider.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace elements {

Slider::Slider(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2f& size,
			const axl::math::Vec4f& padding,
			axl::gl::gfx::ui::Layout::Size layout_width,
			axl::gl::gfx::ui::Layout::Size layout_height) :
	axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::ET_SLIDER, ptr_context, container, position, size, padding, layout_width, layout_height),
	slider_orientation(Slider::OR_HORIZONTAL),
	slider_value(0.0f),
	slider_min_value(0.0f),
	slider_max_value(1.0f),
	slider_is_sliding(false)
{}
Slider::~Slider()
{
	this->destroy();
}

void Slider::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::ui::Element::setContext(ptr_context);
}
bool Slider::isValid() const
{
	return axl::gl::gfx::ui::Element::isValid();
}
bool Slider::setSize(const axl::math::Vec2f& size)
{
	using namespace GL;
	if(!axl::gl::gfx::ui::Element::setSize(size)) return false;
	this->update();
	return true;
}
bool Slider::setSliderProgram(axl::gl::gfx::ui::elements::Slider::Program* slider_program_ptr)
{
	if(slider_program_ptr && this->ctx_context != slider_program_ptr->getContext()) return false;
	this->slider_program_ptr = slider_program_ptr;
	this->onModify();
	return true;
}
bool Slider::setSliderOrientation(Slider::Orientation slider_orientation)
{
	if(!this->onSliderOrientationChange(slider_orientation)) return false;
	this->slider_orientation = slider_orientation;
	this->onModify();
	return true;
}
bool Slider::setValue(float value)
{
	if(value < slider_min_value || value > slider_max_value)
		return false;
	this->slider_value = value;
	this->onModify();
	return true;
}
bool Slider::setMinValue(float min_value)
{
	if(min_value >= slider_max_value)
		return false;
	this->slider_min_value = min_value;
	this->onModify();
	return true;
}
bool Slider::setMaxValue(float max_value)
{
	if(max_value <= slider_min_value)
		return false;
	this->slider_max_value = max_value;
	this->onModify();
	return true;
}
Slider::Orientation Slider::getSliderOrientation() const
{
	return this->slider_orientation;
}
float Slider::getValue() const
{
	return slider_value;
}
float Slider::getMinValue() const
{
	return slider_min_value;
}
float Slider::getMaxValue() const
{
	return slider_max_value;
}
bool Slider::update()
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	GLfloat* verteces = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(!verteces)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return false;
	}
	axl::math::Vec2f total_padding((component_padding.x + component_padding.z), (component_padding.y + component_padding.w));
	float width = (component_size.x <= total_padding.x) ? 0.0f : (component_size.x - total_padding.x);
	float height = (component_size.y <= total_padding.y) ? 0.0f : (component_size.y - total_padding.y);
	verteces[0] = 0.0f; verteces[1] = 0.0f;
	verteces[2] = width; verteces[3] = 0.0f;
	verteces[4] = width; verteces[5] = height;
	verteces[6] = width; verteces[7] = height;
	verteces[8] = 0.0f; verteces[9] = height;
	verteces[10] = 0.0f; verteces[11] = 0.0f;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->onModify();
	return true;
}

void Slider::onSlide(float value)
{
	if(this->isValid())
		this->setValue(value);
}

bool Slider::iCreate()
{
	using namespace GL;
	if(!axl::gl::gfx::ui::Element::iCreate() ||
		!slider_program_ptr ||
		!slider_program_ptr->isValid()
	) return false;
	GLCLEARERROR();
	GLuint vertex_array_id, vertex_buffer_id;
	glGenVertexArrays(1, &vertex_array_id);
	glGenBuffers(1, &vertex_buffer_id);
	if(vertex_array_id == -1 || vertex_buffer_id == -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteBuffers(1, &vertex_buffer_id);
		glDeleteVertexArrays(1, &vertex_array_id);
		return false;
	}
	glBindVertexArray(vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*12), 0, GL_STATIC_DRAW);
	if(glGetError() != GL_NO_ERROR)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteBuffers(1, &vertex_buffer_id);
		glDeleteVertexArrays(1, &vertex_array_id);
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_vertex_buffer_id = vertex_buffer_id;
	m_vertex_array_id = vertex_array_id;
	this->update();
	this->onModify();
	return true;
}
bool Slider::iDestroy()
{
	return axl::gl::gfx::ui::Element::iDestroy();
}
bool Slider::iRender(axl::gl::camera::Camera3Df* camera)
{
	using namespace GL;
	axl::math::Mat4f mvp(
		(slider_orientation == OR_HORIZONTAL ? (slider_value / (slider_max_value - slider_min_value)) : 1.0f), 0.0f, 0.0f, 0.0f,
		0.0f, (slider_orientation == OR_VERTICAL ? (slider_value / (slider_max_value - slider_min_value)) : 1.0f), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		component_padding.x, component_padding.y, 0.0f, 1.0f
	);
	if(camera)
	{
		if(camera->projection)
			mvp = camera->projection->matrix  * camera->view_matrix * mvp;
		else
			mvp = camera->view_matrix * mvp;
	}
	else
		mvp = this->transform.getMatrix();
	if(!slider_program_ptr ||
		!slider_program_ptr->setUniformMat4fv(slider_program_ptr->getMVPLocation(), mvp.values) ||
		!slider_program_ptr->setUniform4fv(slider_program_ptr->getColorLocation(), &this->component_foreground_color.x) ||
		!slider_program_ptr->use()
	) return false;
	GLCLEARERROR();
	glBindVertexArray(m_vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	bool no_err = glGetError() == GL_NO_ERROR;
	slider_program_ptr->unuse();
	return no_err;
}
axl::math::Vec2f Slider::getContentSize() const
{
	return axl::math::Vec2f(0, 0);
}
bool Slider::onSliderOrientationChange(Orientation slider_orientation)
{
	return true;
}

//
// Slider::Program
//

Slider::Program::Program(axl::gl::Context* ptr_context) :
	axl::gl::gfx::Program(ptr_context),
	uloc_mvp(-1),
	uloc_color(-1)
{
	vertex_shader_code = "# version 330 core\n"
	"layout(location = 0) in vec2 atr_Position;\n"
	"uniform mat4 u_MVP = mat4(1.0);\n"
	"void main() {\n"
	"	gl_Position = u_MVP * vec4(atr_Position.xy, 0.0, 1.0);"
	"}\n";
	fragment_shader_code = "# version 330 core\n"
	"uniform vec4 u_Color = vec4(1.0);\n"
	"void main() {\n"
	"	gl_FragColor = u_Color;"
	"}\n";
}
Slider::Program::~Program()
{
	this->destroy();
}
bool Slider::Program::iCreate()
{
	using namespace GL;
	if(!axl::gl::gfx::Program::iCreate()) return false;
	Shader vertex_shader(this->ctx_context, GL_VERTEX_SHADER), fragment_shader(this->ctx_context, GL_FRAGMENT_SHADER);
	if(	!vertex_shader.create() ||
		!fragment_shader.create() ||
		!vertex_shader.setSource(vertex_shader_code) ||
		!fragment_shader.setSource(fragment_shader_code) ||
		!vertex_shader.compile() ||
		!fragment_shader.compile() ||
		!vertex_shader.attach(*this) ||
		!fragment_shader.attach(*this) ||
		!this->link()
	)
		return false;
	vertex_shader.detach(*this);
	fragment_shader.detach(*this);
	uloc_mvp = this->getUniformLocation("u_MVP");
	uloc_color = this->getUniformLocation("u_Color");
	return uloc_mvp != -1 && uloc_color != -1;
}
bool Slider::Program::iDestroy()
{
	uloc_mvp = -1;
	uloc_color = -1;
	return axl::gl::gfx::Program::iDestroy();
}
axl::glfl::GLint Slider::Program::getMVPLocation() const
{
	return uloc_mvp;
}
axl::glfl::GLint Slider::Program::getColorLocation() const
{
	return uloc_color;
}

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
