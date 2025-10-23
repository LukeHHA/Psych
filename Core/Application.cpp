#include "Application.h"
#include "Debug/Assert.h"

namespace Core
{

    Application *Application::s_Application = nullptr;

    Application::Application(const ApplicationSpecification &specification)
        : m_Specification(specification)
    {
        CORE_PROFILE_FUNCTION();
        CORE_ASSERT(!s_Application, "Application already exists")
        s_Application = this;
    }

    Application::~Application()
    {
        CORE_PROFILE_FUNCTION();
        CORE_ASSERT(s_Application == this, "Static Application Pointer Is Corrupt On Teardown");
        s_Application = nullptr;
    }

    void Application::Run()
    {
        CORE_PROFILE_FUNCTION();
        m_Running = true;

        // Main Application loop
        while (m_Running)
        {
            std::string input;
            std::cin >> input;
            if (input != "yes")
            {
                m_Running = false;
            }

            for (const std::unique_ptr<Layer> &layer : m_LayerStack)
                layer->OnUpdate();

            for (const std::unique_ptr<Layer> &layer : m_LayerStack)
                layer->OnRender();
        }
    }

    void Application::Stop()
    {
        CORE_PROFILE_FUNCTION();
        m_Running = false;
    }

    Application &Application::Get()
    {
        CORE_PROFILE_FUNCTION();
        CORE_ASSERT(s_Application != nullptr, "Application is NULLPTR whilst trying to GET()")
        return *s_Application;
    }

    void Application::PushLayer(std::unique_ptr<Layer> layer)
    {
        CORE_PROFILE_FUNCTION();
        m_LayerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(std::unique_ptr<Layer> layer)
    {
        CORE_PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(std::move(layer));
    }
}
