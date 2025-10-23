#pragma once

#include <iostream>
#include <vector>

#include "Core.h"

namespace Core
{
    struct ApplicationSpecification
    {
        std::string Name = "Application";
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification &specification = ApplicationSpecification());
        ~Application();

        void Run();
        void Stop();

        void PushLayer(std::unique_ptr<Layer> layer);
        void PushOverlay(std::unique_ptr<Layer> layer);

        static Application &Get();

    private:
        ApplicationSpecification m_Specification;
        bool m_Running = false;
        static Application *s_Application;
        LayerStack
            m_LayerStack;
    };

    std::unique_ptr<Application> CreateApplication();
} // namespace Core
