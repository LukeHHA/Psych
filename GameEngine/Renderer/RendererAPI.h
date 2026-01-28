#pragma once

namespace ge {
    enum class RendererAPIType { None, Opengl, Vulkan, Metal };

    class RendererAPI {
    public:
        static RendererAPIType SetAPI(RendererAPIType api) {
            s_RendererAPI = api;
            return s_RendererAPI;
        }

        static RendererAPIType Current() { return s_RendererAPI; }

    private:
        static RendererAPIType s_RendererAPI;
    };
} // namespace ge
