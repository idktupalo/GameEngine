#include "ElementBuffer.hpp"
#include "Log.hpp"

namespace GameEngine 
{

	constexpr GLenum usage_to_GLenum(const drawUsage usage) 
	{
		switch (usage) 
		{
			case drawUsage::Dynamic:	return GL_DYNAMIC_DRAW;
			case drawUsage::Static:		return GL_STATIC_DRAW;
			case drawUsage::Stream:		return GL_STREAM_DRAW;
		}
		LOG_INFO("[drawUsage] : unknow buffer usage (be used GL_STATIC_DRAW by default)");
		return GL_STATIC_DRAW;
	}

	ElementBuffer::ElementBuffer(const size_t size, const void* data, const drawUsage usage)
		: m_count(size)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), data, usage_to_GLenum(usage));
	}

	ElementBuffer::ElementBuffer(ElementBuffer&& elBuff) noexcept
		: m_id(elBuff.m_id),
		  m_count(elBuff.m_count)
	{
		elBuff.m_id = 0;
		elBuff.m_count = 0;
	}

	ElementBuffer& ElementBuffer::operator=(ElementBuffer&& elBuff) noexcept 
	{
		m_id = elBuff.m_id;
		m_count = elBuff.m_count;
		elBuff.m_id = 0;
		elBuff.m_count = 0;
		return *this;
	}

	ElementBuffer::~ElementBuffer() 
	{
		glDeleteBuffers(1, &m_id);
	}

	void ElementBuffer::bind() const 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void ElementBuffer::unbind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}