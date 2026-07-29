#pragma once

#include "Core/Core.h"
#include "FileSystem/FileSystem.h"
#include "Panel.h"
#include <string>
#include <vector>

namespace psych::ui
{
class MainFileTreePanel : public Panel
{
public:
  using SelectedCallbackFn                               = std::function<void(const std::filesystem::path&)>;

  MainFileTreePanel()                                    = default;
  MainFileTreePanel(const MainFileTreePanel&)            = default;
  MainFileTreePanel(MainFileTreePanel&&)                 = delete;
  MainFileTreePanel& operator=(const MainFileTreePanel&) = default;
  MainFileTreePanel& operator=(MainFileTreePanel&&)      = delete;
  ~MainFileTreePanel() override                          = default;

  [[nodiscard]] static Unique<MainFileTreePanel> Create();
  [[nodiscard]] static std::string GetID();

  void OnImGuiRender() override;
  void OnUpdate() override;
  void SetSelectedCallbackFn(SelectedCallbackFn fn);
  void SetRootNode(const FileNode* node);
  [[nodiscard]] std::filesystem::path GetCurrentSelected() const;

private:
  void DrawFileTreeNode(const FileNode* node);

  const FileNode* m_RootNode_ = nullptr;
  std::filesystem::path m_CurrentSelected_;
  std::vector<SelectedCallbackFn> m_SelectedCallbacks_;
};
} // namespace psych::ui
