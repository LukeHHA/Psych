#pragma once

#include "Core/Base.h"
#include "Renderer/RendererContext.h"
#include "GLFW/glfw3.h"
#include "ge_expected"
#include "Errors/Errors.h"
#include "Core/Event.h"
#include "Core/Window.h"

#include <string>
#include <functional>

namespace ge {
    class EngineWindow : public Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        EngineWindow(const std::string& title, unsigned int width, unsigned int height);
        virtual ~EngineWindow();

        util::expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height) override;
        util::expected<void, errors::WindowError> Shutdown() override;
        void OnUpdate() override;
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

    private:
        Shared<RendererContext> m_RendererContext;
        UniqueGLFWwindow m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // namespace ge