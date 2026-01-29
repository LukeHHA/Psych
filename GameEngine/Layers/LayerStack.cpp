// Modified from Hazel’s LayerStack (Apache 2.0);
// renamed namespace hz → Core and prefixed macros with CORE_…

#include "LayerStack.h"
#include "Debug/Instrumentor.h"

namespace ge {
    LayerStack::~LayerStack() {
        CORE_PROFILE_FUNCTION();
        for (const std::unique_ptr<Layer>& layer : m_Layers) {
            layer->OnDetach();
        }
    }

    void LayerStack::PushLayer(std::unique_ptr<Layer> layer) {
        CORE_PROFILE_FUNCTION();
        layer->OnAttach();
        m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay) {
        CORE_PROFILE_FUNCTION();
        overlay->OnAttach();
        m_Layers.push_back(std::move(overlay));
    }

    void LayerStack::PopLayer(std::unique_ptr<Layer> layer) {
        CORE_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex) {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(std::unique_ptr<Layer> overlay) {
        CORE_PROFILE_FUNCTION();
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
} // namespace ge