#include "Window.hpp"
#include "Log.hpp"
#include "Rendering/ShaderProgram.hpp"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexArray.hpp"
#include "Rendering/ElementBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace GameEngine 
{
	static bool s_GLFW_isInit = false;

	GLfloat positions_colors[] = {
		 -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
		  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	std::unique_ptr<VertexArray> p_vao_buffer;
	std::unique_ptr<VertexBuffer> p_vertx_color_vbo;
	std::unique_ptr<ElementBuffer> p_element_buffer;
	std::unique_ptr<ShaderProgram> p_shader_program;

	const char* vertex_shader = R"(
		#version 460
		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec3 vertex_color;
		uniform mat4 model_matrix;
		out vec3 color;
		void main(){
			color = vertex_color;
			gl_Position = model_matrix * vec4(vertex_position, 1.f);
		}
	)";

	const char* fragment_shader = R"(
		#version 460
		in vec3 color;
		out vec4 frag_color;
		void main(){
			frag_color = vec4(color, 1.f);
		}
	)";

	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };

	Window::Window(unsigned int width, unsigned int height, std::string title)
		: m_WindowData({ width, height, std::move(title)})
	{
		int returnCode = init();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	}

	Window::~Window() 
	{
		shutdown();
	}

	int Window::init() 
	{
		LOG_INFO("Creating window '{0}' with size {1}x{2}", m_WindowData.title, m_WindowData.width, m_WindowData.height);

		if (!s_GLFW_isInit) 
		{
			if (!glfwInit()) 
			{
				LOG_CRITICAL("Cant initialize GLFW");
				return -1;
			}
			s_GLFW_isInit = true;
		}

		m_pWindow = glfwCreateWindow(m_WindowData.width, m_WindowData.height, m_WindowData.title.c_str(), nullptr, nullptr);
		if (!m_pWindow) 
		{
			LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_WindowData.title, m_WindowData.width, m_WindowData.height);
			glfwTerminate();
			return -2;
		}

		glfwMakeContextCurrent(m_pWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_CRITICAL("Failed to initialize GLAD");
			return -3;
		}

		glfwSetWindowUserPointer(m_pWindow, &m_WindowData);

		glfwSetWindowSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height) 
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				data.width = width;
				data.height = height;

				EventWindowResize event(width, height);
				data.eventCallbackFn(event);
			}
		);

		glfwSetCursorPosCallback(m_pWindow,
			[](GLFWwindow* pWindow, double x, double y) 
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(m_pWindow,
			[](GLFWwindow* pWindow) 
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventWindowClose event;
				data.eventCallbackFn(event);
			}
		);

		glfwSetFramebufferSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height) 
			{
				glViewport(0, 0, width, height);
			}
		);

		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->isCompiled()) 
		{
			return false;
		}

		BufferLayout buffersLayout_vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};
		p_vao_buffer = std::make_unique<VertexArray>();
		p_vertx_color_vbo = std::make_unique<VertexBuffer>(sizeof(positions_colors), positions_colors, buffersLayout_vec3);
		p_element_buffer = std::make_unique<ElementBuffer>(sizeof(indices) / sizeof(GLuint), indices);
		p_vao_buffer->add_vertex_buffer(*p_vertx_color_vbo);
		p_vao_buffer->add_element_buffer(*p_element_buffer);

		return 0;
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::on_update() 
	{
		glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		
		p_shader_program->bind();

		glm::mat4 scale_matrix(
			scale[0],        0,		    0,	     0,
               0,   scale[1],        0,        0,
               0,         0,   scale[2],       0,
               0,         0,         0,        1
		);


		float rotate_to_radians = glm::radians(rotate);
		glm::mat4 rotate_matrix(
			cos(rotate_to_radians),  sin(rotate_to_radians),  0,  0,
			-sin(rotate_to_radians), cos(rotate_to_radians),  0,  0,
			0,								 0,                       1,  0,
			0,								 0,							  0,  1
		);

		glm::mat4 translate_matrix(
			1,				  0,				 0,				0,
			0,				  1,				 0,				0,
			0,				  0,				 1,				0,
			translate[0], translate[1], translate[2], 1 
		);

		glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
		p_shader_program->setMatrix4x4("model_matrix", model_matrix);

		p_vao_buffer->bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao_buffer->get_indices_count()), GL_UNSIGNED_INT, (void*)0);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Background Color Window");
		ImGui::ColorEdit4("Background Color", m_background_color);
		ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
		ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
		ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}