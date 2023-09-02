#include "ShaderProgram.hpp"
#include "Log.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace GameEngine 
{
	bool ShaderProgram::create_shader(const char* src_code, const GLenum shader_type, GLuint& shader_id) 
	{
		shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &src_code, nullptr);
		glCompileShader(shader_id);

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) 
		{
			char info_log[512];
			glGetShaderInfoLog(shader_id, 512, nullptr, info_log);

			LOG_CRITICAL("[Compile shader] Sahder compilation error. \n{}", info_log);
			return false;
		}
		return true;
	}

	ShaderProgram::ShaderProgram(const char* vertex_src, const char* fragment_src) 
	{
		GLuint vertx_shader = 0;
		if (!create_shader(vertex_src, GL_VERTEX_SHADER, vertx_shader)) 
		{
			LOG_CRITICAL("SHADER::VERTEX::COMPILE_ERROR");
			glDeleteShader(vertx_shader);
			return;
		}

		GLuint fragm_shader = 0;
		if (!create_shader(fragment_src, GL_FRAGMENT_SHADER, fragm_shader)) 
		{
			LOG_CRITICAL("SHADER::FRAGMENT::COMPILE_ERROR");
			glDeleteShader(vertx_shader);
			glDeleteShader(fragm_shader);
			return;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertx_shader);
		glAttachShader(m_id, fragm_shader);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			LOG_CRITICAL("PROGRAM::SHADER::LINKING: Link-time error:\n{0}", info_log);
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertx_shader);
			glDeleteShader(fragm_shader);
			return;
		}
		else
		{
			m_is_Compiled = true;
		}

		glDetachShader(m_id, vertx_shader);
		glDetachShader(m_id, fragm_shader);
		glDeleteShader(vertx_shader);
		glDeleteShader(fragm_shader);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_id = shaderProgram.m_id;
		m_is_Compiled = shaderProgram.m_is_Compiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_is_Compiled = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_id);
		m_id = shaderProgram.m_id;
		m_is_Compiled = shaderProgram.m_is_Compiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_is_Compiled = false;
		return *this;
	}

	ShaderProgram::~ShaderProgram() 
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgram::bind() const 
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind() 
	{
		glUseProgram(0);
	}

	void ShaderProgram::setMatrix4x4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}


}