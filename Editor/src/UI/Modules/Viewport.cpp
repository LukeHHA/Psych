#include "Viewport.h"
#include "Core/PsychEngine.h"
#include <cstdint>
#include <imgui.h>

namespace psych::ui
{
Unique<ViewportPanel> ViewportPanel::Create() { return CreateUnique<ViewportPanel>(); }

std::string ViewportPanel::GetID() { return "ViewportPanel"; }

void ViewportPanel::OnImGuiRender()
{
  ImGuiWindowFlags windowFlags = 0;
  windowFlags |= ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
  windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;

  if (!ImGui::Begin("Viewport", nullptr, windowFlags)) {
    ImGui::End();
    return;
  }

  const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
  if (viewportSize.x > 0.0F && viewportSize.y > 0.0F) {
    auto& framebuffer         = PsychEngine::Get().GetFramebuffer();
    const auto viewportWidth  = static_cast<uint32_t>(viewportSize.x);
    const auto viewportHeight = static_cast<uint32_t>(viewportSize.y);

    if (viewportWidth != framebuffer.GetWidth() || viewportHeight != framebuffer.GetHeight()) {
      framebuffer.Resize(viewportWidth, viewportHeight);
    }

    const auto framebufferTexture = static_cast<ImTextureID>(framebuffer.GetColorAttachmentID());
    ImGui::Image(framebufferTexture, viewportSize, ImVec2(0.0F, 1.0F), ImVec2(1.0F, 0.0F));
  }

  ImGui::End();
}

void ViewportPanel::OnUpdate() {}
} // namespace psych::ui
