 
/**************************************************************************/
/*  EngineWindow.cpp                                                      */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "EngineWindow.h"
#include "Core/Core.h"
#include "Debug/Instrumentor.h"
#include "Events/Event.h"
#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include <GLFW/glfw3.h>

namespace psych
{
static void GLFWErrorCallback(int error, const char* description) { CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description); }

EngineWindow::~EngineWindow()
{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("EngineWindow::Shutdown");
    Shutdown();
  }
  CORE_LOG_INFO("Window Shutdown");
}

Expected<void, errors::WindowError> EngineWindow::Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler)
{
  CORE_PROFILE_FUNCTION();

  if (width == 0 || height == 0) {
    return Unexpected(errors::WindowError::InvalidDimensions);
  }

  // This is just to ensure one window for now but will be reference counted in
  // the future
  if (!m_Window) {
    m_Data.Title         = title;
    m_Data.Width         = width;
    m_Data.Height        = height;
    m_Data.EventsHandler = &eventHandler;

    CORE_PROFILE_SCOPE("glfwInit");
    int success = glfwInit();
    if (success == 0) {
      return Unexpected(errors::WindowError::InitializationFailed);
    }

    CORE_LOG_INFO("GLFW initialized successfully");
    glfwSetErrorCallback(GLFWErrorCallback);
  } else {
    return Unexpected(errors::WindowError::WindowAlreadyExists);
  }

  {
    CORE_PROFILE_SCOPE("glfeCreateWindow");
#if defined(CORE_DEBUG)
    if (RendererAPI::Current() == RendererAPIType::OPENGL) {
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#if defined(__linux__)
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
#endif

    m_Window = UniqueGLFWwindow(glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr), GLFWwindowDeleter{});
    if (!m_Window) {
      return Unexpected(errors::WindowError::NativeWindowCreationFailed);
    }

    CORE_LOG_INFO("GLFW Window '{0}' created successfully", title);

    auto contextResult = RendererContext::Create(m_Window.get());
    if (!contextResult) {
      return Unexpected(errors::WindowError::ContextCreationFailed);
    }

    m_RendererContext = std::move(contextResult.value());
    auto contextInit  = m_RendererContext->Init();
    if (!contextInit) {
      return Unexpected(errors::WindowError::ContextInitializationFailed);
    }

    CORE_LOG_INFO("Initialized Context of type '{0}'", static_cast<int>(m_RendererContext->GetCurrentAPI()));

    glfwSetWindowUserPointer(m_Window.get(), &m_Data);
    SetVSync(true);
    CORE_LOG_INFO("EngineWindow Initialized successfully");

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

      auto event       = CreateUnique<WindowResizeEvent>(width, height);
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetFramebufferSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
      auto event       = CreateUnique<FramebufferResizeEvent>(width, height);
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
      auto event       = CreateUnique<WindowCloseEvent>();
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

      switch (action) {
      case GLFW_PRESS: {
        auto event = CreateUnique<KeyPressedEvent>(key, 0);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      case GLFW_RELEASE: {
        auto event = CreateUnique<KeyReleasedEvent>(key);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      case GLFW_REPEAT: {
        auto event = CreateUnique<KeyPressedEvent>(key, true);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      default:
        break;
      }
    });

    glfwSetCharCallback(m_Window.get(), [](GLFWwindow* window, unsigned int keycode) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
      auto event       = CreateUnique<KeyTypedEvent>(keycode);
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

      switch (action) {
      case GLFW_PRESS: {
        auto event = CreateUnique<MouseButtonPressedEvent>(button);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      case GLFW_RELEASE: {
        auto event = CreateUnique<MouseButtonReleasedEvent>(button);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      default:
        break;
      }
    });

    glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

      auto event       = CreateUnique<MouseScrolledEvent>((float)xOffset, (float)yOffset);
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double xPos, double yPos) {
      WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

      auto event       = CreateUnique<MouseMovedEvent>((float)xPos, (float)yPos);
      data.EventsHandler->QueueEvent(std::move(event));
    });
  }
  return {};
}

Expected<void, errors::WindowError> EngineWindow::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  m_RendererContext.reset();
  m_Window.reset();
  CORE_LOG_INFO("EngineWindow Shutdown successfully");
  return {};
}

void EngineWindow::PollEvents() { glfwPollEvents(); }

void EngineWindow::OnUpdate()
{
  CORE_PROFILE_FUNCTION();
  m_RendererContext->SwapBuffers();
}

void EngineWindow::HandleEvents(Unique<Event> event)
{
  switch (event->GetEventType()) {
  case EventType::WindowResize:
  case EventType::None:
  case EventType::WindowClose:
  case EventType::WindowShouldClose:
  case EventType::WindowFocus:
  case EventType::WindowLostFocus:
  case EventType::WindowMoved:
  case EventType::FramebufferResize:
  case EventType::AppTick:
  case EventType::AppUpdate:
  case EventType::AppRender:
  case EventType::KeyPressed:
  case EventType::KeyReleased:
  case EventType::KeyTyped:
  case EventType::MouseButtonPressed:
  case EventType::MouseButtonReleased:
  case EventType::MouseMoved:
  case EventType::MouseScrolled:
    break;
  }
}
void EngineWindow::SetVSync(bool enabled)
{
  CORE_PROFILE_FUNCTION();

  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }

  m_Data.VSync = enabled;
}

bool EngineWindow::IsVSync() const { return m_Data.VSync; }

} // namespace psych
