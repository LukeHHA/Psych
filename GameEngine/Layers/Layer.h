#pragma once

#include "Events/Event.h"

namespace ge {

    class Layer {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnEvent(Event& event) = 0;
        virtual void OnUpdate(float ts = 1) = 0;
        virtual void OnRender() = 0;
    };

} // namespace ge