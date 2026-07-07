// Modified from ge’s LayerStack (Apache 2.0);
// renamed namespace hz → Core and prefixed macros with CORE_…
#pragma once

#include "Layer.h"

#include <memory>
#include <vector>

namespace psych
{
class LayerStack
{
public:
  // For now the Application is the only object that should own a LayerStack
  // And therefore there is no reason to copy or move it for now. If the time
  // Comes where a copy of the LayerStack would make sense (maybe to copy state)
  // The copy ctor will be implemented
  LayerStack();
  ~LayerStack();
  LayerStack(const LayerStack& other)            = delete;
  LayerStack(LayerStack&& other)                 = delete;
  LayerStack& operator=(const LayerStack& other) = delete;
  LayerStack& operator=(LayerStack&& other)      = delete;

  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> overlay);
  void PopLayer(std::unique_ptr<Layer> layer);
  void PopOverlay(std::unique_ptr<Layer> overlay);

public:
  std::vector<std::unique_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
  std::vector<std::unique_ptr<Layer>>::iterator end() { return m_Layers.end(); }
  std::vector<std::unique_ptr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
  std::vector<std::unique_ptr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

  std::vector<std::unique_ptr<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
  std::vector<std::unique_ptr<Layer>>::const_iterator end() const { return m_Layers.end(); }
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

private:
  std::vector<std::unique_ptr<Layer>> m_Layers;
  unsigned int m_LayerInsertIndex = 0;
};
} // namespace psych
