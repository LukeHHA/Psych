#pragma once

#include "Renderer/RendererAPI.h"

namespace ge::platform {
    enum class APICapability {
        Tessellation,
        ComputeShaders,
        RayTracing
    };
    struct PlatformCapabilities {
        static constexpr bool Check_Cap_of(const RendererAPIType api, const APICapability cap) {
            switch (api) {
                case RendererAPIType::Opengl:
                    switch (cap) {
                        case APICapability::Tessellation:
                            return true;
                        case APICapability::ComputeShaders:
                            return true;
                        case APICapability::RayTracing:
                            return false;
                        default:
                            return false;
                    }
                case RendererAPIType::Vulkan:
                    switch (cap) {
                        case APICapability::Tessellation:
                            return true;
                        case APICapability::ComputeShaders:
                            return true;
                        case APICapability::RayTracing:
                            return true;
                        default:
                            return false;
                    }
                case RendererAPIType::Metal:
                    switch (cap) {
                        case APICapability::Tessellation:
                            return true;
                        case APICapability::ComputeShaders:
                            return true;
                        case APICapability::RayTracing:
                            return true;
                        default:
                            return false;
                    }
                default:
                    return false;
            }
        }
    };
}