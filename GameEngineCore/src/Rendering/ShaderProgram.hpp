#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

namespace GameEngine 
{
	class ShaderProgram 
	{
	public:
		ShaderProgram(const char* vertex_src, const char* fragment_src);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		~ShaderProgram();

		void bind() const;
		static void unbind();
		static bool create_shader(const char* src_code, const GLenum shader_type, GLuint& shader_id);
		bool isCompiled() const { return m_is_Compiled; }
		void setMatrix4x4(const char* name, const glm::mat4& matrix) const;

	private:
		bool m_is_Compiled = false;
		unsigned int m_id = 0;
	};
}