#pragma once

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"

namespace GameEngine 
{
	class VertexArray 
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;
		~VertexArray();

		void add_vertex_buffer(const VertexBuffer& VBO);
		void add_element_buffer(const ElementBuffer& EBO);
		void bind() const;
		static void unbind();
		size_t get_indices_count() const { return m_indices_count; }

	private:
		unsigned int m_id = 0;
		unsigned int m_vbo_count = 0;
		size_t m_indices_count = 0;
	};
}