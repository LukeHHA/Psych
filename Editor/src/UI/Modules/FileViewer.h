#pragma once

#include "Core/Core.h"
#include "Panel.h"
#include <filesystem>
#include <string>

namespace psych::ui
{
class FileViewerPanel : public Panel
{
public:
  FileViewerPanel()                                  = default;
  FileViewerPanel(const FileViewerPanel&)            = default;
  FileViewerPanel(FileViewerPanel&&)                 = delete;
  FileViewerPanel& operator=(const FileViewerPanel&) = default;
  FileViewerPanel& operator=(FileViewerPanel&&)      = delete;
  ~FileViewerPanel() override                        = default;

  [[nodiscard]] static Unique<FileViewerPanel> Create();
  [[nodiscard]] static std::string GetID();

  void OnImGuiRender() override;
  void OnUpdate() override;
  void SetFilePath(const std::filesystem::path& path);

private:
  std::filesystem::path m_Path_;
};
} // namespace psych::ui
