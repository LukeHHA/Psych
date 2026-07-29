#pragma once

#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Panel.h"
#include "expected.h"
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace psych
{

namespace errors
{
enum class PanelError : uint8_t {
  None = 0,
  PanelAlreadyExists,
  PanelNotFound,
};
} // namespace errors

class PanelManager
{
public:
  PanelManager()                               = default;
  PanelManager(const PanelManager&)            = delete;
  PanelManager(PanelManager&&)                 = delete;
  PanelManager& operator=(const PanelManager&) = delete;
  PanelManager& operator=(PanelManager&&)      = delete;
  ~PanelManager()                              = default;

  template <typename PanelT>
  PanelT* AddPanel()
  {
    auto result = TryAddPanel<PanelT>();
    CORE_ASSERT(result, "Panel already exists");

    return result ? result.value() : nullptr;
  }

  template <typename PanelT>
  Expected<PanelT*, errors::PanelError> TryAddPanel()
  {
    static_assert(std::is_base_of_v<Panel, PanelT>, "Panel must inherit from Panel class");
    const std::string id = PanelT::GetID();
    if (m_PanelLookup_.contains(id)) {
      return Unexpected(errors::PanelError::PanelAlreadyExists);
    }

    auto panel          = PanelT::Create();
    PanelT* panelHandle = panel.get();

    m_Panels_.push_back(std::move(panel));
    m_PanelLookup_.emplace(id, panelHandle);

    return panelHandle;
  }

  template <typename PanelT>
  Expected<PanelT*, errors::PanelError> TryGetPanel()
  {
    static_assert(std::is_base_of_v<Panel, PanelT>, "Panel must inherit from Panel class");
    const auto panel = m_PanelLookup_.find(PanelT::GetID());
    if (panel == m_PanelLookup_.end()) {
      return Unexpected(errors::PanelError::PanelNotFound);
    }

    auto* typedPanel = dynamic_cast<PanelT*>(panel->second);
    if (typedPanel == nullptr) {
      return Unexpected(errors::PanelError::PanelNotFound);
    }

    return typedPanel;
  }

  template <typename PanelT>
  PanelT* GetPanel()
  {
    auto result = TryGetPanel<PanelT>();
    CORE_ASSERT(result, "Panel does not exist");
    return result ? result.value() : nullptr;
  }

  void OnImGuiRender()
  {
    for (auto& panel : m_Panels_) {
      panel->OnImGuiRender();
    }
  }

  void OnUpdate()
  {
    for (auto& panel : m_Panels_) {
      panel->OnUpdate();
    }
  }

  void OnEvent(Event& event)
  {
    for (auto& panel : m_Panels_) {
      panel->OnEvent(event);
    }
  }

private:
  std::vector<Unique<Panel>> m_Panels_;
  std::unordered_map<std::string, Panel*> m_PanelLookup_;
};
} // namespace psych
