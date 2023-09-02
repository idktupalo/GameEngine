#pragma once

#include <vector>
#include <glad/glad.h>

namespace GameEngine 
{
	enum class ShaderDataType 
	{
		Int,
		Int2,
		Int3,
		Int4,
		Float,
		Float2,
		Float3,
		Float4
	};

	struct BufferElement 
	{
		ShaderDataType type;
		uint32_t component_type;
		GLint components_count;
		GLint size; // stride
		size_t offset;

		BufferElement(const ShaderDataType type);
	};

	class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(std::move(elements))
        {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        const std::vector<BufferElement>& get_elements() const { return m_elements; }
        size_t get_stride() const { return m_stride; }

    private:
        std::vector<BufferElement> m_elements;
        size_t m_stride = 0;
    };

	class VertexBuffer 
	{
	public:

		enum class Usage 
		{
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const size_t size, const void* data, BufferLayout buffer_layout, const Usage usage = Usage::Static);
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer() = delete;

		~VertexBuffer();

		void bind() const;
		static void unbind();
		const BufferLayout& get_buffer_layout() const { return m_buffer_layout; }

	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;
	};
}