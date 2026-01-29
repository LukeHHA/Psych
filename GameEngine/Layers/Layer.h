#pragma once

#include "Core/Event.h"

namespace ge {

    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnEvent(Event& event) {}

        virtual void OnUpdate(float ts = 1) {}
        virtual void OnRender() {}
    };

} // namespace ge