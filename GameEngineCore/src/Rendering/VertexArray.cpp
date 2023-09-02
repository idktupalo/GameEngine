#include "VertexArray.hpp"
#include <glad/glad.h>

namespace GameEngine 
{
	VertexArray::VertexArray() 
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() 
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		m_vbo_count = vertexArray.m_vbo_count;
		vertexArray.m_id = 0;
		vertexArray.m_vbo_count = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		m_vbo_count = vertexArray.m_vbo_count;
		vertexArray.m_id = 0;
		vertexArray.m_vbo_count = 0;
		return *this;
	}

	void VertexArray::bind() const 
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() 
	{
		glBindVertexArray(0);
	}

	void VertexArray::add_vertex_buffer(const VertexBuffer& VBO) 
	{
		bind();
		VBO.bind();
		
		for (const BufferElement& element : VBO.get_buffer_layout().get_elements())
		{
			glEnableVertexAttribArray(m_vbo_count);
			glVertexAttribPointer(
				m_vbo_count,
				element.components_count,
				element.component_type,
				GL_FALSE,
				static_cast<GLsizei>(VBO.get_buffer_layout().get_stride()),
				reinterpret_cast<const void*>(element.offset)
			);
			++m_vbo_count;
		}
	}

	void VertexArray::add_element_buffer(const ElementBuffer& EBO) 
	{
		bind();
		EBO.bind();
		m_indices_count = EBO.get_count();
	}
}