#include "Window.h"

namespace ge {
    Window::Window(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION()
        Init(title, width, height);

        m_RendererContext = RendererContext::Create(m_Window.get());
    }

    util::expected<bool, std::string> Window::Init(const std::string& title, unsigned int width, unsigned int height){
        CORE_PROFILE_FUNCTION()}

    Shared<Window> Window::Create(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION()
        return CreateShared<Window>(title, width, height);
    }
} // namespace ge