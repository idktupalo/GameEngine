#pragma once

#include "VertexBuffer.hpp"

namespace GameEngine 
{
	using drawUsage = VertexBuffer::Usage;

	class ElementBuffer 
	{
	public:
		ElementBuffer(const size_t size, const void* data, const drawUsage usage = drawUsage::Static);
		ElementBuffer(const ElementBuffer&) = delete;
		ElementBuffer& operator=(const ElementBuffer&) = delete;
		ElementBuffer(ElementBuffer&&) noexcept;
		ElementBuffer& operator=(ElementBuffer&&) noexcept;
		~ElementBuffer();

		void bind() const;
		static void unbind();
		size_t get_count() const { return m_count; }

	private:
		unsigned int m_id = 0;
		size_t m_count = 0;
	};
}
