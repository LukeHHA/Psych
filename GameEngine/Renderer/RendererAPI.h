#pragma once
#include <Debug/Assert.h>

namespace ge {
    enum class RendererAPIType { None,
        Opengl,
        Vulkan,
        Metal };

    class RendererAPI {
    public:
        static RendererAPIType SetAPI(RendererAPIType api) {
            s_RendererAPI = api;
            return s_RendererAPI;
        }

        static RendererAPIType Current() {
            CORE_ASSERT(s_RendererAPI != RendererAPIType::None, "RendererAPI::Current() called but no API is set!");
            return s_RendererAPI;
        }

    private:
        inline static RendererAPIType s_RendererAPI;
    };
} // namespace ge
