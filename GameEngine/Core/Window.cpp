#include "Window.h"
#include "Window/EngineWindow.h"
#include "Window/HeadlessWindow.h"
#include "Debug/Instrumentor.h"
#include "Debug/Assert.h"

namespace ge {

    Shared<Window> Window::Create(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();

        switch (RendererAPI::Current()) {
            case RendererAPIType::TEST_HEADLESS:
                return CreateShared<HeadlessWindow>(title, width, height);

            case RendererAPIType::OPENGL:
            case RendererAPIType::VULKAN:
            case RendererAPIType::METAL:
            case RendererAPIType::NONE:
                return CreateShared<EngineWindow>(title, width, height);
        }

        CORE_ASSERT(false, "Unknown Renderer type, Cannot create window!");
        return nullptr;
    }
} // namespace ge