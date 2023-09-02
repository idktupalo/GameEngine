#include <iostream>

#include "Application.hpp"
#include "Window.hpp"
#include <Log.hpp>
#include "Event.hpp"

namespace GameEngine 
{
	Application::Application()
	{
		LOG_INFO("Start application");
	}

	Application::~Application() 
	{
		LOG_INFO("Application finish");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) 
	{
		m_pWindow = std::make_unique<Window>(window_width, window_height, title);

		m_eventDispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event) 
			{
				//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_eventDispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
			}
		);

		m_eventDispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event) 
			{
				LOG_INFO("[WindowClose]");
				m_closeWindow = true;
			}
		);

		m_pWindow->set_event_callback(
			[&](BaseEvent& event) 
			{
				//LOG_INFO("[EVENT] Changed size to {0}x{1}", event.width, event.height);
				m_eventDispatcher.dispatch(event);
			}
		);

		while (!m_closeWindow) 
		{
			m_pWindow->on_update();
		}
		m_pWindow = nullptr;
		return 0;
	}
}