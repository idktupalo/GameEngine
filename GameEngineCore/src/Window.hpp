#pragma once

#include "Event.hpp"
#include <string>
#include <functional>

struct GLFWwindow;

namespace GameEngine 
{
	class Window 
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(const unsigned int m_width, const unsigned int m_height, std::string m_title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();
		unsigned int get_width() const { return m_WindowData.width; }
		unsigned int get_height() const { return m_WindowData.height; }

		void set_event_callback(const EventCallbackFn& callback) 
		{
			m_WindowData.eventCallbackFn = callback;
		}

	private:

		struct WindowData 
		{
			unsigned int width;
			unsigned int height;
			std::string title;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData m_WindowData;
		float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };
	};
}