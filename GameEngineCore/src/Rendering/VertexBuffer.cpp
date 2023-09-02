#include "VertexBuffer.hpp"
#include "Log.hpp"

#include <glad/glad.h>

namespace GameEngine 
{
	constexpr GLint shader_data_type_to_components_count(const ShaderDataType data_type) 
	{
		switch (data_type)
		{
			case ShaderDataType::Int:
			case ShaderDataType::Float:
				return 1;

			case ShaderDataType::Int2:
			case ShaderDataType::Float2:
				return 2;

			case ShaderDataType::Int3:
			case ShaderDataType::Float3:
				return 3;

			case ShaderDataType::Int4:
			case ShaderDataType::Float4:
				return 4;
		}
		LOG_ERROR("[shader_data_type_to_components_count] : unknown ShaderDataType");
		return 0;
	}

	constexpr GLint shader_data_type_size(const ShaderDataType data_type) 
	{
		switch (data_type)
		{
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return sizeof(GLint) * shader_data_type_to_components_count(data_type);
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return sizeof(GLfloat) * shader_data_type_to_components_count(data_type);
		}
		LOG_ERROR("[shader_data_type_size] : unknown ShaderDataType");
		return 0;
	}

	constexpr size_t shader_data_type_to_component_type(const ShaderDataType data_type)
	{
		switch (data_type)
		{
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return GL_FLOAT;
		}
		LOG_ERROR("[shader_data_type_to_component_type] : unknown ShaderDataType");
		return 0;
	}

	constexpr GLenum usage_to_GLenum(const VertexBuffer::Usage usage) 
	{
		switch (usage) 
		{
			case VertexBuffer::Usage::Static: return GL_STATIC_DRAW;
			case VertexBuffer::Usage::Dynamic: return GL_DYNAMIC_DRAW;
			case VertexBuffer::Usage::Stream: return GL_STREAM_DRAW;
		}
		LOG_ERROR("[GL_X_DRAW] VERTEX_BUFFER::USAGE wrong draw selection. (be used GL_STATIC_DRAW by default.)");
		return GL_STATIC_DRAW;
	}

	BufferElement::BufferElement(const ShaderDataType _type) 
		: type(_type),
		  component_type(shader_data_type_to_component_type(_type)),
		  components_count(shader_data_type_to_components_count(_type)),
		  size(shader_data_type_size(_type)),
		  offset(0)
	{
	}

	VertexBuffer::VertexBuffer(const size_t size, const void* data, BufferLayout buffer_layout, const Usage usage)
		: m_buffer_layout(std::move(buffer_layout))
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
		: m_id(vertexBuffer.m_id),
		  m_buffer_layout(std::move(vertexBuffer.m_buffer_layout))
	{
		vertexBuffer.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept 
	{
		m_id = vertexBuffer.m_id;
		m_buffer_layout = std::move(vertexBuffer.m_buffer_layout);
		vertexBuffer.m_id = 0;
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::bind() const 
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}