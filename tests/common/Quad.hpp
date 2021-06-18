#pragma once
#include "stdafx.hpp"

namespace Test {

class Quad : public axl::gl::ContextObject
{
	public:
		class Program : public axl::gl::gfx::Program
		{
			public:
				Program(axl::gl::Context* ptr_context = 0);
				virtual ~Program();
			protected:
				bool iCreate();
				bool iDestroy();
			public:
				bool isValid() const;
			public:
				axl::util::String vertex_shader_code;
				axl::util::String fragment_shader_code;
				axl::util::String vertex_shader_info_log;
				axl::util::String fragment_shader_info_log;
				const axl::glfl::GLint& aloc_position;
				const axl::glfl::GLint& aloc_texture_coord;
				const axl::glfl::GLint& uloc_projection_matrix;
				const axl::glfl::GLint& uloc_view_matrix;
				const axl::glfl::GLint& uloc_model_matrix;
				const axl::glfl::GLint& uloc_texture0;
				const axl::glfl::GLint& uloc_sample_texture;
				const axl::glfl::GLint& uloc_object_color;
			private:
				axl::glfl::GLint m_aloc_position;
				axl::glfl::GLint m_aloc_texture_coord;
				axl::glfl::GLint m_uloc_projection_matrix;
				axl::glfl::GLint m_uloc_view_matrix;
				axl::glfl::GLint m_uloc_model_matrix;
				axl::glfl::GLint m_uloc_texture0;
				axl::glfl::GLint m_uloc_sample_texture;
				axl::glfl::GLint m_uloc_object_color;
		};
	public:
		Quad(axl::gl::Context* ptr_context = 0, const axl::math::Vec2f& size = axl::math::Vec2f(0.f,0.f));
		virtual ~Quad();
	protected:
		bool iCreate();
		bool iDestroy();
	public:
		bool isValid() const;
		bool setQuadProgram(Quad::Program* quad_program_ptr);
		bool updateQuadProgram();
		bool setSize(const axl::math::Vec2f& size);
		bool render(axl::gl::camera::Camera3Df* camera = 0);
	public:
		const axl::math::Vec2f& size;
		axl::math::Transform4f transform;
		axl::gl::gfx::Texture2D* texture;
		axl::math::Vec4f color;
	private:
		axl::math::Vec2f m_size;
		Program *m_program_ptr;
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
};

} // Test
